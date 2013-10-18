#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <err.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sqlite3.h>

#include <openssl/sha.h>

using namespace std;

class ABMultiValue
{
public:
	size_t person_record_id; /* The row-id from ABPersons */
	string label; /* Could be empty, JOIN'd from ABMultiValueLabel */
	int property;
	string value; /* Could be empty */

	ABMultiValue() : person_record_id(0), property(0) { }
};
typedef vector<ABMultiValue> ABMultiValueVector;

class ABAddress
{
public:
	size_t person_record_id;
	string country;
	string street;
	string ZIP;
	string city;
	string country_code;
	string state;
	string sub_locality;
};
typedef vector<ABAddress> ABAddressVector;

class ABPerson
{
public:
	size_t rowid;
	string First;
	string Last;
	string Middle;
	string Nickname;
	string Prefix;
	string Suffix;
	string Organization;
	string Department;
	string Note;
	string Birthday;
	string JobTitle;
	string DisplayName;

	ABPerson() : rowid(0) { }

	string GetName() const
	{
		string name;

		if (!First.empty())
			name = First;
		if (!Middle.empty()) {
			if (!name.empty())
				name += " ";
			name += Middle;
		}
		if (!Last.empty()) {
			if (!name.empty())
				name += " ";
			name += Last;
		}

		/* If the name is still empty, try other fields */
		if (name.empty() && !Organization.empty())
			name = Organization;
		if (name.empty() && !Department.empty())
			name = Department;
		if (name.empty() && !JobTitle.empty())
			name = JobTitle;
		if (name.empty() && !DisplayName.empty())
			name = DisplayName;

		/* Can this happen? */
		if (name.empty()) {
			cerr << "Warning: found ABPerson record without recognizable name (rowID = " << rowid << ")" << endl;
			stringstream ss;
			ss << "Uknown(RowID=" << rowid << ")";
			name = ss.str();
		}
		return name;
	}

	ABMultiValueVector values;
};
typedef vector<ABPerson> ABPersonsVector;


/* Global Variables */
std::string program_name;
std::string backup_directory;

/* Command line options */
const struct option mbdb_options[] = {
	{"help",	no_argument,	0,	'h'},
	{0,		0,		0,	0}
};

/* Show a usage-related error message,
   with additional "try --help" message,
   then exit with exit code 1 */
void usage_error(const char *error_message,...)
{
	if (error_message!=NULL && strlen(error_message)>0) {
		va_list ap;
		va_start(ap, error_message);
		vfprintf(stderr,error_message,ap);
		va_end(ap);
	}
	cerr << "Try '" << program_name
		<< " --help' for more information\n";
	exit(EXIT_FAILURE);
}

/* Show detailed help screen  */
void show_help()
{
	cout<<
"AddressBook - iOS AddressBook parser\n" \
"\n" \
"Usage: " << program_name << " [OPTIONS] DIR\n" \
"\n" \
"Required Parameters:\n" \
"  DIR  - The backup directory\n" \
"\n" \
"Options:\n" \
"  --help   -  Show this help screen.\n" \
"\n"
	;
}

/* Returns NON-ZERO if 'dir' is a valid directory */
int is_valid_directory(const char* dir)
{
	struct stat sb;
	return ((stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode)));
}

/* Returns NON-ZERO if 'file' is an existing regular file */
int is_valid_regular_file(const char* file)
{
	struct stat sb;
	return ((stat(file, &sb) == 0 && S_ISREG(sb.st_mode)));
}


void validate_idevice_bacup_directory(const char* dir)
{
	if (!is_valid_directory(dir))
		errx(1,"error: '%s' is not a valid directory",dir);

	string f(dir);
	f += "/Manifest.mbdb";
	if (!is_valid_regular_file(f.c_str()))
		errx(1,"error: '%s' is not a valid iDevice backup " \
		       "directory (can't find required file " \
		       "'%s/Manifest.mbdb')", dir, dir);
}

/* Given a domain (e.g. "CameraRollDomain")
   and a file path, calculates the SHA1 hash of the filename.

   SHA1 output is stored in "sha1" variable, which MUST have at least 20 bytes allocated.
   No error checking is performed. */
std::string sha1hex(const std::string& text)
{
	string result;
	unsigned char sha1[20];
	char sha1_str[41];

	SHA_CTX shactx;
	SHA1_Init(&shactx);
	SHA1_Update(&shactx, text.c_str(), text.length());
	SHA1_Final(sha1, &shactx);

	size_t i;
	for (i = 0; i < 20; i++) {
		snprintf(&sha1_str[i*2],3,"%02x", sha1[i]);
	}
	result = sha1_str;
	return result;
}


std::string sha1_iOS_file(const std::string& domain, const std::string& file)
{
	return sha1hex(domain + "-" + file);
}


/* Parses the command-line arguments,
   The function always succeeds, or terminates on error.*/
void parse_command_line(int argc, char* argv[])
{
	int c,i;
	while ( (c=getopt_long(argc,argv,"h",mbdb_options,&i)) != -1) {
		switch (c)
		{
		case 'h':
			show_help();
			exit(0);
			break;
		default:
			break;
		}
	}

	/* We require at least three extra parameters: DIR UDID CMD */
	if ( optind + 1 > argc )
		usage_error("Missing required parameters (iOS Backup directory).\n");

	validate_idevice_bacup_directory(argv[optind]);
	backup_directory = argv[optind];
}

string sqlite3_get_text_column(sqlite3_stmt* stmt,int column)
{
	const unsigned char* ch = sqlite3_column_text(stmt,column);

	if (ch==NULL)
		return "";
	return (char*)ch;
}

ABPersonsVector load_ABPersons(sqlite3 *db)
{
	ABPersonsVector persons;
	sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,
"Select First,Last,Middle,Nickname,Prefix,Suffix," \
"       Organization,Department,Note, "\
"       Birthday,JobTitle,DisplayName,rowid from ABPerson"\
"       Order by rowid",-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 failed: %s", sqlite3_errmsg(db));

	/* Because SQLite3's ROW-ID starts with 1,
	   insert a dummy person in index 0 */
	ABPerson dummy;
	persons.push_back(dummy);

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		ABPerson p;
		p.First = sqlite3_get_text_column(stmt,0);
		p.Last = sqlite3_get_text_column(stmt,1);
		p.Middle = sqlite3_get_text_column(stmt,2);
		p.Nickname = sqlite3_get_text_column(stmt,3);
		p.Prefix = sqlite3_get_text_column(stmt,4);
		p.Suffix = sqlite3_get_text_column(stmt,5);
		p.Organization = sqlite3_get_text_column(stmt,6);
		p.Department = sqlite3_get_text_column(stmt,7);
		p.Note = sqlite3_get_text_column(stmt,8);
		p.Birthday = sqlite3_get_text_column(stmt,9);
		p.JobTitle = sqlite3_get_text_column(stmt,10);
		p.DisplayName = sqlite3_get_text_column(stmt,11);
		size_t rowid = p.rowid = (size_t)sqlite3_column_int(stmt,12);

		persons.push_back(p);

		/* Sanity Check:the index should be the same as row_id */
		if (persons.size()<=rowid)
			errx(1,"Internal error: ABPerson record rowid %zu is out of order (persons.size()==%zu)", rowid, persons.size());

		if (persons.at(rowid).rowid != rowid)
			errx(1,"Internal error: ABPerson record rowid %zu is out of order (persons.at(%zu).rowid=%zu)",rowid, rowid, persons.at(rowid).rowid);
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return persons;
}

ABMultiValueVector load_ABMultiValues(sqlite3 *db)
{
	ABMultiValueVector values;
	sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,
"select "\
" record_id, property, ABMultiValueLabel.value, ABMultiValue.value " \
"from ABMultiValue "\
"left outer join ABMultiValueLabel " \
"on ABMultiValue.label = ABMultiValueLabel.rowid " \
"order by ABMultiValue.record_id",-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 failed: %s", sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		ABMultiValue v;
		v.person_record_id = (size_t)sqlite3_column_int(stmt,0);
		v.property = (size_t)sqlite3_column_int(stmt,1);
		v.label = sqlite3_get_text_column(stmt,2);
		v.value = sqlite3_get_text_column(stmt,3);

		values.push_back(v);
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return values;
}

ABMultiValueVector load_ABMultiValueEntry(sqlite3 *db)
{
	ABMultiValueVector values;
	sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,
"Select parent_id as person_id, ABMultiValueEntry.key as key, " \
"ABMultiValueEntryKey.value as label, ABMultiValueEntryKey.value as value " \
"from ABMultiValueEntry " \
"Left outer join ABMultiValueEntryKey on " \
"ABMultiValueEntry.key = ABMultiValueEntryKey.rowid " \
"order by parent_id asc, key asc "
,-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 failed: %s", sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		ABMultiValue v;
		v.person_record_id = (size_t)sqlite3_column_int(stmt,0);
		v.property = (size_t)sqlite3_column_int(stmt,1);
		v.label = sqlite3_get_text_column(stmt,2);
		v.value = sqlite3_get_text_column(stmt,3);

		values.push_back(v);
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return values;
}



int load_iOS_addressbook(const std::string& directory)
{
	int i;
	sqlite3 *db=NULL;
	string addressbook_db_file = directory + "/" +
		sha1_iOS_file("HomeDomain","Library/AddressBook/AddressBook.sqlitedb");

	i = sqlite3_open_v2(addressbook_db_file.c_str(),&db,
			SQLITE_OPEN_READONLY,NULL);
	if (i!=SQLITE_OK) {
		if (db==NULL)
			errx(1,"sqlite3_open failed, memory error");
		errx(1,"sqlite3_open failed: %s", sqlite3_errmsg(db));
	}

	ABPersonsVector p = load_ABPersons(db);
	ABMultiValueVector phone_emails = load_ABMultiValues(db);
	ABMultiValueVector addresses = load_ABMultiValueEntry(db);

	i = sqlite3_close(db);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_close failed: %s", sqlite3_errmsg(db));

	return 0;
}

int main(int argc, char* argv[])
{
	parse_command_line(argc,argv);

	load_iOS_addressbook(backup_directory);

	return 0;
}

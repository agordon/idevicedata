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

#include <sqlite3.h>

#include "idd_common.h"
#include "AddressBookDatabase.h"
#include "AddressBookMemory.h"

using namespace std;

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

	validate_idevice_backup_directory(argv[optind]);
	backup_directory = argv[optind];
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

	ABPersonsVector db_persons = load_ABPersons(db);
	ABMultiValueVector db_contacts = load_ABMultiValues(db);
	ABMultiValueEntryVector db_addresses = load_ABMultiValueEntry(db);

	PersonVector v = BuildPersonVector(db_persons,db_contacts,db_addresses);

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

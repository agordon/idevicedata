/*
   C++ representation of iOS's AddressBook SQLite3 database.


   TODO: remove "errx" calls, replace with returning error-codes.
         (a library could should never terminate a program)
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>

#include <err.h>
#include <stdio.h>

#include <openssl/sha.h>

#include "idd_common.h"

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

void validate_idevice_backup_directory(const char* dir)
{
	if (!is_valid_directory(dir))
		errx(1,"error: '%s' is not a valid directory",dir);

	std::string f(dir);
	f += "/Manifest.mbdb";
	if (!is_valid_regular_file(f.c_str()))
		errx(1,"error: '%s' is not a valid iDevice backup " \
				"directory (can't find required file " \
				"'%s/Manifest.mbdb')", dir, dir);
}



std::string sqlite3_get_text_column(sqlite3_stmt* stmt,int column)
{
	const unsigned char* ch = sqlite3_column_text(stmt,column);

	if (ch==NULL)
		return "";
	return (char*)ch;
}


std::string sha1hex(const std::string& text)
{
	std::string result;
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

sqlite3* open_iOS_database(const std::string &directory,
			   const std::string &domain,
			   const std::string &db_file_name)
{
	int i;
	sqlite3 *db=NULL;
	std::string hashed_db_file = directory + "/" +
		sha1_iOS_file(domain,db_file_name);

	if (!is_valid_directory(directory.c_str()))
		errx(1,"Can't open database, backup directory (%s) is not a valid directory", directory.c_str());

	if (!is_valid_regular_file(hashed_db_file.c_str()))
		errx(1,"Can't open database (Domain = '%s', File = '%s'), " \
			"hashed	file does not exist (%s)",
			domain.c_str(), db_file_name.c_str(),
			hashed_db_file.c_str());

	i = sqlite3_open_v2(hashed_db_file.c_str(),&db,
			SQLITE_OPEN_READONLY,NULL);
	if (i!=SQLITE_OK) {
		if (db==NULL)
			errx(1,"sqlite3_open(%s) failed, memory error",
				hashed_db_file.c_str());
		errx(1,"sqlite3_open(%s) failed: %s",
				hashed_db_file.c_str(), sqlite3_errmsg(db));
	}

	return db;
}

void close_iOS_database(sqlite3 **db)
{
	if (db == NULL)
		return ;
	if (*db == NULL)
		return ;

	int i = sqlite3_close(*db);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_close failed: %s", sqlite3_errmsg(*db));
	*db = NULL ;
}

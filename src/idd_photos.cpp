#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <climits>

#include <err.h>
#include <getopt.h>

#include <sqlite3.h>

#include "common/idd_common.h"

#include "Photos/GenericAssetRecord.h"
#include "Photos/GenericAssetRecordLoader.h"

using namespace std;

/* Global Variables */
bool dump_generic_assets = false;
std::string program_name;
std::string backup_directory;

/* Command line options */
enum {
	OPTION_DUMP_GENERIC_ASSETS = CHAR_MAX+1,
};
const struct option mbdb_options[] = {
	{"help",	no_argument,	0,	'h'},
	{"dump-generic-assets",  no_argument,	0,	OPTION_DUMP_GENERIC_ASSETS},
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
"Messages - iOS Messages parser\n" \
"\n" \
"Usage: " << program_name << " [OPTIONS] DIR\n" \
"\n" \
"Required Parameters:\n" \
"  DIR  - The backup directory\n" \
"         (created with idevicebackup2)\n"\
"\n" \
"Options:\n" \
"  --help    -  Show this help screen.\n" \
"  --dump-generic-assets - (debug) dump 'ZGENERICASSET' table\n" \
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
		case OPTION_DUMP_GENERIC_ASSETS:
			dump_generic_assets = true;
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

int main(int argc, char* argv[])
{
	parse_command_line(argc,argv);

	sqlite3 *db = open_iOS_database(backup_directory,
					"CameraRollDomain",
					"Media/PhotoData/Photos.sqlite");
	if (dump_generic_assets) {
		GenericAssetRecords a = LoadGenericAssetRecords(db);
		cout << a;
	}

	close_iOS_database(&db);
	return 0;
}

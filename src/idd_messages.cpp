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

#include "Messages/ChatRecord.h"
#include "Messages/ChatRecordLoader.h"
#include "Messages/HandleRecord.h"
#include "Messages/HandleRecordLoader.h"
#include "Messages/MessageRecord.h"
#include "Messages/MessageRecordLoader.h"

using namespace std;

/* Global Variables */
bool dump_chats = false;
bool dump_handles = false;
bool dump_messages = false;
bool dump_last_messages = false;
bool dump_chat_30_messages = false;
std::string program_name;
std::string backup_directory;

/* Command line options */
enum {
	OPTION_DUMP_CHATS = CHAR_MAX+1,
	OPTION_DUMP_HANDLES,
	OPTION_DUMP_MESSAGES,
	OPTION_DUMP_LAST_MESSAGES,
	OPTION_DUMP_CHAT_30_MESSAGES,
};
const struct option mbdb_options[] = {
	{"help",	no_argument,	0,	'h'},
	{"dump-chats",  no_argument,	0,	OPTION_DUMP_CHATS},
	{"dump-handles",no_argument,	0,	OPTION_DUMP_HANDLES},
	{"dump-messages",no_argument,	0,	OPTION_DUMP_MESSAGES},
	{"dump-last-messages",no_argument,0,	OPTION_DUMP_LAST_MESSAGES},
	{"dump-chat-30-messages",no_argument,0,	OPTION_DUMP_CHAT_30_MESSAGES},
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
"  --dump-chats - (debug) dump 'chat' table\n" \
"  --dump-handles - (debug) dump 'handle' table\n" \
"  --dump-messages - (debug) dump 'message' table\n" \
"  --dump-last-messages - (debug) dump the last 'message' for each 'chat'\n" \
"  --dump-chat-30-messages - (debug) dump the last 30 messages for each 'chat'\n" \
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
		case OPTION_DUMP_CHATS:
			dump_chats = true;
			break;
		case OPTION_DUMP_HANDLES:
			dump_handles = true;
			break;
		case OPTION_DUMP_MESSAGES:
			dump_messages = true;
			break;
		case OPTION_DUMP_LAST_MESSAGES:
			dump_last_messages = true;
			break;
		case OPTION_DUMP_CHAT_30_MESSAGES:
			dump_chat_30_messages = true;
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
					"HomeDomain",
					"Library/SMS/sms.db");
	if (dump_chats) {
		chatRecords a;
		a = LoadchatRecords(db);
		cout << a;
	}

	if (dump_handles) {
		handleRecords h = LoadhandleRecords(db);
		cout << h;
	}

	if (dump_messages) {
		messageRecords h = LoadAllMessageRecords(db);
		cout << h;
	}
	if (dump_last_messages) {
		messageRecords h = LoadLastMessageRecords(db);
		cout << h;
	}
	if (dump_chat_30_messages) {
		chatRecords a = LoadchatRecords(db);
		for (auto chat_itr = a.begin();
				chat_itr != a.end(); ++chat_itr) {
			const chatRecord& chat = chat_itr->second;
			const int64_t chat_id = chat._row_id;
			messageRecords h = LoadChatMessageRecords(db,
					chat_id,30);

			cout << chat << endl
				<< h << endl;
		}
	}

	close_iOS_database(&db);
	return 0;
}

/*
	C++ Wrapper for SQLite table chat
*/
#ifndef __chat_Record_H__
#define __chat_Record_H__


#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>

class chatRecord
{

public:
	int64_t _row_id;
	int64_t ROWID;
	std::string guid;
	int64_t style;
	int64_t state;
	std::string account_id;
	std::vector<unsigned char> properties;
	std::string chat_identifier;
	std::string service_name;
	std::string room_name;
	std::string account_login;
	int64_t is_archived;
	std::string last_addressed_handle;

public:

	chatRecord() :
		_row_id(0),
		ROWID(0),
		style(0),
		state(0),
		is_archived(0)
	{}
	virtual ~chatRecord() { }
};

/* Debug Helper to dump chatRecord to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const chatRecord& r)
{
	strm << "chatRecord = ("
		<< "    _row_id = " << r._row_id << std::endl
		<< "    ROWID = " << r.ROWID << std::endl
		<< "    guid = \"" << r.guid << "\"" << std::endl
		<< "    style = " << r.style << std::endl
		<< "    state = " << r.state << std::endl
		<< "    account_id = \"" << r.account_id << "\"" << std::endl
		<< "    properties = (BLOB, not printed)" << std::endl
		<< "    chat_identifier = \"" << r.chat_identifier << "\"" << std::endl
		<< "    service_name = \"" << r.service_name << "\"" << std::endl
		<< "    room_name = \"" << r.room_name << "\"" << std::endl
		<< "    account_login = \"" << r.account_login << "\"" << std::endl
		<< "    is_archived = " << r.is_archived << std::endl
		<< "    last_addressed_handle = \"" << r.last_addressed_handle << "\"" << std::endl
		<< ")" << std::endl;
	return strm;
}


#endif

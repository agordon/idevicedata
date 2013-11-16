/*
	C++ Wrapper for SQLite table message
*/
#ifndef __message_Record_H__
#define __message_Record_H__


#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>

class messageRecord
{

public:
	int64_t _row_id;
	int64_t ROWID;
	std::string guid;
	std::string text;
	int64_t replace;
	std::string service_center;
	int64_t handle_id;
	std::string subject;
	std::string country;
	std::vector<unsigned char> attributedBody;
	int64_t version;
	int64_t type;
	std::string service;
	std::string account;
	std::string account_guid;
	int64_t error;
	int64_t date;
	int64_t date_read;
	int64_t date_delivered;
	int64_t is_delivered;
	int64_t is_finished;
	int64_t is_emote;
	int64_t is_from_me;
	int64_t is_empty;
	int64_t is_delayed;
	int64_t is_auto_reply;
	int64_t is_prepared;
	int64_t is_read;
	int64_t is_system_message;
	int64_t is_sent;
	int64_t has_dd_results;
	int64_t is_service_message;
	int64_t is_forward;
	int64_t was_downgraded;
	int64_t is_archive;
	int64_t cache_has_attachments;
	std::string cache_roomnames;
	int64_t was_data_detected;
	int64_t was_deduplicated;

public:

	messageRecord() :
		_row_id(0),
		ROWID(0),
		replace(0),
		handle_id(0),
		version(0),
		type(0),
		error(0),
		date(0),
		date_read(0),
		date_delivered(0),
		is_delivered(0),
		is_finished(0),
		is_emote(0),
		is_from_me(0),
		is_empty(0),
		is_delayed(0),
		is_auto_reply(0),
		is_prepared(0),
		is_read(0),
		is_system_message(0),
		is_sent(0),
		has_dd_results(0),
		is_service_message(0),
		is_forward(0),
		was_downgraded(0),
		is_archive(0),
		cache_has_attachments(0),
		was_data_detected(0),
		was_deduplicated(0)
	{}
	virtual ~messageRecord() { }
};

/* Debug Helper to dump messageRecord to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const messageRecord& r)
{
	strm << "messageRecord = ("
		<< "    _row_id = " << r._row_id << std::endl
		<< "    ROWID = " << r.ROWID << std::endl
		<< "    guid = \"" << r.guid << "\"" << std::endl
		<< "    text = \"" << r.text << "\"" << std::endl
		<< "    replace = " << r.replace << std::endl
		<< "    service_center = \"" << r.service_center << "\"" << std::endl
		<< "    handle_id = " << r.handle_id << std::endl
		<< "    subject = \"" << r.subject << "\"" << std::endl
		<< "    country = \"" << r.country << "\"" << std::endl
		<< "    attributedBody = (BLOB, not printed)" << std::endl
		<< "    version = " << r.version << std::endl
		<< "    type = " << r.type << std::endl
		<< "    service = \"" << r.service << "\"" << std::endl
		<< "    account = \"" << r.account << "\"" << std::endl
		<< "    account_guid = \"" << r.account_guid << "\"" << std::endl
		<< "    error = " << r.error << std::endl
		<< "    date = " << r.date << std::endl
		<< "    date_read = " << r.date_read << std::endl
		<< "    date_delivered = " << r.date_delivered << std::endl
		<< "    is_delivered = " << r.is_delivered << std::endl
		<< "    is_finished = " << r.is_finished << std::endl
		<< "    is_emote = " << r.is_emote << std::endl
		<< "    is_from_me = " << r.is_from_me << std::endl
		<< "    is_empty = " << r.is_empty << std::endl
		<< "    is_delayed = " << r.is_delayed << std::endl
		<< "    is_auto_reply = " << r.is_auto_reply << std::endl
		<< "    is_prepared = " << r.is_prepared << std::endl
		<< "    is_read = " << r.is_read << std::endl
		<< "    is_system_message = " << r.is_system_message << std::endl
		<< "    is_sent = " << r.is_sent << std::endl
		<< "    has_dd_results = " << r.has_dd_results << std::endl
		<< "    is_service_message = " << r.is_service_message << std::endl
		<< "    is_forward = " << r.is_forward << std::endl
		<< "    was_downgraded = " << r.was_downgraded << std::endl
		<< "    is_archive = " << r.is_archive << std::endl
		<< "    cache_has_attachments = " << r.cache_has_attachments << std::endl
		<< "    cache_roomnames = \"" << r.cache_roomnames << "\"" << std::endl
		<< "    was_data_detected = " << r.was_data_detected << std::endl
		<< "    was_deduplicated = " << r.was_deduplicated << std::endl
		<< ")" << std::endl;
	return strm;
}


#endif

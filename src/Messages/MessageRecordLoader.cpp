#include <algorithm>
#include <iostream>
#include <cstring>
#include <err.h>
#include <sstream>
#include "idd_common.h"
#include "MessageRecord.h"
#include "MessageRecordLoader.h"

using namespace std;


/* Internal function: loads the messages from 'message' table,
   Based on the given query.
*/
messageRecords LoadMessageRecords(sqlite3 *db,
				  const std::string& query)\
{
	messageRecords data;
        sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,query.c_str(),-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 (%s) failed: %s",
			query.c_str(), sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		messageRecord p;
		p._row_id = (size_t)sqlite3_column_int(stmt,0);
		p.ROWID = p._row_id;
		p.guid = sqlite3_get_text_column(stmt,1);
		p.text = sqlite3_get_text_column(stmt,2);
		p.replace = sqlite3_column_int(stmt,3);
		p.service_center = sqlite3_get_text_column(stmt,4);
		p.handle_id = sqlite3_column_int(stmt,5);
		p.subject = sqlite3_get_text_column(stmt,6);
		p.country = sqlite3_get_text_column(stmt,7);
		const void* ptr = sqlite3_column_blob(stmt,8);
		size_t blob_size = sqlite3_column_bytes(stmt,8);
		p.attributedBody.resize(blob_size);
		memcpy(p.attributedBody.data(),ptr,blob_size);
		p.version = sqlite3_column_int(stmt,9);
		p.type = sqlite3_column_int(stmt,10);
		p.service = sqlite3_get_text_column(stmt,11);
		p.account = sqlite3_get_text_column(stmt,12);
		p.account_guid = sqlite3_get_text_column(stmt,13);
		p.error = sqlite3_column_int(stmt,14);
		p.date = sqlite3_column_int(stmt,15);
		p.date_read = sqlite3_column_int(stmt,16);
		p.date_delivered = sqlite3_column_int(stmt,17);
		p.is_delivered = sqlite3_column_int(stmt,18);
		p.is_finished = sqlite3_column_int(stmt,19);
		p.is_emote = sqlite3_column_int(stmt,20);
		p.is_from_me = sqlite3_column_int(stmt,21);
		p.is_empty = sqlite3_column_int(stmt,22);
		p.is_delayed = sqlite3_column_int(stmt,23);
		p.is_auto_reply = sqlite3_column_int(stmt,24);
		p.is_prepared = sqlite3_column_int(stmt,25);
		p.is_read = sqlite3_column_int(stmt,26);
		p.is_system_message = sqlite3_column_int(stmt,27);
		p.is_sent = sqlite3_column_int(stmt,28);
		p.has_dd_results = sqlite3_column_int(stmt,29);
		p.is_service_message = sqlite3_column_int(stmt,30);
		p.is_forward = sqlite3_column_int(stmt,31);
		p.was_downgraded = sqlite3_column_int(stmt,32);
		p.is_archive = sqlite3_column_int(stmt,33);
		p.cache_has_attachments = sqlite3_column_int(stmt,34);
		p.cache_roomnames = sqlite3_get_text_column(stmt,35);
		p.was_data_detected = sqlite3_column_int(stmt,36);
		p.was_deduplicated = sqlite3_column_int(stmt,37);

		data.insert(pair<size_t,messageRecord>(p._row_id,p));
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return data;
}

messageRecords LoadLastMessageRecords(sqlite3 *db)
{
	const char *query =
		"Select " \
		  "message.ROWID, " \
		  "guid, " \
		  "text, " \
		  "replace, " \
		  "service_center, " \
		  "handle_id, " \
		  "subject, " \
		  "country, " \
		  "attributedBody, " \
		  "version, " \
		  "type, " \
		  "service, " \
		  "account, " \
		  "account_guid, " \
		  "error, " \
		  "date, " \
		  "date_read, " \
		  "date_delivered, " \
		  "is_delivered, " \
		  "is_finished, " \
		  "is_emote, " \
		  "is_from_me, " \
		  "is_empty, " \
		  "is_delayed, " \
		  "is_auto_reply, " \
		  "is_prepared, " \
		  "is_read, " \
		  "is_system_message, " \
		  "is_sent, " \
		  "has_dd_results, " \
		  "is_service_message, " \
		  "is_forward, " \
		  "was_downgraded, " \
		  "is_archive, " \
		  "cache_has_attachments, " \
		  "cache_roomnames, " \
		  "was_data_detected, " \
		  "was_deduplicated " \
		"FROM " \
		"  message "
		"JOIN " \
		"  (SELECT " \
		"      max(message_id) as last_message_id " \
		"   FROM " \
		"      chat_message_join " \
		"   GROUP BY " \
		"      chat_id) " \
		"ON message.ROWID = last_message_id" ;

	return LoadMessageRecords(db, query);
}

messageRecords LoadAllMessageRecords(sqlite3 *db)
{
	const char *query =
		"Select " \
		  "ROWID, " \
		  "guid, " \
		  "text, " \
		  "replace, " \
		  "service_center, " \
		  "handle_id, " \
		  "subject, " \
		  "country, " \
		  "attributedBody, " \
		  "version, " \
		  "type, " \
		  "service, " \
		  "account, " \
		  "account_guid, " \
		  "error, " \
		  "date, " \
		  "date_read, " \
		  "date_delivered, " \
		  "is_delivered, " \
		  "is_finished, " \
		  "is_emote, " \
		  "is_from_me, " \
		  "is_empty, " \
		  "is_delayed, " \
		  "is_auto_reply, " \
		  "is_prepared, " \
		  "is_read, " \
		  "is_system_message, " \
		  "is_sent, " \
		  "has_dd_results, " \
		  "is_service_message, " \
		  "is_forward, " \
		  "was_downgraded, " \
		  "is_archive, " \
		  "cache_has_attachments, " \
		  "cache_roomnames, " \
		  "was_data_detected, " \
		  "was_deduplicated " \
		"FROM " \
		"  message ";

	return LoadMessageRecords(db, query);
}

messageRecords LoadChatMessageRecords(sqlite3 *db,
					int64_t chat_id,
					int64_t limit)
{
	std::stringstream query;
	query <<
		"Select " \
		  "message.ROWID, " \
		  "guid, " \
		  "text, " \
		  "replace, " \
		  "service_center, " \
		  "handle_id, " \
		  "subject, " \
		  "country, " \
		  "attributedBody, " \
		  "version, " \
		  "type, " \
		  "service, " \
		  "account, " \
		  "account_guid, " \
		  "error, " \
		  "date, " \
		  "date_read, " \
		  "date_delivered, " \
		  "is_delivered, " \
		  "is_finished, " \
		  "is_emote, " \
		  "is_from_me, " \
		  "is_empty, " \
		  "is_delayed, " \
		  "is_auto_reply, " \
		  "is_prepared, " \
		  "is_read, " \
		  "is_system_message, " \
		  "is_sent, " \
		  "has_dd_results, " \
		  "is_service_message, " \
		  "is_forward, " \
		  "was_downgraded, " \
		  "is_archive, " \
		  "cache_has_attachments, " \
		  "cache_roomnames, " \
		  "was_data_detected, " \
		  "was_deduplicated " \
		"FROM " \
		"  message "
		"JOIN " \
		"  (SELECT " \
		"      message_id" \
		"   FROM " \
		"      chat_message_join " \
		"   WHERE " \
		"      chat_id = " << chat_id << \
		"   ORDER BY " \
		"      message_id DESC ";
	if (limit>0)
		query << " LIMIT " << limit ;
	query <<"    )" \
		"ON message.ROWID = message_id" ;

	return LoadMessageRecords(db, query.str());
}

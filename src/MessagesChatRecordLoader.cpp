#include <algorithm>
#include <iostream>
#include <cstring>
#include <err.h>
#include "idd_common.h"
#include "MessagesChatRecord.h"
#include "MessagesChatRecordLoader.h"

using namespace std;

chatRecords LoadchatRecords(sqlite3 *db)
{
	chatRecords data;
	sqlite3_stmt *stmt=NULL;


	const char *query =
		"Select " \
		  "ROWID, " \
		  "ROWID, " \
		  "guid, " \
		  "style, " \
		  "state, " \
		  "account_id, " \
		  "properties, " \
		  "chat_identifier, " \
		  "service_name, " \
		  "room_name, " \
		  "account_login, " \
		  "is_archived, " \
		  "last_addressed_handle " \
		"FROM " \
		"  chat ";

	int i = sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 (%s) failed: %s",
			query, sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		chatRecord p;
		p._row_id = (size_t)sqlite3_column_int(stmt,1);
		p.ROWID = sqlite3_column_int(stmt,2);
		p.guid = sqlite3_get_text_column(stmt,3);
		p.style = sqlite3_column_int(stmt,4);
		p.state = sqlite3_column_int(stmt,5);
		p.account_id = sqlite3_get_text_column(stmt,6);
		const void* ptr = sqlite3_column_blob(stmt,7);
		size_t blob_size = sqlite3_column_bytes(stmt,7);
		p.properties.resize(blob_size);
		memcpy(p.properties.data(),ptr,blob_size);
		p.chat_identifier = sqlite3_get_text_column(stmt,8);
		p.service_name = sqlite3_get_text_column(stmt,9);
		p.room_name = sqlite3_get_text_column(stmt,10);
		p.account_login = sqlite3_get_text_column(stmt,11);
		p.is_archived = sqlite3_column_int(stmt,12);
		p.last_addressed_handle = sqlite3_get_text_column(stmt,13);

		data.insert(pair<size_t,chatRecord>(p._row_id,p));
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return data;
}

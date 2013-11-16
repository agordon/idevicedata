#include <algorithm>
#include <iostream>
#include <cstring>
#include <err.h>
#include "idd_common.h"
#include "HandleRecord.h"
#include "HandleRecordLoader.h"

using namespace std;

handleRecords LoadhandleRecords(sqlite3 *db)
{
	handleRecords data;
        sqlite3_stmt *stmt=NULL;

	const char *query =
		"Select " \
		  "ROWID, " \
		  "ROWID, " \
		  "id, " \
		  "country, " \
		  "service, " \
		  "uncanonicalized_id " \
		"FROM " \
		"  handle ";

	int i = sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 (%s) failed: %s",
			query, sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		handleRecord p;
		p._row_id = (size_t)sqlite3_column_int(stmt,0);
		p.ROWID = sqlite3_column_int(stmt,1);
		p.id = sqlite3_get_text_column(stmt,2);
		p.country = sqlite3_get_text_column(stmt,3);
		p.service = sqlite3_get_text_column(stmt,4);
		p.uncanonicalized_id = sqlite3_get_text_column(stmt,5);

		data.insert(pair<size_t,handleRecord>(p._row_id,p));
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return data;
}


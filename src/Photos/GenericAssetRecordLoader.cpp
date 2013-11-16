#include <algorithm>
#include <iostream>
#include <cstring>
#include <err.h>
#include <time.h>
#include "idd_common.h"
#include "GenericAssetRecord.h"
#include "GenericAssetRecordLoader.h"

using namespace std;

GenericAssetRecords LoadGenericAssetRecords(sqlite3 *db)
{
	GenericAssetRecords data;
        sqlite3_stmt *stmt=NULL;

	const char *query =
		"Select " \
		  "ROWID, " \
		  "Z_PK, " \
		  "Z_ENT, " \
		  "Z_OPT, " \
		  "ZCLOUDHASCOMMENTSBYME, " \
		  "ZCLOUDHASCOMMENTSCONVERSATION, " \
		  "ZCLOUDHASUNSEENCOMMENTS, " \
		  "ZCLOUDPLACEHOLDERKIND, " \
		  "ZCOMPLETE, " \
		  "ZFLAGGED, " \
		  "ZHEIGHT, " \
		  "ZKIND, " \
		  "ZORIENTATION, " \
		  "ZSAVEDASSETTYPE, " \
		  "ZTHUMBNAILINDEX, " \
		  "ZWIDTH, " \
		  "ZADDITIONALATTRIBUTES, " \
		  "STRFTIME('%s',ZCLOUDBATCHPUBLISHDATE+978307200,'unixepoch','localtime'), " \
		  "STRFTIME('%s',ZCLOUDLASTVIEWEDCOMMENTDATE+978307200,'unixepoch','localtime'), " \
		  "STRFTIME('%s',ZDATECREATED+978307200,'unixepoch','localtime'), " \
		  "STRFTIME('%s',ZMODIFICATIONDATE+978307200,'unixepoch','localtime'), " \
		  "ZSORTTOKEN, " \
		  "ZCLOUDASSETGUID, " \
		  "ZCLOUDASSETKIND, " \
		  "ZCLOUDBATCHID, " \
		  "ZCLOUDCOLLECTIONGUID, " \
		  "ZDIRECTORY, " \
		  "ZFILENAME, " \
		  "ZTITLE, " \
		  "ZUNIFORMTYPEIDENTIFIER, " \
		  "ZCLOUDMETADATA, " \
		  "ZFACERECTANGLES, " \
		  "ZUUID, " \
		  "ZLOCATIONDATA, " \
		  "ZIMAGEURLDATA, " \
		  "ZTHUMBNAILURLDATA " \
		"FROM " \
		"  ZGENERICASSET ";

	int i = sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 (%s) failed: %s",
			query, sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		GenericAssetRecord p;
		p._row_id = (size_t)sqlite3_column_int(stmt,0);
		p.Z_PK = sqlite3_column_int(stmt,1);
		p.Z_ENT = sqlite3_column_int(stmt,2);
		p.Z_OPT = sqlite3_column_int(stmt,3);
		p.ZCLOUDHASCOMMENTSBYME = sqlite3_column_int(stmt,4);
		p.ZCLOUDHASCOMMENTSCONVERSATION = sqlite3_column_int(stmt,5);
		p.ZCLOUDHASUNSEENCOMMENTS = sqlite3_column_int(stmt,6);
		p.ZCLOUDPLACEHOLDERKIND = sqlite3_column_int(stmt,7);
		p.ZCOMPLETE = sqlite3_column_int(stmt,8);
		p.ZFLAGGED = sqlite3_column_int(stmt,9);
		p.ZHEIGHT = sqlite3_column_int(stmt,10);
		p.ZKIND = sqlite3_column_int(stmt,11);
		p.ZORIENTATION = sqlite3_column_int(stmt,12);
		p.ZSAVEDASSETTYPE = sqlite3_column_int(stmt,13);
		p.ZTHUMBNAILINDEX = sqlite3_column_int(stmt,14);
		p.ZWIDTH = sqlite3_column_int(stmt,15);
		p.ZADDITIONALATTRIBUTES = sqlite3_column_int(stmt,16);
		p.ZCLOUDBATCHPUBLISHDATE = sqlite3_column_int(stmt, 17) ;
		localtime_r(&p.ZCLOUDBATCHPUBLISHDATE, &p.ZCLOUDBATCHPUBLISHDATE_tm);
		char tmptime[100];
		strftime(tmptime,sizeof(tmptime),"%Y-%m-%d %H:%M:%S",&p.ZCLOUDBATCHPUBLISHDATE_tm);
		p.ZCLOUDBATCHPUBLISHDATE_str = tmptime;
		p.ZCLOUDLASTVIEWEDCOMMENTDATE = sqlite3_column_int(stmt, 18) ;
		localtime_r(&p.ZCLOUDLASTVIEWEDCOMMENTDATE, &p.ZCLOUDLASTVIEWEDCOMMENTDATE_tm);
		strftime(tmptime,sizeof(tmptime),"%Y-%m-%d %H:%M:%S",&p.ZCLOUDLASTVIEWEDCOMMENTDATE_tm);
		p.ZCLOUDLASTVIEWEDCOMMENTDATE_str = tmptime;
		p.ZDATECREATED = sqlite3_column_int(stmt, 19) ;
		localtime_r(&p.ZDATECREATED, &p.ZDATECREATED_tm);
		strftime(tmptime,sizeof(tmptime),"%Y-%m-%d %H:%M:%S",&p.ZDATECREATED_tm);
		p.ZDATECREATED_str = tmptime;
		p.ZMODIFICATIONDATE = sqlite3_column_int(stmt, 20) ;
		localtime_r(&p.ZMODIFICATIONDATE, &p.ZMODIFICATIONDATE_tm);
		strftime(tmptime,sizeof(tmptime),"%Y-%m-%d %H:%M:%S",&p.ZMODIFICATIONDATE_tm);
		p.ZMODIFICATIONDATE_str = tmptime;
		p.ZSORTTOKEN = sqlite3_column_double(stmt,21);
		p.ZCLOUDASSETGUID = sqlite3_get_text_column(stmt,22);
		p.ZCLOUDASSETKIND = sqlite3_get_text_column(stmt,23);
		p.ZCLOUDBATCHID = sqlite3_get_text_column(stmt,24);
		p.ZCLOUDCOLLECTIONGUID = sqlite3_get_text_column(stmt,25);
		p.ZDIRECTORY = sqlite3_get_text_column(stmt,26);
		p.ZFILENAME = sqlite3_get_text_column(stmt,27);
		p.ZTITLE = sqlite3_get_text_column(stmt,28);
		p.ZUNIFORMTYPEIDENTIFIER = sqlite3_get_text_column(stmt,29);
		const void* ptr = sqlite3_column_blob(stmt,30);
		size_t blob_size = sqlite3_column_bytes(stmt,30);
		p.ZCLOUDMETADATA.resize(blob_size);
		memcpy(p.ZCLOUDMETADATA.data(),ptr,blob_size);
		ptr = sqlite3_column_blob(stmt,31);
		blob_size = sqlite3_column_bytes(stmt,31);
		p.ZFACERECTANGLES.resize(blob_size);
		memcpy(p.ZFACERECTANGLES.data(),ptr,blob_size);
		ptr = sqlite3_column_blob(stmt,32);
		blob_size = sqlite3_column_bytes(stmt,32);
		p.ZUUID.resize(blob_size);
		memcpy(p.ZUUID.data(),ptr,blob_size);
		ptr = sqlite3_column_blob(stmt,33);
		blob_size = sqlite3_column_bytes(stmt,33);
		p.ZLOCATIONDATA.resize(blob_size);
		memcpy(p.ZLOCATIONDATA.data(),ptr,blob_size);
		ptr = sqlite3_column_blob(stmt,34);
		blob_size = sqlite3_column_bytes(stmt,34);
		p.ZIMAGEURLDATA.resize(blob_size);
		memcpy(p.ZIMAGEURLDATA.data(),ptr,blob_size);
		ptr = sqlite3_column_blob(stmt,35);
		blob_size = sqlite3_column_bytes(stmt,35);
		p.ZTHUMBNAILURLDATA.resize(blob_size);
		memcpy(p.ZTHUMBNAILURLDATA.data(),ptr,blob_size);

		data.insert(pair<size_t,GenericAssetRecord>(p._row_id,p));
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return data;
}


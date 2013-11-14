/*
	C++ Wrapper for SQLite table chat
	*/
#ifndef __chat_Record_Loader_H__
#define __chat_Record_Loader_H__

#include "MessagesChatRecord.h"

#include <unordered_map>
#include <ostream>
#include <sqlite3.h>

typedef std::unordered_map<int,chatRecord> chatRecords;

/* Debug Helper to dump All the records from chatRecords
   to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const chatRecords& r)
{
	strm << "chat table content =" << std::endl;
	for (auto i = r.begin(); i != r.end(); ++i) {
		strm << "-- record " << i->first << " --" << std::endl;
		strm << i->second;
		strm << "-- end record " << i->first << " -- " << std::endl;
	}
	return strm;
}

chatRecords LoadchatRecords(sqlite3 *db);

#endif


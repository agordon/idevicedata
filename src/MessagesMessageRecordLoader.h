/*
	C++ Wrapper for SQLite table message
*/
#ifndef __message_Record_Loader_H__
#define __message_Record_Loader_H__

#include "MessagesMessageRecord.h"

#include <unordered_map>
#include <ostream>
#include <sqlite3.h>

typedef std::unordered_map<int,messageRecord> messageRecords;

/* Debug Helper to dump All the records from messageRecords
   to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const messageRecords& r)
{
        strm << "message table content =" << std::endl;
	std::vector<messageRecords::key_type> keys;
	keys.reserve(r.size());
	for (auto i = r.begin(); i != r.end(); ++i)
		keys.push_back(i->first);
	std::sort(keys.begin(),keys.end());

	for (size_t i=0;i<keys.size();++i) {
		auto key = keys[i];
		auto v = r.find(key);
		strm << "-- record " << key << " --" << std::endl;
		strm << v->second;
		strm << "-- end record " << key << " -- " << std::endl;
	}
        return strm;
}

messageRecords LoadmessageRecords(sqlite3 *db);

#endif


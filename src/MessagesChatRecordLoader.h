/*
	C++ Wrapper for SQLite table chat
	*/
#ifndef __chat_Record_Loader_H__
#define __chat_Record_Loader_H__

#include "MessagesChatRecord.h"

#include <unordered_map>
#include <algorithm>
#include <ostream>
#include <sqlite3.h>
#include <vector>

typedef std::unordered_map<int,chatRecord> chatRecords;

/* Debug Helper to dump All the records from chatRecords
   to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const chatRecords& r)
{
	strm << "chat table content =" << std::endl;
	std::vector<chatRecords::key_type> keys;
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

chatRecords LoadchatRecords(sqlite3 *db);

#endif


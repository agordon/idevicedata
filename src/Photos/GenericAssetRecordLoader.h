/*
	C++ Wrapper for SQLite table ZGENERICASSET
*/
#ifndef __ZGENERICASSET_Record_Loader_H__
#define __ZGENERICASSET_Record_Loader_H__

#include "GenericAssetRecord.h"

#include <algorithm>
#include <unordered_map>
#include <ostream>
#include <sqlite3.h>

typedef std::unordered_map<int,GenericAssetRecord> GenericAssetRecords;

/* Debug Helper to dump All the records from ZGENERICASSETRecords
   to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const GenericAssetRecords& r)
{
        strm << "ZGENERICASSET table content =" << std::endl;
	std::vector<GenericAssetRecords::key_type> keys;
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

GenericAssetRecords LoadGenericAssetRecords(sqlite3 *db);

#endif

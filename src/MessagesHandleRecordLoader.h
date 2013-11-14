#ifndef __handle_Record_Loader_H__
#define __handle_Record_Loader_H__

#include "MessagesHandleRecord.h"

#include <unordered_map>
#include <ostream>
#include <sqlite3.h>

typedef std::unordered_map<int,handleRecord> handleRecords;

/* Debug Helper to dump All the records from handleRecords
   to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const handleRecords& r)
{
        strm << "handle table content =" << std::endl;
        for (auto i = r.begin(); i != r.end(); ++i) {
                strm << "-- record " << i->first << " --" << std::endl;
                strm << i->second;
                strm << "-- end record " << i->first << " -- " << std::endl;
        }
        return strm;
}

handleRecords LoadhandleRecords(sqlite3 *db);

#endif

/*
	C++ Wrapper for SQLite table handle
*/
#ifndef __handle_Record_H__
#define __handle_Record_H__


#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>

class handleRecord
{

public:
	int64_t _row_id;
	int64_t ROWID;
	std::string id;
	std::string country;
	std::string service;
	std::string uncanonicalized_id;

public:

	handleRecord() :
		_row_id(0),
		ROWID(0)
	{}
	virtual ~handleRecord() { }
};

/* Debug Helper to dump handleRecord to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const handleRecord& r)
{
	strm << "handleRecord = ("
		<< "    _row_id = " << r._row_id << std::endl
		<< "    ROWID = " << r.ROWID << std::endl
		<< "    id = \"" << r.id << "\"" << std::endl
		<< "    country = \"" << r.country << "\"" << std::endl
		<< "    service = \"" << r.service << "\"" << std::endl
		<< "    uncanonicalized_id = \"" << r.uncanonicalized_id << "\"" << std::endl
		<< ")" << std::endl;
	return strm;
}


#endif

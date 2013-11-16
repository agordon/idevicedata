/*
	C++ Wrapper for SQLite table ZGENERICASSET
*/
#ifndef __ZGENERICASSET_Record_H__
#define __ZGENERICASSET_Record_H__


#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>

class GenericAssetRecord
{

public:
	int64_t _row_id;
	int64_t Z_PK;
	int64_t Z_ENT;
	int64_t Z_OPT;
	int64_t ZCLOUDHASCOMMENTSBYME;
	int64_t ZCLOUDHASCOMMENTSCONVERSATION;
	int64_t ZCLOUDHASUNSEENCOMMENTS;
	int64_t ZCLOUDPLACEHOLDERKIND;
	int64_t ZCOMPLETE;
	int64_t ZFLAGGED;
	int64_t ZHEIGHT;
	int64_t ZKIND;
	int64_t ZORIENTATION;
	int64_t ZSAVEDASSETTYPE;
	int64_t ZTHUMBNAILINDEX;
	int64_t ZWIDTH;
	int64_t ZADDITIONALATTRIBUTES;
	time_t ZCLOUDBATCHPUBLISHDATE;
	struct tm ZCLOUDBATCHPUBLISHDATE_tm;
	std::string ZCLOUDBATCHPUBLISHDATE_str;
	time_t ZCLOUDLASTVIEWEDCOMMENTDATE;
	struct tm ZCLOUDLASTVIEWEDCOMMENTDATE_tm;
	std::string ZCLOUDLASTVIEWEDCOMMENTDATE_str;
	time_t ZDATECREATED;
	struct tm ZDATECREATED_tm;
	std::string ZDATECREATED_str;
	time_t ZMODIFICATIONDATE;
	struct tm ZMODIFICATIONDATE_tm;
	std::string ZMODIFICATIONDATE_str;
	double ZSORTTOKEN;
	std::string ZCLOUDASSETGUID;
	std::string ZCLOUDASSETKIND;
	std::string ZCLOUDBATCHID;
	std::string ZCLOUDCOLLECTIONGUID;
	std::string ZDIRECTORY;
	std::string ZFILENAME;
	std::string ZTITLE;
	std::string ZUNIFORMTYPEIDENTIFIER;
	std::vector<unsigned char> ZCLOUDMETADATA;
	std::vector<unsigned char> ZFACERECTANGLES;
	std::vector<unsigned char> ZUUID;
	std::vector<unsigned char> ZLOCATIONDATA;
	std::vector<unsigned char> ZIMAGEURLDATA;
	std::vector<unsigned char> ZTHUMBNAILURLDATA;

public:

	GenericAssetRecord() :
		_row_id(0),
		Z_PK(0),
		Z_ENT(0),
		Z_OPT(0),
		ZCLOUDHASCOMMENTSBYME(0),
		ZCLOUDHASCOMMENTSCONVERSATION(0),
		ZCLOUDHASUNSEENCOMMENTS(0),
		ZCLOUDPLACEHOLDERKIND(0),
		ZCOMPLETE(0),
		ZFLAGGED(0),
		ZHEIGHT(0),
		ZKIND(0),
		ZORIENTATION(0),
		ZSAVEDASSETTYPE(0),
		ZTHUMBNAILINDEX(0),
		ZWIDTH(0),
		ZADDITIONALATTRIBUTES(0),
		ZCLOUDBATCHPUBLISHDATE(0),
		ZCLOUDLASTVIEWEDCOMMENTDATE(0),
		ZDATECREATED(0),
		ZMODIFICATIONDATE(0),
		ZSORTTOKEN(0)
	{}
	virtual ~GenericAssetRecord() { }
};

/* Debug Helper to dump ZGENERICASSETRecord to an output stream (e.g. cerr) */
inline std::ostream& operator <<(std::ostream& strm, const GenericAssetRecord& r)
{
	strm << "GenericAssetRecord = ("
		<< "    _row_id = " << r._row_id << std::endl
		<< "    Z_PK = " << r.Z_PK << std::endl
		<< "    Z_ENT = " << r.Z_ENT << std::endl
		<< "    Z_OPT = " << r.Z_OPT << std::endl
		<< "    ZCLOUDHASCOMMENTSBYME = " << r.ZCLOUDHASCOMMENTSBYME << std::endl
		<< "    ZCLOUDHASCOMMENTSCONVERSATION = " << r.ZCLOUDHASCOMMENTSCONVERSATION << std::endl
		<< "    ZCLOUDHASUNSEENCOMMENTS = " << r.ZCLOUDHASUNSEENCOMMENTS << std::endl
		<< "    ZCLOUDPLACEHOLDERKIND = " << r.ZCLOUDPLACEHOLDERKIND << std::endl
		<< "    ZCOMPLETE = " << r.ZCOMPLETE << std::endl
		<< "    ZFLAGGED = " << r.ZFLAGGED << std::endl
		<< "    ZHEIGHT = " << r.ZHEIGHT << std::endl
		<< "    ZKIND = " << r.ZKIND << std::endl
		<< "    ZORIENTATION = " << r.ZORIENTATION << std::endl
		<< "    ZSAVEDASSETTYPE = " << r.ZSAVEDASSETTYPE << std::endl
		<< "    ZTHUMBNAILINDEX = " << r.ZTHUMBNAILINDEX << std::endl
		<< "    ZWIDTH = " << r.ZWIDTH << std::endl
		<< "    ZADDITIONALATTRIBUTES = " << r.ZADDITIONALATTRIBUTES << std::endl
		<< "    ZCLOUDBATCHPUBLISHDATE = " << r.ZCLOUDBATCHPUBLISHDATE_str << std::endl
		<< "    ZCLOUDLASTVIEWEDCOMMENTDATE = " << r.ZCLOUDLASTVIEWEDCOMMENTDATE_str << std::endl
		<< "    ZDATECREATED = " << r.ZDATECREATED_str << std::endl
		<< "    ZMODIFICATIONDATE = " << r.ZMODIFICATIONDATE_str << std::endl
		<< "    ZSORTTOKEN = " << r.ZSORTTOKEN << std::endl
		<< "    ZCLOUDASSETGUID = \"" << r.ZCLOUDASSETGUID << "\"" << std::endl
		<< "    ZCLOUDASSETKIND = \"" << r.ZCLOUDASSETKIND << "\"" << std::endl
		<< "    ZCLOUDBATCHID = \"" << r.ZCLOUDBATCHID << "\"" << std::endl
		<< "    ZCLOUDCOLLECTIONGUID = \"" << r.ZCLOUDCOLLECTIONGUID << "\"" << std::endl
		<< "    ZDIRECTORY = \"" << r.ZDIRECTORY << "\"" << std::endl
		<< "    ZFILENAME = \"" << r.ZFILENAME << "\"" << std::endl
		<< "    ZTITLE = \"" << r.ZTITLE << "\"" << std::endl
		<< "    ZUNIFORMTYPEIDENTIFIER = \"" << r.ZUNIFORMTYPEIDENTIFIER << "\"" << std::endl
		<< "    ZCLOUDMETADATA = (BLOB, not printed)" << std::endl
		<< "    ZFACERECTANGLES = (BLOB, not printed)" << std::endl
		<< "    ZUUID = (BLOB, not printed)" << std::endl
		<< "    ZLOCATIONDATA = (BLOB, not printed)" << std::endl
		<< "    ZIMAGEURLDATA = (BLOB, not printed)" << std::endl
		<< "    ZTHUMBNAILURLDATA = (BLOB, not printed)" << std::endl
		<< ")" << std::endl;
	return strm;
}


#endif

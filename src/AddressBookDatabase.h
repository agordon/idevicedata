/*
   C++ representation of iOS's AddressBook SQLite3 database.
 */
#ifndef _IOS_ADDRESSBOOK_DATABASE_H__
#define _IOS_ADDRESSBOOK_DATABASE_H__

#include <string>
#include <vector>
#include <sqlite3.h>

class ABMultiValue
{
public:
	size_t uid; /* The ID of this record, used witht the address's parent field */
	size_t person_record_id; /* The row-id from ABPersons */
	int property;
	int identifier;
	int label_num;
	std::string label; /* Could be empty, JOIN'd from ABMultiValueLabel */
	std::string value; /* Could be empty */

	ABMultiValue() : uid(0), person_record_id(0),
			property(0), identifier(0), label_num(0) { }

	/* Returns TRUE if this object is empty/uninitialized */
	bool empty() const
	{
		return uid==0
			&&
			person_record_id==0
			&&
			property==0
			&&
			identifier==0
			&&
			label_num==0
			&&
			label.empty()
			&&
			value.empty();
	}
};
typedef std::vector<ABMultiValue> ABMultiValueVector;

class ABMultiValueEntry
{
public:
	size_t parent_id; /* UID field in ABMultiValue */
	size_t key_num;
	std::string key; /* from ABMultiValueEntryKey */
	std::string value;

	ABMultiValueEntry() : parent_id(0), key_num(0) { }
};
typedef std::vector<ABMultiValueEntry> ABMultiValueEntryVector;

class ABPerson
{
public:
	size_t rowid;
	std::string First;
	std::string Last;
	std::string Middle;
	std::string Nickname;
	std::string Prefix;
	std::string Suffix;
	std::string Organization;
	std::string Department;
	std::string Note;
	std::string Birthday;
	std::string JobTitle;
	std::string DisplayName;

	ABPerson() : rowid(0) { }
};
typedef std::vector<ABPerson> ABPersonsVector;


ABPersonsVector load_ABPersons(sqlite3 *db);
ABMultiValueVector load_ABMultiValues(sqlite3 *db);
ABMultiValueEntryVector load_ABMultiValueEntry(sqlite3 *db);

#endif

/*

 */
#ifndef _IOS_ADDRESSBOOK_H__
#define _IOS_ADDRESSBOOK_H__

#include "AddressBookDatabase.h"
#include "AddressBookMemory.h"

class AddressBook
{
public:
	PersonVector persons;

	AddressBook() {}

	/* TODO: Implement, searching all records */
	const Person& Find(const std::string& str);
};
AddressBook LoadAddressBookDatabase(sqlite3 *db);

AddressBook LoadAddressBookDirectory(const std::string& directory);


#endif

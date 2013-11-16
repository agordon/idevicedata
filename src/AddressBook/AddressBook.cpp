/*
   
 */
#include <string>
#include <err.h>

#include "idd_common.h"

#include "AddressBook.h"

using namespace std;

AddressBook LoadAddressBookDatabase(sqlite3 *db)
{
	ABPersonsVector db_persons = load_ABPersons(db);
	ABMultiValueVector db_contacts = load_ABMultiValues(db);
	ABMultiValueEntryVector db_addresses = load_ABMultiValueEntry(db);

	AddressBook ab;
	ab.persons = BuildPersonVector(db_persons,db_contacts,db_addresses);

	return ab;
}

AddressBook LoadAddressBookDirectory(const std::string& directory)
{
	sqlite3 *db=NULL;

	db = open_iOS_database(directory,
				"HomeDomain",
				"Library/AddressBook/AddressBook.sqlitedb");

	AddressBook ab = LoadAddressBookDatabase(db);

	close_iOS_database(&db);

	return ab;
}

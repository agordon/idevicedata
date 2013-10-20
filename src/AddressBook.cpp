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
	int i;
	sqlite3 *db=NULL;
	string addressbook_db_file = directory + "/" +
		sha1_iOS_file("HomeDomain","Library/AddressBook/AddressBook.sqlitedb");

	i = sqlite3_open_v2(addressbook_db_file.c_str(),&db,
			SQLITE_OPEN_READONLY,NULL);
	if (i!=SQLITE_OK) {
		if (db==NULL)
			errx(1,"sqlite3_open failed, memory error");
		errx(1,"sqlite3_open failed: %s", sqlite3_errmsg(db));
	}

	AddressBook ab = LoadAddressBookDatabase(db);

	i = sqlite3_close(db);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_close failed: %s", sqlite3_errmsg(db));

	return ab;
}

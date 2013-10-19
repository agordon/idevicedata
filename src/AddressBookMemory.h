/*
   In-Memory, hierarchical (non-normalized) representation
   of iOS AddressBook database.
*/
#ifndef __ADDRESS_BOOK_MEMORY_H__
#define __ADDRESS_BOOK_MEMORY_H__

#include <string>
#include <vector>

#include "AddressBookDatabase.h"

/* A "single-value" contact information record:
   email, phone number, website */
class ContactInfo
{
public:
	std::string label;
	std::string value;

	/* Technical Info */
	size_t record_id;
};
typedef std::vector<ContactInfo> ContactInfoVector;

/* Physical Address of a Person */
class Address
{
public:
	std::string country;
	std::string street;
	std::string ZIP;
	std::string city;
	std::string country_code;
	std::string state;
	std::string sub_locality;
};
typedef std::vector<Address> AddressVector;

class Person
{
public:
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

	ContactInfoVector contacts;
	AddressVector addresses;

	Person() {}
};
typedef std::vector<Person> PersonVector;

PersonVector BuildPersonVector(const ABPersonsVector &db_person,
				const ABMultiValueVector &db_contacts,
				const ABMultiValueEntryVector &db_addresses);

#endif

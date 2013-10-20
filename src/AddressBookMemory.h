/*
   In-Memory, hierarchical (non-normalized) representation
   of iOS AddressBook database.
*/
#ifndef __ADDRESS_BOOK_MEMORY_H__
#define __ADDRESS_BOOK_MEMORY_H__

#include <string>
#include <vector>
#include <ostream>

#include "AddressBookDatabase.h"

/* A "single-value" contact information record:
   email, phone number, website */
class ContactInfo
{
public:
	size_t label_num;
	std::string label;
	std::string value;

	/* Technical Info */
	size_t rowid; /* in ABMultiValue table */
	size_t uid;   /* in ABMultiValue table */
};
typedef std::vector<ContactInfo> ContactInfoVector;

inline std::ostream& operator <<(std::ostream& stream, const ContactInfo& i)
{
	stream << "ContactInfo(label='" << i.label
		<< "', value='" << i.value
		<< "', uid=" << i.uid
		<< ", rowid=" << i.rowid
		<< ")" << std::endl;
	return stream;
}


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
	std::string type; /* Home, Work, Other */

	/* Technical Info */
	size_t parent_id; /* ForeignKey into ABMultiValue */
	size_t person_record_id; /* ForiegnKey into ABPerson */

	bool empty() const
	{
		return country.empty()
			&&
			street.empty()
			&&
			ZIP.empty()
			&&
			city.empty()
			&&
			country_code.empty()
			&&
			state.empty()
			&&
			sub_locality.empty();
	}
};
typedef std::vector<Address> AddressVector;

inline std::ostream& operator <<(std::ostream& stream, const Address& a)
{
	stream << "Address(type='" << a.type << "'):" << std::endl
		<< "  Street='" << a.street << std::endl
		<< "  city='" << a.city << std::endl
		<< "  zip=" << a.ZIP << std::endl
		<< "  country=" << a.country << std::endl
		<< "  country_code=" << a.country_code << std::endl
		<< "  sub_locality=" << a.sub_locality << std::endl
		<< "  parent_id=" << a.parent_id << std::endl
		<< "  person_id=" << a.person_record_id << std::endl;
	return stream;
}


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

	ContactInfoVector emails;
	ContactInfoVector phones;
	ContactInfoVector others; /* HomePage URL and OTHER */

	AddressVector addresses;

	/* Technical Information */
	size_t rowid; /* from ABPerson table */

	Person() {}

	std::string GetName() const;
	const std::string& GetSortKey() const;
	std::string GetEmail() const;
	std::string GetPhone() const;
};
typedef std::vector<Person> PersonVector;

inline std::ostream& operator <<(std::ostream& stream, const Person& p)
{
	stream << "Person (" << p.GetName() << "):" << std::endl
		<< "   First: '" << p.First << "'" << std::endl
		<< "   Middle: '" << p.Middle << "'" << std::endl
		<< "   Last: '" << p.Last << "'" << std::endl
		<< "   Nickname: '" << p.Nickname << "'" << std::endl
		<< "   Prefix: '" << p.Prefix << "'" << std::endl
		<< "   Suffix: '" << p.Suffix << "'" << std::endl
		<< "   Organization: '" << p.Organization << "'" << std::endl
		<< "   Deparetment: '" << p.Department << "'" << std::endl
		<< "   Birthday: '" << p.Birthday << "'" << std::endl
		<< "   JobTitle: '" << p.JobTitle << "'" << std::endl
		<< "   DisplayName: '" << p.DisplayName << "'" << std::endl
		<< "   Note: '" << p.Note << "'" << std::endl;
	for (size_t i=0;i<p.phones.size();++i)
		stream << p.phones.at(i);
	for (size_t i=0;i<p.emails.size();++i)
		stream << p.emails.at(i);
	for (size_t i=0;i<p.others.size();++i)
		stream << p.others.at(i);
	for (size_t i=0;i<p.addresses.size();++i)
		stream << p.addresses.at(i);
	return stream;
}

PersonVector BuildPersonVector(const ABPersonsVector &db_person,
				const ABMultiValueVector &db_contacts,
				const ABMultiValueEntryVector &db_addresses);

#endif

/*
   In-Memory, hierarchical (non-normalized) representation
   of iOS AddressBook database.
 */

#include <iostream>
#include <sstream>
#include <algorithm>

#include <err.h>

#include "AddressBookMemory.h"

using namespace std;

class PersonSorter
{
public:
	bool operator() (const Person& p1, const Person& p2) const
	{
		return p1.GetSortKey() < p2.GetSortKey();
	}
};

const std::string& Person::GetSortKey() const
{
	if (!Last.empty())
		return Last;
	if (!First.empty())
		return First;
	if (!Middle.empty())
		return Middle;

	/* If the name is still empty, try other fields */
	if (!Organization.empty())
		return Organization;
	if (!Department.empty())
		return Department;
	if (!JobTitle.empty())
		return JobTitle;
	if (!DisplayName.empty())
		return DisplayName;

	/* Can this happen? */
	return Last;
}

string Person::GetName() const
{
	string name;

	if (!First.empty())
		name = First;
	if (!Middle.empty()) {
		if (!name.empty())
			name += " ";
		name += Middle;
	}
	if (!Last.empty()) {
		if (!name.empty())
			name += " ";
		name += Last;
	}

	/* If the name is still empty, try other fields */
	if (name.empty() && !Organization.empty())
		name = Organization;
	if (name.empty() && !Department.empty())
		name = Department;
	if (name.empty() && !JobTitle.empty())
		name = JobTitle;
	if (name.empty() && !DisplayName.empty())
		name = DisplayName;

	/* Can this happen? */
	if (name.empty()) {
		cerr << "Warning: found ABPerson record without recognizable name (rowID = " << rowid << ")" << endl;
		stringstream ss;
		ss << "Uknown(RowID=" << rowid << ")";
		name = ss.str();
	}
	return name;
}

PersonVector BuildPersonVector(const ABPersonsVector & db_person,
				const ABMultiValueVector & db_contacts,
				const ABMultiValueEntryVector & db_addresses)
{
	size_t i;
	PersonVector db;

	/* Populate Persons vector - one element per contact */
	db.resize(db_person.size());
	for (i=1;i<db_person.size();++i) {
		Person &p = db.at(i);
		const ABPerson& abp = db_person.at(i);

		/* Populate person's values */
		p.rowid = abp.rowid;
		p.First = abp.First;
		p.Last = abp.Last;
		p.Middle = abp.Middle;
		p.Nickname = abp.Nickname;
		p.Prefix = abp.Prefix;
		p.Suffix = abp.Suffix;
		p.Organization = abp.Organization;
		p.Department = abp.Department;
		p.Note = abp.Note;
		p.Birthday = abp.Birthday;
		p.JobTitle = abp.JobTitle;
		p.DisplayName = abp.DisplayName;

		/* Sanity Check */
		if (abp.rowid != i)
			errx(1,"Internal error: BuildPersonVector: " \
				"ABPerson.rowid(%zu) doesn't match " \
				"index(i=%zu)", abp.rowid,i);
	}

	/* Add Contact Information for each person */
	for (i=1;i<db_contacts.size();++i) {
		const ABMultiValue &mv = db_contacts.at(i);

		/* We use a vector instead of a list,
		   with the UID as the index - so there are
		   possible gaps if a value was deleted - skip it.*/
		if (mv.empty())
			continue;

		ContactInfo ci;
		ci.rowid = mv.rowid;
		ci.uid   = mv.uid;
		ci.label_num = mv.label_num;
		ci.label = mv.label;
		ci.value = mv.value;

		/* Sanity Check */
		size_t person_index = mv.person_record_id;
		if (person_index>=db.size())
			errx(1,"Internal error: BuildPersonVector: "\
				"Found ABMultiValue(rowid=%zu, uid=%zu) "\
				"with invalid person_record_id(%zu) " \
				"[db.size()=%zu]",
				mv.rowid, mv.uid,person_index,db.size());

		/* This record has no direct value -
		   it is used as a ForigenKey with ABMultiValueEnty:
		   The ABMultiValueEntry points to this record,
		   and this record points to the Person's record. */
		if (ci.value.empty())
			continue;

		Person &p = db.at(person_index);
		
		/* Hard-coded options, based on ABMultiValueLabel table */
		if (ci.label == "Mobile"
		    ||
		    ci.label == "Home"
		    ||
		    ci.label == "Work"
		    ||
		    ci.label == "iPhone"
		    ||
		    ci.label == "WorkFAX"
		    ||
		    ci.label == "HomeFAX")
			p.phones.push_back(ci);
		else if (ci.label == "Email")
			p.emails.push_back(ci);
		else
			p.others.push_back(ci);
	}

	/* Collate Address Information into objects */
	AddressVector addrs;
	for (i=0;i<db_addresses.size();++i) {
		const ABMultiValueEntry &mve = db_addresses.at(i);
		size_t parent_id = mve.parent_id;

		if (parent_id >= db_contacts.size())
			errx(1,"Internal error: BuildPersonVector: "\
				"ABMultiValueEntry.parent_id(%zu) points "\
				"to non-existing ABMultiValue record",
				parent_id);
		const ABMultiValue &mv = db_contacts.at(parent_id);
		if (mv.empty())
			errx(1,"Internal error: BuildPersonVector: "\
				"ABMultiValueEntry.parent_id(%zu) points "\
				"to an empty ABMultiValue record",
				parent_id);

		if (addrs.size() <= parent_id)
			addrs.resize(parent_id+1);

		Address& ad = addrs.at(parent_id);
		ad.parent_id = parent_id;
		ad.person_record_id = mv.person_record_id;
		ad.type = mv.label;

		/* From ABMultiValueEntryKey Table */
		switch (mve.key_num)
		{
		case 1:
			ad.country = mve.value;
			break;
		case 2:
			ad.street = mve.value;
			break;
		case 3:
			ad.ZIP = mve.value;
			break;
		case 4:
			ad.city = mve.value;
			break;
		case 5:
			ad.country_code = mve.value;
			break;
		case 6:
			ad.state = mve.value;
			break;
		case 7:
			ad.sub_locality = mve.value;
			break;
		default:
			/* This should not happen */
			errx(1,"Internal Error: BuildPersonVector(): " \
				"Found unknown ABMultiValueEntryKey " \
				"(value = %zu , name = '%s')",
				mve.key_num,mve.key.c_str());
			break;

		}
	}

	/* Combine Addresses into persons */
	for (i=0;i<addrs.size();++i) {
		const Address &ad = addrs.at(i);
		if (ad.empty())
			continue;

		size_t person_id = ad.person_record_id;

		Person &p = db.at(person_id);
		p.addresses.push_back(ad);
	}

	sort(db.begin(),db.end(),PersonSorter());
	return db;
}

/*
   In-Memory, hierarchical (non-normalized) representation
   of iOS AddressBook database.
 */

#include <iostream>
#include <sstream>

#include <err.h>

#include "AddressBookMemory.h"

using namespace std;



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
				const ABMultiValueEntryVector & /*db_addresses*/)
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

		Person &p = db.at(person_index);
		p.contacts.push_back(ci);
	}
	return db;
}

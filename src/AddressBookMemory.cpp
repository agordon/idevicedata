/*
   In-Memory, hierarchical (non-normalized) representation
   of iOS AddressBook database.
 */

#include "AddressBookMemory.h"

PersonVector BuildPersonVector(const ABPersonsVector & /*db_person*/,
				const ABMultiValueVector & /*db_contacts*/,
				const ABMultiValueEntryVector & /*db_addresses*/)
{
	PersonVector v;

	return v;
}

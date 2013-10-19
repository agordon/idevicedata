/*
   C++ representation of iOS's AddressBook SQLite3 database.


   TODO: remove "errx" calls, replace with returning error-codes.
         (a library could should never terminate a program)
 */
#include <err.h>
#include "AddressBookDatabase.h"

using namespace std;

static string sqlite3_get_text_column(sqlite3_stmt* stmt,int column)
{
	const unsigned char* ch = sqlite3_column_text(stmt,column);

	if (ch==NULL)
		return "";
	return (char*)ch;
}

ABPersonsVector load_ABPersons(sqlite3 *db)
{
	ABPersonsVector persons;
	sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,
"Select First,Last,Middle,Nickname,Prefix,Suffix," \
"       Organization,Department,Note, "\
"       Birthday,JobTitle,DisplayName,rowid from ABPerson"\
"       Order by rowid",-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 failed: %s", sqlite3_errmsg(db));

	/* Because SQLite3's ROW-ID starts with 1,
	   insert a dummy person in index 0 */
	ABPerson dummy;
	persons.push_back(dummy);

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		ABPerson p;
		p.First = sqlite3_get_text_column(stmt,0);
		p.Last = sqlite3_get_text_column(stmt,1);
		p.Middle = sqlite3_get_text_column(stmt,2);
		p.Nickname = sqlite3_get_text_column(stmt,3);
		p.Prefix = sqlite3_get_text_column(stmt,4);
		p.Suffix = sqlite3_get_text_column(stmt,5);
		p.Organization = sqlite3_get_text_column(stmt,6);
		p.Department = sqlite3_get_text_column(stmt,7);
		p.Note = sqlite3_get_text_column(stmt,8);
		p.Birthday = sqlite3_get_text_column(stmt,9);
		p.JobTitle = sqlite3_get_text_column(stmt,10);
		p.DisplayName = sqlite3_get_text_column(stmt,11);
		size_t rowid = p.rowid = (size_t)sqlite3_column_int(stmt,12);

		persons.push_back(p);

		/* Sanity Check:the index should be the same as row_id */
		if (persons.size()<=rowid)
			errx(1,"Internal error: ABPerson record rowid %zu is out of order (persons.size()==%zu)", rowid, persons.size());

		if (persons.at(rowid).rowid != rowid)
			errx(1,"Internal error: ABPerson record rowid %zu is out of order (persons.at(%zu).rowid=%zu)",rowid, rowid, persons.at(rowid).rowid);
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return persons;
}

ABMultiValueVector load_ABMultiValues(sqlite3 *db)
{
	ABMultiValueVector values;
	sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,
"select "\
"UID, record_id, property, identifier,label as label_num,  " \
"ABMultiValueLabel.value as label, ABMultiValue.value as value " \
"from ABMultiValue "\
"left outer join ABMultiValueLabel " \
"on ABMultiValue.label = ABMultiValueLabel.rowid " \
"order by ABMultiValue.record_id",-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"loadABMultiValues/sqlite3_prepare_v2 failed: %s", sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		ABMultiValue v;
		v.uid = (size_t)sqlite3_column_int(stmt,0);
		v.person_record_id = (size_t)sqlite3_column_int(stmt,1);
		v.property = (size_t)sqlite3_column_int(stmt,2);
		v.identifier = (size_t)sqlite3_column_int(stmt,3);
		v.label_num = (size_t)sqlite3_column_int(stmt,4);
		v.label = sqlite3_get_text_column(stmt,5);
		v.value = sqlite3_get_text_column(stmt,6);

		/* NOTE: this is memory-wasteful, as the UIDs in the table
		   are not sequencial (there are gaps for deleted entries).
		   But it will make searching very quick */
		values.resize(v.uid+1);
		values.at(v.uid) = v;
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return values;
}

ABMultiValueEntryVector load_ABMultiValueEntry(sqlite3 *db)
{
	ABMultiValueEntryVector values;
	sqlite3_stmt *stmt=NULL;

	int i = sqlite3_prepare_v2(db,
"Select parent_id, ABMultiValueEntry.key as key_num, " \
"ABMultiValueEntryKey.value as key, ABMultiValueEntry.value as value " \
"from ABMultiValueEntry " \
"Left outer join ABMultiValueEntryKey on " \
"ABMultiValueEntry.key = ABMultiValueEntryKey.rowid " \
"order by parent_id asc, key asc "
,-1,&stmt,NULL);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_prepare_v2 failed: %s", sqlite3_errmsg(db));

	while ( (i=sqlite3_step(stmt))==SQLITE_ROW ) {
		ABMultiValueEntry v;
		v.parent_id = (size_t)sqlite3_column_int(stmt,0);
		v.key_num = (size_t)sqlite3_column_int(stmt,1);
		v.key = sqlite3_get_text_column(stmt,2);
		v.value = sqlite3_get_text_column(stmt,3);

		values.push_back(v);
	}
	if (i!=SQLITE_DONE)
		errx(1,"sqlite3_step failed (code=%d=%s): %s",
				i,sqlite3_errstr(i),sqlite3_errmsg(db));

	i = sqlite3_finalize(stmt);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_finalize failed: %s", sqlite3_errmsg(db));

	return values;
}


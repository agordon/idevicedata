#include <iostream>
#include <cassert>
#include <sqlite3.h>
#include <err.h>

using namespace std;

int main(int /*argc*/, char* /*argv*/[])
{
	sqlite3 *db = NULL;
	int i = sqlite3_open(":memory:", &db);
	if (i!=SQLITE_OK) {
		if (db==NULL)
			errx(1,"sqlite3_open failed, memory error");
		errx(1,"sqlite3_open failed: %s", sqlite3_errmsg(db));
	}

	i = sqlite3_close(db);
	if (i!=SQLITE_OK)
		errx(1,"sqlite3_close failed: %s", sqlite3_errmsg(db));

	return 0;
}

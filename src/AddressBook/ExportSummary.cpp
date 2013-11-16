#include <iostream>
#include <string>
#include <sstream>
#include "ExportSummary.h"

using namespace std;

static std::string string_replace ( const std::string& text,
				const std::string& from,
				const std::string& to )
{
	size_t i;
	std::string output = text;

	i = output.find(from);
	while (	i != string::npos ) {
		output.replace(i,from.length(),to);
		i = output.find(from);
	}

	return output;
}

static std::string remove_tab_nl ( const std::string& text )
{
	std::string output(text);
	output = string_replace(output,"\t"," ");
	output = string_replace(output,"\n"," ");
	return output;
}


std::string AddressBookExportSummary(const AddressBook & ab)
{
	stringstream ss;
	for (size_t j=1;j<ab.persons.size();++j) {

		const Person& p = ab.persons.at(j);

		string name = remove_tab_nl(p.GetName());
		string email = remove_tab_nl(p.GetEmail());
		string phone = remove_tab_nl(p.GetPhone());

		ss << name << "\t" << phone << "\t" << email << "\n" ;
	}

	return ss.str();
}

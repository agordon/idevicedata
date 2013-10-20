#include <iostream>
#include <string>
#include <sstream>
#include "AddressBookExportHTML.h"


using namespace std;

/*TODO:
   * replace hard-coded HTML with a proper template engine.
   * add META-TAG Generator + Date (based on generation date and
                                    iOS file date)
   * Set <title> based on Date/iDevice Name
*/
static const char* html_top =
"<!DOCTYPE html>\n" \
"<html>\n" \
"<head>\n" \
"<meta charset='utf-8'>\n" \
"<title>iOS AddressBook</title>\n" \
"<style type=\"text/css\">\n" \
"body {\n" \
"font-family: Arial, Helvetica, sans-serif;\n" \
"background-image:-webkit-gradient(linear, left top, right top,\n" \
"color-stop(.31, #c8cdd7),\n" \
"color-stop(.31, #cbd0d9),\n" \
"color-stop(.69, #cbd0d9)\n" \
");\n" \
"-webkit-background-size: 20px 20px;\n" \
"}\n" \
"div.contat_list {\n" \
"}\n" \
"div.contact {\n" \
"}\n" \
"div.contact_header {\n" \
"}\n" \
"div.contact_photo {\n" \
"float: left ;\n" \
"}\n" \
"div.contact_photo_placeholder {\n" \
"background: white ;\n" \
"border: 1px solid #ababab;\n" \
"width: 100px;\n" \
"height: 100px;\n" \
"}\n" \
"div.contact_title {\n" \
"float: left;\n" \
"}\n" \
"div.contact_maintitle {\n" \
"padding-top: 1em;\n" \
"padding-left: 1em;\n" \
"font-size: 120%;\n" \
"font-weight: bold;\n" \
"}\n" \
"div.contact_subtitle {\n" \
"padding-left: 1.1em;\n" \
"height 50px;\n" \
"}\n" \
"div.contact_info_separator {\n" \
"clear: both;\n" \
"height: 20px;\n" \
"}\n" \
"div.contact_oneline_info {\n" \
"background: white ;\n" \
"padding: 5px;\n" \
"border-radius: 15px;\n" \
"-moz-border-radius: 15px;\n" \
"margin-top: 5px;\n" \
"overflow:hidden;\n" \
"border: 1px solid #ababab;\n" \
"}\n" \
"div.contact_oneline_label {\n" \
"background: white;\n" \
"color: #516691;\n" \
"float: left;\n" \
"width: 100px;\n" \
"padding-right: 5px;\n" \
"text-align:right;\n" \
"}\n" \
"div.contact_oneline_value {\n" \
"background: white;\n" \
"color: black;\n" \
"font-size: 120%;\n" \
"float: left;\n" \
"padding-left: 5px;\n" \
"}\n" \
"div.contact_address_block {\n" \
"background: white ;\n" \
"padding: 5px;\n" \
"border-radius: 15px;\n" \
"-moz-border-radius: 15px;\n" \
"margin-top: 5px;\n" \
"overflow:hidden;\n" \
"border: 1px solid #ababab;\n" \
"}\n" \
"div.contact_address_info {\n" \
"background: white;\n" \
"color: #516691;\n" \
"clear:both;\n" \
"overflow:hidden;\n" \
"margin-right: 5px;\n" \
"padding-top: 5px;\n"\
"}\n" \
"div.contact_address_info:not(:first-of-type) {\n" \
"border-top: 1px solid black;\n" \
"}\n" \
"div.contact_address_label {\n" \
"background: white;\n" \
"color: #516691;\n" \
"float: left;\n" \
"width: 100px;\n" \
"padding-right: 5px;\n" \
"text-align:right;\n" \
"}\n" \
"div.contact_address_value {\n" \
"background: white;\n" \
"float: left;\n" \
"color: black;\n" \
"font-size: 120%;\n" \
"float: left;\n" \
"padding-left: 5px;\n" \
"}\n" \
"div.contact_person_separator {\n" \
"clear: both;\n" \
"margin-top: 2em;\n" \
"margin-bottom: 2em;\n" \
"}\n" \
"</style>\n" \
"</head>\n" \
"<body>\n";

static const char* html_bottom =
"</body>\n" \
"</html>\n" ;

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

static std::string htmlize ( const std::string& text )
{
	std::string output(text);
	output = string_replace(output,"<","&lt;");
	output = string_replace(output,">","&gt;");
	output = string_replace(output,"\n","<br/>");
	return output;
}


std::string AddressBookExportHTML(const AddressBook & ab )
{
	string result;

	result += string(html_top);

	result += "<div class=\"contact_list\">\n" ;
	result += "<ol>\n";
	for (size_t j=1;j<ab.persons.size();++j) {
		const Person& p = ab.persons.at(j);
		stringstream ss;
		ss << "<li><a href=\"#" << p.rowid << "\">"
			<< htmlize(p.GetName()) << "</a></li>\n" ;
		result += ss.str();
	}
	result += "</ol></div>\n" ;
	for (size_t j=1;j<ab.persons.size();++j) {

		const Person& p = ab.persons.at(j);

		stringstream ss;
		ss << "<div class=\"contact\">\n"
			<< "<a name=\"" << p.rowid << "\"></a>\n"

			/* contact Header: Photo + Name */
			<< "<div class=\"contact_header\">\n"

				/* TODO: Add image, if any */
				<< "<div class=\"contact_photo\">\n"
				<< "<div class=\"contact_photo_placeholder\"></div>"
				<< "</div>\n"

			<< "<div class=\"contact_title\">"
			<< "<div class=\"contact_maintitle\">"
			/* TODO: This is incorrect -
				 if the contact only has "Company name" or "Organization", it will appear twice */
			<< htmlize(p.GetName())
			<< "</div>\n"
			<< "<div class=\"contact_subtitle\">"
			<< htmlize(p.Organization)
			<< "</div>\n" /* end of "contact_title" div */
			<< "</div>\n" /* end of "contact_header" div */
			;

		/* One-liner contact information (email,mobile,URL) */
		ss << "<div class=\"contact_info_separator\"></div>\n" ;
		for (size_t i=0;i<p.contacts.size();++i) {
			const ContactInfo& ci = p.contacts.at(i);

			if (ci.value.empty())
				continue;

			ss << "<div class=\"contact_oneline_info\">\n"
				<< "<div class=\"contact_oneline_label\">"
				<< htmlize(ci.label)
				<< "</div>\n"
				<< "<div class=\"contact_oneline_value\">"
				<< htmlize(ci.value)
				<< "</div>\n"
				<< "</div>\n\n";
		}

		/* Address Information */
		if (p.addresses.size()>0) {
			ss << "<div class=\"contact_info_separator\"></div>\n" ;
			ss << "<div class=\"contact_address_block\">\n";
			for  (size_t i=0;i<p.addresses.size();++i) {
				const Address &ad = p.addresses.at(i);
				ss << "<div class=\"contact_address_info\">\n";

				ss << "<div class=\"contact_address_label\">"
					<< htmlize(ad.type) << "</div>\n" ;

				ss << "<div class=\"contact_address_value\">"
					<< htmlize(ad.street) << "<br/>"
					<< htmlize(ad.ZIP) << " " << htmlize(ad.city) << "<br/>"
					<< htmlize(ad.country)	<< "</div>\n" ;

				ss << "</div>\n";
			}
			ss << "</div>\n" ; /* end of "contact_address_block" */
		}

		if (!p.Note.empty()) {
			ss << "<div class=\"contact_info_separator\"></div>\n" ;
			ss << "<div class=\"contact_oneline_info\">\n"
				<< "<div class=\"contact_oneline_label\">"
				<< "Notes"
				<< "</div>\n"
				<< "<div class=\"contact_oneline_value\">"
				<< htmlize(p.Note)
				<< "</div>\n"
				<< "</div>\n\n";
		}

		ss << "</div>" ; /* end of "contact" div */

		ss << "<div class=\"contact_person_separator\"></div>\n" ;

		result += ss.str();
	}

	result += string(html_bottom);

	return result;
}

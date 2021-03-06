#pragma once
/////////////////////////////////////////////////////////////////////
// Paylod.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Tao Jun Lin, CSE687 - Object Oriented Design, Spring 2018       //
// tlin19@syr.edu                                                  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type.
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database
*
*
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* XmlDocument.h, XmlDocument.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 04 Jan 2018
* - added methods to add and delete key/value pairs.
* - added methods to edit relationships and text metadata and replace DbElement.
* - added methods to restore from, augment from and write to a xml file.
* ver 1.0 : 10 Jan 2018
*/
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <regex>
#include "../XmlDocument/XmlDocument/XmlDocument.h"


namespace Payload 
{
	/////////////////////////////////////////////////////////////////////
	// File class
	// - demonstrates an example of payload type, containing a string and a container for strings.
	class File 
	{
	public:
		using Category = std::string;
		using Categories = std::unordered_set<Category>;
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

		//constructors: a default ctor and a constructor takes a xml string
		File() : path_(".*"), categories_(Categories()) {}
		File(std::string Str);

		//methods to get and set payload fields
		std::string& path() { return path_; }
		std::string path() const { return path_; }
		File& path(const std::string& path) { path_ = path; return *this; }

		Categories& categories() { return categories_; }
		Categories categories() const { return categories_; }
		File& categories(const Categories& categories) { categories_ = categories; return *this; }

		friend std::ostream& operator<<(std::ostream& out, const File& f);
		//functionality 

		bool compare(const File p);
		File& addCategory(const Category& category);
		File& delCategory(const Category& category);
		operator std::string();
		bool operator==(const File& f);
		
		

	private:
		std::string path_;
		Categories categories_;
	};

}

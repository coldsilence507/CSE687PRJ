/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

using namespace Utilities; 

int main()
{
	DbCore<std::string> db;
	DbElement<std::string> demoElem;
	DateTime time1(DateTime::makeTime(2018, 1, 15));
	//Create and add first DbElement "Fawcett"
	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(time1);
	demoElem.payLoad("The good news is ...");
	db.addRecord("Fawcett", demoElem);

  return 0;
}
#endif

#pragma once
/////////////////////////////////////////////////////////////////////
// Test.h - Implements Requirement test cases for Project 1        //
// ver 1.0,                                                        //
// Tao Jun Lin, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a test class:
*
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* StringUtilities.h, StringUtilities.cpp
* XmlDocument.h, XmlDocument.cpp
* 
*
* Maintenance History:
* --------------------
* ver 1.1 : 04 Jan 2018
* - added methods to add and delete key/value pairs.
* - added methods to edit relationships and text metadata and replace DbElement.
* - added methods to restore from, augment from and write to a xml file.
* ver 1.0 : 10 Jan 2018
*/

//#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include "../Query/Query.h"
#include "../Payload/Payload.h"

using namespace NoSqlDb;




///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------
namespace Test {
	class Test
	{
	public:

		static bool testR1();
		static bool testR2();
		static bool testR3a();
		static bool testR3b();
		static bool testR4();
		static bool testR5();
		static bool testR6a();
		static bool testR6b();
		static bool testR6c();
		static bool testR7();
		static bool testR8();
		static bool testR9();

	private:
		static DbCore<std::string> dbstring_;
		static DbCore<Payload::File> dbpayload_;

	};

}
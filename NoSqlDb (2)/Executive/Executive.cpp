/////////////////////////////////////////////////////////////////////
// Executive.cpp - Implements NoSql database prototype             //
// ver 1.0                                                         //
// Tao Jun Lin, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include "../Test/Test.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

//----< reduce the number of characters to type >----------------------
auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

#ifdef TEST_EXECUTIVE

int main() {
	Utilities::Title("Testing NoSql Database Prototype - Sample Repository Database");
	putLine();
	std::cout << "  The requirements will be demonstrated with two DbCore's containing both std::string and a customized generic type (payload).";
	TestExecutive ex;
	Test::Test test1;

	// define test structures with test function and message
	TestExecutive::TestStr ts1{ test1.testR1, "R#1: Use C++11" };
	TestExecutive::TestStr ts2{ test1.testR2, "R#2: Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ test1.testR3a, "R#3a: creating DbElement<std::string> and DbCore<std::string>" };
	TestExecutive::TestStr ts3b{ test1.testR3b, "R#3b: creating DbElement<Payload> and DbCore<Payload>" };
	TestExecutive::TestStr ts4{ test1.testR4, "R#4: Addition and deletion of key/value pairs" };
	TestExecutive::TestStr ts5{ test1.testR5, "R#5: Editing of values, including relationships, text metadata, and replace DbElement<T> of a key with another DbElement<T>." };
	TestExecutive::TestStr ts6a{ test1.testR6a,  "R#6a - Query for value and children of a specified key, or set of keys matching a specified regular-expression pattern." };
	TestExecutive::TestStr ts6b{ test1.testR6b,  "R#6b - Query with a condition object." };
	TestExecutive::TestStr ts6c{ test1.testR6c,  "R#6c - Query with text metadata, date-time interval and payload." };
	TestExecutive::TestStr ts7{ test1.testR7,  "R#7 - Support intersection/and, union/or operations." };
	TestExecutive::TestStr ts8{ test1.testR8,  "R#8 - Restore and augment database from .xml files." };
	TestExecutive::TestStr ts9{ test1.testR9,  "R#9 - Implementation of a Payload type" };

	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6a);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);

	// run tests
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	getchar();
	return 0;
}

#endif
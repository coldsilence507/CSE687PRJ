
#include "Test.h"

using namespace NoSqlDb;


namespace Test{
	DbCore<std::string> Test::dbstring_;
	DbCore<Payload::File> Test::dbpayload_;


	bool Test::testR1()
	{
		Utilities::title("\n  Demonstrating Requirement #1");
		std::cout << "\n  " << typeid(std::function<bool()>).name()
			<< ", declared in this function, "
			<< "\n  is only valid for C++11 and later versions.\n";
		return true; // would not compile unless C++11
	}

	//----< demo requirement #2 >------------------------------------------

	bool Test::testR2()
	{
		Utilities::title("\n  Demonstrating Requirement #2");
		std::cout << "\n  A visual examination of all the submitted code "
			<< "will show only\n  use of streams and operators new and delete for all heap-based memory management.\n";
		return true;
	}

	//----< demo requirement #3 >------------------------------------------

	bool Test::testR3a()
	{
		Utilities::title("\n  Demonstrating Requirement #3a - creating DbElement<std::string> and DbCore<std::string>.");
		std::cout << "\n  Creating some DbElement<std::string> and establish child relationship.";
		DbElement<std::string> demoElem;
		DateTime time1(DateTime::makeTime(2018, 1, 15));
		//Create and add first DbElement "Fawcett"
		demoElem.name("Jim");
		demoElem.descrip("Instructor for CSE687");
		demoElem.dateTime(time1);
		demoElem.payLoad("The good news is ...");
		dbstring_.addRecord("Fawcett", demoElem);
		std::cout << "\n  Create and add first DbElement \"Fawcett\".";
		//if any of the following conditions return false, DbElement is not created and added successfully to DbCore.
		if (!dbstring_.contains("Fawcett"))
			return false;
		if (demoElem.name() != "Jim")
			return false;
		if (demoElem.descrip() != "Instructor for CSE687")
			return false;
		if (std::string(demoElem.dateTime()) != std::string(time1))
			return false;
		if (demoElem.payLoad() != "The good news is ...")
			return false;
		if (dbstring_.size() != 1)
			return false;

		//Create and add second DbElement "Salman" 
		demoElem.name("Ammar").descrip("TA for CSE687").dateTime(DateTime().now()).payLoad("You should try ...");
		dbstring_.addRecord("Salman", demoElem);
		dbstring_.addChild("Fawcett", "Salman");
		std::cout << "\n  Create and add second DbElement \"Salman\", and establish child relationship with \"Fawcett\".";

		//Create and add third DbElement "Sun" 
		demoElem.name("Jianan").payLoad("Dr. Fawcett said ...");
		dbstring_.addRecord("Sun", demoElem);
		dbstring_.addChild("Fawcett", "Sun");
		std::cout << "\n  Create and add second DbElement \"Sun\", and establish child relationship with \"Fawcett\".";

		//assertion for size of db and children of record with key "Fawcett"
		if (dbstring_.size() != 3)
			return false;
		if (dbstring_["Fawcett"].children().size() != 2)
			return false;

		dbstring_.showDb();
		showKeys(dbstring_);
		std::cout << "\n";
		return true;
	}

	bool Test::testR3b()
	{
		Utilities::title("\n  Demonstrating Requirement #3b - DbElement<Payload> and DbCore<Payload>");
		std::cout << "\n  Creating some DbElements<Payload> and establish child relationship\n";

		DbElement<Payload::File> demoElem;
		//DbElements<Payload> 1
		Payload::File f1;
		f1.path("/cse687ood2018sp/").addCategory("Midterm").addCategory("Project").addCategory("Exam").addCategory("Helpsession");
		demoElem.name("Object Oriented Design").descrip("2018Sp, Instructor: Jim Fawcett").dateTime(DateTime::makeTime(2018, 1, 15)).payLoad(f1);
		dbpayload_.addRecord("CSE687", demoElem);
		//DbElements<Payload> 2
		Payload::File f2;
		f2.path("/cis554oop2017fa/").addCategory("Homework").addCategory("Midterm").addCategory("Exam");
		demoElem.name("Object Oriented Programming").descrip("2017Fa, Instructor: Roger Chen").dateTime(DateTime::makeTime(2017, 9, 1)).payLoad(f2);
		dbpayload_.addRecord("CIS554", demoElem);
		dbpayload_.addChild("CSE687", "CIS554");
		//DbElements<Payload> 3
		Payload::File f3;
		f3.path("/cse661aca2018sp/").addCategory("Homework").addCategory("Quiz").addCategory("Termpaper").addCategory("Midterm").addCategory("Exam");
		demoElem.name("Advanced Computer Architecture").descrip("2018Sp, Instructor: Ehat Ercanli").dateTime(DateTime::makeTime(2018, 1, 15)).payLoad(f3);
		dbpayload_.addRecord("CSE661", demoElem);
		//DbElements<Payload> 4
		Payload::File f4;
		f4.path("/cse681aca2017fa/").addCategory("Assignment").addCategory("Midterm").addCategory("Exam");
		demoElem.name("Software Model Analysis").descrip("2017Fa, Instructor: Jim Fawcett").dateTime(DateTime::makeTime(2017, 9, 1)).payLoad(f4);
		dbpayload_.addRecord("CSE681", demoElem);
		dbpayload_.addChild("CSE687", "CSE681");
		dbpayload_.addChild("CSE681", "CIS554");

		//assert the number of DbElement<Payload> in DbCore, and also child relationships
		if (!dbpayload_.contains("CSE687") || !dbpayload_.contains("CIS554"))
			return false;
		if (dbpayload_.size() != 4)
			return false;
		if (dbpayload_["CSE687"].children().size() != 2)
			return false;
		if (dbpayload_["CSE681"].children().size() != 1)
			return false;

		dbpayload_.showDb();
		showKeys(dbpayload_);
		std::cout << "\n";
		return true;
	}

	//----< demo requirement #4 >------------------------------------------

	bool Test::testR4()
	{
		Utilities::title("\n  Demonstrating Requirement #4 - Addition and deletion of key/value pairs.");
		std::cout << "\n  Use member functions of DbCore as interfaces to provide functionalities for addition and deletion of key/value pairs.";
		std::cout << "\n \n  Addition of key/value pairs is already demonstrated in Requirement #3a and #3b, please see code of testR3a/b";
		std::cout << "\n  and member function DbCore<T>::addRecord(const Key key, const DbElement<T>& DbElem) for more details.\n";
		std::cout << "\n  Demonstration of deletion of key/value pairs on DbCore<std::string>:";

		dbstring_.showDb();
		showKeys(dbstring_);
		DbElement<std::string> tempElem = dbstring_["Sun"];

		std::cout << "\n  After deleting a record with delRecord(\"Sun\") function, record with key \"Sun\" is deleted, and also removed from children of \"Fawcett\". \n";
		dbstring_.delRecord("Sun");
		dbstring_.showDb();
		showKeys(dbstring_);
		//check for changes have been made to the DbCore<String> and Children of "Fawcett"
		if (dbstring_.contains("Sun"))
			return false;
		if (dbstring_.size() != 2)
			return false;
		if (dbstring_["Fawcett"].children().size() != 1)
			return false;

		std::cout << "\n  Demonstration of deletion of key/value pairs on DbCore<payload>:\n";
		dbpayload_.showDb();
		showKeys(dbpayload_);
		DbElement<Payload::File> tempElem1 = dbpayload_["CSE681"];
		std::cout << "\n  After deleting a record with delRecord(\"CSE681\") function, record with key \"CSE681\" is deleted, and also removed from children of \"CSE687\". \n";
		dbpayload_.delRecord("CSE681");
		dbpayload_.showDb();
		showKeys(dbpayload_);

		//assertion for changes have been made to the DbCore<payload> and Children of "CSE687"
		if (dbpayload_.size() != 3)
			return false;
		if (dbpayload_["CSE687"].children().size() != 1)
			return false;

		//recover data deletion
		dbstring_.addRecord("Sun", tempElem);
		dbstring_.addChild("Fawcett", "Sun");
		dbpayload_.addRecord("CSE681", tempElem1);
		dbpayload_.addChild("CSE687", "CSE681");

		return true;
	}

	//----< demo requirement #5 >------------------------------------------

	bool Test::testR5() {
		Utilities::title("\n  Demonstrating Requirement #5 - Editing of values, including relationships, text metadata, and replace DbElement<T> of a key with another DbElement<T>.");
		std::cout << "\n  Use member functions of DbCore as interfaces to provide functionalities for editing and replacing value.";
		std::cout << "\n \n  Addition of child relationship is already demonstrated in Requirement #4, please see code and demo of testR4. \n";
		std::cout << "\n  Demonstration of Editing of values:";

		//save two temporary copies of DbElement
		DbElement<Payload::File> tempElem = dbpayload_["CSE661"];
		DbElement<Payload::File> tempElem1 = dbpayload_["CSE687"];
		dbpayload_.showDb();

		// editing text metadata, datetime, and payload.
		Payload::File f1;
		f1.path("/cse661aca2018fa/").addCategory("unknown");
		DateTime t1(DateTime::makeTime(2018, 9, 15));
		dbpayload_.editName("CSE661", "ACA").editDescript("CSE661", "2018Fa, Instructor: Unknown").editDatetime("CSE661", t1).editPayload("CSE661", f1);
		dbpayload_.delChild("CSE687", "CSE681");
		std::cout << "\n  After editing all the text metadata and its payload of key \"CSE661\", and deleting \"CSE681\" from children list of \"CSE687\".\n";
		dbpayload_.showDb();
		//assertion for editing of values
		if (dbpayload_["CSE687"].children().size() != 1)
			return false;
		if (dbpayload_["CSE661"].name() != "ACA")
			return false;
		if (dbpayload_["CSE661"].descrip() != "2018Fa, Instructor: Unknown")
			return false;
		if (std::string(dbpayload_["CSE661"].dateTime()) != std::string(t1))
			return false;
		if (!(dbpayload_["CSE661"].payLoad() == f1))
			return false;
		//replace current value with temporary copies
		std::cout << "\n  The previous value is saved as a temporary DbElement copy of \"CSE661\" and \"CSE687\", \n";
		std::cout << "\n  simply replace the temporary DbElement back with DbCore<T>::replace() function, child relationship is also restored.\n";
		dbpayload_.replace("CSE661", tempElem);
		dbpayload_.replace("CSE687", tempElem1);
		dbpayload_.showDb();
		//same interfaces teseted on DbCore<std::string>
		std::cout << "\n  Demonstrate the same interfaces with DbCore<std::string>, but only editPayload and replace, since others are the same.";
		dbstring_.showDb();
		DbElement<std::string> tempElem2 = dbstring_["Salman"];
		dbstring_.editPayload("Salman", "Thanks for the helpsession.");
		std::cout << "\n  After editing the payload of key \"Salman\"";
		dbstring_.showDb();
		dbstring_.replace("Salman", tempElem2);
		std::cout << "\n  And replaced back with initial DbElement";
		dbstring_.showDb();

		return true;
	}

	//----< demo requirement #6 >------------------------------------------

	bool Test::testR6a()
	{
		Utilities::title("\n  Demonstrating Requirement #6a - Query for value and children of a specified key, or set of keys matching a specified regular-expression pattern.");
		std::cout << "\n  Using member functions from Query class as interfaces, and Query instances for key query.";
		std::cout << "\n  Demo for DbCore<Payload>, since query for value and children with key works the same for both std::string and payload";
		Query::Query<Payload::File> q1(dbpayload_);

		dbpayload_.showDb();
		showKeys(dbpayload_);
		//query for values with a specified key
		std::cout << "\n  The above is the DbCore<Payload> we query from, first we perform a specified key queries using strings\n";
		std::cout << "  \"CSE687\":\n";
		q1.selectByKey("CSE687");
		q1.showKeys();
		if (q1.keys().size() != 1)
			return false;
		std::cout << "  \"CIS554\":\n";
		q1.reset().selectByKey("CIS554");
		q1.showKeys();

		std::cout << "  \"CSE674\":\n";
		q1.reset().selectByKey("CSE674");
		q1.showKeys();
		if (q1.keys().size() != 0)
			return false;
		//query for children keys with a specified key
		std::cout << "\n  Query for children of key \"CSE687\":\n";
		q1.reset().selectChildren("CSE687");
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;
		//query for key sets with a regular-expression pattern
		std::cout << "\n  Then we use a regular-expression pattern \"CSE.*\" to perform query, query result will contain 3 keys:\n";
		q1.reset().selectByKey("CSE.*");
		q1.showKeys();
		if (q1.keys().size() != 3)
			return false;

		std::cout << "\n  Displaying query results with value is also supported:";
		q1.show();

		return true;
	}

	bool Test::testR6b()
	{
		Utilities::title("\n  Demonstrating Requirement #6b - Query with a condition object.");
		std::cout << "\n  Using member functions from Query class as interfaces, and Query instances with a condition object.";
		std::cout << "\n  Demo query with a condition object using both DbCore<Payload> and DbCore<std::string> to show handling different T.\n";

		//query DbCore<Payload> with a condition object 
		Query::Query<Payload::File> q1(dbpayload_);
		Query::Condition<Payload::File> c1;
		dbpayload_.showDb();
		showKeys(dbpayload_);

		std::cout << "\n  Demonstrating a query with a default condition object, which returns all keys from the above database,\n";
		std::cout << "  (for details of condition object, see condition class in Query.h)\n";
		q1.select(c1);
		q1.showKeys();
		if (q1.keys().size() != 4)
			return false;
		std::cout << "\n  changed condition parameters, key = \"CSE.*\", description = \".*Jim.*\"\n";
		c1.key("CSE.*").descrip(".*Jim.*");
		q1.reset().select(c1);
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;
		std::cout << "\n  Set the path with regular expression pattern \".*cse.*\" and added a category \"Project\" to the payload variable of the condition object, query should return only 1 key\n";
		c1.payload().path(".*cse.*").addCategory("Project");
		q1.reset().select(c1);
		q1.showKeys();
		if (q1.keys().size() != 1)
			return false;
		std::cout << "\n  use a condition containing only two categories \"Homework\" and \"Midterm\" to the payload variable of the condition object, query should return two keys\n";
		c1.reset().payload().addCategory("Homework").addCategory("Midterm");
		q1.reset().select(c1);
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;

		//query DbCore<std::string> with a condition object
		Query::Query<std::string> q2(dbstring_);
		Query::Condition<std::string> c2;
		dbstring_.showDb();
		showKeys(dbstring_);

		std::cout << "\n  Demonstrating a query with a default condition object, which returns all keys from the above database,\n";
		q2.select(c2);
		q2.showKeys();
		if (q2.keys().size() != 3)
			return false;
		return true;
	}

	bool Test::testR6c()
	{
		Utilities::title("\n  Demonstrating Requirement #6b - Query with text metadata, date-time interval and payload.");
		std::cout << "\n  Using member functions from Query class as interfaces, and Query instances with DbElement values.";
		std::cout << "\n  Demo query with payload using both DbCore<Payload> and DbCore<std::string> to show handling different T.\n";
		dbpayload_.showDb();
		showKeys(dbpayload_);

		std::cout << "\n  Query name of records using regular-expression pattern \"Object.*\", result should return 2 keys:\n";
		Query::Query<Payload::File> q1(dbpayload_);
		q1.selectByName("Object.*");
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;

		std::cout << "\n  Query description of records using regular-expression pattern \".*Jim.*\", result should return 2 keys:\n";
		q1.reset().selectByDescrip(".*Jim.*");
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;

		std::cout << "\n  Query datetime of records using datetime objects, interval from 1 Jan 2018 to now, result should return 2 keys:\n";
		DateTime t1(DateTime::makeTime(2018, 1, 1));
		q1.reset().selectByTime(t1);
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;
		std::cout << "\n  Query datetime of records using datetime objects, interval from 1 Sept 2017 to 1 Jan 2018, result should return 2 keys:\n";
		DateTime t2(DateTime::makeTime(2017, 9, 1));
		q1.reset().selectByTime(t2, t1);
		q1.showKeys();
		if (q1.keys().size() != 2)
			return false;

		std::cout << "\n  Query payload of records, query with category is already demonstrated in R#6b, \n  so here demonstrates query for any payload with path matches pattern \".*cse.*\" result should return 3 keys:\n";
		Payload::File f1;
		f1.path(".*cse.*");
		q1.reset().selectByPayload(f1);
		q1.showKeys();
		if (q1.keys().size() != 3)
			return false;

		std::cout << "\n  Query payload of records, when T is std::string, with regular-expression pattern \".*Fawcett.*\", the result should return one key\n";
		Query::Query<std::string> q2(dbstring_);
		q2.selectByPayload(".*Fawcett.*");
		q2.showKeys();
		if (q2.keys().size() != 1)
			return false;

		return true;
	}

	//----< demo requirement #7 >------------------------------------------

	bool Test::testR7()
	{
		Utilities::title("Demonstrating Requirement #7 - Support intersection/\"and\", union/\"or\" operations.");
		std::cout << "\n Using member functions from Query class as interfaces, operate Intersection and Union on many query results.";
		dbpayload_.showDb();
		showKeys(dbpayload_);
		//generate some query results
		Query::Query<Payload::File> q1(dbpayload_), q2(dbpayload_), q3(dbpayload_), q4(dbpayload_);
		std::cout << "\n  I defined three queries: \n  Query 1: select keys with pattern \"CSE.*\", 3 keys expected to return: ";
		q1.selectByKey("CSE.*");
		q1.showKeys();
		std::cout << "\n  Query 2: select description with pattern \".*Jim.*\", 2 keys expected to return: ";
		q2.selectByDescrip(".*Jim.*");
		q2.showKeys();
		std::cout << "\n  Query 3: select description with pattern \".*Roger.*\", 1 keys expected to return: ";
		q3.selectByDescrip(".*Roger.*");
		q3.showKeys();

		// intersection and union operations
		std::cout << "\n  For detailed implementation please see code and class Query ";
		std::cout << "\n  Result of union Query 2 and Query 3, 3 keys expected to return: ";
		q4.from(q2).unionQuery(q3);
		q4.showKeys();
		if (q4.keys().size() != 3)
			return false;

		std::cout << "\n  Result of intersecting Query 1 and Query 2, 2 keys expected to return: ";
		q4.from(q1).intersectQuery(q2);
		q4.showKeys();
		if (q4.keys().size() != 2)
			return false;

		std::cout << "\n  Result of union Query 1, Query 2 and Query 3, 4 keys expected to return: ";
		q4.from(q1).unionQuery(q2).unionQuery(q3);
		q4.showKeys();
		if (q4.keys().size() != 4)
			return false;

		std::cout << "\n  Result of union Query 1, Query 2 and Query 3, 0 keys expected to return: ";
		q4.from(q1).intersectQuery(q2).intersectQuery(q3);
		q4.showKeys();
		if (q4.keys().size() != 0)
			return false;
		std::cout << "\n";

		return true;
	}

	//----< demo requirement #8 >------------------------------------------

	bool Test::testR8()
	{
		Utilities::title("\n  Demonstrating Requirement #8 - Restore and augment database from .xml files.");
		std::cout << "\n Demonstrate using interfaces in class DbCore to read from and write data to .xml files.";
		//Demonstrating reading a xml file of a database with std::string as payload 
		std::cout << "\n  Writing the existing database to a .xml file using DbCore::WriteToXml() funtion, (please see ../src/R8StringDb1.xml for details)\n";
		dbstring_.WriteToXml("../src/R8StringDb1.xml");
		dbstring_.showDb();
		dbstring_.Reset();
		std::cout << "\n  The database is now cleared:\n";
		dbstring_.showDb();
		std::cout << "\n  Restore the database using the .xml file just written :\n";
		dbstring_.ReadFromXml("../src/R8StringDb1.xml");
		dbstring_.showDb();
		std::cout << "\n  And then augment it with larger database, please see (../src/R8StringDb2.xml for details) :\n";
		dbstring_.ReadFromXml("../src/R8StringDb2.xml");
		dbstring_.showDb();

		//Demonstrating reading a xml file of a database with Payload::File as payload 
		std::cout << "\n  Repeat the above process with the database with Payload::File as payload, (please see ../src/R8PayloadDb1.xml for details)\n";
		dbpayload_.WriteToXml("../src/R8PayloadDb1.xml");
		dbpayload_.showDb();
		dbpayload_.Reset();
		std::cout << "\n  The database is now cleared:\n";
		dbpayload_.showDb();
		std::cout << "\n  Restore the database using the .xml file just written :\n";
		dbpayload_.RestoreFromXml("../src/R8PayloadDb1.xml");
		dbpayload_.showDb();
		std::cout << "\n  And then augment it with larger database using DbCore::ReadFromXml(), (please see ../src/R8PayloadDb2.xml for details) :\n";
		dbpayload_.ReadFromXml("../src/R8PayloadDb2.xml");
		dbpayload_.showDb();

		return true;
	}

	//----< demo requirement #9 >------------------------------------------

	bool Test::testR9()
	{
		Utilities::title("\n  Demonstrating Requirement #9 - Implementation of a Payload type");
		std::cout << "\n  Details of payload type used in the previous demonstrations can be found in the Payload Package.";
		std::cout << "\n In this test function, instantiation of a Payload::File object is demonstrated, ";
		std::cout << "\n  the object contains a string path and a container storing strings as categories, where I used a unordered_map<std::string> to implement it.\n";
		//declaration and setting attributes
		Payload::File f1;
		f1.path("/cse687ood2018sp/").addCategory("Midterm").addCategory("Project").addCategory("Exam").addCategory("Helpsession");
		std::cout << "\n  To declare and instantiate a Payload::File object:\n  ";
		std::cout << "Payload::File f1;\n  ";
		std::cout << "f1.path(\"/cse687ood2018sp/\").addCategory(\"Midterm\").addCategory(\"Project\").addCategory(\"Exam\").addCategory(\"Helpsession\");\n  ";
		std::cout << "and display the object with std::cout:\n  " << f1 << "\n";

		return true;
	}
}

#ifdef TEST_TEST
int main()
{
	Test::Test test;
	test.testR1();
	test.testR2();
	test.testR3a();
	test.testR3b();
	test.testR4();
	test.testR5();
	test.testR6a();
	test.testR6b();
	test.testR6c();
	test.testR7();
	test.testR8();
	test.testR9();

	return 0;
}

#endif

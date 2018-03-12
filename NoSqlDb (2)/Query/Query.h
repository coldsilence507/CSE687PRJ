#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h - Implements Queries for a NoSql database prototype     //
// ver 1.0                                                         //
// Tao Jun Lin, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - Query implements query operations for the NoSql database
*   of DbCore
* - Condition implements a class storing query conditions, which contains
*   
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* 
* Maintenance History:
* --------------------
* ver 1.0 : 05 Feb 2018
*/
#include<string>
#include<vector>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<functional>
#include<regex>
#include<unordered_set>
#include "../DbCore/DbCore.h"

namespace Query {

	template <typename T>
	class Condition {
	public:
		using Key = std::string;
		//a default constructor of condition

		Condition() : key_(".*"), name_(".*"), descrip_(".*"), begintime_(DateTime().epoch()), endtime_(DateTime().now()), payload_(T()) {}

		// methods to get and set DbElement fields
		Condition& reset();

		Key& key() { return key_; }
		Key key() const { return key_; } 
		Condition& key(const Key key) { key_ = key; return *this; }

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		Condition& name(const std::string& name) { name_ = name; return *this; }

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		Condition& descrip(const std::string& descrip) { descrip_ = descrip; return *this; }

		DateTime& begintime() { return begintime_; }
		DateTime begintime() const { return begintime_; }
		Condition& begintime(const DateTime datetime) { begintime_ = datetime; return *this; }
		DateTime& endtime() { return endtime_; }
		DateTime endtime() const { return endtime_; }
		Condition& endtime(const DateTime datetime) { endtime_ = datetime; return *this; }

		T& payload () { return payload_; }
		T payload () const { return payload_; }
		Condition& payload(const T t) { payload_= t; return *this; }

	private:
		Key key_;
		std::string name_;
		std::string descrip_;
		DateTime begintime_;
		DateTime endtime_;
		T payload_;
	};

	template <typename T>
	Condition<T>& Condition<T>::reset()
	{
		key_ = ".*";
		name_ = ".*";
		descrip_ = ".*";
		begintime_ = DateTime().epoch();
		endtime_ = DateTime().now();
		payload_ = T();
		return *this;
	}

	template <typename T>
	class Query {
	public:
		using Key = std::string;
		using Keys = std::unordered_set<Key>;
		using Db = NoSqlDb::DbCore<T>;
		using matchfunc = std::function<int(T)>;

		//constructor with a DbCore<T>
		Query<T>(Db& db) : db_(db) {
			for (auto key : db.keys())
				keys_.insert(key);
		}

		// method to access keys
		Keys& keys() { return keys_; }

        // methods for queries by key
		Query<T>& reset();
		Query<T>& selectByKey(const std::string s);
		Query<T>& selectChildren(const Key key);

		// methods for query by values
		Query<T>& select(const Condition<T>& c);
		Query<T>& selectByName(const std::string s);
		Query<T>& selectByDescrip(const std::string s);
		Query<T>& selectByTime(const DateTime begin, const DateTime end = DateTime().now());
		Query<T>& selectByPayload(T t);
		
		// Union and Intersection ("or" and "and") operation of queries
		Query<T>& unionQuery(const Query& q); 
		Query<T>& intersectQuery(const Query& q); 

		//replace this query by another query
		Query<T>& from(const Query& q); 

		bool saveQuerytoXml(std::string xmlfilepath);

		//display functionalities
		void show();
		void showKeys();

	private:
		Keys keys_;
		Db& db_;
	};

	//----< Reseting the keys of 

	template <typename T>
	Query<T>& Query<T>::reset() {
		keys_.clear();
		for (auto key : db_.keys())
			keys_.insert(key);
		return *this;
	}

	//----< Query set of all keys matching a specified regular-expression >----------------------------
	template <typename T>
	Query<T>& Query<T>::selectByKey(const std::string s)
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		std::regex e(s);
		while (it != keys_.end())
		{
			if (!std::regex_match(*it, e))
				it = keys_.erase(it);
			else
				it++;
		}
		
		return *this;
	}

	
	//----< Query children keys of a specified key >---------------------------------

	template <typename T>
	Query<T>& Query<T>::selectChildren(const Key key)
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		Keys childkeys;
		while (it != keys_.end())
		{
			if (*it == key)
				for (auto childkey : db_[key].children()) {
					childkeys.insert(childkey);
				}
			it++;
		}
		keys_ = childkeys;
		return *this;
	}
	
	//----< Query by a condition object >---------------------------------

	template <typename T>
	Query<T>& Query<T>::select(const Condition<T>& c) 
	{
		selectByKey(c.key());
		selectByName(c.name());
		selectByDescrip(c.descrip());
		selectByTime(c.begintime(), c.endtime());
		selectByPayload(c.payload());

		return *this;
	}

	//----< Query for DbRecords by name metadata >---------------------------------
	template <typename T>
	Query<T>& Query<T>::selectByName(const std::string s) 
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		std::regex e(s);
		while (it != keys_.end()) 
		{
			if (!std::regex_match(db_[*it].name(), e))
				it = keys_.erase(it);
			else
				it++;
		}
		
		return *this;
	}
	
	//----< Query for DbRecords written within a specified time-date interval >--------------
	template <typename T>
	Query<T>& Query<T>::selectByDescrip(const std::string s) 
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		std::regex e(s);
		while (it != keys_.end()) 
		{
			if (!std::regex_match(db_[*it].descrip(), e))
				it = keys_.erase(it);
			else
				it++;
		}
		return *this;
	}

	//----< Query for DbRecords written within a specified time-date interval >--------------
	template <typename T>
	Query<T>& Query<T>::selectByTime(const DateTime begin, const DateTime end) 
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		while (it != keys_.end()) 
		{
			if (db_[*it].dateTime() >= begin && db_[*it].dateTime() <= end)
				it++;
			else
				it = keys_.erase(it);
		}
		return *this;
	}
	
	//----< Query by payload and template for comparing two payloads >----------------------
	template <typename T>
	bool compareT(T t1, T t2)
	{
		return t1.compare(t2);
	}

	//----< Specialized query for string type payload, since I cannot redefine std::string.compare()>-----

	template <>
	inline Query<std::string>& Query<std::string>::selectByPayload(std::string s)
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		if (s == "") return *this;
		while (it != keys_.end()) 
		{
			std::regex e(s);
			if (!std::regex_match(db_[*it].payLoad(), e))
				it = keys_.erase(it);
			else
				it++;
		}
		return *this;
	}
	
	//----< Query by payload>-------------------------------

	template <typename T>
	Query<T>& Query<T>::selectByPayload(T t) 
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		while (it != keys_.end())
		{
			if (!compareT(db_[*it].payLoad(), t))
				it = keys_.erase(it);
			else
				it++;
		
		}
		return *this;
	}

	//----< Intersection/And Operation of Queries >----------------------
	template <typename T>
	Query<T>& Query<T>::intersectQuery(const Query<T>& q) 
	{
		std::unordered_set<std::string>::iterator it = keys_.begin();
		while (it != keys_.end()) 
		{
			if (!q.keys_.count(*it))
				it = keys_.erase(it);
			else
				it++;
		}
		return *this;
	}

	//----< Union/Or Operation of Queries>----------------------------
	template <typename T>
	Query<T>& Query<T>::unionQuery(const Query<T>& q) 
	{
		for (auto key : q.keys_) 
		{
			keys_.insert(key);
		}
		return *this;
	}
	
	//----< Copy Keys from another Query>-----------------------
	template <typename T>
	Query<T>& Query<T>::from(const Query<T>& q) 
	{
		keys_.clear();
		for (auto key : q.keys_) 
		{
			keys_.insert(key);
		}
		return *this;
	}

	//----< Display values of all keys from a query >-----------------------
	template <typename T>
	void Query<T>::show() 
	{
		NoSqlDb::showHeader();
		for (auto key : keys_)
		{
			db_.showElem(db_[key]);
		}
		std::cout << "\n  ";
		std::cout << std::setw(26) << std::left << "------------------------";
		std::cout << std::setw(35) << std::left << "---------------------------------";
		std::cout << std::setw(35) << std::left << "---------------------------------";
		std::cout << std::setw(35) << std::left << "---------------------------------\n";
	}

	template <typename T>
	void Query<T>::showKeys() 
	{
		std::cout << "  Query Result (keys): ";
		if (keys_.size() != 0)
		{
			for (auto key : keys_)
			{
				std::cout << key << " ";
			}
			std::cout << "\n";
		}
		else
			std::cout << "No match.\n";
	}
	
	//----< write query result to a .xml file >-----------------------

	template<typename T>
	bool Query<T>::saveQuerytoXml(std::string xmlfilepath)
	{   // Reading xml data from a specified file
		std::ofstream XmlFileOut;
		XmlFileOut.open(xmlfilepath, std::ios::out);
		if (!XmlFileOut.is_open())
			return false;

		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		Sptr pDb = XmlProcessing::makeTaggedElement("DbCore");
		Sptr pDocElem = XmlProcessing::makeDocElement(pDb);
		XmlProcessing::XmlDocument xDoc(pDocElem);

		for (auto key : keys_)
		{
			Sptr pRecord = XmlProcessing::makeTaggedElement("DbRecord");
			pDb->addChild(pRecord);

			Sptr pKey = XmlProcessing::makeTaggedElement("key", key);
			pRecord->addChild(pKey);
			Sptr pDbElem = XmlProcessing::makeTaggedElement("DbElement");
			pRecord->addChild(pDbElem);

			Sptr pName = XmlProcessing::makeTaggedElement("name", db_[key].name());
			pDbElem->addChild(pName);
			Sptr pDescrip = XmlProcessing::makeTaggedElement("description", db_[key].descrip());
			pDbElem->addChild(pDescrip);
			Sptr pDateTime = XmlProcessing::makeTaggedElement("datetime", db_[key].dateTime());
			pDbElem->addChild(pDateTime);
			Sptr pPayLoad = XmlProcessing::makeTaggedElement("payload", db_[key].payLoad());
			pDbElem->addChild(pPayLoad);
			Sptr pChildren = XmlProcessing::makeTaggedElement("children");
			pDbElem->addChild(pChildren);
			for (auto childkey : db_[key].children()) {
				Sptr pChildKey = XmlProcessing::makeTaggedElement("childkey", childkey);
				pChildren->addChild(pChildKey);
			}
		}
		std::string Xml = xDoc.toString();
		XmlFileOut << Xml;
		XmlFileOut.close();
		return true;
	}
}


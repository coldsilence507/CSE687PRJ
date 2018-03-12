#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.1, Source(ver 1.0) from Jim Fawcett                       //
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

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "../DateTime/DateTime.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"



namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database

	template<typename T>
	class DbElement
	{
	public:
		using Key = std::string;
		using Children = std::unordered_set<Key>;

		// methods to get and set DbElement fields

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		DbElement<T>& name(const std::string& name) { name_ = name; return *this; }

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		DbElement<T>& descrip(const std::string& name) { descrip_ = name; return *this; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		DbElement<T>& dateTime(const DateTime& dateTime) { dateTime_ = dateTime; return *this; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		DbElement<T>& children(const Children& children) { children_ = children; return *this; }

		T& payLoad() { return payLoad_; }
		T payLoad() const { return payLoad_; }
		DbElement<T>& payLoad(const T& payLoad) { payLoad_ = payLoad; return *this; }

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		Children children_;
	    T payLoad_;
	};


	//template<typename


	/////////////////////////////////////////////////////////////////////
	// DbCore class
	// - provides core NoSql db operations
	// - does not provide editing, querying, or persistance operations

	template <typename T>
	class DbCore
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using Children = Keys;
		using DbStore = std::unordered_map<Key, DbElement<T>>;
		using iterator = typename DbStore::iterator;
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

		// methods to access database elements

		Keys keys();
		bool contains(const Key& key);
		size_t size();
		DbElement<T>& operator[](const Key& key);
		DbElement<T> operator[](const Key& key) const;
		typename iterator begin() { return dbStore_.begin(); }
		typename iterator end() { return dbStore_.end(); }
		void showElem(DbElement<T>& el, std::ostream& out = std::cout);
		void showDb(std::ostream& out = std::cout);

		// methods to get and set the private database hash-map storage

		DbStore& dbStore() { return dbStore_; }
		DbStore dbStore() const { return dbStore_; }
		void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

		// methods to add and delete key/value pairs 

		bool addRecord(const Key key, const DbElement<T>& DbElem);
		bool delRecord(const Key key);

		//methods to edit relationships and text metadata and replace DbElement

		DbCore<T>& editName(const Key key, const std::string name);
		DbCore<T>& editDescript(const Key key, const std::string descrip);
		DbCore<T>& editDatetime(const Key key, const DateTime time);
		DbCore<T>& editPayload(const Key key, const T t);
		DbCore<T>& addChild(const Key pkey, const Key ckey);
		DbCore<T>& delChild(const Key pkey, const Key ckey);
		DbCore<T>& replace(const Key key, const DbElement<T>& DbElem);



		// methods to restore from, augment from and write to a xml file 
		
		void Reset();
		bool RestoreFromXml(const std::string xmlfilepath);
		bool ReadFromXml(const std::string xmlfilepath);
		bool WriteToXml(const std::string xmlfilepath) const;

	private:
		DbStore dbStore_;

	};
     

	/////////////////////////////////////////////////////////////////////
	// DbCore<T> methods

	//----< does db contain this key? >----------------------------------

	template<typename T>
	bool DbCore<T>::contains(const Key& key)
	{
		iterator iter = dbStore_.find(key);
		return iter != dbStore_.end();
	}
	//----< returns current key set for db >-----------------------------

	template<typename T>
	typename DbCore<T>::Keys DbCore<T>::keys()
	{
		DbCore<T>::Keys dbKeys;
		DbStore& dbs = dbStore();
		size_t size = dbs.size();
		dbKeys.reserve(size);
		for (auto item : dbs)
		{
			dbKeys.push_back(item.first);
		}
		return dbKeys;
	}
	//----< return number of db elements >-------------------------------

	template<typename T>
	size_t DbCore<T>::size()
	{
		return dbStore_.size();
	}
	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes non-const db objects
	*/
	template<typename T>
	DbElement<T>& DbCore<T>::operator[](const Key& key)
	{
		if (!contains(key))
		{
			dbStore_[key] = DbElement<T>();
		}
		return dbStore_[key];
	}
	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes const db objects
	*/
	template<typename T>
	DbElement<T> DbCore<T>::operator[](const Key& key) const
	{
		if (!contains(key))
		{
			dbStore_[key] = DbElement<T>();
		}
		return dbStore_[key];
	}

	/////////////////////////////////////////////////////////////////////
	// display functions

	//----< display database key set >-----------------------------------

	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << " Database Keys: ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
		out << "\n  ";
	}
	//----< show record header items >-----------------------------------

	inline void showHeader(std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(35) << std::left << "Name";
		out << std::setw(35) << std::left << "Description";
		out << std::setw(35) << std::left << "Payload";
		out << "\n  ";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(35) << std::left << "---------------------------------";
		out << std::setw(35) << std::left << "---------------------------------";
		out << std::setw(35) << std::left << "---------------------------------";

	}
	//----< display DbElements >-----------------------------------------

	template<typename T>
	void DbCore<T>::showElem(DbElement<T>& el, std::ostream& out)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(35) << std::left << el.name();
		out << std::setw(35) << std::left << el.descrip();
		out << el.payLoad();

		if (el.children().size() > 0)
		{
			out << "\n    child keys: ";
			auto it = el.children().begin();
			while(it != el.children().end())
			{
				if (contains(*it))
				{
					out << " " << *it;
					it++;
				}
				else
					it = el.children().erase(it);
			}
		}
	}
	//----< display all records in database >----------------------------

	template<typename T>
	void DbCore<T>::showDb(std::ostream& out)
	{
		showHeader(out);
		typename DbCore<T>::DbStore dbs = dbStore();
		for (auto item : dbs)
		{
			showElem(dbStore_[item.first], out);
		}
		out << "\n  ";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(35) << std::left << "---------------------------------";
		out << std::setw(35) << std::left << "---------------------------------";
		out << std::setw(35) << std::left << "---------------------------------\n";
	}

	//----< addition of key/value pairs >----------------------------

	template <typename T>
	bool DbCore<T>::addRecord(const Key key, const DbElement<T>& DbElem) 
	{
		if (contains(key))
			return false;
		else 
		{
			dbStore_[key] = DbElem;
			return true;
		}
	}

	//----< deletion of key/value pairs >----------------------------

	template <typename T>
	bool DbCore<T>::delRecord(const Key key) 
	{
		auto it = dbStore_.find(key);
		if (it != dbStore_.end())
		{
			dbStore_.erase(it);
			return true;
		}
		else
			return false;
	}

	//----< edit name of a DbElement >----------------------------

	template <typename T>
	DbCore<T>& DbCore<T>::editName(const Key key, const std::string name) 
	{
		auto it = dbStore_.find(key);
		if (it != dbStore_.end())
		{
			dbStore_[key].name(name);
		}
		return *this;
	}

	//----< edit description of a DbElement >----------------------------

	template <typename T>
	DbCore<T>& DbCore<T>::editDescript(const Key key, const std::string descrip)
	{
		auto it = dbStore_.find(key);
		if (it != dbStore_.end())
		{
			dbStore_[key].descrip(descrip);
		}
		return *this;
	}

	//----< edit datetime of a DbElement >----------------------------
	template <typename T>
	DbCore<T>& DbCore<T>::editDatetime(const Key key, const DateTime time)
	{
		auto it = dbStore_.find(key);
		if (it != dbStore_.end())
		{
			dbStore_[key].dateTime(time);
		}
		return *this;
	}

	//----< edit payload of a DbElement >----------------------------
	template <typename T>
	DbCore<T>& DbCore<T>::editPayload(const Key key, const T t)
	{
		auto it = dbStore_.find(key);
		if (it != dbStore_.end())
		{
			dbStore_[key].payLoad(t);
		}
		return *this;
	}

	//----< add a child key to a DbElement >----------------------------
	template <typename T>
	DbCore<T>& DbCore<T>::addChild(const Key pkey, const Key ckey)
	{
		if (!dbStore_[pkey].children().count(ckey) && dbStore_.count(ckey))
		{
			dbStore_[pkey].children().insert(ckey);
		}
		return *this;
	}

	//----< delete a child key from a DbElement >----------------------------
	template <typename T>
	DbCore<T>& DbCore<T>::delChild(const Key pkey, const Key ckey)
	{
		auto it = dbStore_[pkey].children().find(ckey);
		if (it != dbStore_[pkey].children().end())
		{
			dbStore_[pkey].children().erase(it);
		}
		return *this;
	}

	//----< replace a DbElement with another DbElement>----------------------------
	template <typename T>
	DbCore<T>& DbCore<T>::replace(const Key key, const DbElement<T>& DbElem)
	{
		auto it = dbStore_.find(key);
		if (it != dbStore_.end())
		{
			it->second.name(DbElem.name());
			it->second.descrip(DbElem.descrip());
			it->second.dateTime(DbElem.dateTime());
			it->second.payLoad(DbElem.payLoad());
			it->second.children(DbElem.children());
		}
		//dbStore_[key] = DbElem;
		else
			std::cout << "\n Replace faild, key not found";
		return *this;
	}

	//----< Reset database >----------------------------

	template <typename T>
	void DbCore<T>::Reset() 
	{
		dbStore_.clear();
	}

	//----< Restore database from a .xml file >----------------------------
	template <typename T>
	bool DbCore<T>::RestoreFromXml(const std::string xmlfilepath) 
	{
		dbStore_.clear();
		return ReadFromXml(xmlfilepath);
	}

	//----< Read database from a .xml file >----------------------------
	inline std::string trim(std::string s) {
		s.erase(s.begin(),s.begin()+3);
		return s;
	}

	template<typename T>
	bool DbCore<T>::ReadFromXml(std::string xmlfilepath) 
	{   // Reading xml data from a specified file
		std::ifstream XmlFileIn;
		XmlFileIn.open(xmlfilepath, std::ios::in);
		if (!XmlFileIn.is_open()) 
			return false;
		std::string DbStr;
		DbStr.assign((std::istreambuf_iterator<char>(XmlFileIn)), (std::istreambuf_iterator<char>()));
		XmlFileIn.close();
		// copy data from xml to db
		XmlProcessing::XmlDocument XmlDoc(DbStr, XmlProcessing::XmlDocument::str);
		std::vector<Sptr> records = XmlDoc.descendents("DbRecord").select();
		for (auto pRecord : records)
		{
			Key key;
			DbElement<T> elem;
			std::vector<Sptr> Pairs = pRecord->children();
			for (auto pPair : Pairs)
			{
				if (pPair->tag() == "key")
					key = pPair->children()[0]->value();
				else{   
					std::vector<Sptr> pElemValues = pPair->children();
					for (auto pElemValue : pElemValues)
					{
						if (pElemValue->tag() == "name")
							elem.name(pElemValue->children()[0]->value());
						if (pElemValue->tag() == "description")
							elem.descrip(pElemValue->children()[0]->value());
						if (pElemValue->tag() == "datetime")
							elem.dateTime(pElemValue->children()[0]->value());
						if (pElemValue->tag() == "payload")
							elem.payLoad(trim(pElemValue->children()[0]->toString()));
						if (pElemValue->tag() == "children")
						{
							std::vector<Sptr> ChildKeys = pElemValue->children();
							for (auto pChildKey : ChildKeys) {
								if (pChildKey->tag() == "childkey")
									elem.children().insert(pChildKey->children()[0]->value());
							}
						}
					}
				}
			}
			dbStore_[key] = elem;
		}
		return true;
	}


	//----<Write database to a .xml file>----------------------------
	template<typename T>
	bool DbCore<T>::WriteToXml(std::string xmlfilepath) const
	{
		std::ofstream XmlFileOut;
		XmlFileOut.open(xmlfilepath, std::ios::out);
		if (!XmlFileOut.is_open())
			return false;
		Sptr pDb = XmlProcessing::makeTaggedElement("DbCore");
		Sptr pDocElem = XmlProcessing::makeDocElement(pDb);
		XmlProcessing::XmlDocument xDoc(pDocElem);
		
		for (auto item : dbStore_)
		{
			Sptr pRecord = XmlProcessing::makeTaggedElement("DbRecord");
			pDb->addChild(pRecord);

			Sptr pKey = XmlProcessing::makeTaggedElement("key", item.first);
			pRecord->addChild(pKey);
			Sptr pDbElem = XmlProcessing::makeTaggedElement("DbElement");
			pRecord->addChild(pDbElem);

			Sptr pName = XmlProcessing::makeTaggedElement("name", item.second.name());
			pDbElem->addChild(pName);
			Sptr pDescrip = XmlProcessing::makeTaggedElement("description", item.second.descrip());
			pDbElem->addChild(pDescrip);
			Sptr pDateTime = XmlProcessing::makeTaggedElement("datetime", item.second.dateTime());
			pDbElem->addChild(pDateTime);
			Sptr pPayLoad = XmlProcessing::makeTaggedElement("payload", item.second.payLoad());
			pDbElem->addChild(pPayLoad);
			Sptr pChildren = XmlProcessing::makeTaggedElement("children");
			pDbElem->addChild(pChildren);
			for (auto childkey : item.second.children()) {
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
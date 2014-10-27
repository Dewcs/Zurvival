#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Functions.h"
#include "tinyxml2.h"

enum type_t {
	TYPE_BOOL,
	TYPE_INT,
	TYPE_CHAR
};

class Config {
	private:
		std::map<std::string, int> intList; // storage for ints
		std::map<std::string, bool> boolList; // sotrage for bools
		std::map<std::string,const char *> stringList; // storage for strings
		char *fname; // file name to store or read data
		bool loaded; // true if the file was read for default purposes
	public:
		Config(char * fname);
		~Config();
		bool isLoaded(); // return the value of loaded

		void save(); // save to file
		void load(); // read from file

		bool getBool(const char * key); // get a bool value
		void setBool(const char * key, bool value); // set a bool value

		int getInt(const char * key); // get a int value
		void setInt(const char * key, int value); // set a int value

		const char* getChar(const char * key); // get a string value
		void setChar(const char * key, const char *value); // set a string value
};


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
		std::map<std::string, int> intList;
		std::map<std::string, bool> boolList;
		std::map<std::string,const char *> stringList;
		char *fname;
		bool loaded;
	public:
		Config(char * fname);
		~Config();
		bool isLoaded();

		void save();
		void load();

		bool getBool(const char * key);
		void setBool(const char * key, bool value);

		int getInt(const char * key);
		void setInt(const char * key, int value);

		const char* getChar(const char * key);
		void setChar(const char * key, const char *value);
};


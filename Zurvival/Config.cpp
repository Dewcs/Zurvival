#include "Config.h"

/*
	fname represents the file where the data will be stored or read
*/
Config::Config(char * fname) {
	this->fname = fname;
	loaded = false;
	// if exists load it
	if (fileExists(fname)) {
		load();
		loaded = true;
	}
}


Config::~Config() {
	// store before closing 
	save();
	intList.clear();
	stringList.clear();
	boolList.clear();
}

bool Config::isLoaded() {
	return loaded;
}

void Config::save() {
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	printer.OpenElement("zurvival");
	// config
	printer.OpenElement("config");
	// add bool
	for (std::map<std::string, bool>::iterator it = boolList.begin(); it != boolList.end(); ++it) {
		printer.OpenElement("element");
		printer.PushAttribute("key", it->first.c_str());
		printer.PushAttribute("type", TYPE_BOOL);
		printer.PushText(it->second);
		printer.CloseElement();
	}
	// add int
	for (std::map<std::string, int>::iterator it = intList.begin(); it != intList.end(); ++it) {
		printer.OpenElement("element");
		printer.PushAttribute("key", it->first.c_str());
		printer.PushAttribute("type", TYPE_INT);
		printer.PushText(it->second);
		printer.CloseElement();
	}
	// add string
	for (std::map<std::string, const char *>::iterator it = stringList.begin(); it != stringList.end(); ++it) {
		printer.OpenElement("element");
		printer.PushAttribute("key", it->first.c_str());
		printer.PushAttribute("type", TYPE_INT);
		printer.PushText(it->second);
		printer.CloseElement();
	}
	printer.CloseElement();

	printer.CloseElement();
	doc.Parse(printer.CStr());
	doc.SaveFile(fname);
}

void Config::load() {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fname);
	// config part
	tinyxml2::XMLNode *rootnode = doc.FirstChild();
	tinyxml2::XMLNode *confignode = rootnode->FirstChild();
	for (tinyxml2::XMLNode* node = confignode->FirstChild(); node; node = node->NextSibling()) {
		const tinyxml2::XMLElement *element = node->ToElement();
		int type;
		element->QueryIntAttribute("type", &type);
		const char * key = element->Attribute("key");
		switch (type) {
			case TYPE_BOOL:
				bool btmp;
				element->QueryBoolText(&btmp);
				setBool(key, btmp);
				break;
			case TYPE_INT:
				int itmp;
				element->QueryIntText(&itmp);
				setInt(key, itmp);
				break;
			case TYPE_CHAR:
				setChar(key, element->GetText());
				break;
			default:
				break;
		} 
	}
	/*
	how to query float and double
	pointElement->FirstChildElement( "x" )->QueryFloatText( &floatValue );
	pointElement->FirstChildElement( "x" )->QueryDoubleText( &doubleValue );
	*/
}

bool Config::getBool(const char * key) {
	if (boolList.find(key) == boolList.end()) std::cerr << "Uknown bool key" << key << std::endl;
	return boolList[key];
}
void Config::setBool(const char * key, bool value) {
	boolList[key] = value;
}

int Config::getInt(const char * key) {
	if (intList.find(key) == intList.end()) std::cerr << "Uknown int key" << key << std::endl;
	return intList[key];
}
void Config::setInt(const char * key, int value) {
	intList[key] = value;
}

const char* Config::getChar(const char * key) {
	if (stringList.find(key) == stringList.end()) std::cerr << "Uknown string key" << key << std::endl;
	return stringList[key];
}
void Config::setChar(const char * key, const char *value) {
	stringList[key] = value;
}

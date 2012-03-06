/*
 * GrubConfigObject.cpp
 *
 *  Created on: 5 Mar 2012
 *      Author: niall
 */

//#define DEBUG_GRUBCONFIGOBJECT
#include "GrubConfigObject.h"
#include "MainDefs.h"
#include <iostream>
#include <fstream>

namespace remenu {

GrubConfigObject::GrubConfigObject(const std::string config_file) :
		configFile(config_file) {
	this->initialiseConfigKeys();
#ifdef DEBUG_GRUBCONFIGOBJECT
	std::cout << "GrubConfigObject::GrubConfigObject: " << "DEBUG: configFile: " << configFile << " configKeys: "
			<< configKeys.size() << std::endl;
#endif
}

GrubConfigObject::~GrubConfigObject() {
}

void GrubConfigObject::initialiseConfigKeys() {
	configKeys.clear();
	configKeys[GRUB_MENUENTRY_TEXT] = std::list<std::string>();
}

void GrubConfigObject::parseConfig() {
	std::ifstream ifs;
	ifs.open(configFile.c_str());
	if (ifs.is_open()) {

		std::string line;
		this->initialiseConfigKeys();
		while (getline(ifs, line)) {
			// forall in configKeys
			{
				std::list<std::string> key_lines;
				std::map<std::string, std::list<std::string> >::iterator it_configKeys = configKeys.begin();
				const std::map<std::string, std::list<std::string> >::const_iterator it_configKeys_end =
						configKeys.end();
				while (it_configKeys != it_configKeys_end) {
					size_t found = line.find(it_configKeys->first);
					if (found != std::string::npos) {
#ifdef DEBUG_GRUBCONFIGOBJECT
						std::cout << "GrubConfigObject::parseConfig: " << "DEBUG: configKeys[" << it_configKeys->first
								<< "]: " << it_configKeys->second.size() << " added " << "'" << line << "'"
								<< std::endl;
#endif
						it_configKeys->second.push_back(line);
					}
					++it_configKeys;
				}
			}
		}
		ifs.close();
	} else {
		std::cout << "GrubConfigObject::parseConfig: " << "ERROR opening file: " << configFile << std::endl;
	}

	// now parse the raw lines
	this->parseMenuEntries();
}

void GrubConfigObject::parseMenuEntries() {
// get each lie from grub.cfg and scan through for menu entries
	// creating a MenuEntry from each one
#ifdef DEBUG_GRUBCONFIGOBJECT
	std::cout << "GrubConfigObject::parseMenuEntries: " << "DEBUG: " << std::endl;
#endif
	std::list<std::string> lines = this->getKeyRawLines(GRUB_MENUENTRY_TEXT);
	if (lines.size() > 0) {
		std::string entry;
		std::list<std::string> entries;
		// forall in lines
		{
			std::list<std::string>::const_iterator it_lines = lines.begin();
			const std::list<std::string>::const_iterator it_lines_end = lines.end();
			while (it_lines != it_lines_end) {
				size_t found = it_lines->find(GRUB_MENUENTRY_TEXT);
				if (found != std::string::npos) {
					size_t first_quote;
					size_t second_quote;
					first_quote = it_lines->find_first_of("'");
					if (first_quote != std::string::npos) {
						second_quote = it_lines->find("'", first_quote + 1);
					} else {
						first_quote = it_lines->find_first_of("\"");
						second_quote = it_lines->find("\"", first_quote + 1);
					}
					entry = it_lines->substr(first_quote + 1, second_quote - first_quote - 1);
#ifdef DEBUG_GRUBCONFIGOBJECT
					std::cout << "GrubConfigObject::parseMenuEntries: " << "DEBUG: Found entry: " << entry << std::endl;
#endif
					entries.push_back(entry);
				}
				++it_lines;
			}

		}
#ifdef DEBUG_GRUBCONFIGOBJECT
		std::cout << "GrubConfigObject::parseMenuEntries: " << "DEBUG: entries: " << entries.size() << std::endl;
#endif
		configEntries[GRUB_MENUENTRY_TEXT] = entries;

	} else {
		std::cout << "GrubConfigObject::parseMenuEntries: " << "INFO: No " << GRUB_MENUENTRY_TEXT << "'s found to parse"
				<< std::endl;
	}

}

const std::list<std::string> GrubConfigObject::getKeyRawLines(const std::string & key) const {
	return (this->getMapValuesByKey(configKeys, key));
}

const std::list<std::string> GrubConfigObject::getEntryValues(const std::string & key) const {
	return (this->getMapValuesByKey(configEntries, key));
}

const std::list<std::string> GrubConfigObject::getEntryOptions(const std::string & key) const {
	return (this->getMapValuesByKey(configEntryOptions, key));
}

const std::map<std::string, std::list<std::string> > & GrubConfigObject::getConfigEntries() const {
	return configEntries;
}

const std::map<std::string, std::list<std::string> > & GrubConfigObject::getConfigEntryOptions() const {
	return configEntryOptions;
}

std::string GrubConfigObject::getConfigFile() const {
	return configFile;
}

const std::map<std::string, std::list<std::string> > & GrubConfigObject::getConfigKeys() const {
	return configKeys;
}

const std::list<std::string> GrubConfigObject::getMapValuesByKey(
		const std::map<std::string, std::list<std::string> > & map , const std::string & key) const {
	std::map<std::string, std::list<std::string> >::const_iterator it_found = map.find(key);
	if (it_found != map.end()) {
		return it_found->second;
	}
	return std::list<std::string>();
}
} /* namespace remenu */

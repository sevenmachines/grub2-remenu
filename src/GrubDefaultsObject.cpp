/*
 * GrubDefaultsObject.cpp
 *
 *  Created on: 6 Mar 2012
 *      Author: niall
 */

#define DEBUG_GRUBDEFAULTSOBJECT

#include "GrubDefaultsObject.h"
#include <iostream>
#include <fstream>

namespace remenu {

GrubDefaultsObject::GrubDefaultsObject(const std::string config_file) :
		configFile(config_file) {

}

GrubDefaultsObject::~GrubDefaultsObject() {
	// TODO Auto-generated destructor stub
}

void GrubDefaultsObject::parseConfig() {
	std::cout << "GrubDefaultsObject::parseConfig: " << "" << std::endl;
	std::ifstream ifs;
	variableMap.clear();

	ifs.open(configFile.c_str());
	if (ifs.is_open()) {
		std::string line;
		while (getline(ifs, line)) {
			std::pair<std::string, std::string> entry = this->getVariableEntry(line);

			if (entry.first != "") {
				variableMap[entry.first] = entry.second;
			}
		}
	} else {
		std::cout << "GrubConfigObject::parseConfig: " << "ERROR opening file: " << configFile << std::endl;
	}
}
std::pair<std::string, std::string> GrubDefaultsObject::getVariableEntry(const std::string & line) {
	std::cout << "GrubDefaultsObject::getVariableEntry: " << "" << std::endl;
	// very basic formatting, not to be trusted
	// try to err on return nothing if not sure its legal
	std::string var;
	std::string val;
	size_t eq_point = line.find("=");
	size_t start_point = line.find_first_not_of("#");
	if (eq_point != std::string::npos) {
		var = line.substr(start_point, eq_point - start_point);
		val = line.substr(eq_point + 1);
	} else {
		std::cout << "GrubDefaultsObject::getVariableEntry: " << "INFO: Ignoring: " << line << std::endl;
	}
	std::cout << "GrubDefaultsObject::getVariableEntry: " << var << " : " << val << std::endl;
	return std::pair<std::string, std::string>(var, val);
}

const std::map<std::string, std::string> & GrubDefaultsObject::getVariableMap() const {
	return variableMap;
}

} /* namespace remenu */

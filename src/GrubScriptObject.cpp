/*
 * GrubScriptObject.cpp
 *
 *  Created on: 5 Mar 2012
 *      Author: niall
 */

#include "GrubScriptObject.h"
#include "MainDefs.h"
#include <iostream>
#include <sstream>
#include <fstream>

namespace remenu {

GrubScriptObject::GrubScriptObject(const std::string scriptin) {
	this->parseScript(scriptin);
}

void GrubScriptObject::parseScript(const std::string script) {
	// clear this object
	this->clear();

	// check script is active and readable
	if (this->checkFilePermissions(script), R_OK | X_OK) {

		// begin file parse stage
		std::ifstream ifs;
		ifs.open(script.c_str());
		if (ifs.is_open()) {

			std::string line;
			while (getline(ifs, line)) {
				rawScript.push_back(line);
				std::pair<std::string, std::string> entry = this->getRenameEntryFromLine(line);
				if (entry.first != "") {
					renames[entry.first] = entry.second;
				}
			}
			ifs.close();
		} else {
			std::cout << "GrubScriptObject::generateGrubScript: " << "Error opening file " << script << std::endl;
		}
	}
}
GrubScriptObject::~GrubScriptObject() {
}

std::pair<std::string, std::string> GrubScriptObject::getRename(const std::string obj1) {
	std::pair<std::string, std::string> found_str("", "");
	std::map<std::string, std::string>::const_iterator it_found = renames.find(obj1);
	if (it_found != renames.end()) {
		found_str.first = it_found->first;
		found_str.second = it_found->second;
	}
	return found_str;
}

std::pair<std::string, std::string> GrubScriptObject::getReverseRename(const std::string obj1) {
	std::pair<std::string, std::string> found_str("", "");
	bool found = false;
// forall in map
	{
		std::map<std::string, std::string>::const_iterator it_map = renames.begin();
		const std::map<std::string, std::string>::const_iterator it_map_end = renames.end();
		while ((it_map != it_map_end) && found == false) {
			if (it_map->second == obj1) {
				found_str.first = it_map->first;
				found_str.second = it_map->second;
			}
			++it_map;

		}
	}
	return found_str;
}

void GrubScriptObject::setRename(const std::string obj1, const std::string obj2) {
	renames[obj1] = obj2;
}

bool GrubScriptObject::clearRename(const std::string obj1) {
	std::map<std::string, std::string>::iterator it_found = renames.find(obj1);
	bool is_found = false;
	if (it_found != renames.end()) {
		renames.erase(it_found);
		is_found = true;
	}
	return is_found;
}

void GrubScriptObject::generateGrubScript(const std::string file_path) {
	std::ofstream ofs;
	rawScript.clear();
	ofs.open(file_path.c_str());
	if (ofs.is_open()) {
		// generate script thatll do the job of renaming
		rawScript.push_back("#!/bin/sh ");
		rawScript.push_back("echo \"Renaming entries....\" >&2 ");
		rawScript.push_back(this->getRenameScript());
		// forall in rawScript
		{
			std::list<std::string>::const_iterator it_rawScript = rawScript.begin();
			const std::list<std::string>::const_iterator it_rawScript_end = rawScript.end();
			while (it_rawScript != it_rawScript_end) {
				ofs << *it_rawScript << std::endl;
				++it_rawScript;
			}
		}
		ofs.close();
		this->setFilePermissions(file_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
	} else {
		std::cout << "GrubScriptObject::generateGrubScript: " << "Error opening file " << file_path << std::endl;
	}
}

void GrubScriptObject::setFilePermissions(const std::string file, int perms) const {
	int code = chmod(file.c_str(), perms);
	if (code != 0) {
		std::cout << "GrubScriptObject::setFilePermissions: " << "Error setting file permissions on " << file << " ("
				<< perms << ")" << " Errcode: " << code << std::endl;
	}
}

bool GrubScriptObject::checkFilePermissions(const std::string file, int perms) {
	int check = access(file.c_str(), perms);
	bool success = false;
	if (check == 0) {
		success = true;
	}
	return success;
}

std::string GrubScriptObject::getRenameScript() const {
	std::stringstream ss;
	// forall in map
	{
		std::map<std::string, std::string>::const_iterator it_map = renames.begin();
		const std::map<std::string, std::string>::const_iterator it_map_end = renames.end();
		while (it_map != it_map_end) {
			ss << "sed -i 's|" << (it_map->first) << "|" << (it_map->second) << "|g' " << GRUB_CFG_TMP_PATH << ";"
					<< std::endl;
			++it_map;
		}
	}
	return ss.str();
}

std::pair<std::string, std::string> GrubScriptObject::getRenameEntryFromLine(const std::string line) {
	std::pair<std::string, std::string> entry("", "");

	// check for menu entries
	{
		size_t found = line.find("sed");
		if (found != std::string::npos) {
			size_t sedpost1 = line.find_first_of("|");
			if (found != std::string::npos) {
				size_t sedpost2 = line.find_first_of("|", sedpost1 + 1);
				if (found != std::string::npos) {
					size_t sedpost3 = line.find_first_of("|", sedpost2+1);
					if (found != std::string::npos) {
						//extract the rename
						std::string key = line.substr(sedpost1 + 1, sedpost2 - sedpost1-1);
						std::string var = line.substr(sedpost2+1, sedpost2 - sedpost3-1);
						entry.first = key;
						entry.second = var;
					} else {
						std::cout << "GrubScriptObject::getRenameEntryFromLine: " << "Error parsing script line: "
								<< line << std::endl;
					}
				} else {
					std::cout << "GrubScriptObject::getRenameEntryFromLine: " << "Error parsing script line: " << line
							<< std::endl;
				}
			} else {
				std::cout << "GrubScriptObject::getRenameEntryFromLine: " << "Error parsing script line: " << line
						<< std::endl;
			}
		}
	} //end menu check
	return entry;
}

void GrubScriptObject::clear() {
	renames.clear();
	rawScript.clear();
}


const std::list<std::string> & GrubScriptObject::getRawScript() const {
	return rawScript;
}
} /* namespace remenu */

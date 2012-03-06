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

GrubScriptObject::GrubScriptObject() {

}

GrubScriptObject::~GrubScriptObject() {
}

std::pair<std::string, std::string> GrubScriptObject::getRenameMenu(const std::string obj1) {
	return this->getRename(obj1, this->menuRenames);
}
std::pair<std::string, std::string> GrubScriptObject::getReverseRenameMenu(const std::string obj1) {
	return this->getReverseRename(obj1, this->menuRenames);
}
void GrubScriptObject::setRenameMenu(const std::string obj1, const std::string obj2) {
	this->setRename(obj1, obj2, this->menuRenames);
}

bool GrubScriptObject::clearRenameMenu(const std::string obj1) {
	return this->clearRename(obj1, menuRenames);
}

std::pair<std::string, std::string> GrubScriptObject::getRenameMisc(const std::string obj1) {
	return this->getRename(obj1, miscRenames);
}

std::pair<std::string, std::string> GrubScriptObject::getReverseRenameMisc(const std::string obj1) {
	return this->getReverseRename(obj1, miscRenames);
}

void GrubScriptObject::setRenameMisc(const std::string obj1, const std::string obj2) {
	this->setRename(obj1, obj2, miscRenames);
}

bool GrubScriptObject::clearRenameMisc(const std::string obj1) {
	return this->clearRename(obj1, miscRenames);
}

std::pair<std::string, std::string> GrubScriptObject::getRename(const std::string obj1,
		std::map<std::string, std::string> & map) {
	std::pair<std::string, std::string> found_str("", "");
	std::map<std::string, std::string>::const_iterator it_found = map.find(obj1);
	if (it_found != map.end()) {
		found_str.first = it_found->first;
		found_str.second = it_found->second;
	}
	return found_str;
}

std::pair<std::string, std::string> GrubScriptObject::getReverseRename(const std::string obj1,
		std::map<std::string, std::string> & map) {
	std::pair<std::string, std::string> found_str("", "");
	bool found = false;
// forall in map
	{
		std::map<std::string, std::string>::const_iterator it_map = map.begin();
		const std::map<std::string, std::string>::const_iterator it_map_end = map.end();
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

void GrubScriptObject::setRename(const std::string obj1, const std::string obj2,
		std::map<std::string, std::string> & map) {
	map[obj1] = obj2;
}

bool GrubScriptObject::clearRename(const std::string obj1, std::map<std::string, std::string> & map) {
	std::map<std::string, std::string>::iterator it_found = map.find(obj1);
	bool is_found = false;
	if (it_found != map.end()) {
		map.erase(it_found);
		is_found = true;
	}
	return is_found;
}

const std::map<std::string, std::string> & GrubScriptObject::getMenuRenames() {
	return menuRenames;
}

const std::map<std::string, std::string> & GrubScriptObject::getMiscRenames() {
	return miscRenames;
}

void GrubScriptObject::generateGrubScript(const std::string file_path) {
	std::ofstream ofs;
	rawScript.clear();
	ofs.open(file_path.c_str());
	if (ofs.is_open()) {
		// generate script thatll do the job of renaming
		rawScript.push_back("#!/bin/sh ");
		rawScript.push_back("echo \"Renaming entries....\" >&2 ");
		rawScript.push_back(this->getRenameMenuScript());
		rawScript.push_back("echo \"Renaming miscallaneous....\" >&2 ");
		rawScript.push_back(this->getRenameMiscScript());
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

std::string GrubScriptObject::getRenameMenuScript() const {
	return this->getRenameScript(menuRenames);
}
std::string GrubScriptObject::getRenameMiscScript() const {
	return this->getRenameScript(miscRenames);
}

std::string GrubScriptObject::getRenameScript(const std::map<std::string, std::string> & map) const {
	std::stringstream ss;
	// forall in map
	{
		std::map<std::string, std::string>::const_iterator it_map = map.begin();
		const std::map<std::string, std::string>::const_iterator it_map_end = map.end();
		while (it_map != it_map_end) {
			ss << "sed -i 's|" << (it_map->first) << "|" << (it_map->second) << "|g' " << GRUB_CFG_TMP_PATH << ";"
					<< std::endl;
			++it_map;
		}
	}
	return ss.str();
}

void GrubScriptObject::clear() {
	menuRenames.clear();
	miscRenames.clear();
}

const std::list<std::string> & GrubScriptObject::getRawScript() const {
	return rawScript;
}
} /* namespace remenu */

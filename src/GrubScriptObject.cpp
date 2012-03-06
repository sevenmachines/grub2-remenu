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

namespace remenu {

GrubScriptObject::GrubScriptObject() {

}

GrubScriptObject::~GrubScriptObject() {
}

std::pair<std::string, std::string> GrubScriptObject::getRenameMenu(const std::string obj1) {
	return this->getRename(obj1, menuRenames);
}

void GrubScriptObject::setRenameMenu(const std::string obj1, const std::string obj2) {
	this->setRename(obj1, obj2, menuRenames);
}

bool GrubScriptObject::clearRenameMenu(const std::string obj1) {
	return this->clearRename(obj1, menuRenames);
}

std::pair<std::string, std::string> GrubScriptObject::getRenameMisc(const std::string obj1) {
	return this->getRename(obj1, miscRenames);
}

void GrubScriptObject::setRenameMisc(const std::string obj1, const std::string obj2) {
	this->setRename(obj1, obj2, miscRenames);
}

bool GrubScriptObject::clearRenameMisc(const std::string obj1) {
	return this->clearRename(obj1, miscRenames);
}

std::pair<std::string, std::string> GrubScriptObject::getRename(const std::string obj1, std::map<std::string, std::string> & map) {
	std::pair<std::string, std::string> found_str("", "");
	std::map<std::string, std::string>::const_iterator it_found = map.find(obj1);
	if (it_found!=map.end()){
		found_str.first=it_found->first;
		found_str.second=it_found->second;
	}
	return found_str;
}

void GrubScriptObject::setRename(const std::string obj1, const std::string obj2,
		std::map<std::string, std::string> & map) {
}

bool GrubScriptObject::clearRename(const std::string obj1, std::map<std::string, std::string> & map) {
	std::map<std::string, std::string>::iterator it_found = map.find(obj1);
	bool is_found=false;
	if (it_found!=map.end()){
		map.erase(it_found);
		is_found=true;
	}
	return is_found;
}

const std::map<std::string, std::string> & GrubScriptObject::getMenuRenames() {
	return menuRenames;
}

const std::map<std::string, std::string> & GrubScriptObject::getMiscRenames() {
	return miscRenames;
}

std::string GrubScriptObject::getFullRenameScript() const {
	// generate script thatll do the job of renaming
	std::stringstream ss;
	ss << "#!/bin/sh " << std::endl;
	ss << "echo \"Renaming entries....\" >&2 " << std::endl;
	ss << this->getRenameMenuScript() << std::endl;
	ss << "echo \"Renaming miscallaneous....\" >&2 " << std::endl;
	ss << this->getRenameMiscScript() << std::endl;
	return ss.str();
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

void GrubScriptObject::clear(){
	menuRenames.clear();
	miscRenames.clear();
}
} /* namespace remenu */

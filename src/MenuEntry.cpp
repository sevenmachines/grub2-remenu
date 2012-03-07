/*
 * MenuEntry.cpp
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#include "MenuEntry.h"
#include <iostream>

namespace remenu {

MenuEntry::MenuEntry() :
		GenericEntry() {
}
MenuEntry::MenuEntry(std::string key, std::string val, bool activate) :
		GenericEntry(key, val, activate) {
}

MenuEntry::~MenuEntry() {
	// TODO Auto-generated destructor stub
}

void MenuEntry::on_enabledCheckBox_clicked() {
	std::cout << "MenuEntry::on_enabledCheckBox_clicked: " << "" << std::endl;
}

} /* namespace remenu */

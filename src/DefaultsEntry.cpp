/*
 * DefaultsEntry.cpp
 *
 *  Created on: 6 Mar 2012
 *      Author: niall
 */

#include "DefaultsEntry.h"
#include <iostream>

namespace remenu {

DefaultsEntry::DefaultsEntry(const std::string & key, const std::string & val, const bool activate) :GenericEntry(key, val, activate){
}

DefaultsEntry::~DefaultsEntry() {
}

void DefaultsEntry::on_enableCheckButton_clicked() {
	std::cout << "DefaultsEntry::on_enableCheckButton_clicked: " << enableCheckButton->get_active() << std::endl;
}
} /* namespace remenu */

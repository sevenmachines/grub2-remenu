/*
 * GenericEntry.cpp
 *
 *  Created on: 7 Mar 2012
 *      Author: niall
 */

#include "GenericEntry.h"
#include <iostream>

namespace remenu {

GenericEntry::GenericEntry(){
	this->initialise();
}
GenericEntry::GenericEntry(const std::string & key, const std::string & val, const bool activate) {
	this->initialise(key, val, activate);
}

GenericEntry::~GenericEntry() {
}

std::string GenericEntry::getKeyEntryText() {
	return (keyEntry->get_text());
}
void GenericEntry::setKeyEntryText(const std::string str) {
	keyEntry->set_text(str);
}

std::string GenericEntry::getValueEntryText() {
	return (valueEntry->get_text());
}

void GenericEntry::setValueEntryText(const std::string str) {
	valueEntry->set_text(str);
}

bool GenericEntry::isEnabled() const {
	return (enableCheckButton->get_active());
}

void GenericEntry::setEnabled(bool b) {
	enableCheckButton->set_active(b);
}

void GenericEntry::initialise(const std::string & key, const std::string & val, const bool activate){
	keyEntry = new Gtk::Entry();
	valueEntry = new Gtk::Entry();
	enableCheckButton = new Gtk::CheckButton();

	keyEntry->set_editable(false);
	this->enableCheckButton->signal_clicked().connect(
			sigc::mem_fun(*this, &GenericEntry::on_enableCheckButton_clicked));

	keyEntry->set_text(key);
	valueEntry->set_text(val);

	enableCheckButton->set_active(activate);

	this->pack_start(*keyEntry);
	this->pack_start(*valueEntry);
	this->pack_end(*enableCheckButton, Gtk::PACK_SHRINK);
	this->show();
}
void GenericEntry::on_enableCheckButton_clicked() {
	std::cout << "GenericEntry::on_enableCheckButton_clicked: " << enableCheckButton->get_active() << std::endl;
}
} /* namespace remenu */

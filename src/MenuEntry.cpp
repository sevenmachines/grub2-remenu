/*
 * MenuEntry.cpp
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#include "MenuEntry.h"

namespace remenu {

MenuEntry::MenuEntry(std::string current_ent, std::string new_ent, bool enable) {
	currentEntryText = new Gtk::Entry();
	newEntryText = new Gtk::Entry();
	enabledCheckBox = new Gtk::CheckButton();

	enabledCheckBox->signal_clicked().connect(sigc::mem_fun(*this, &MenuEntry::on_enabledCheckBox_clicked));

	this->setCurrentEntry(current_ent);
	this->setNewEntry(new_ent);
	this->setEnabled(enable);

	this->pack_start(*currentEntryText);
	this->pack_start(*newEntryText);
	this->pack_start(*enabledCheckBox);
	this->show();

}

MenuEntry::~MenuEntry() {
	// TODO Auto-generated destructor stub
}

std::string MenuEntry::getCurrentEntry() const {
	return currentEntryText->get_text();
}
std::string MenuEntry::getNewEntry() const {
	return newEntryText->get_text();
}

bool MenuEntry::isEnabled() const {
	return enabledCheckBox->get_active();
}

void MenuEntry::setCurrentEntry(std::string s) {
	currentEntryText->set_text(s);
}
void MenuEntry::setNewEntry(std::string s) {
	newEntryText->set_text(s);
}
void MenuEntry::setEnabled(bool b) {
	enabledCheckBox->set_active(b);
}

void MenuEntry::on_enabledCheckBox_clicked(){
	this->setEnabled(enabledCheckBox->get_active());
}

} /* namespace remenu */

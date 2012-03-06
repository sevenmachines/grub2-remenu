/*
 * DefaultsEntry.cpp
 *
 *  Created on: 6 Mar 2012
 *      Author: niall
 */

#include "DefaultsEntry.h"
#include <iostream>

namespace remenu {

DefaultsEntry::DefaultsEntry(const std::string & key, const std::string & val, const bool activate) {
	keyEntry = new Gtk::Entry();
	valEntry = new Gtk::Entry();
	applyButton = new Gtk::CheckButton();

	keyEntry->set_editable(false);
	this->applyButton->signal_clicked().connect(sigc::mem_fun(*this, &DefaultsEntry::on_applyButton_clicked));

	keyEntry->set_text(key);
	valEntry->set_text(val);

	applyButton->set_active(activate);

	this->pack_start(*keyEntry);
	this->pack_start(*valEntry);
	this->pack_end(*applyButton, Gtk::PACK_SHRINK);
	this->show();

}

DefaultsEntry::~DefaultsEntry() {
	// TODO Auto-generated destructor stub
}

void DefaultsEntry::on_applyButton_clicked() {
	std::cout << "DefaultsEntry::on_applyButton_clicked: " << applyButton->get_active() << std::endl;
}
} /* namespace remenu */

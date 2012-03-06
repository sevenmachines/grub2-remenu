/*
 * DefaultsEntry.h
 *
 *  Created on: 6 Mar 2012
 *      Author: niall
 */

#ifndef DEFAULTSENTRY_H_
#define DEFAULTSENTRY_H_

#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/entry.h>

namespace remenu {

class DefaultsEntry: public Gtk::HBox {
public:
	DefaultsEntry(const std::string & key, const std::string & val, const bool activate);
	virtual ~DefaultsEntry();
protected:
	Gtk::Entry * keyEntry;
	Gtk::Entry * valEntry;
	Gtk::CheckButton * applyButton;

	void on_applyButton_clicked();

};

} /* namespace remenu */
#endif /* DEFAULTSENTRY_H_ */

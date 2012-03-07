/*
 * GenericEntry.h
 *
 *  Created on: 7 Mar 2012
 *      Author: niall
 */

#ifndef GENERICENTRY_H_
#define GENERICENTRY_H_

#include <string>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/entry.h>

namespace remenu {

class GenericEntry : public Gtk::HBox {
public:
	GenericEntry();
	GenericEntry(const std::string & key, const std::string & val, const bool activate);
	virtual ~GenericEntry();

	std::string getKeyEntryText();
	void setKeyEntryText(const std::string str);
	std::string getValueEntryText();
	void setValueEntryText(const std::string str);

	bool isEnabled()const;
	void setEnabled(bool b);

protected:
	Gtk::Entry * keyEntry;
	Gtk::Entry * valueEntry;
	Gtk::CheckButton * enableCheckButton;

	virtual void on_enableCheckButton_clicked();
	void initialise(const std::string & key = "", const std::string & val ="", const bool activate = false);
};

} /* namespace remenu */
#endif /* GENERICENTRY_H_ */

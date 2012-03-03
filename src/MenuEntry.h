/*
 * MenuEntry.h
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#ifndef MENUENTRY_H_
#define MENUENTRY_H_

#include <string>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>

namespace remenu {

class MenuEntry : public Gtk::HBox{
public:
	MenuEntry(std::string current_ent = "", std::string new_ent = "", bool enable = false);
	virtual ~MenuEntry();

	std::string getCurrentEntry() const;
	std::string getNewEntry() const;
	bool isEnabled() const;
	void setCurrentEntry(std::string) ;
	void setNewEntry(std::string) ;
	void setEnabled(bool b) ;

protected:
	Gtk::Entry * currentEntryText;
	Gtk::Entry * newEntryText;
	Gtk::CheckButton * enabledCheckBox;

	void on_enabledCheckBox_clicked();

};

} /* namespace remenu */
#endif /* MENUENTRY_H_ */

/*
 * MainWindow.cpp
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

//#define DEBUG_MAINWINDOW
#include "MainWindow.h"
#include "MainDefs.h"
#include <gtkmm.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <sys/stat.h>
#include <cstdlib>
#include <libgksu.h>

namespace remenu {

MainWindow::MainWindow(int argc, char **argv) :
		configObject(GRUB_CFG_PATH), defaultsObject(GRUB_DEFAULTS_PATH), scriptObject(GRUBD_SCRIPT_PATH) {
	Gtk::Main kit(argc, argv);

	builder = Gtk::Builder::create();

	try {
		std::string gladepath = DATADIR + "remenu.ui";
		builder->add_from_file(gladepath);
		if (builder) {
			if (this->initialise()) {
				kit.run(*main_window);
			}
		}
	} catch (const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
	} catch (const Glib::MarkupError& ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
	} catch (const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
	}


	this->generateMenuEntries();

}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

bool MainWindow::initialise() {
	bool success = false;
	// get pointers to all the widgets we need from the builder
	builder->get_widget("main_window", main_window);
	builder->get_widget("menuentry_vbox", menuentry_vbox);
	builder->get_widget("defaults_vbox", defaults_vbox);
	builder->get_widget("refresh_button", refresh_button);
	builder->get_widget("grub_textview", grub_textview);
	builder->get_widget("script_textview", script_textview);

	grub_textbuffer = Gtk::TextBuffer::create();
	script_textbuffer = Gtk::TextBuffer::create();
	script_textview->set_buffer(script_textbuffer);
	grub_textview->set_buffer(grub_textbuffer);

	main_window->override_background_color(Gdk::RGBA("black"));
	// connect up widget signals
	refresh_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_refresh_button_clicked));

	success = (main_window != 0) && (menuentry_vbox != 0) && (refresh_button != 0);

	success = success && this->generateMenuEntries();
	main_window->show();

	// refresh all the views
	this->updateScriptViewText();

	configObject.parseConfig();
	defaultsObject.parseConfig();
	this->updateDefaultsView();
	this->updateGrubViewText();
	this->generateMenuEntries();

	return success;
}

void MainWindow::on_refresh_button_clicked() {
#ifdef DEBUG_MAINWINDOW
	std::cout << "MainWindow::on_refresh_button_clicked" << std::endl;
#endif

	scriptObject.clear();

	// refresh script object from edited entries
	// forall in menuEntries
	{
		std::map<std::string, MenuEntry *>::const_iterator it_menuEntries = menuEntries.begin();
		const std::map<std::string, MenuEntry *>::const_iterator it_menuEntries_end = menuEntries.end();
		while (it_menuEntries != it_menuEntries_end) {
			if (it_menuEntries->second->isEnabled()) {
				scriptObject.setRename(it_menuEntries->second->getKeyEntryText(),
						it_menuEntries->second->getValueEntryText());
			}
			++it_menuEntries;
		}
	}
	scriptObject.generateGrubScript();
	this->updateGrub();
	this->updateScriptViewText();
	this->clearAllMenuEntries();

	configObject.parseConfig();
	defaultsObject.parseConfig();

	this->updateDefaultsView();
	this->updateGrubViewText();
	this->generateMenuEntries();
}

bool MainWindow::generateMenuEntries() {
	// get each lie from grub.cfg and scan through for menu entries
	// creating a MenuEntry from each one

	const std::list<std::string> & entry_vals = configObject.getEntryValues(GRUB_MENUENTRY_TEXT);
	// forall in entry_vals
	{
		std::list<std::string>::const_iterator it_entry_vals = entry_vals.begin();
		const std::list<std::string>::const_iterator it_entry_vals_end = entry_vals.end();
		while (it_entry_vals != it_entry_vals_end) {
			// Search current script for entry
			std::pair<std::string, std::string> current_script_entry = scriptObject.getReverseRename(
					*it_entry_vals);
#ifdef DEBUG_MAINWINDOW
			std::cout << "MainWindow::generateMenuEntries: DEBUG: " << "Searched for " << *it_entry_vals << ", found ("
					<< current_script_entry.first << ", " << current_script_entry.second << ")" << std::endl;
#endif
			if (current_script_entry.first == "") {
				MenuEntry & new_me = this->addMenuEntry(*it_entry_vals, "", false);
				// wasnt in script so mustnt have been activated previously
				new_me.setEnabled(false);
			} else {
				// switch first menu entry val to undo a current renaming
				MenuEntry & new_me = this->addMenuEntry(current_script_entry.first, current_script_entry.second, true);
				// and set MenuEntry to activated if found
				new_me.setEnabled(true);
			}
			++it_entry_vals;
		}
	}
	menuentry_vbox->show_all();
	return true;
}

void MainWindow::clearAllMenuEntries() {
	// forall in menuEntries
	{
		std::map<std::string, MenuEntry *>::const_iterator it_menuEntries = menuEntries.begin();
		const std::map<std::string, MenuEntry *>::const_iterator it_menuEntries_end = menuEntries.end();
		while (it_menuEntries != it_menuEntries_end) {
			menuentry_vbox->remove(*(it_menuEntries->second));

			++it_menuEntries;
		}
	}
	menuEntries.clear();
}

void MainWindow::deleteMenuEntry(MenuEntry * menu_entry) {
	if (menu_entry != 0) {
		scriptObject.clearRename(menu_entry->getKeyEntryText());
		menuEntries.erase(menu_entry->getKeyEntryText());
		menuentry_vbox->remove(*(menu_entry));
		delete (menu_entry);
		(menu_entry) = 0;
		scriptObject.clearRename(menu_entry->getKeyEntryText());
	}
}

MenuEntry & MainWindow::addMenuEntry(std::string entry1, std::string entry2, bool activated) {

	MenuEntry * me = new MenuEntry(entry1, entry2, activated);
	menuEntries[entry1] = me;
	menuentry_vbox->add(*me);
	scriptObject.setRename(entry1, entry2);
	return *me;
}

void MainWindow::clearAllDefaultsEntries() {
	// forall in defaultsEntries
	{
		std::map<std::string, DefaultsEntry *>::iterator it_defaultsEntries = defaultsEntries.begin();
		const std::map<std::string, DefaultsEntry *>::const_iterator it_defaultsEntries_end = defaultsEntries.end();
		while (it_defaultsEntries != it_defaultsEntries_end) {
			defaults_vbox->remove(*(it_defaultsEntries->second));
			delete (it_defaultsEntries->second);
			++it_defaultsEntries;
		}
	}



	defaultsEntries.clear();
}

DefaultsEntry & MainWindow::addDefaultsEntry(std::string entry1, std::string entry2, bool activated) {

	DefaultsEntry * de = new DefaultsEntry(entry1, entry2, activated);
	defaultsEntries[entry1] = de;
	defaults_vbox->add(*de);
	return *de;
}

bool MainWindow::updateGrub() {
	int success = system("update-grub");

	if (success == 0) {
		return true;
	}
	return false;
}

void MainWindow::updateGrubViewText() {
	std::stringstream ss;
	const std::list<std::string> & rawtext = configObject.getRawConfig();
	// forall in rawtext
	{
		std::list<std::string>::const_iterator it_rawtext = rawtext.begin();
		const std::list<std::string>::const_iterator it_rawtext_end = rawtext.end();
		while (it_rawtext != it_rawtext_end) {
			ss << *it_rawtext << std::endl;
			++it_rawtext;
		}
	}
	grub_textbuffer->set_text(ss.str());
}

void MainWindow::updateScriptViewText() {
	std::stringstream ss;
	const std::list<std::string> & rawtext = scriptObject.getRawScript();
	// forall in rawtext
	{
		std::list<std::string>::const_iterator it_rawtext = rawtext.begin();
		const std::list<std::string>::const_iterator it_rawtext_end = rawtext.end();
		while (it_rawtext != it_rawtext_end) {
			ss << *it_rawtext << std::endl;
			++it_rawtext;
		}
	}
	script_textbuffer->set_text(ss.str());
}

void MainWindow::updateDefaultsView() {
	this->clearAllDefaultsEntries();
	const std::map<std::string, std::string> & varmap = defaultsObject.getVariableMap();
	// forall in varmap
	{
		std::map<std::string, std::string>::const_iterator it_varmap = varmap.begin();
		const std::map<std::string, std::string>::const_iterator it_varmap_end = varmap.end();
		while (it_varmap != it_varmap_end) {
			DefaultsEntry * de = new DefaultsEntry(it_varmap->first, it_varmap->second, true);
			defaultsEntries[it_varmap->first]= de;
			defaults_vbox->pack_start(*de);
			++it_varmap;
		}
	}
	defaults_vbox->show_all();

}

} /* namespace remenu */

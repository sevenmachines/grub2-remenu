/*
 * MainWindow.cpp
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#define DEBUG_MAINWINDOW
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
		configObject(GRUB_CFG_PATH) {
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

	configObject = GrubConfigObject(GRUB_CFG_PATH);
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

bool MainWindow::initialise() {
	bool success = false;
	// get pointers to all the widgets we need from the builder
	builder->get_widget("main_window", main_window);
	builder->get_widget("menuentry_vbox", menuentry_vbox);
	builder->get_widget("refresh_button", refresh_button);

	// connect up widget signals
	refresh_button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_refresh_button_clicked));

	success = (main_window != 0) && (menuentry_vbox != 0) && (refresh_button != 0);

	success = success && this->generateMenuEntries();
	main_window->show();
	return success;
}

void MainWindow::on_refresh_button_clicked() {
#ifdef DEBUG_MAINWINDOW
	std::cout << "MainWindow::on_refresh_button_clicked" << std::endl;
#endif
	this->generateGrubScript();
	this->runCommandAsRoot("update-grub");
	this->generateMenuEntries();
}

bool MainWindow::generateMenuEntries() {
#ifdef DEBUG_MAINWINDOW
	std::cout << "MainWindow::generateMenuEntries: " << "" << std::endl;
#endif
	// get each lie from grub.cfg and scan through for menu entries
	// creating a MenuEntry from each one
	this->clearAllMenuEntries();
	configObject.parseConfig();
	const std::list<std::string> & entry_vals = configObject.getEntryValues(GRUB_MENUENTRY_TEXT);
	// forall in entry_vals
	{
		std::list<std::string>::const_iterator it_entry_vals = entry_vals.begin();
		const std::list<std::string>::const_iterator it_entry_vals_end = entry_vals.end();
		while (it_entry_vals != it_entry_vals_end) {
			this->addMenuEntry(*it_entry_vals);
			++it_entry_vals;
		}
	}
	menuentry_vbox->show_all();
	return true;
}

bool MainWindow::generateGrubScript() {
#ifdef 	DEBUG_MAINWINDOW
	std::cout << "MainWindow::generateGrubScript: " << "" << std::endl;
#endif
	// generate script thatll do the job of renaming
	std::string cmds = this->getScriptCommands();
	std::ofstream ofs;
	ofs.open(GRUBD_SCRIPT_PATH.c_str());
	if (ofs.is_open()) {
		std::cout<<"MainWindow::generateGrubScript: "<<GRUBD_SCRIPT_PATH.c_str()<<std::endl;
		std::cout<<"MainWindow::generateGrubScript: "<<cmds<<std::endl;
		ofs << cmds;
		ofs.close();
		this->setFilePermissions(GRUBD_SCRIPT_PATH);
	} else {
		std::cout << "MainWindow::generateGrubScript: " << "Error opening file " << GRUBD_SCRIPT_PATH << std::endl;
	}

//	/int update_success = system(GRUB_UPDATE_CMD.c_str());
	return true;
}

void MainWindow::clearAllMenuEntries() {
	// create copy for deletion
	std::map<std::string, MenuEntry *> delete_entries(menuEntries.begin(), menuEntries.end());
	// forall in delete_entries
	{
		std::map<std::string, MenuEntry *>::const_iterator it_delete_entries = delete_entries.begin();
		const std::map<std::string, MenuEntry *>::const_iterator it_delete_entries_end = delete_entries.end();
		while (it_delete_entries != it_delete_entries_end) {
			deleteMenuEntry(it_delete_entries->second);
			++it_delete_entries;
		}
	}
	menuEntries.clear();
}

void MainWindow::deleteMenuEntry(MenuEntry * menu_entry) {
	if (menu_entry != 0) {
		menuEntries.erase(menu_entry->getCurrentEntry());
		menuentry_vbox->remove(*(menu_entry));
		delete (menu_entry);
		(menu_entry) = 0;
	}
}

void MainWindow::addMenuEntry(std::string entry) {

	MenuEntry * me = new MenuEntry(entry, entry, false);
	menuEntries[entry] = me;
	menuentry_vbox->add(*me);
}

GError * MainWindow::runCommandAsRoot(const std::string & cmd_str) {
	std::vector<char> chars(cmd_str.begin(), cmd_str.end());
	GError * gerr;
	gchar * cmd_line = &chars[0];
	gksu_run(cmd_line, &gerr);
	return gerr;
}

std::string MainWindow::getScriptCommands() const {
	// generate script thatll do the job of renaming
	std::stringstream ss;
	ss << "#!/bin/sh " << std::endl;
	ss << "echo \"Renaming entries....\" >&2 " << std::endl;

	// forall in menuEntries
	{
		std::map<std::string, MenuEntry *>::const_iterator it_menuEntries = menuEntries.begin();
		const std::map<std::string, MenuEntry *>::const_iterator it_menuEntries_end = menuEntries.end();
		while (it_menuEntries != it_menuEntries_end) {
			if ((it_menuEntries->second)->isEnabled()) {
				ss << "sed -i 's|" << (it_menuEntries->second)->getCurrentEntry() << "|"
						<< (it_menuEntries->second)->getNewEntry() << "|g' " << GRUB_CFG_TMP_PATH << ";" << std::endl;
			}
			++it_menuEntries;
		}
	}
	return ss.str();
}

void MainWindow::setFilePermissions(const std::string file, int perms) {
	if (perms == 0) {
		perms = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;
	}
	int code = chmod(file.c_str(), perms);
	if (code != 0) {
		std::cout << "MainWindow::setFilePermissions: " << "Error setting file permissions on " << file << " (" << perms
				<< ")"<< " Errcode: "<<code<< std::endl;
	}
}

} /* namespace remenu */

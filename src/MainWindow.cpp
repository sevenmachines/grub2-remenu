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
	this->updateGrub();
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
			// Search current script for entry
			std::pair<std::string, std::string> current_script_entry = scriptObject.getRenameMenu(*it_entry_vals);
			if (current_script_entry.first == "") {
				this->addMenuEntry(*it_entry_vals, *it_entry_vals, false);
			} else {
				// switch first menu entry val to undo a current renaming
				this->addMenuEntry(current_script_entry.first, current_script_entry.second, true);
				// and set MenuEntry to activated if found

			}
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
	std::ofstream ofs;
	ofs.open(GRUBD_SCRIPT_PATH.c_str());
	if (ofs.is_open()) {
		std::cout << "MainWindow::generateGrubScript: " << GRUBD_SCRIPT_PATH.c_str() << std::endl;
		std::cout << "MainWindow::generateGrubScript: " << scriptObject.getFullRenameScript() << std::endl;
		ofs << scriptObject.getFullRenameScript();
		ofs.close();
		this->setFilePermissions(GRUBD_SCRIPT_PATH);
	} else {
		std::cout << "MainWindow::generateGrubScript: " << "Error opening file " << GRUBD_SCRIPT_PATH << std::endl;
		std::cout << std::endl;
		std::cout << scriptObject.getFullRenameScript() << std::endl;
		std::cout << std::endl;
	}

//	/int update_success = system(GRUB_UPDATE_CMD.c_str());
	return true;
}

void MainWindow::clearAllMenuEntries() {
	scriptObject.clear();
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
		scriptObject.clearRenameMenu(menu_entry->getCurrentEntry());
		menuEntries.erase(menu_entry->getCurrentEntry());
		menuentry_vbox->remove(*(menu_entry));
		delete (menu_entry);
		(menu_entry) = 0;
		scriptObject.clearRenameMenu(menu_entry->getCurrentEntry());
	}
}

void MainWindow::addMenuEntry(std::string entry1, std::string entry2, bool activated) {

	MenuEntry * me = new MenuEntry(entry1, entry2, activated);
	menuEntries[entry1] = me;
	menuentry_vbox->add(*me);
	scriptObject.setRenameMenu(entry1, entry2);
}

bool MainWindow::updateGrub() {
	int success = system("update-grub");

	if (success == 0){
		return true;
	}
	return false;
}

void MainWindow::setFilePermissions(const std::string file, int perms) {
	if (perms == 0) {
		perms = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;
	}
	int code = chmod(file.c_str(), perms);
	if (code != 0) {
		std::cout << "MainWindow::setFilePermissions: " << "Error setting file permissions on " << file << " (" << perms
				<< ")" << " Errcode: " << code << std::endl;
	}
}

} /* namespace remenu */

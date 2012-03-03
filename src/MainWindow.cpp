/*
 * MainWindow.cpp
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

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

namespace remenu {

MainWindow::MainWindow(int argc, char **argv) {
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
	std::cout << "MainWindow::on_refresh_button_clicked" << std::endl;
	this->generateMenuEntries();
	this->generateGrubScript();
	this->generateMenuEntries();
}

bool MainWindow::generateMenuEntries() {
	std::cout << "MainWindow::generateMenuEntries: " << "" << std::endl;

	// get each lie from grub.cfg and scan through for menu entries
	// creating a MenuEntry from each one
	std::string if_str = GRUB_DIR + GRUB_CFG;
	std::ifstream ifs;
	ifs.open(if_str.c_str());
	if (ifs.is_open()) {
		std::string line;
		std::string entry;
		int line_count = 0;
		while (getline(ifs, line)) {
			++line_count;
			size_t found = line.find(GRUB_MENUENTRY_TEXT);

			if (found != std::string::npos) {
				size_t first_quote;
				size_t second_quote;
				first_quote = line.find_first_of("'");
				if (first_quote != std::string::npos) {
					second_quote = line.find("'", first_quote + 1);
				} else {
					first_quote = line.find_first_of("\"");
					second_quote = line.find("\"", first_quote + 1);
				}
				std::cout << first_quote << ":" << second_quote << std::endl;
				entry = line.substr(first_quote + 1, second_quote - first_quote - 1);
				std::cout << "Found grub menu entry text... " << entry << std::endl;

				// forall in menuEntries
				{
					bool found_changed_entry = false;
					std::map<std::string, MenuEntry *>::const_iterator it_menuEntries = menuEntries.begin();
					const std::map<std::string, MenuEntry *>::const_iterator it_menuEntries_end = menuEntries.end();
					while (found_changed_entry == false && (it_menuEntries != it_menuEntries_end)) {
						if (it_menuEntries->second->isEnabled() && (it_menuEntries->second->getNewEntry() == entry)) {
							found_changed_entry = true;
						}
						++it_menuEntries;

					}
					if (found_changed_entry == false && (menuEntries.find(entry) == menuEntries.end())) {

						MenuEntry * me = new MenuEntry(entry, entry, false);
						menuEntries[entry] = me;
						menuentry_vbox->add(*me);
					}
				}
			}
		}
	}
	ifs.close();
	menuentry_vbox->show_all();
	return true;
}

bool MainWindow::generateGrubScript() {
	std::string grub_tmp_cfg = GRUB_DIR + GRUB_CFG_TMP;
	std::string grub_script = GRUBD_DIR + GRUBD_SCRIPT;
	// generate script thatll do the job of renaming
	std::stringstream ss;
	ss << "#!/bin/sh " << std::endl;
	ss << "echo \"Renaming entries....\" " << std::endl;

	// forall in menuEntries
	{
		std::map<std::string, MenuEntry *>::const_iterator it_menuEntries = menuEntries.begin();
		const std::map<std::string, MenuEntry *>::const_iterator it_menuEntries_end = menuEntries.end();
		while (it_menuEntries != it_menuEntries_end) {
			if ((it_menuEntries->second)->isEnabled()) {
				ss << "sed -i 's|" << (it_menuEntries->second)->getCurrentEntry() << "|"
						<< (it_menuEntries->second)->getNewEntry() << "|g' " << grub_tmp_cfg << ";" << std::endl;
			}
			++it_menuEntries;
		}
	}

	std::cout << ss.str() << std::endl;
	std::ofstream ofs;
	ofs.open(grub_script.c_str());
	if (ofs.is_open()) {
		ofs << ss.str();
		int mod_success = chmod(grub_script.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
		if (mod_success != 0) {
			std::cout << "Failed to write permissions to " << grub_script << " errno: " << mod_success << std::endl;
		} else {
			std::cout << "Successfully changed permissions of " << grub_script << std::endl;
		}
	} else {
		std::cout << "Failed to open " << grub_script << std::endl;
	}
	ofs.close();
	//int update_success = system(GRUB_UPDATE_CMD.c_str());
	return true;
}

void MainWindow::clearAllMenuEntries() {
	// forall in menuEntries
	{
		std::map<std::string, MenuEntry *>::iterator it_menuEntries = menuEntries.begin();
		const std::map<std::string, MenuEntry *>::const_iterator it_menuEntries_end = menuEntries.end();
		while (it_menuEntries != it_menuEntries_end) {
			menuentry_vbox->remove(*(it_menuEntries->second));
			if (it_menuEntries->second != 0) {
				delete (it_menuEntries->second);
				(it_menuEntries->second) = 0;
			}
			++it_menuEntries;
		}
	}
	menuEntries.clear();
}
MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

} /* namespace remenu */

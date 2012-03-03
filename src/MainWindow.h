/*
 * MainWindow.h
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "MenuEntry.h"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <map>

namespace remenu {

class MainWindow {
public:
	MainWindow(int argc, char **argv);
	virtual ~MainWindow();

protected:
	Glib::RefPtr<Gtk::Builder> builder ;
	Gtk::Window * main_window;
	Gtk::VBox * menuentry_vbox;
	Gtk::Button * refresh_button;
	std::map<std::string, MenuEntry *> menuEntries;

	void on_refresh_button_clicked();
	bool generateMenuEntries();
	bool generateGrubScript();

	bool initialise();
	void clearAllMenuEntries() ;
};

} /* namespace remenu */
#endif /* MAINWINDOW_H_ */

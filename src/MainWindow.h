/*
 * MainWindow.h
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "MenuEntry.h"
#include "DefaultsEntry.h"
#include "MainDefs.h"
#include "GrubConfigObject.h"
#include "GrubScriptObject.h"
#include "GrubDefaultsObject.h"
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/textbuffer.h>
#include <map>

namespace remenu {

class MainWindow {
public:
	MainWindow(int argc, char **argv);
	virtual ~MainWindow();

protected:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window * main_window;
	Gtk::VBox * menuentry_vbox;
	Gtk::VBox * defaults_vbox;
	Gtk::Button * refresh_button;
	Gtk::TextView * grub_textview;
	Gtk::TextView * script_textview;
	Glib::RefPtr<Gtk::TextBuffer> grub_textbuffer;
	Glib::RefPtr<Gtk::TextBuffer> script_textbuffer;

	std::map<std::string, MenuEntry *> menuEntries;
	std::map<std::string, DefaultsEntry *> defaultsEntries;

	GrubConfigObject configObject;
	GrubScriptObject scriptObject;
	GrubDefaultsObject defaultsObject;

	void on_refresh_button_clicked();
	bool generateMenuEntries();

	bool initialise();

	MenuEntry & addMenuEntry(std::string entry1, std::string entry2, bool activated);
	void deleteMenuEntry(MenuEntry * menu_entry);
	void clearAllMenuEntries();

	void clearAllDefaultsEntries() ;
	DefaultsEntry & addDefaultsEntry(std::string entry1, std::string entry2, bool activated) ;


	bool updateGrub();
	std::string getScriptCommands() const;
	void updateGrubViewText() ;
	void updateScriptViewText() ;
	void updateDefaultsView() ;

};

} /* namespace remenu */
#endif /* MAINWINDOW_H_ */

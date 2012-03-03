/*
 * MainDefs.h
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#ifndef MAINDEFS_H_
#define MAINDEFS_H_

#include <string>

static const std::string DATADIR = "data/";
static const std::string GRUB_DIR = "/boot/grub/";
static const std::string GRUBD_DIR = "/etc/grub.d/";
static const std::string GRUB_CFG = "grub.cfg";
static const std::string GRUB_CFG_TMP = "grub.cfg.new";
static const std::string GRUB_MENUENTRY_TEXT = "menuentry";
static const std::string GRUBD_SCRIPT = "99_rename_menu_entries";
static const std::string GRUB_UPDATE_CMD = "update-grub";

#endif /* MAINDEFS_H_ */

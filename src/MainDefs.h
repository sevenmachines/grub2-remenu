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

static const std::string GRUB_CFG_DIR = "/boot/grub/";
static const std::string GRUB_CFG_FILE = "grub.cfg";
static const std::string GRUB_CFG_TMP = "grub.cfg.new";
static const std::string GRUB_CFG_PATH =GRUB_CFG_DIR+GRUB_CFG_FILE;
static const std::string GRUB_CFG_TMP_PATH =GRUB_CFG_DIR+GRUB_CFG_TMP;

static const std::string GRUBD_DIR = "/etc/grub.d/";
static const std::string GRUBD_SCRIPT_FILE = "99_rename_menu_entries";
static const std::string GRUBD_SCRIPT_PATH =GRUBD_DIR+GRUBD_SCRIPT_FILE;

static const std::string GRUB_UPDATE_CMD = "update-grub";

static const std::string GRUB_MENUENTRY_TEXT = "menuentry";
static const std::string GRUB_DEFAULTS_PATH = "/etc/default/grub";

#endif /* MAINDEFS_H_ */

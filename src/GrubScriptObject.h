/*
 * GrubScriptObject.h
 *
 *  Created on: 5 Mar 2012
 *      Author: niall
 */

#ifndef GRUBSCRIPTOBJECT_H_
#define GRUBSCRIPTOBJECT_H_

#include "MainDefs.h"
#include <string>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <list>

namespace remenu {

class GrubScriptObject {
public:
	GrubScriptObject(const std::string scriptin = GRUBD_SCRIPT_PATH);
	virtual ~GrubScriptObject();

	void parseScript(const std::string scriptin);
	void generateGrubScript(const std::string file_path = GRUBD_SCRIPT_PATH);

	std::pair<std::string, std::string> getRename(const std::string obj1);
	std::pair<std::string, std::string> getReverseRename(const std::string obj1);
	void setRename(const std::string obj1, const std::string obj2);
	bool clearRename(const std::string obj1);

	const std::map<std::string, std::string> getRenames() const;

	void clear();

	const std::list<std::string> & getRawScript() const;

protected:
	std::string getRenameScript() const;
	std::pair<std::string, std::string> getRenameEntryFromLine(const std::string line);

	void setFilePermissions(const std::string file, int perms = S_IRUSR | S_IXUSR | S_IRGRP | S_IROTH) const;
	bool checkFilePermissions(const std::string file, int perms = R_OK | W_OK | X_OK);

	std::map<std::string, std::string> renames;

	std::list<std::string> rawScript;
};

} /* namespace remenu */
#endif /* GRUBSCRIPTOBJECT_H_ */

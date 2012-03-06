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
#include <list>

namespace remenu {

class GrubScriptObject {
public:
	GrubScriptObject();
	virtual ~GrubScriptObject();

	void generateGrubScript(const std::string file_path= GRUBD_SCRIPT_PATH) ;
	std::string getRenameMenuScript() const;
	std::string getRenameMiscScript() const;

	std::pair<std::string, std::string> getRenameMenu(const std::string obj1);
	std::pair<std::string, std::string> getReverseRenameMenu(const std::string obj1);
	void setRenameMenu(const std::string obj1, const std::string obj2);
	bool clearRenameMenu(const std::string obj1);

	std::pair<std::string, std::string> getRenameMisc(const std::string obj1);
	std::pair<std::string, std::string> getReverseRenameMisc(const std::string obj1);
	void setRenameMisc(const std::string obj1, const std::string obj2);
	bool clearRenameMisc(const std::string obj1);

	const std::map<std::string, std::string> & getMenuRenames();
	const std::map<std::string, std::string> & getMiscRenames();

	void clear();

	const std::list<std::string> & getRawScript()const;
protected:

	std::string getRenameScript(const std::map<std::string, std::string> & map) const;
	std::pair<std::string, std::string> getRename(const std::string obj1, std::map<std::string, std::string> & map);
	std::pair<std::string, std::string> getReverseRename(const std::string obj1, std::map<std::string, std::string> & map);
	void setRename(const std::string obj1, const std::string obj2, std::map<std::string, std::string> & map);
	bool clearRename(const std::string obj1, std::map<std::string, std::string> & map);
	void setFilePermissions(const std::string file, int perms = S_IRUSR |S_IXUSR | S_IRGRP | S_IROTH )  const ;

	std::map<std::string, std::string> menuRenames;
	std::map<std::string, std::string> miscRenames;

	std::list<std::string> rawScript;
};

} /* namespace remenu */
#endif /* GRUBSCRIPTOBJECT_H_ */

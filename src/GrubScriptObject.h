/*
 * GrubScriptObject.h
 *
 *  Created on: 5 Mar 2012
 *      Author: niall
 */

#ifndef GRUBSCRIPTOBJECT_H_
#define GRUBSCRIPTOBJECT_H_

#include <string>
#include <map>

namespace remenu {

class GrubScriptObject {
public:
	GrubScriptObject();
	virtual ~GrubScriptObject();

	std::string getFullRenameScript() const;
	std::string getRenameMenuScript() const;
	std::string getRenameMiscScript() const;

	std::pair<std::string, std::string> getRenameMenu(const std::string obj1);
	void setRenameMenu(const std::string obj1, const std::string obj2);
	bool clearRenameMenu(const std::string obj1);

	std::pair<std::string, std::string> getRenameMisc(const std::string obj1);
	void setRenameMisc(const std::string obj1, const std::string obj2);
	bool clearRenameMisc(const std::string obj1);

	const std::map<std::string, std::string> & getMenuRenames();
	const std::map<std::string, std::string> & getMiscRenames();

	void clear();

protected:

	std::string getRenameScript(const std::map<std::string, std::string> & map) const;
	std::pair<std::string, std::string> getRename(const std::string obj1, std::map<std::string, std::string> & map);
	void setRename(const std::string obj1, const std::string obj2, std::map<std::string, std::string> & map);
	bool clearRename(const std::string obj1, std::map<std::string, std::string> & map);

	std::map<std::string, std::string> menuRenames;
	std::map<std::string, std::string> miscRenames;
};

} /* namespace remenu */
#endif /* GRUBSCRIPTOBJECT_H_ */

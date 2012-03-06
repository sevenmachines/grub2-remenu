/*
 * GrubDefaultsObject.h
 *
 *  Created on: 6 Mar 2012
 *      Author: niall
 */

#ifndef GRUBDEFAULTSOBJECT_H_
#define GRUBDEFAULTSOBJECT_H_

#include "MainDefs.h"
#include <map>
#include <string>


namespace remenu {

class GrubDefaultsObject {
public:
	GrubDefaultsObject(const std::string config_file);
	virtual ~GrubDefaultsObject();
	void parseConfig() ;
	const std::map<std::string, std::string> & getVariableMap()const;

protected:
	const std::string configFile;
	std::map<std::string, std::string> variableMap;
	 std::pair<std::string, std::string> getVariableEntry(const std::string & line) ;
};

} /* namespace remenu */
#endif /* GRUBDEFAULTSOBJECT_H_ */

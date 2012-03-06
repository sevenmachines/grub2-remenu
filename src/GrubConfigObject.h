/*
 * GrubConfigObject.h
 *
 *  Created on: 5 Mar 2012
 *      Author: niall
 */

#ifndef GRUBCONFIGOBJECT_H_
#define GRUBCONFIGOBJECT_H_

#include <string>
#include <map>
#include <list>
#include <sstream>

namespace remenu {

class GrubConfigObject {
public:
	GrubConfigObject(const std::string config_file);
	virtual ~GrubConfigObject();
	void initialiseConfigKeys();
		void parseConfig();
	void parseMenuEntries();
	const std::list<std::string> &  getRawConfig()const;
	const std::list<std::string>  getEntryValues(const std::string & key)const;
	const std::list<std::string>  getEntryOptions(const std::string & key)const;

    const std::map<std::string,std::list<std::string> > & getConfigEntries() const;
    const std::map<std::string,std::list<std::string> >  & getConfigEntryOptions() const;
    std::string getConfigFile() const;
    const std::map<std::string,std::list<std::string> > & getConfigKeys() const;

protected:
	std::string configFile;
	std::list<std::string>	rawConfig;
	 std::map<std::string, std::list<std::string> > configKeys;

	 std::map<std::string, std::list<std::string> > configEntries;
	std::map<std::string, std::list<std::string> > configEntryOptions;

	std::string getMenuEntries();
	const std::list<std::string>  getKeyRawLines(const std::string & key) const;
	const std::list<std::string> getMapValuesByKey(const std::map<std::string, std::list<std::string> > & map, const std::string & key)const;

};

} /* namespace remenu */
#endif /* GRUBCONFIGOBJECT_H_ */

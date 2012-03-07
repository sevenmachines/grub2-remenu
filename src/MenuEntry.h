/*
 * MenuEntry.h
 *
 *  Created on: 3 Mar 2012
 *      Author: niall
 */

#ifndef MENUENTRY_H_
#define MENUENTRY_H_

#include "GenericEntry.h"

namespace remenu {

class MenuEntry : public GenericEntry{
public:
	MenuEntry();
	MenuEntry(std::string key, std::string val, bool activate);
	virtual ~MenuEntry();

protected:
	virtual void on_enabledCheckBox_clicked();

};

} /* namespace remenu */
#endif /* MENUENTRY_H_ */

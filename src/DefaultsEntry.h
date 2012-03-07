/*
 * DefaultsEntry.h
 *
 *  Created on: 6 Mar 2012
 *      Author: niall
 */

#ifndef DEFAULTSENTRY_H_
#define DEFAULTSENTRY_H_

#include "GenericEntry.h"

namespace remenu {

class DefaultsEntry : public GenericEntry{
public:
	DefaultsEntry(const std::string & key, const std::string & val, const bool activate);
	virtual ~DefaultsEntry();
protected:
	virtual  void on_enableCheckButton_clicked();
};

} /* namespace remenu */
#endif /* DEFAULTSENTRY_H_ */

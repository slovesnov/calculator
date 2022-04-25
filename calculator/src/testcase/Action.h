/*
 * Action.h
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef TESTCASE_ACTION_H_
#define TESTCASE_ACTION_H_

#include <ostream>
#include <string>

namespace TestCase {

class Action {
public:
	enum ActionEnum{
		STATIC_COMPILE,
		COMPILE_CALCULATE
	};
	ActionEnum action;
	Action();
	virtual ~Action();
	void set(std::string s,int line);
	std::string toString()const;
};

std::ostream& operator<<(std::ostream& os, const Action& a);

} /* namespace TestCase */

#endif /* TESTCASE_ACTION_H_ */

/*
 * Action.cpp
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include <cassert>

#include "aslov.h"
#include "Action.h"

namespace TestCase {

void Action::set(std::string s,int line) {
	if(s=="scompile"){
		action=ActionEnum::STATIC_COMPILE;
	}
	else if(s=="compile_calculate"){
		action=ActionEnum::COMPILE_CALCULATE;
	}
	else{
		printel("invalid string",s,"at line",line);
		assert(0);
	}
}

std::string Action::toString() const {
	return "todo";
}


} /* namespace TestCase */

std::ostream& TestCase::operator <<(std::ostream &os, const Action &a) {
    os << a.toString();
    return os;
}

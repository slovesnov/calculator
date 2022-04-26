/*
 * Result.cpp
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include <cassert>
#include <cstdlib>
#include <map>

#include "aslov.h"
#include "Result.h"

namespace TestCase {

std::map <std::string, Result::ErrorCode> map {
	{ "ok", Result::ErrorCode::OK },
  { "compile_error", Result::ErrorCode::COMPILE_ERROR },
  { "calculate_error", Result::ErrorCode::CALCULATE_ERROR },
};

//toString() already defined in aslov.h so use another name
std::string etoString(Result::ErrorCode c) {
	auto r = std::find_if(map.begin(), map.end(), [c](const auto &o) {
		return o.second == c;
	});

	assert(r!=map.end());
	return r->first;
}

void Result::set(std::string s,int line) {
	auto f = map.find(s);
	if (f == map.end()) {
		errorCode = OK;
#ifndef NDEBUG
		bool b=
#endif
		parseString(s,value);
		assert(b);
	} else {
		errorCode = f->second;
	}
}

bool Result::operator ==(const Result &a) const {
	if(errorCode==OK && a.errorCode==OK){
		return value==a.value;
	}
	else{
		return a.errorCode==errorCode;
	}
}

void Result::set(ErrorCode code, double v) {
	errorCode=code;
	value=v;
}

bool Result::operator !=(const Result &a) const {
	return !(*this==a);
}

std::string Result::toString() const {
	if(errorCode==OK){
		return forma(value);
	}
	else{
		return etoString(errorCode);
	}
}


} /* namespace TestCase */

std::ostream& TestCase::operator <<(std::ostream &os, const Result &a) {
    os << a.toString();
    return os;
}


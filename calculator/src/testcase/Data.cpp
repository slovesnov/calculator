/*
 * Data.cpp
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */
#include <cassert>

#include "Data.h"

VString splitR(std::string const &s, std::string const &separator="\\s+");

namespace TestCase {

Data::Data() {
}

Data::~Data() {
}


void Data::set(std::string s,int line) {
	auto p=s.find('"');
	assert(p!=std::string::npos);
	p++;
	assert(s.length()>p);
	auto p1=s.find('"',p);
	assert(p1!=std::string::npos);
	compile=s.substr(p, p1-p);

	variables.clear();
	values.clear();

	p=s.find('"',p1+1);
	if(p==std::string::npos){
		return;
	}
	p++;
	assert(s.length()>p);
	p1=s.find('"',p);
	variables=splitR(s.substr(p, p1-p));

	p=s.find('"',p1+1);
	assert(p!=std::string::npos);
	p++;
	assert(s.length()>p);
	p1=s.find('"',p);
	for(auto& v:splitR(s.substr(p, p1-p))){
		values.push_back(std::stod(v));
	}

}

std::string Data::toString() const {
	int i=0;
	std::string s="compile="+compile;
	if(!variables.empty()){
		s+=" variables="+joinV(variables,' ')+" values=";
		for(auto&a:values){
			if(i){
				s+=' ';
			}
			s+=std::to_string(a);
			i++;
		}
	}
	return s;
}

} /* namespace TestCase */

std::ostream& TestCase::operator <<(std::ostream &os, const Data &a) {
    os << a.toString();
    return os;
}

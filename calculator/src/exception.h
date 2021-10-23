/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

#ifndef _exception_h_
#define _exception_h_

#include <exception>
#include <string>

class Exception: std::exception { //this class is created for g++ compiler
	std::string m_message;
public:
	Exception(const char *message) :
			std::exception() {
		m_message = message;
	}

	const char* what() const noexcept {
		return m_message.c_str();
	}

	~Exception() noexcept {
	}
};

class IllegalArgumentException: Exception {
public:
	IllegalArgumentException() :
			Exception("IllegalArgumentException") {
	}
};

#endif

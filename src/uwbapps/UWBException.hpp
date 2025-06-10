// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBEXCEPTION_HPP
#define UWBEXCEPTION_HPP

#include <string>
#include <iostream>
using namespace std;
class UWBException:public exception {

public:
	UWBException(){};
	UWBException(string msg) {
		this->msg = msg;
	}
	virtual const char* what() const throw() {
		return this->msg.c_str();
	}
private:
	string msg;          
};

#endif

/*
 * exceptions.h
 *
 *  Created on: 31 июля 2014 г.
 *      Author: User
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <string>

class base_except{
	std::string msg;
public:
	base_except(std::string s){ msg=s; }
	std::string get_msg() { return msg; }
};

class bad_fstate: public base_except{

public:
	bad_fstate(std::string s): base_except(s){}
};

class bad_write: public base_except{

public:
	bad_write(std::string s): base_except(s){}
};

class bad_signal: public base_except{
public:
	bad_signal(std::string s): base_except(s){}
};




#endif /* EXCEPTIONS_H_ */

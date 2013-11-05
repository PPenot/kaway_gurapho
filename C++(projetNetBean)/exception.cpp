#include <iostream>
#include "exception.h"

using namespace std;

Exception::Exception(const string& _name, const string& _fname) throw():name(_name),fname(_fname)
{
}


//~ Exception::~Exception() throw()
//~ {
	//~ if(name)
		//~ delete[] name;
	//~ if(fname)
		//~ delete[] fname;
//~ }

const char* Exception::what() const throw()
{
	string tmp = "in function \'" + fname + "\': " + name;
	return tmp.c_str();
}


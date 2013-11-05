
#include <iostream>
#include <fstream>
#include <ctype.h>
#include "fwrite.h"

using namespace std;

void toUpperCase(string& s)
{
	int size = s.size();
	for(int i=0; i<size;i++)
		s[i] = toupper(s[i]);
}

string tab(unsigned int size)
{
	return string(3*size,' ');
}

string title1(string t, unsigned int spaces, unsigned int size)
{
	string tmp(79,'-');
	toUpperCase(t);

	string s = tab(spaces);

	return s + tmp + '\n' + s + string((size-t.size())/2,' ') + t + '\n' + s + tmp + '\n';
}

string title1(const char* t, unsigned int spaces, unsigned int size)
{
	return title1(string(t),spaces, size);
}

string title2(string t, unsigned int spaces)
{
	string tmp(t.size(),'-');
	string s = tab(spaces);

	return s + tmp +'\n' + s + t + '\n' + s + tmp + '\n';
}

string title3(const char* t, unsigned int spaces)
{
	return title3(string(t),spaces);
}

string title3(string t, unsigned int spaces)
{
	string tmp(t.size(),'-');
	string s = tab(spaces);

	return s + t + '\n' + s + tmp + '\n';
}

string title2(const char* t, unsigned int spaces)
{
	return title2(string(t),spaces);
}


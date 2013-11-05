#ifndef HEADER_FWRITE
#define HEADER_FWRITE
#include <string>

void toUpperCase(std::string&);

std::string tab(unsigned int =0);

std::string title1(std::string, unsigned int spaces=0, unsigned int size=79);

std::string title1(const char* t, unsigned int spaces=0, unsigned int size=79);

std::string title2(std::string, unsigned int spaces=0);

std::string title2(const char*, unsigned int spaces=0);

std::string title3(std::string, unsigned int spaces=0);

std::string title3(const char*, unsigned int spaces=0);

void f(int=0,int=2);

#endif

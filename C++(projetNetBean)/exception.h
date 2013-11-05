#ifndef HEADER_EXCEPTION
#define HEADER_EXCEPTION
#include <exception>
#include <string>

class Exception
{
	private:
		std::string name;
		std::string fname;

	public:
		Exception(const std::string& = std::string("unknown"), const std::string& = std::string("unknown")) throw();

		~Exception() throw() {};
		virtual const char* what() const throw();
};

#endif

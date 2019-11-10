// Michael Ivanitskiy

#ifndef ERROR_UTILITIES
#define ERROR_UTILITIES

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

// STANDARD: Errors will print the message (and perform other actions) if the input statement is true
// they will also return the value of the input statement

// real error checking
void WARNING( std::string const& msg );
void ERROR( std::string const& msg );
void ERROR_FATAL( std::string const& msg );
bool CHK_WARNING( bool cond, std::string const& msg );
bool CHK_ERROR( bool cond, std::string const& msg );
bool CHK_ERROR_FATAL( bool cond, std::string const& msg );
void TEST( std::string const& msg );
void CHK_TEST( bool cond, std::string const& msg );
void TEST();
void __DEBUG( std::string const& msg );
void __DEBUG();

int num_tests = 0;
const bool PRINT_DEBUG = true;
const bool PRINT_TEST = true;

// const bool PRINT_DEBUG = true;
// const bool PRINT_TEST = true;

// print error

inline void WARNING( std::string const& msg )
{
	std::cerr << "WARNING:\t" << msg << "\n";
}

inline void ERROR( std::string const& msg )
{
	std::cerr << "ERROR:\t" << msg << "\n";
}

inline void ERROR_FATAL( std::string const& msg )
{
	std::cerr << "===============================\n";
	std::cerr << "FATAL ERROR:\t" << msg << "\n";
	exit(1);
}

// print upon condition

inline bool CHK_WARNING( bool cond, std::string const& msg )
{
	if (cond) WARNING(msg);
	return cond;
}

inline bool CHK_ERROR( bool cond, std::string const& msg )
{
	if (cond) ERROR(msg);
	return cond;
}

inline bool CHK_ERROR_FATAL( bool cond, std::string const& msg )
{
	if (cond) ERROR_FATAL(msg);
	return cond;
}

// testing/debug prints

inline void TEST( std::string const& msg )
{
	std::cerr << "\tTEST:\t" << msg << "\n";
}

inline void CHK_TEST( bool cond, std::string const& msg )
{
	if (cond) TEST(msg);
}

inline void TEST()
{
	if (PRINT_TEST)
	{
		std::cerr << "\tTEST__" << num_tests << "\n";
		num_tests++;
	}
}

inline void TEST(int input)
{
	if (PRINT_TEST)
	{
		std::cerr << "\tTEST__" << input << "\n";
		num_tests = input + 1;
	}
}

inline void __DEBUG( std::string const& msg )
{
	if (PRINT_DEBUG) std::cerr << "\tDEBUG:\t" << msg << "\n";
}

inline void __DEBUG()
{
	if (PRINT_DEBUG) 
	{
		std::cerr << "\tDEBUG__" << num_tests << "\n";
		num_tests++;
	}
}

inline void DEBUG_VEC_CONTENTS(std::vector < size_t > & vec)
{
	for ( size_t x : vec )
	{
		std::cerr << x << " ";
	}
	std::cerr << std::endl;
}

#endif

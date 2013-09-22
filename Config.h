/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once
// This is a configuration file For PythonCpp


// UnComment to enable std::shared_ptr support
// with this option the library treats
// pointers to a class similar to a shared_ptr to the class
// and considers the the same object
#define PYTHON_CPP_SHARED_PTR_SUPPORT

// ===  Native String  ===
// =======================
// Here you can introduce you native string to PythonCpp
// THIS Means that the library will treat you String object
// as a string rather then a Custom object
// 

/**
#include "< native string header >"
#define PYTHON_CPP_NATIVE_STRING < native string type >

namespace PythonCpp
{
	// Convert char array to native String < IMPLEMENT IN YOUR CODE >
	PYTHON_CPP_NATIVE_STRING	CharToNativeString( const char* strIn );

	// Convert char array to native String < IMPLEMENT IN YOUR CODE >
	const char*			NativeStringToChar( const PYTHON_CPP_NATIVE_STRING& strIn );
}

// Uncomment if Strings Are unicode
// #define PYTHON_CPP_USE_UNICODE_STRING

// Uncomment if you need to delete the char* created in 'NativeStringToChar'
// #define PYTHON_CPP_NATIVE_STRING_CLEANUP

// Maximum number of string arguments in a method
// If you have methods that take more then 16 Strings as arguments increase this value
#define PYTHON_CPP_MAX_METHOD_STRINGS 16
*/

#include "../Tor/Base/String.h"
#define PYTHON_CPP_NATIVE_STRING Tor::String

namespace PythonCpp
{
	PYTHON_CPP_NATIVE_STRING	CharToNativeString( const char* strIn );
	const char*			NativeStringToChar( const PYTHON_CPP_NATIVE_STRING& strIn );
}

#define PYTHON_CPP_MAX_METHOD_STRINGS 16
/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
#pragma once
#include "Config.h"
#ifdef PYTHON_CPP_NATIVE_STRING
#include "Conversions.h"

#ifdef PYTHON_CPP_USE_UNICODE_STRING
#define PYBYTES_AS_STRING( str ) PyBytes_AsString( PyUnicode_AsUTF8String( str ) )
#else
#define PYBYTES_AS_STRING( str )  PyBytes_AsString( PyUnicode_AsUTF8String(str) )
#endif

namespace PythonCpp
{

	// Specialization of conversion functions for native string type

	/** PyObject -> String */
	template<>	static PYTHON_CPP_NATIVE_STRING			FromPy<PYTHON_CPP_NATIVE_STRING>( PyObject* o )
	{
		return CharToNativeString( PYBYTES_AS_STRING( o ) );
	}

	/** PyObject -> const String */
	template<>	static const PYTHON_CPP_NATIVE_STRING			FromPy<const PYTHON_CPP_NATIVE_STRING>( PyObject* o )
	{
		return CharToNativeString( PYBYTES_AS_STRING( o ) );
	}


	// Holds the number of temporary native strings
	static	unsigned int g_uiNumberOfTempNativeStrings;

	// Array of temporary native strings for referenced string conversion
	static PYTHON_CPP_NATIVE_STRING		g_arrTempNativeStrings[ PYTHON_CPP_MAX_METHOD_STRINGS ];

	/** PyObject -> String& :::::   Thinking about not supporting it as changes done to the string will not propagete */
	template<>	static PYTHON_CPP_NATIVE_STRING&			FromPy<PYTHON_CPP_NATIVE_STRING&>( PyObject* o )
	{
		if ( o )
		{
			g_arrTempNativeStrings[ g_uiNumberOfTempNativeStrings ] = CharToNativeString( PYBYTES_AS_STRING( o ) );
		}
		else
		{
			g_arrTempNativeStrings[ g_uiNumberOfTempNativeStrings ] = PYTHON_CPP_NATIVE_STRING( "null" );
		}
		++g_uiNumberOfTempNativeStrings;
		return g_arrTempNativeStrings[ g_uiNumberOfTempNativeStrings - 1 ];
	}

	/** PyObject -> const String& */
	template<>	static const PYTHON_CPP_NATIVE_STRING&			FromPy<const PYTHON_CPP_NATIVE_STRING&>( PyObject* o )
	{
		if ( o )
		{
			g_arrTempNativeStrings[ g_uiNumberOfTempNativeStrings ] = CharToNativeString( PYBYTES_AS_STRING( o ) );
		}
		else
		{
			g_arrTempNativeStrings[ g_uiNumberOfTempNativeStrings ] = PYTHON_CPP_NATIVE_STRING( "null" );
		}
		++g_uiNumberOfTempNativeStrings;
		return g_arrTempNativeStrings[ g_uiNumberOfTempNativeStrings - 1 ];
	}

	/** RValue String -> PyObject */
	template<>	static PyObject*	ToPy<PYTHON_CPP_NATIVE_STRING&&>( PYTHON_CPP_NATIVE_STRING&& o )
	{
		const char* str = NativeStringToChar( o );
		PyObject* res = PyBytes_FromString( str );
#ifdef PYTHON_CPP_NATIVE_STRING_CLEANUP
		delete[ ] str;
#endif
		return res;
	}

	/** const RValue String -> PyObject */
	template<>	static PyObject*	ToPy<const PYTHON_CPP_NATIVE_STRING&&>( const PYTHON_CPP_NATIVE_STRING&& o )
	{
		const char* str = NativeStringToChar( o );
		PyObject* res = PyBytes_FromString( str );
#ifdef PYTHON_CPP_NATIVE_STRING_CLEANUP
		delete[ ] str;
#endif
		return res;
	}

	/** String Reference -> PyObject */
	template<>	static PyObject*	ToPy<PYTHON_CPP_NATIVE_STRING&>( PYTHON_CPP_NATIVE_STRING& o )
	{
		const char* str = NativeStringToChar( o );
		PyObject* res = PyBytes_FromString( str );
#ifdef PYTHON_CPP_NATIVE_STRING_CLEANUP
		delete[ ] str;
#endif
		return res;
	}

	/** const String Reference -> PyObject */
	template<>	static PyObject*	ToPy<const PYTHON_CPP_NATIVE_STRING&>( const PYTHON_CPP_NATIVE_STRING& o )
	{
		const char* str = NativeStringToChar( o );
		PyObject* res = PyBytes_FromString( str );
#ifdef PYTHON_CPP_NATIVE_STRING_CLEANUP
		delete[ ] str;
#endif
		return res;
	}

	/** String -> PyObject */
	template<>	static PyObject*	ToPy<PYTHON_CPP_NATIVE_STRING>( PYTHON_CPP_NATIVE_STRING o )
	{
		const char* str = NativeStringToChar( o );
		PyObject* res = PyBytes_FromString( str );
#ifdef PYTHON_CPP_NATIVE_STRING_CLEANUP
		delete[ ] str;
#endif
		return res;
	}

	/** const String -> PyObject */
	template<>	static PyObject*	ToPy<const PYTHON_CPP_NATIVE_STRING>( const PYTHON_CPP_NATIVE_STRING o )
	{
		const char* str = NativeStringToChar( o );
		PyObject* res = PyBytes_FromString( str );
#ifdef PYTHON_CPP_NATIVE_STRING_CLEANUP
		delete[ ] str;
#endif
		return res;
	}

}
#endif
/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
#pragma once
#include "Instance.h"
#include "Helpers.h"

//////////////////////////////////////////////////////////////////////////
/////// Convert from python to c++
//////////////////////////////////////////////////////////////////////////

namespace PythonCpp
{
	//---------------------------------------------------------------------------------
	// From Python to C++
	//---------------------------------------------------------------------------------
	// The type is the desired type of the object
	template< typename T>
	static typename std::enable_if<!std::is_enum<T>::value, T>::type FromPy( PyObject* o )
	{		
		Instance<T>*	pBase = reinterpret_cast<Instance<T>*>(o);
		return pBase->m_pInstance->value;
	}

	// In case of Enumeration type
	template <class T>
	static typename std::enable_if<std::is_enum<T>::value, T>::type
		FromPy( PyObject* o ) { return static_cast<T>( PyLong_AsLong( o ) ); }

	// Some specialization for primitives
	template<>	static char				FromPy<char>( PyObject* o ) { return PyBytes_AsString( o )[ 0 ]; }
	template<>	static unsigned char	FromPy<unsigned char>( PyObject* o ) { return static_cast<unsigned char>( PyBytes_AsString( o )[ 0 ] ); }
	template<>	static signed char		FromPy<signed char>( PyObject* o ) { return static_cast<signed char>( PyBytes_AsString( o )[ 0 ] ); }
	template<>	static short			FromPy<short>( PyObject* o ) { return static_cast<short>( PyLong_AsLong( o ) ); }
	template<>	static int				FromPy<int>( PyObject* o ) { return static_cast<int>( PyLong_AsLong( o ) ); }
	template<>	static long				FromPy<long>( PyObject* o ) { return PyLong_AsLong( o ); }
	template<>	static __int64			FromPy<__int64>( PyObject* o ) { return static_cast<__int64>( PyLong_AsLong( o ) ); }
	template<>	static unsigned short	FromPy<unsigned short>( PyObject* o ) { return static_cast<unsigned short>( PyLong_AsLong( o ) ); }
	template<>	static unsigned int		FromPy<unsigned int>( PyObject* o ) { return static_cast<unsigned int>( PyLong_AsLong( o ) ); }
	template<>	static unsigned long	FromPy<unsigned long>( PyObject* o ) { return static_cast<unsigned long>( PyLong_AsLong( o ) ); }
	template<>	static unsigned __int64	FromPy<unsigned __int64>( PyObject* o ) { return static_cast<unsigned __int64>( PyLong_AsLong( o ) ); }
	template<>	static bool				FromPy<bool>( PyObject* o ) { return PyLong_AsLong( o ) != 0; }
	template<>	static double			FromPy<double>( PyObject* o ) { return PyFloat_AsDouble( o ); }
	template<>	static float			FromPy<float>( PyObject* o ) { return static_cast<float>( PyFloat_AsDouble( o ) ); }
	template<>	static char*			FromPy<char*>( PyObject* o ) { return PyBytes_AsString( PyUnicode_AsUTF8String( o ) ); }


	//---------------------------------------------------------------------------------
	// From C++  to Python
	//---------------------------------------------------------------------------------
	template< typename T >
	static PyObject*	ToPy( typename std::enable_if<!std::is_enum<T>::value, T>::type o )
	{
		Instance< T >* pNew = PyObject_NEW(Instance< T >, &(tagPythonObjectTypes< T >::type));
		if ( pNew )
		{
			pNew->m_pInstance = new tagInstanceHolder<T>( o );
			return reinterpret_cast<PyObject*>( pNew );
		}
		Py_RETURN_NONE;
	}

	// In case of enum
	template< typename T >
	static PyObject*	ToPy( typename std::enable_if<std::is_enum<T>::value, T>::type o )
	{
		return PyLong_FromLong( static_cast<unsigned int>( o ) );
	}

	template<>	static PyObject*	ToPy<long>( long o ) { return PyLong_FromLong( o ); }
	template<>	static PyObject*	ToPy<int>( int o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<unsigned int>( unsigned int o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<short>( short o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<bool>( bool o ) { return PyBool_FromLong( static_cast<long>( o ? 1 : 0 ) ); }
	template<>	static PyObject*	ToPy<double>( double o ) { return PyFloat_FromDouble( o ); }
	template<>	static PyObject*	ToPy<float>( float o ) { return PyFloat_FromDouble( static_cast<double>( o ) ); }
	template<>	static PyObject*	ToPy<char*>( char* o ) { return PyBytes_FromString( o ); }

	template<>	static PyObject*	ToPy<long&&>( long&& o ) { return PyLong_FromLong( o ); }
	template<>	static PyObject*	ToPy<int&&>( int&& o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<unsigned int&&>( unsigned int&& o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<short&&>( short&& o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<bool&&>( bool&& o ) { return PyBool_FromLong( static_cast<long>( o ? 1 : 0 ) ); }
	template<>	static PyObject*	ToPy<double&&>( double&& o ) { return PyFloat_FromDouble( o ); }
	template<>	static PyObject*	ToPy<float&&>( float&& o ) { return PyFloat_FromDouble( static_cast<double>( o ) ); }
	template<>	static PyObject*	ToPy<char*&&>( char*&& o ) { return PyBytes_FromString( o ); }


	template<>	static PyObject*	ToPy<long&>( long& o ) { return PyLong_FromLong( o ); }
	template<>	static PyObject*	ToPy<int&>( int& o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<unsigned int&>( unsigned int& o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<short&>( short& o ) { return PyLong_FromLong( static_cast<long>( o ) ); }
	template<>	static PyObject*	ToPy<bool&>( bool& o ) { return PyBool_FromLong( static_cast<long>( o ? 1 : 0 ) ); }
	template<>	static PyObject*	ToPy<double&>( double& o ) { return PyFloat_FromDouble( o ); }
	template<>	static PyObject*	ToPy<float&>( float& o ) { return PyFloat_FromDouble( static_cast<double>( o ) ); }
	template<>	static PyObject*	ToPy<char*&>( char*& o ) { return PyBytes_FromString( o ); }

	// if you want to compile /w4 /wx you must set this flag, as not all conversions functions will be in use
#pragma warning( disable : 4505 )
}

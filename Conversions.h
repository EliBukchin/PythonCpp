/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once
#include <cmath>
#include <Python.h>
#include <memory>
#include "Config.h"

namespace PythonCpp
{

	// This is a wrapper that holds a pointer to an object
	// which was created in Cpp.
	// The python handle will only have permissions to use this object
	// and cannot create or destroy the allocated base object from within Python
	template< class T >
	struct ClassInstance
	{
		// A special header from holding Python related data
		PyObject_HEAD

		// A pointer to the wrapped object
		T		m_pInstance;

		// A Python structure describing the object
		static	PyTypeObject	s_pyTypeObject;

		ClassInstance() : m_pInstance( nullptr ) {}
	};

	// Template implementation
	template< class T >	PyTypeObject PythonCpp::ClassInstance<T>::s_pyTypeObject;

	
	// Converter functions From PyObjects
	// It is assumed that By default a ClassInstance<>* type is the object
	template< typename T >
	static T		FromPy( PyObject* o ) { return reinterpret_cast< ClassInstance<T>* >( o )->m_pInstance; }

	// Some specialization for primitives
	template<>	static char				FromPy( PyObject* o ) { return PyBytes_AsString(o)[0]; }
	template<>	static unsigned char	FromPy( PyObject* o ) { return static_cast<unsigned char>(PyBytes_AsString(o)[0]); }
	template<>	static signed char		FromPy( PyObject* o ) { return static_cast<signed char>(PyBytes_AsString(o)[0]); }
	template<>	static short			FromPy( PyObject* o ) { return static_cast<short>(PyLong_AsLong(o)); }
	template<>	static int				FromPy( PyObject* o ) { return static_cast<int>(PyLong_AsLong(o)); }
	template<>	static long				FromPy( PyObject* o ) { return PyLong_AsLong(o); }
	template<>	static __int64			FromPy( PyObject* o ) { return static_cast<__int64>(PyLong_AsLong(o)); }
	template<>	static unsigned short	FromPy( PyObject* o ) { return static_cast<unsigned short>(PyLong_AsLong(o)); }
	template<>	static unsigned int		FromPy( PyObject* o ) { return static_cast<unsigned int>(PyLong_AsLong(o)); }
	template<>	static unsigned long	FromPy( PyObject* o ) { return static_cast<unsigned long>(PyLong_AsLong(o)); }
	template<>	static unsigned __int64	FromPy( PyObject* o ) { return static_cast<unsigned __int64>(PyLong_AsLong(o)); }
	template<>	static bool				FromPy( PyObject* o ) { return PyLong_AsLong(o) != 0; }
	template<>	static double			FromPy( PyObject* o ) { return PyFloat_AsDouble(o); }
	template<>	static float			FromPy( PyObject* o ) { return static_cast<float>(PyFloat_AsDouble(o)); }
	template<>	static char*			FromPy( PyObject* o ) { return PyBytes_AsString(PyUnicode_AsUTF8String(o)); }		

	// declare a primitive meta type to allow passing shared pointers
	template<typename T> struct primitive_type
	{
		typedef T type;

		static T		Get( T a ) { return a; }
	};

	template<typename T> struct primitive_type< std::shared_ptr< T > >
	{
		typedef T* type;

		static T*		Get( std::shared_ptr< T > o ) { return o.get(); }
	};

	// Converter functions to PyObject
	template< typename T >
	static PyObject*	ToPy( T o )
	{
		ClassInstance< primitive_type<T>::type >* pNew = PyObject_NEW( ClassInstance< primitive_type<T>::type >, &(ClassInstance< primitive_type<T>::type >::s_pyTypeObject) );
		if ( pNew )
		{
			pNew->m_pInstance = primitive_type<T>::Get( o );
			return reinterpret_cast<PyObject*>(pNew);
		}
		Py_RETURN_NONE;
	}


	// Some specialization for primitives
	template<>	static PyObject*	ToPy( long o ) { return PyLong_FromLong(o); }
	template<>	static PyObject*	ToPy( int o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( unsigned int o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( short o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( bool o ) { return PyBool_FromLong( static_cast<long>(o?1:0) ); }
	template<>	static PyObject*	ToPy( double o ) { return PyFloat_FromDouble(o); }
	template<>	static PyObject*	ToPy( float o ) { return PyFloat_FromDouble( static_cast<double>(o) ); }
	template<>	static PyObject*	ToPy( char* o ) { return PyBytes_FromString(o); }

	template<>	static PyObject*	ToPy( long&& o ) { return PyLong_FromLong(o); }
	template<>	static PyObject*	ToPy( int&& o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( unsigned int&& o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( short&& o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( bool&& o ) { return PyBool_FromLong( static_cast<long>(o?1:0) ); }
	template<>	static PyObject*	ToPy( double&& o ) { return PyFloat_FromDouble(o); }
	template<>	static PyObject*	ToPy( float&& o ) { return PyFloat_FromDouble( static_cast<double>(o) ); }
	template<>	static PyObject*	ToPy( char*&& o ) { return PyBytes_FromString(o); }
	

	template<>	static PyObject*	ToPy( long& o ) { return PyLong_FromLong(o); }
	template<>	static PyObject*	ToPy( int& o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( unsigned int& o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( short& o ) { return PyLong_FromLong( static_cast<long>(o) ); }
	template<>	static PyObject*	ToPy( bool& o ) { return PyBool_FromLong( static_cast<long>(o?1:0) ); }
	template<>	static PyObject*	ToPy( double& o ) { return PyFloat_FromDouble(o); }
	template<>	static PyObject*	ToPy( float& o ) { return PyFloat_FromDouble( static_cast<double>(o) ); }
	template<>	static PyObject*	ToPy( char*& o ) { return PyBytes_FromString(o); }
	

	// if you want to compile /w4 /wx you must set this flag, as not all conversions functions will be in use
	#pragma warning( disable : 4505 )
}
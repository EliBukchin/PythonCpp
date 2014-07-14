/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2014, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once

#include <python.h>
#include <memory>

namespace PythonCpp
{
	// A holder for a specific class instance
	template< class T >
	struct tagInstanceHolder
	{
		//Constructor
		tagInstanceHolder( T o ) : value( o ) {}

		T	value;	// value

		// Copy constructor - deleted
		tagInstanceHolder(const tagInstanceHolder&) = delete;

		// assignment operator - deleted
		tagInstanceHolder&	operator=(tagInstanceHolder&) = delete;
	};

	// Specific class Instance holder
	template< class T >
	class Instance
	{
	public:
		// Special python header
		PyObject_HEAD

		// remove assignment operator
		Instance&	operator=( const Instance & ) = delete;

		// Empty Constructor
		Instance( ) {}

		// Empty destructor
		~Instance( ) {}

		// The instance holder
		tagInstanceHolder<T>*	m_pInstance;

		// Destructo for Python
		static void		PyDealloc( PyObject* o )
		{
			//destructor
			Instance<T>*	pInst = reinterpret_cast<Instance<T>*>( o );
			delete pInst->m_pInstance;
			Py_TYPE( o )->tp_free( o );
		}
	};

	// Object type
	template< class T>
	struct tagPythonObjectTypes
	{
		static PyTypeObject type;
		static bool bInitialized;
	};	
}

// define static python object types
template< class T > PyTypeObject PythonCpp::tagPythonObjectTypes<T>::type;
template< class T > bool PythonCpp::tagPythonObjectTypes<T>::bInitialized = false;

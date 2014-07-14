/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
#pragma once

// Helpers for type templates

/////////////////////////////////////////////////////
//// Extraxt a base type from a type
////////////////////////////////////////////////////
#include "Config.h"

#ifdef PYTHON_CPP_SHARED_PTR_SUPPORT
#include <memory>
#endif

// Default
namespace PythonCpp
{
	template< class T > struct base_type{ typedef T type; };
	template< class T > struct base_type<T*>{ typedef T type; };
	template< class T > struct base_type<const T*>{ typedef T type; };
	template< class T > struct base_type<T&>{ typedef T type; };
	template< class T > struct base_type<const T&>{ typedef T type; };
	template< class T > struct base_type<T* const >{ typedef T type; };
	template< class T > struct base_type<std::shared_ptr<T>>{ typedef T type; };

	template< class T> struct is_pointer{ static constexpr bool value = false; };
	template< class T> struct is_pointer<T*>{ static constexpr bool value = true; };

#ifdef PYTHON_CPP_SHARED_PTR_SUPPORT
	template< class T> struct is_pointer<std::shared_ptr<T>>{ static constexpr bool value = true; };
#endif

	constexpr uint8_t	TYPE_VAUE = 0;
	constexpr uint8_t	TYPE_POINTER = 1;
	constexpr uint8_t	TYPE_SMART_PONTER = 2;

	template< class T> struct type_type { static constexpr uint8_t value = TYPE_VAUE; };
	template< class T> struct type_type<T*> { static constexpr uint8_t value = TYPE_POINTER; };

#ifdef PYTHON_CPP_SHARED_PTR_SUPPORT
	template< class T> struct type_type<std::shared_ptr<T>> { static constexpr uint8_t value = TYPE_SMART_PONTER; };
#endif
}

#include <Python.h>

namespace PythonCpp
{
	static Py_ssize_t&	PrevIndex(Py_ssize_t& i) { (--i); return i; }
}

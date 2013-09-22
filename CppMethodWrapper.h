/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once
#include "Conversions.h"
#include "NativeString.h"

namespace PythonCpp
{
	// A wrapper for Regular Members of a class
	// ID is used to create Unique class for each method even if there are different methods with a similar signature
	template< int ID, typename CppClass, typename Result, typename... Args>
	class MethodWrapper
	{
	public:
		// A definition of a Method pointer
		typedef	Result ( CppClass::*Method ) ( Args...);
		typedef	Result ( CppClass::*ConstMethod ) ( Args...) const;

		// a static member to hold the pointer to CppClass' member
		static	Method			s_pMethod;
		static	ConstMethod		s_pConstMethod;

		// This function will be invoked by Python
		static PyObject*	Do( PyObject* self, PyObject* args )
		{
			(args);			
			// retrieve the calling object's instance
			ClassInstance<CppClass*>* pObj = reinterpret_cast<ClassInstance<CppClass*>*>( self );

			// prepare argument iterator counter
			Py_ssize_t iTupleItem = sizeof...(Args)-1;
			(iTupleItem);

			// run the method and parse the arguments by expanding Args
			PyObject*	res;
			if ( s_pMethod)
			{
				res =  ToPy<Result>( (pObj->m_pInstance->*(s_pMethod)) ( FromPy<Args>( PyTuple_GetItem( args, iTupleItem-- ) )... ) );
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING

			}
			else if ( s_pConstMethod )
			{
				res =  ToPy<Result>( (pObj->m_pInstance->*(s_pConstMethod)) ( FromPy<Args>( PyTuple_GetItem( args, iTupleItem-- ) )... ) );
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING
			}
			else
			{
				Py_RETURN_NONE;
			}

			return res;
		}
	};

	// A specialization of the class to handle methods returning void
	template< int ID, typename CppClass, typename... Args>
	class MethodWrapper< ID, CppClass, void, Args... >
	{
	public:
		typedef	void ( CppClass::*Method ) ( Args...);
		typedef	void ( CppClass::*ConstMethod ) ( Args...) const;
		static	Method	s_pMethod;
		static	ConstMethod	s_pConstMethod;
		static PyObject*	Do( PyObject* self, PyObject* args )
		{
			ClassInstance<CppClass*>* pObj = reinterpret_cast<ClassInstance<CppClass*>*>( self );
			Py_ssize_t iTupleItem = sizeof...(Args)-1;
			(args);
			(iTupleItem);
			if ( s_pMethod )
			{
				(pObj->m_pInstance->*(s_pMethod)) ( FromPy<Args>( PyTuple_GetItem( args, iTupleItem-- ) )... );
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING
			}
			else if ( s_pConstMethod )
			{
				(pObj->m_pInstance->*(s_pConstMethod)) ( FromPy<Args>( PyTuple_GetItem( args, iTupleItem-- ) )... );
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING
			}
			Py_RETURN_NONE;
		}
	};

	// implementation of the two Member pointers
	template< int ID, typename CppClass, typename Result, typename... Args >
	typename MethodWrapper< ID, CppClass, Result, Args... >::Method MethodWrapper< ID, CppClass, Result, Args... >::s_pMethod = nullptr;

	template< int ID, typename CppClass, typename... Args >
	typename MethodWrapper< ID, CppClass, void, Args... >::Method MethodWrapper< ID, CppClass, void, Args... >::s_pMethod = nullptr;

	template< int ID, typename CppClass, typename Result, typename... Args >
	typename MethodWrapper< ID, CppClass, Result, Args... >::ConstMethod MethodWrapper< ID, CppClass, Result, Args... >::s_pConstMethod = nullptr;

	template< int ID, typename CppClass, typename... Args >
	typename MethodWrapper< ID, CppClass, void, Args... >::ConstMethod MethodWrapper< ID, CppClass, void, Args... >::s_pConstMethod = nullptr;


	// a wrapper for static functions.
	// in static functions in python, the self in always null
	template< int ID, typename CppClass, typename Result, typename... Args>
	class StaticMethodWrapper
	{
	public:
		// A definition of a Method pointer
		typedef	Result ( *Method ) ( Args... );

		// a static member to hold the pointer to CppClass' member
		static	Method	s_pMethod;

		// This function will be invoked by Python
		static PyObject*	Do( PyObject* self, PyObject* args )
		{
			// ignore self
			(self);

			// in case of no arguments prevent warning
			(args);
			// prepare argument iterator counter
			Py_ssize_t iTupleItem = sizeof...(Args)-1;
			(iTupleItem);

			// run the method and parse the arguments by expanding Args
			if ( s_pMethod )
			{
				PyObject* res = ToPy<Result>( (*s_pMethod) ( FromPy<Args>( PyTuple_GetItem( args, iTupleItem-- ) )... ) );
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING
				return res;
			}
			Py_RETURN_NONE;
		}
	};

	// and the void specialization
	template< int ID, typename CppClass, typename... Args>
	class StaticMethodWrapper< ID, CppClass, void, Args... >
	{
	public:
		typedef	void ( *Method ) ( Args...);
		static	Method	s_pMethod;
		static PyObject*	Do( PyObject* self, PyObject* args )
		{
			(self);
			Py_ssize_t iTupleItem = sizeof...(Args)-1;
			(iTupleItem);
			(args);
			if ( s_pMethod )
			{
				(*s_pMethod) ( FromPy<Args>( PyTuple_GetItem( args, iTupleItem-- ) )... );
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING
			}
			Py_RETURN_NONE;
		}
	};

	// implementation of the two Member pointers
	template< int ID, typename CppClass, typename Result, typename... Args >
	typename StaticMethodWrapper< ID, CppClass, Result, Args... >::Method StaticMethodWrapper< ID, CppClass, Result, Args... >::s_pMethod = nullptr;

	template< int ID, typename CppClass, typename... Args >
	typename StaticMethodWrapper< ID, CppClass, void, Args... >::Method StaticMethodWrapper< ID, CppClass, void, Args... >::s_pMethod = nullptr;
}

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
	// This class has static members that define and hold a pointer to a method
	// ID - id to allow different functions with similar signatures
	// CppClass - the base cpp class of the method
	// Result - the result of the method
	// Args - arguments of the method
	template< int ID, typename CppClass, typename Result, typename... Args>
	class Method
	{
	public:
		// Definition of a pointer to a method
		typedef Result( CppClass::*PMethod ) ( Args... );

		// Definition of a pointer to a const method
		typedef Result( CppClass::*PConstMethod ) ( Args... ) const;

		// Definition of a pointer to a static method
		typedef Result(*PStaticMethod) (Args...);

		// pointer to the method
		static	PMethod		s_pMethod;

		// pointer to the const method
		static	PConstMethod	s_pConstMethod;

		// pointer to the static method
		static	PStaticMethod	s_pStaticMethod;
	};


	// Implementation of static Method pointers
	template< int ID, typename CppClass, typename Result, typename... Args>
	typename Method< ID, CppClass, Result, Args... >::PMethod
		Method< ID, CppClass, Result, Args... >::s_pMethod = nullptr;

	// Implementation of static const Method pointers
	template< int ID, typename CppClass, typename Result, typename... Args>
	typename Method< ID, CppClass, Result, Args... >::PConstMethod
		Method< ID, CppClass, Result, Args... >::s_pConstMethod = nullptr;

	// Implementation of static const Method pointers
	template< int ID, typename CppClass, typename Result, typename... Args>
	typename Method< ID, CppClass, Result, Args... >::PStaticMethod
		Method< ID, CppClass, Result, Args... >::s_pStaticMethod = nullptr;

	///////////////////////////////////////////////////////////////////////////////
	// Do method - execute methods
	///////////////////////////////////////////////////////////////////////////////
	template< int ID, uint8_t TyprType, typename CallingClass, typename Result, typename... Args >
	struct MethodWrapper
	{
		static PyObject* Do(PyObject* self, PyObject* args)
		{
			(args); // In case ars in never called		
			Instance<CallingClass>* pInstance = reinterpret_cast<Instance<CallingClass>*>(self); // get the calling class instance		
			Py_ssize_t iTupleItem = sizeof...(Args); (iTupleItem); // Get number of arguments

			PyObject* res = nullptr;
			if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pMethod)
			{
				res = ToPy<Result>((pInstance->m_pInstance->value.*(Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pConstMethod)
			{
				res = ToPy<Result>((pInstance->m_pInstance->value.*(Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pConstMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pStaticMethod)
			{
				res = ToPy<Result>((Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pStaticMethod) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else
			{
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif
				Py_RETURN_NONE;
			}
#ifdef PYTHON_CPP_NATIVE_STRING
			g_uiNumberOfTempNativeStrings = 0;
#endif
			return res;
		}
	};

	// For pointers
	template< int ID, typename CallingClass, typename Result, typename... Args >
	struct MethodWrapper<ID, TYPE_POINTER,CallingClass, Result, Args...>
	{		
		static PyObject* Do( PyObject* self, PyObject* args )
		{
			( args ); // In case ars in never called		
			Instance<CallingClass>* pInstance = reinterpret_cast<Instance<CallingClass>*>( self ); // get the calling class instance		
			Py_ssize_t iTupleItem = sizeof...(Args); ( iTupleItem ); // Get number of arguments

			PyObject* res = nullptr;
			if ( Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pMethod )
			{
				res = ToPy<Result>((pInstance->m_pInstance->value->*(Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else if ( Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pConstMethod )
			{
				res = ToPy<Result>((pInstance->m_pInstance->value->*(Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pConstMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pStaticMethod)
			{
				res = ToPy<Result>((Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pStaticMethod) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else
			{
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif
				Py_RETURN_NONE;
			}
#ifdef PYTHON_CPP_NATIVE_STRING
			g_uiNumberOfTempNativeStrings = 0;
#endif
			return res;
		}
	};

	// For smart pointers
	template< int ID, typename CallingClass, typename Result, typename... Args >
	struct MethodWrapper<ID, TYPE_SMART_PONTER, CallingClass, Result, Args...>
	{
		static PyObject* Do(PyObject* self, PyObject* args)
		{
			(args); // In case ars in never called		
			Instance<CallingClass>* pInstance = reinterpret_cast<Instance<CallingClass>*>(self); // get the calling class instance		
			Py_ssize_t iTupleItem = sizeof...(Args); (iTupleItem); // Get number of arguments

			PyObject* res = nullptr;
			if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pMethod)
			{
				res = ToPy<Result>((pInstance->m_pInstance->value.get()->*(Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pConstMethod)
			{
				res = ToPy<Result>((pInstance->m_pInstance->value.get()->*(Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pConstMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else if (Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pStaticMethod)
			{
				res = ToPy<Result>((Method< ID, base_type<CallingClass>::type, Result, Args... >::s_pStaticMethod) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...));
			}
			else
			{
#ifdef PYTHON_CPP_NATIVE_STRING
				g_uiNumberOfTempNativeStrings = 0;
#endif
				Py_RETURN_NONE;
			}
#ifdef PYTHON_CPP_NATIVE_STRING
			g_uiNumberOfTempNativeStrings = 0;
#endif
			return res;
		}
	};
	

	// In case of void Result
	template< int ID, typename CallingClass, typename... Args >
	struct MethodWrapper< ID, TYPE_VAUE, CallingClass, void, Args... >
	{
		static PyObject* Do(PyObject* self, PyObject* args)
		{
			(args); // In case ars in never called		
			Instance<CallingClass>* pInstance = reinterpret_cast<Instance<CallingClass>*>(self); // get the calling class instance		
			Py_ssize_t iTupleItem = sizeof...(Args); (iTupleItem); // Get number of arguments

			if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pMethod)
			{
				(pInstance->m_pInstance->value.*(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
			else if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pConstMethod)
			{
				(pInstance->m_pInstance->value.*(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pConstMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
			else if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pStaticMethod)
			{
				(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pStaticMethod) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
#ifdef PYTHON_CPP_NATIVE_STRING
			g_uiNumberOfTempNativeStrings = 0;
#endif
			Py_RETURN_NONE;
		}
	};

	template< int ID, typename CallingClass, typename... Args >
	struct MethodWrapper< ID, TYPE_POINTER, CallingClass, void, Args... >
	{
		// In case of void and pointer
		static PyObject* Do( PyObject* self, PyObject* args )
		{
			( args ); // In case ars in never called		
			Instance<CallingClass>* pInstance = reinterpret_cast<Instance<CallingClass>*>( self ); // get the calling class instance		
			Py_ssize_t iTupleItem = sizeof...(Args); ( iTupleItem ); // Get number of arguments

			if ( Method< ID, base_type<CallingClass>::type, void, Args... >::s_pMethod )
			{
				(pInstance->m_pInstance->value->*(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
			else if ( Method< ID, base_type<CallingClass>::type, void, Args... >::s_pConstMethod )
			{
				(pInstance->m_pInstance->value->*(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pConstMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
			else if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pStaticMethod)
			{
				(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pStaticMethod) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
#ifdef PYTHON_CPP_NATIVE_STRING
			g_uiNumberOfTempNativeStrings = 0;
#endif
			Py_RETURN_NONE;
		}
	};

	// for smart pointers, return void
	template< int ID, typename CallingClass, typename... Args >
	struct MethodWrapper< ID, TYPE_SMART_PONTER, CallingClass, void, Args... >
	{
		// In case of void and pointer
		static PyObject* Do(PyObject* self, PyObject* args)
		{
			(args); // In case ars in never called		
			Instance<CallingClass>* pInstance = reinterpret_cast<Instance<CallingClass>*>(self); // get the calling class instance		
			Py_ssize_t iTupleItem = sizeof...(Args); (iTupleItem); // Get number of arguments

			if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pMethod)
			{
				(pInstance->m_pInstance->value.get()->*(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
			else if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pConstMethod)
			{
				(pInstance->m_pInstance->value.get()->*(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pConstMethod)) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
			else if (Method< ID, base_type<CallingClass>::type, void, Args... >::s_pStaticMethod)
			{
				(Method< ID, base_type<CallingClass>::type, void, Args... >::s_pStaticMethod) (FromPy<Args>(PyTuple_GetItem(args, PrevIndex(iTupleItem)))...);
			}
#ifdef PYTHON_CPP_NATIVE_STRING
			g_uiNumberOfTempNativeStrings = 0;
#endif
			Py_RETURN_NONE;
		}
	};
}
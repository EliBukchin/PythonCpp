/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2014, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once

#ifndef IGNORE_PYTHON_CPP
#include "Module.h"

namespace PythonCpp
{
	bool	StartPython( const char* strModuleName );
	bool	StopPython( );
	bool	EvalExpression( const char* strExpression );

	typedef void( *PythonPrinterFunc ) ( const char * );

	bool	SetPythonStandardOutput( PythonPrinterFunc pFunc );
	bool	SetPythonStandardError( PythonPrinterFunc pFunc );
}

#define ADD_CLASS_TO_PYTHON( Namespace, ClassName ) \
	const static PythonCpp::Class< Namespace::ClassName >* ClassName##_pPythonCppClass_AUTOGEN = PythonCpp::Module::CreateClass<  Namespace::ClassName >(#ClassName); \
	template<> \
	void	PythonCpp::Class< Namespace::ClassName >::ClassInitialization()

#define ADD_CLASS_REFERENCE_TO_PYTHON( Namespace, ClassName ) \
	const static PythonCpp::Class< Namespace::ClassName& >* ClassName##_Ref_pPythonCppClass_AUTOGEN = PythonCpp::Module::CreateClass<  Namespace::ClassName& >(#ClassName "&" ); \
	template<> \
	void	PythonCpp::Class< Namespace::ClassName& >::ClassInitialization()

#define ADD_CLASS_POINTER_TO_PYTHON( Namespace, ClassName ) \
	const static PythonCpp::Class< Namespace::ClassName* >* P##ClassName##_pPythonCppClass_AUTOGEN = PythonCpp::Module::CreateClass<  Namespace::ClassName* >(#ClassName "*"); \
	template<> \
	void	PythonCpp::Class< Namespace::ClassName* >::ClassInitialization()

#define SET_PARENT( Parent ) \
	SetParent<Parent>()

#define EXPORT_METHOD( Method ) \
	AddMethod<__COUNTER__>(#Method, nullptr, &base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method)

#define EXPORT_METHOD_DESC( Method, description ) \
	AddMethod<__COUNTER__>(#Method, description, &base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method)

#define EXPORT_STATIC_METHOD( Method ) \
	AddStaticMethod<__COUNTER__>(#Method, nullptr, &base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method)

#define EXPORT_STATIC_METHOD_DESC( Method, description ) \
	AddStaticMethod<__COUNTER__>(#Method, description, &base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method)


#define PYTHON_ADD_METHOD_OVERLOADED( Method, ReturnType, ... ) \
	AddMethod<__COUNTER__>(#Method, nullptr, static_cast<ReturnType(base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::*)(__VA_ARGS__)>(&base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method));

#define PYTHON_ADD_METHOD_OVERLOADED_DESC( Method, desc, ReturnType, ... ) \
	AddMethod<__COUNTER__>(#Method, desc, static_cast<ReturnType(base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::*)(__VA_ARGS__)>(&base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method));

#define PYTHON_ADD_METHOD_OVERLOADED_CONST( Method, ReturnType, ... ) \
	AddMethod<__COUNTER__>(#Method, nullptr, static_cast<ReturnType(base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::*)(__VA_ARGS__) const >(&base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method));

#define PYTHON_ADD_METHOD_OVERLOADED_CONST_DESC( Method, desc, ReturnType, ... ) \
	AddMethod<__COUNTER__>(#Method, desc, static_cast<ReturnType(base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::*)(__VA_ARGS__) const>(&base_type<std::decay<base_type<typename decltype(this)>::type::type>::type>::type::Method));
	
#else

#define ADD_CLASS_TO_PYTHON( Namespace, Class ) namespace __PythonCPPPlaceHolderNamespace
#define SET_PARENT( Parent )
#define EXPORT_METHOD( Method )
#define EXPORT_METHOD_DESC( Method, description )
#define EXPORT_STATIC_METHOD( Method )
#define EXPORT_STATIC_METHOD_DESC( Method, description )
#define PYTHON_ADD_METHOD_OVERLOADED( Method, ReturnType, ... )
#define PYTHON_ADD_METHOD_OVERLOADED_DESC( Method, desc, ReturnType, ... )
#define PYTHON_ADD_METHOD_OVERLOADED_CONST( Method, ReturnType, ... )
#define PYTHON_ADD_METHOD_OVERLOADED_CONST_DESC( Method, desc, ReturnType, ... )
#endif
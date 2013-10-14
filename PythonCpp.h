/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once
#include "PythonModule.h"

namespace PythonCpp
{
	bool	StartPython( const char* strModuleName );
	bool	StopPython();
	bool	EvalExpression( const char* strExpression );

	typedef void	(*PythonPrinterFunc) ( const char *);

	bool	SetPythonStandardOutput( PythonPrinterFunc pFunc );
	bool	SetPythonStandardError( PythonPrinterFunc pFunc );
	
}

template < typename T >
struct StripPointer
{
	typedef T type;
};

template < typename T >
struct StripPointer<T*>
{
	typedef typename StripPointer<T>::type type;
};

#define ADD_CLASS_TO_PYTHON( Namespace, Class ) \
	const static PythonCpp::ClassWrapper<Namespace::Class>* Class##_pPythonCppClass_AUTOGEN = PythonCpp::Module::CreateClass<Namespace::Class>( #Class ); \
	template<> \
	void	PythonCpp::ClassWrapper<Namespace::Class>::AddClassDescription()

#define SET_PARENT( Parent ) \
	SetParent<Parent>()

#define EXPORT_METHOD( Method ) \
	AddMethod<__COUNTER__>( #Method, nullptr, &StripPointer<decltype(this)>::type::wrapped_class_type::Method )

#define EXPORT_METHOD_DESC( Method, description ) \
	AddMethod<__COUNTER__>( #Method, description, &StripPointer<decltype(this)>::type::wrapped_class_type::Method )

#define EXPORT_STATIC_METHOD( Method ) \
	AddStaticMethod<__COUNTER__>( #Method, nullptr, &StripPointer<decltype(this)>::type::wrapped_class_type::Method )

#define EXPORT_STATIC_METHOD_DESC( Method, description ) \
	AddStaticMethod<__COUNTER__>( #Method, description, &StripPointer<decltype(this)>::type::wrapped_class_type::Method )

#define PYTHON_ADD_METHOD_OVERLOADED( Method, ReturnType, ... ) \
	AddMethod<__COUNTER__>( #Method, nullptr, static_cast<ReturnType (StripPointer<decltype(this)>::type::wrapped_class_type::*)(__VA_ARGS__)>(&StripPointer<decltype(this)>::type::wrapped_class_type::Method) );

#define PYTHON_ADD_METHOD_OVERLOADED_DESC( Method, desc, ReturnType, ... ) \
	AddMethod<__COUNTER__>( #Method, desc, static_cast<ReturnType (StripPointer<decltype(this)>::type::wrapped_class_type::*)(__VA_ARGS__)>(&StripPointer<decltype(this)>::type::wrapped_class_type::Method) );

/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
#pragma once

#include "Method.h"

namespace PythonCpp
{
	// Default Python object definition
	extern PyTypeObject	g_pyDefaultType;

	// Abase class to hold all exported classes
	class ClassBase
	{
		friend class Module;
	public:
		// Destructor
		virtual ~ClassBase();

		// Get python type
		virtual PyTypeObject*	GetType() { return nullptr; }

		// get class name
		const char*	GetName() const { return m_strName; }

	private:
		// Register the class with python
		virtual bool	Register( PyObject* pyModule ) { ( pyModule ); return false; }

	protected:
		// Constructor
		ClassBase( const char* strName );

	public:
		// Next class in list
		ClassBase*	m_pNext;
	private:
		char*	m_strName;
	};

	// Hold metadata of a method of a class
	struct MethodDesc
	{
		PyCFunction		m_pFunc;	// A pointer to the 'Do' static class
		char*			m_strName;	// name of the method
		char*			m_strDoc;	// documantatin of the class
		Py_ssize_t		m_numArgs;	// number of arguments that the method recieaves
		bool			m_bStatic;	// Is the method static
		MethodDesc*		m_pNext;	// Next method in list

		// Constructor
		MethodDesc( const char* strName, const char* strDoc ) :
			m_pNext( nullptr ),
			m_strName( nullptr ),
			m_strDoc( nullptr )
		{
			if ( strName )
			{
				m_strName = new char[ strlen( strName ) + 1 ];
				strcpy_s( m_strName, strlen( strName ) + 1, strName );
				m_strName[ strlen( strName ) ] = 0;
			}

			if ( strDoc )
			{
				m_strDoc = new char[ strlen( strDoc ) + 1 ];
				strcpy_s( m_strDoc, strlen( strDoc ) + 1, strDoc );
				m_strDoc[ strlen( strDoc ) ] = 0;
			}
		}

		// The two strings are released when 'm_arrPyMethodDefs' is released
	};


	// holds a specific class
	template< class CppClass >
	class Class : public ClassBase
	{
		friend class Module;
	public:
		typedef CppClass type;

		// Methods for adding methond to the cpp class
		template< int ID, typename Result, typename...Args>
		bool	AddMethod( const char* strName, const char* strDoc, Result( base_type<CppClass>::type::*method ) ( Args... ) );

		template< int ID, typename Result, typename...Args>
		bool	AddMethod( const char* strName, const char* strDoc, Result( base_type<CppClass>::type::*method ) ( Args... ) const );

		template< int ID, typename Result, typename...Args>
		bool	AddStaticMethod( const char* strName, const char* strDoc, Result( *method ) ( Args... ) );

		// Set a parent to the class
		template< class ParentClass >
		void	SetParent();

		// Destructor
		virtual ~Class();

		// Override
		virtual PyTypeObject*	GetType() override { return &(tagPythonObjectTypes<CppClass>::type); }

	private:
		// Constructor
		Class( const char* strName );

		// disable copy constructor and assignment operator
		Class( const Class& o ) = delete;
		Class&	operator=( const Class& rhs ) = delete;

		// Override
		virtual bool	Register( PyObject* pyModule ) override;

		// A class That is run upon PythonCpp2 initialization
		// Here the methods of the class are registered to Class class
		__inline void	ClassInitialization();

	private:
		// method definitions for python
		PyMethodDef*	m_arrPyMethodDefs;

		// A temporary list that holds all the methods of the class
		// It stores and configures method definition for initializing python
		// the list is released as soon as python is initialized
		MethodDesc*		m_listMethods;

		// Number of methods
		unsigned int	m_uiNumMethods;
	};

#include "ClassImpl.h"
}
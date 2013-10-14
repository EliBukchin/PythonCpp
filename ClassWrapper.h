/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/

#pragma once
#include "CppMethodWrapper.h"

namespace PythonCpp
{
	extern PyTypeObject	g_pyDefualtType;

	// A base class for the cpp class wrapper to use in collections
	class ClassWrapperBase
	{
		friend class Module;
	public:		
		virtual ~ClassWrapperBase();
		virtual PyTypeObject*	GetType() { return nullptr; }
		const char* GetName() const { return m_strName; }

		ClassWrapperBase*		m_pNext;

	private:
		virtual bool Register( PyObject* pyModule ) { (pyModule); return false; }

	protected:
		ClassWrapperBase( const char* strName );

	private:
		char* m_strName;
	};

	// template class wrapper class
	template< class CppClass >
	class ClassWrapper : public ClassWrapperBase
	{
		friend class Module;
	public:
		// For adding regular methods
		// strName: Name of the function in python
		// strDoc: A short documentation about the function ( can be nullptr )
		// method: a pointer to the method
		template< int ID, typename Result, typename... Args>
		bool	AddMethod( const char* strName, const char* strDoc, Result ( CppClass::*method ) ( Args...) );

		template< int ID, typename Result, typename... Args>
		bool	AddMethod( const char* strName, const char* strDoc, Result ( CppClass::*method ) ( Args...) const );

		// For adding static methods
		// strName: Name of the function in python
		// strDoc: A short documentation about the function ( can be nullptr )
		// method: a pointer to the method
		template< int ID, typename Result, typename... Args>
		bool	AddStaticMethod( const char* strName, const char* strDoc, Result ( *method ) ( Args...) );

		// Set A parent Class for this class
		template < class Parent >
		void	SetParent();

		// destructor
		virtual ~ClassWrapper();

		// Override
		virtual PyTypeObject*	GetType() { return &(ClassInstance<CppClass*>::s_pyTypeObject); }

		// export the type of the class wrapped
		typedef CppClass wrapped_class_type;
	private:
		// constructor
		ClassWrapper( const char* strName );

		// disable copy constructor and assignment operator
		ClassWrapper( const ClassWrapper& o);
		ClassWrapper&	operator=(const ClassWrapper& rhs );

		// Override
		virtual bool	Register( PyObject* pyModule );

		INLINE void	AddClassDescription();

		// A structure to hold metadata on a method
		struct MethodDesc
		{
			PyCFunction		m_pFunc;		// A pointer to the Python function ( Do )
			char*			m_strName;		// The name of the method
			char*			m_strDoc;		// The documentation of the method
			Py_ssize_t		m_numArgs;		// The number of arguments the method expects
			bool			m_bStatic;		// Flag is method is static
			MethodDesc*		m_pNext;		// Next method in the list

			// constructor
			MethodDesc( const char* strName, const char* strDoc ) : m_pNext( nullptr ) , m_strName( nullptr ), m_strDoc( nullptr )
			{
				if ( strName )
				{
					m_strName = new char[ strlen(strName) + 1];
					strcpy_s( m_strName, strlen(strName) + 1, strName );
					m_strName[strlen(strName)] = 0;
				}

				if ( strDoc )
				{
					m_strDoc = new char[ strlen(strDoc) + 1];
					strcpy_s( m_strDoc, strlen(strDoc) + 1, strDoc );
					m_strDoc[strlen(strDoc)] = 0;
				}				
			}

			//destructor
			~MethodDesc()
			{
				// These strings will be released when m_arrPyMethodDefs is deleted
				//if ( m_strName ) delete[] m_strName;
				//if ( m_strDoc ) delete[] m_strDoc;
			}
		};

		// method definitions for python
		PyMethodDef*	m_arrPyMethodDefs;

		// A temporary list that holds all the methods of the class
		// It stores and configures method definition for initializing python
		// the list is released as soon as python is initialized
		MethodDesc*		m_listMethods;

		// Number of methods
		unsigned int	m_uiNumMethods;
	};

	template< class CppClass >
	template< class Parent >
	void PythonCpp::ClassWrapper<CppClass>::SetParent()
	{
		ClassInstance<CppClass*>::s_pyTypeObject.tp_base = &(ClassInstance<Parent>::s_pyTypeObject);
	}

	template< class CppClass >
	PythonCpp::ClassWrapper<CppClass>::ClassWrapper( const char* strName ) : ClassWrapperBase( strName ),
		m_arrPyMethodDefs( nullptr ),
		m_listMethods( nullptr ),
		m_uiNumMethods( 0 )
	{
		ClassInstance<CppClass*>::s_pyTypeObject = g_pyDefualtType;		
	}

	template< class CppClass >
	PythonCpp::ClassWrapper<CppClass>::~ClassWrapper()
	{
		if ( m_arrPyMethodDefs )
		{
			for ( unsigned int i = 0 ; i < m_uiNumMethods; ++i )
			{
				delete[] m_arrPyMethodDefs[i].ml_name;
				delete[] m_arrPyMethodDefs[i].ml_doc;
			}
			delete[] m_arrPyMethodDefs;
			m_arrPyMethodDefs = nullptr;
		}
		else
		{
			MethodDesc* pNewHead = m_listMethods;
			while ( pNewHead )
			{				
				delete[] pNewHead->m_strName;
				delete[] pNewHead->m_strDoc;
				pNewHead = pNewHead->m_pNext;
			}			
		}
		
		while ( m_listMethods )
		{
			MethodDesc* pNewHead = m_listMethods->m_pNext;
			delete m_listMethods;
			m_listMethods = pNewHead;
		}		
	}

	template< class CppClass >
	template< int ID, typename Result, typename... Args >
	bool PythonCpp::ClassWrapper< CppClass >::AddMethod( const char* strName, const char* strDoc, Result ( CppClass::*method ) ( Args...) )
	{
		MethodWrapper< ID, CppClass, Result, Args... >::s_pMethod = method;
		MethodDesc*		pNew = new MethodDesc( strName, strDoc );
		pNew->m_numArgs = sizeof...( Args );
		pNew->m_bStatic = false;
		pNew->m_pFunc = MethodWrapper< ID, CppClass, Result, Args... >::Do;
		pNew->m_pNext = m_listMethods;
		m_listMethods = pNew;
		++m_uiNumMethods;
		return true;
	}

	template< class CppClass >
	template< int ID, typename Result, typename... Args >
	bool PythonCpp::ClassWrapper< CppClass >::AddMethod( const char* strName, const char* strDoc, Result ( CppClass::*method ) ( Args...) const )
	{
		MethodWrapper< ID, CppClass, Result, Args... >::s_pConstMethod = method;
		MethodDesc*		pNew = new MethodDesc( strName, strDoc );
		pNew->m_numArgs = sizeof...( Args );
		pNew->m_bStatic = false;
		pNew->m_pFunc = MethodWrapper< ID, CppClass, Result, Args... >::Do;
		pNew->m_pNext = m_listMethods;
		m_listMethods = pNew;
		++m_uiNumMethods;
		return true;
	}

	template< class CppClass >
	template< int ID, typename Result, typename... Args>
	bool PythonCpp::ClassWrapper< CppClass >::AddStaticMethod( const char* strName, const char* strDoc, Result ( *method ) ( Args...) )
	{
		StaticMethodWrapper< ID, CppClass, Result, Args... >::s_pMethod = method;
		MethodDesc*		pNew = new MethodDesc( strName, strDoc );
		pNew->m_numArgs = sizeof...( Args );
		pNew->m_bStatic = true;
		pNew->m_pFunc = StaticMethodWrapper< ID, CppClass, Result, Args... >::Do;
		pNew->m_pNext = m_listMethods;
		m_listMethods = pNew;
		++m_uiNumMethods;
		return true;
	}

	template< class CppClass >
	bool PythonCpp::ClassWrapper<CppClass>::Register( PyObject* pyModule )
	{
		if ( !pyModule ) return false;

		AddClassDescription();

		// fill the def array
		m_arrPyMethodDefs = new PyMethodDef[ m_uiNumMethods + 1 ];
		unsigned int i = 0;
		while( m_listMethods )
		{
			m_arrPyMethodDefs[i].ml_name = m_listMethods->m_strName;
			m_arrPyMethodDefs[i].ml_doc = m_listMethods->m_strDoc;
			m_arrPyMethodDefs[i].ml_meth = m_listMethods->m_pFunc;
			m_arrPyMethodDefs[i].ml_flags = m_listMethods->m_numArgs ? METH_VARARGS : METH_NOARGS;
			m_arrPyMethodDefs[i].ml_flags |= m_listMethods->m_bStatic ? METH_STATIC : 0;
			MethodDesc* temp = m_listMethods;			
			m_listMethods = m_listMethods->m_pNext;
			delete temp;
			++i;
		}

		m_arrPyMethodDefs[i].ml_name = 0;
		m_arrPyMethodDefs[i].ml_doc = 0;
		m_arrPyMethodDefs[i].ml_meth = 0;
		m_arrPyMethodDefs[i].ml_flags = 0;

		// Update Class definition
		PyTypeObject* pPyTypeObject = &(ClassInstance<CppClass*>::s_pyTypeObject);
		pPyTypeObject->tp_name = GetName();
		pPyTypeObject->tp_basicsize = sizeof(ClassInstance<CppClass*>);
		pPyTypeObject->tp_doc = "Embedded C++ class";
		pPyTypeObject->tp_methods = m_arrPyMethodDefs;
		//m_PyObjectType.tp_new = PyType_GenericNew; // This class cannot be created from within Python

		// Register the class with python
		int res = PyType_Ready( pPyTypeObject );
		if ( res == 0 )
		{
			Py_INCREF( pPyTypeObject );
			res = PyModule_AddObject( pyModule, GetName(), reinterpret_cast<PyObject*>( pPyTypeObject ) );
		}
		else
		{
			PyErr_Print();
		}

		return res == 0;
	}

}

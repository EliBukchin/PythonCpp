/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
#pragma once

#include "Class.h"

namespace PythonCpp
{
	class Module
	{
		friend bool	StartPython( const char* strModuleName );
		friend bool StopPython();

	public:
		template< class T >
		static Class<T>*	CreateClass( const char* strClassName )
		{
			Class<T>*	pClass = nullptr;
			ClassBase* pIter = s_listClasses;

			while ( pIter != nullptr )
			{
				if ( strcmp( pIter->GetName(), strClassName ) == 0 )
				{
					pClass = static_cast<Class<T>*>( pIter );
				}
				pIter = pIter->m_pNext;
			}

			if ( pClass == nullptr )
			{
				pClass = new Class<T>( strClassName );
				pClass->m_pNext = nullptr;

				ClassBase*	pList = s_listClasses;
				if ( pList == nullptr )
				{
					s_listClasses = static_cast<ClassBase*>( pClass );
				}
				else
				{
					while ( pList->m_pNext ) { pList = pList->m_pNext; }
					pList->m_pNext = static_cast<ClassBase*>( pClass );
				}
			}	

			return pClass;
		}

		static	void	SetName( const char* strName );

	private:
		static PyObject*	__Init( );
		static bool			Release( );

	private:
		static ClassBase*	s_listClasses;
		static PyObject*	s_pyModule;
		static PyModuleDef	s_pyModuleDef;
	};
}
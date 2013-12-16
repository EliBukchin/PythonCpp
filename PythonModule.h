/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/


#pragma once
#include "Conversions.h"
#include "NativeString.h"
#include "CppMethodWrapper.h"
#include "ClassWrapper.h"

namespace PythonCpp
{
	// Manages an entire module. As of this version there can be only one module
	class Module
	{
		friend bool StartPython( const char* strModuleName );
		friend bool StopPython();
	public:

		template< class T >
		static ClassWrapper<T>*		CreateClass( const char* strClassName )
		{
			// Prevent duplicates
			ClassWrapper<T>*	pClass = nullptr;
			ClassWrapperBase* pIter = s_listClasses;
			while( pIter != nullptr )
			{
				if ( strcmp(pIter->GetName(), strClassName) == 0 )
				{
					pClass = static_cast<ClassWrapper<T>*>(pIter);
				}
				pIter = pIter->m_pNext;
			}

			if ( pClass == nullptr )
			{
				pClass = new ClassWrapper<T>( strClassName );
				pClass->m_pNext = nullptr;

				ClassWrapperBase* pList = s_listClasses;
				if ( pList == nullptr )
				{
					s_listClasses = static_cast<ClassWrapperBase*>( pClass );
				}
				else
				{
					while ( pList->m_pNext )
					{
						pList = pList->m_pNext;
					}
					pList->m_pNext = static_cast<ClassWrapperBase*>( pClass );
				}
			}						
			return pClass;
		}

		static	void	SetName( const char* strName );

	private:
		static PyObject*	__Init();
		static bool			Release();

	private:
		static ClassWrapperBase*		s_listClasses;
		static PyObject*				s_pyModule;
		static PyModuleDef				s_pyModuleDef;
	};
}
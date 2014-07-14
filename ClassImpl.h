/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
template< class CppClass >
template< int ID, typename Result, typename...Args>
bool	PythonCpp::Class< CppClass >::AddMethod( const char* strName, const char* strDoc, Result( base_type<CppClass>::type::*method ) ( Args... ) )
{
	Method<ID, base_type<CppClass>::type, Result, Args... >::s_pMethod = method;
	MethodDesc*	pNew = new MethodDesc( strName, strDoc );
	pNew->m_numArgs = sizeof...( Args );
	pNew->m_bStatic = false;
	pNew->m_pFunc = MethodWrapper<ID, type_type<CppClass>::value, CppClass, Result, Args...>::Do;
	pNew->m_pNext = m_listMethods;
	m_listMethods = pNew;
	++m_uiNumMethods;
	return true;
}

template< class CppClass >
template< int ID, typename Result, typename...Args>
bool	PythonCpp::Class< CppClass >::AddMethod( const char* strName, const char* strDoc, Result( base_type<CppClass>::type::*method ) ( Args... ) const )
{
	Method<ID, base_type<CppClass>::type, Result, Args... >::s_pConstMethod = method;
	MethodDesc*	pNew = new MethodDesc( strName, strDoc );
	pNew->m_numArgs = sizeof...( Args );
	pNew->m_bStatic = false;
	pNew->m_pFunc = MethodWrapper<ID, type_type<CppClass>::value, CppClass, Result, Args...>::Do;
	pNew->m_pNext = m_listMethods;
	m_listMethods = pNew;
	++m_uiNumMethods;
	return true;
}

template< class CppClass >
template< int ID, typename Result, typename...Args>
bool	PythonCpp::Class< CppClass >::AddStaticMethod( const char* strName, const char* strDoc, Result( *method ) ( Args... ) )
{
	Method<ID, base_type<CppClass>::type, Result, Args... >::s_pStaticMethod = method;
	MethodDesc*	pNew = new MethodDesc( strName, strDoc );
	pNew->m_numArgs = sizeof...( Args );
	pNew->m_bStatic = true;
	pNew->m_pFunc = MethodWrapper<ID, type_type<CppClass>::value, CppClass, Result, Args...>::Do;
	pNew->m_pNext = m_listMethods;
	m_listMethods = pNew;
	++m_uiNumMethods;
	return true;
}

template< class CppClass >
template< class Parent >
void PythonCpp::Class<CppClass>::SetParent( )
{
	tagPythonObjectTypes<CppClass>::type.tp_base = &(tagPythonObjectTypes<Parent>::type);
}

template< class CppClass >
PythonCpp::Class<CppClass>::~Class( )
{
	if ( m_arrPyMethodDefs )
	{
		for ( unsigned int i = 0 ; i < m_uiNumMethods; ++i )
		{
			delete[ ] m_arrPyMethodDefs[ i ].ml_name;
			delete[ ] m_arrPyMethodDefs[ i ].ml_doc;
		}
		delete[ ] m_arrPyMethodDefs;
		m_arrPyMethodDefs = nullptr;
	}
	else
	{
		MethodDesc* pNewHead = m_listMethods;
		while ( pNewHead )
		{
			delete[ ] pNewHead->m_strName;
			delete[ ] pNewHead->m_strDoc;
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
PythonCpp::Class<CppClass>::Class( const char* strName ) : ClassBase( strName ),
m_arrPyMethodDefs( nullptr ),
m_listMethods( nullptr ),
m_uiNumMethods( 0 )
{
	tagPythonObjectTypes<CppClass>::type = g_pyDefaultType;
	tagPythonObjectTypes<CppClass>::type.tp_dealloc = Instance<CppClass>::PyDealloc;
	tagPythonObjectTypes<CppClass>::bInitialized = true;
}

template< class CppClass >
bool PythonCpp::Class<CppClass>::Register( PyObject* pyModule )
{
	if ( !pyModule ) return false;

	ClassInitialization();

	// fill the def array
	m_arrPyMethodDefs = new PyMethodDef[ m_uiNumMethods + 1 ];
	unsigned int i = 0;
	while ( m_listMethods )
	{
		m_arrPyMethodDefs[ i ].ml_name = m_listMethods->m_strName;
		m_arrPyMethodDefs[ i ].ml_doc = m_listMethods->m_strDoc;
		m_arrPyMethodDefs[ i ].ml_meth = m_listMethods->m_pFunc;
		m_arrPyMethodDefs[ i ].ml_flags = m_listMethods->m_numArgs ? METH_VARARGS : METH_NOARGS;
		m_arrPyMethodDefs[ i ].ml_flags |= m_listMethods->m_bStatic ? METH_STATIC : 0;
		MethodDesc* temp = m_listMethods;
		m_listMethods = m_listMethods->m_pNext;
		delete temp;
		++i;
	}

	m_arrPyMethodDefs[ i ].ml_name = 0;
	m_arrPyMethodDefs[ i ].ml_doc = 0;
	m_arrPyMethodDefs[ i ].ml_meth = 0;
	m_arrPyMethodDefs[ i ].ml_flags = 0;

	// Update Class definition
	PyTypeObject* pPyTypeObject = &(tagPythonObjectTypes<CppClass>::type);
	pPyTypeObject->tp_name = GetName( );
	pPyTypeObject->tp_basicsize = sizeof( Instance<CppClass> );
	pPyTypeObject->tp_doc = "Embedded C++ class";
	pPyTypeObject->tp_methods = m_arrPyMethodDefs;
	//m_PyObjectType.tp_new = PyType_GenericNew; // This class cannot be created from within Python

	// Register the class with python
	int res = PyType_Ready( pPyTypeObject );
	if ( res == 0 )
	{
		Py_INCREF( pPyTypeObject );
		res = PyModule_AddObject( pyModule, GetName( ), reinterpret_cast<PyObject*>( pPyTypeObject ) );
	}
	else
	{
		PyErr_Print( );
	}

	return res == 0;
}
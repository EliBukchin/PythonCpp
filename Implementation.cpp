/**          == Python Cpp ==
	A library that reflects C++ classes to Python
	Copyright 2012-2013, Eli Bukchin.                
	
	The program is distributed under the terms of the GNU General Public License
	see LICENSE.txt
*/
#include "PythonCpp.h"

///////////////////////////////////////////////////
// Class.h
///////////////////////////////////////////////////

PyTypeObject	PythonCpp::g_pyDefaultType =
{
	PyVarObject_HEAD_INIT( 0, 0 )
	0, /* tp_name */
	0, /* tp_basicsize */
	0, /* tp_itemsize */
	0, /* tp_dealloc */
	0, /* tp_print */
	0, /* tp_getattr */
	0, /* tp_setattr */
	0, /* tp_reserved */
	0, /* tp_repr */
	0, /* tp_as_number */
	0, /* tp_as_sequence */
	0, /* tp_as_mapping */
	0, /* tp_hash */
	0, /* tp_call */
	0, /* tp_str */
	0, /* tp_getattro */
	0, /* tp_setattro */
	0, /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, /* tp_flags */
	0, /* tp_doc */
	0, /* tp_traverse */
	0, /* tp_clear */
	0, /* tp_richcompare */
	0, /* tp_weaklistoffset */
	0, /* tp_iter */
	0, /* tp_iternext */
	0, /* tp_methods */
	0, /* tp_members */
	0, /* tp_getset */
	0, /* tp_base */
	0, /* tp_dict */
	0, /* tp_descr_get */
	0, /* tp_descr_set */
	0, /* tp_dictoffset */
	0, /* tp_init */
	0, /* tp_alloc */
	0, /* tp_new */
};

PythonCpp::ClassBase::ClassBase( const char* strName )
{
	m_strName = new char[ strlen( strName ) + 1 ];
	strcpy_s( m_strName, strlen( strName ) + 1, strName );
	m_strName[ strlen( strName ) ] = 0;
}

PythonCpp::ClassBase::~ClassBase()
{
	if ( m_strName )
	{
		delete[ ] m_strName;
		m_strName = nullptr;
	}
}

///////////////////////////////////////////////////
// Module.h
///////////////////////////////////////////////////

PythonCpp::ClassBase*		PythonCpp::Module::s_listClasses = nullptr;
PyObject*					PythonCpp::Module::s_pyModule = nullptr;
PyModuleDef					PythonCpp::Module::s_pyModuleDef =
{
	PyModuleDef_HEAD_INIT,
	nullptr,
	"C++ Python Bindings",
	-1,
	0,
	0,
	0,
	0,
	0
};

void	PythonCpp::Module::SetName( const char* strName )
{
	char* pNew = new char[ strlen( strName ) + 1 ];
	strcpy_s( pNew, strlen( strName ) + 1, strName );
	pNew[ strlen( strName ) ] = 0;
	s_pyModuleDef.m_name = pNew;
}

PyObject* PythonCpp::Module::__Init( )
{
	s_pyModule = PyModule_Create( &s_pyModuleDef );
	if ( s_pyModule != nullptr )
	{
		ClassBase* i = s_listClasses;
		while ( i )
		{
			i->Register( s_pyModule );
			i = i->m_pNext;
		}
	}

	return s_pyModule;
}

bool PythonCpp::Module::Release( )
{
	ClassBase* i;
	while ( s_listClasses )
	{
		i = s_listClasses;
		s_listClasses = s_listClasses->m_pNext;
		delete i;
	}

	delete[ ] s_pyModuleDef.m_name;

	return true;
}

///////////////////////////////////////////////////
// PythonCpp.h
///////////////////////////////////////////////////
namespace PythonCpp
{
	PyObject*		pyStdOverrideModule;
	PyModuleDef		pyStdOverrideModuleDef =
	{
		PyModuleDef_HEAD_INIT,
		"CppPython",
		"CppPython Std Override",
		-1,
		0,
		0,
		0,
		0,
		0
	};

	PyObject*		pyStdOut = nullptr;
	PyObject*		pyStdOutOld = nullptr;
	PyObject*		pyStdErr = nullptr;
	PyObject*		pyStdErrOld = nullptr;

	PyTypeObject	pyStdOutType = g_pyDefaultType;
	PyTypeObject	pyStdErrType = g_pyDefaultType;

	struct _PyStdOut { PyObject_HEAD };
	PythonPrinterFunc	pStdOutFunc = nullptr;

	PyObject* PyStdflush( PyObject* self, PyObject* args )
	{
		( self );
		( args );
		return Py_BuildValue( "" );
	}

	PyObject*	PyStdOut( PyObject* self, PyObject* args )
	{
		( self );
		char* data = nullptr;
		PyArg_ParseTuple( args, "s", &data );
		pStdOutFunc( data );
		return PyLong_FromSize_t( strlen( data ) );

	}

	PyMethodDef		pyStdOut_methods[ ] =
	{
		{ "write", PyStdOut, METH_VARARGS, "Output Override" },
		{ "flush", PyStdflush, METH_VARARGS, "Flush" },
		{ 0, 0, 0, 0 }
	};

	struct _PyStdErr { PyObject_HEAD };
	PythonPrinterFunc	pStdErrFunc = nullptr;

	PyObject*	PyStdErr( PyObject* self, PyObject* args )
	{
		( self );
		char* data = nullptr;
		PyArg_ParseTuple( args, "s", &data );
		pStdErrFunc( data );
		return PyLong_FromSize_t( strlen( data ) );
	}

	PyMethodDef		pyStdErr_methods[ ] =
	{
		{ "write", PyStdErr, METH_VARARGS, "Error Override" },
		{ "flush", PyStdflush, METH_VARARGS, "Flush" },
		{ 0, 0, 0, 0 }
	};

	PyObject*	__InitStdOverride( )
	{
		if ( pStdOutFunc )
		{
			pyStdOutType.tp_name = "pyStdOutType";
			pyStdOutType.tp_basicsize = sizeof( _PyStdOut );
			pyStdOutType.tp_doc = "Cpp Python StdOut Override Type";
			pyStdOutType.tp_methods = pyStdOut_methods;
			pyStdOutType.tp_new = PyType_GenericNew;
			if ( PyType_Ready( &pyStdOutType ) < 0 ) { return nullptr; }
		}

		if ( pStdErrFunc )
		{
			pyStdErrType.tp_name = "pyStdErrType";
			pyStdErrType.tp_basicsize = sizeof( _PyStdErr );
			pyStdErrType.tp_doc = "Cpp Python StdErr Override Type";
			pyStdErrType.tp_methods = pyStdErr_methods;
			pyStdErrType.tp_new = PyType_GenericNew;
			if ( PyType_Ready( &pyStdErrType ) < 0 ) { return nullptr; }
		}

		pyStdOverrideModule = PyModule_Create( &pyStdOverrideModuleDef );
		if ( pyStdOverrideModule != nullptr )
		{
			if ( pStdOutFunc )
			{
				Py_INCREF( &pyStdOutType );
				PyModule_AddObject( pyStdOverrideModule, "pyStdOutType", reinterpret_cast<PyObject*>( &pyStdOutType ) );
			}

			if ( pStdErrFunc )
			{
				Py_INCREF( &pyStdErrType );
				PyModule_AddObject( pyStdOverrideModule, "pyStdErrType", reinterpret_cast<PyObject*>( &pyStdErrType ) );
			}
		}

		return pyStdOverrideModule;
	}

}



bool	PythonCpp::StartPython( const char* strModuleName )
{
#ifdef PYTHON_CPP_NATIVE_STRING
	g_uiNumberOfTempNativeStrings = 0;
#endif // PYTHON_CPP_NATIVE_STRING

	Module::SetName( strModuleName );

	if ( PyImport_AppendInittab( strModuleName, Module::__Init ) == -1 ) return false;

	if ( pStdOutFunc || pStdErrFunc )
	{
		if ( PyImport_AppendInittab( pyStdOverrideModuleDef.m_name, PythonCpp::__InitStdOverride ) == -1 ) return false;
	}


	Py_Initialize( );
	wchar_t* progname = { L"Tyr" };

	PySys_SetArgvEx( 1, &progname, 0 );

	if ( pStdOutFunc || pStdErrFunc )
	{
		PyImport_ImportModule( pyStdOverrideModuleDef.m_name );
	}

	if ( pStdOutFunc != nullptr )
	{
		pyStdOutOld = PySys_GetObject( "stdout" );
		pyStdOut = pyStdOutType.tp_new( &pyStdOutType, 0, 0 );

		if ( PySys_SetObject( "stdout", pyStdOut ) == -1 )
		{
			return false;
		}
	}

	if ( pStdErrFunc )
	{
		pyStdErrOld = PySys_GetObject( "stderr" );
		pyStdErr = pyStdErrType.tp_new( &pyStdErrType, 0, 0 );

		if ( PySys_SetObject( "stderr", pyStdErr ) == -1 )
		{
			return false;
		}
	}

	return true;
}

bool	PythonCpp::StopPython( )
{
#pragma warning( push )
#pragma warning( disable : 4127 )
	if ( pyStdOutOld )
	{
		PySys_SetObject( "stdout", pyStdOutOld );
		Py_XDECREF( pyStdOut );
		pyStdOut = nullptr;
	}

	if ( pyStdErrOld )
	{
		PySys_SetObject( "stderr", pyStdErrOld );
		Py_XDECREF( pyStdErr );
		pyStdErr = nullptr;
	}

	Py_Finalize( );
	Module::Release( );
#pragma warning( pop )
	return true;
}

bool	PythonCpp::SetPythonStandardOutput( PythonCpp::PythonPrinterFunc pFunc )
{
	pStdOutFunc = pFunc;
	return true;
}

bool	PythonCpp::SetPythonStandardError( PythonCpp::PythonPrinterFunc pFunc )
{
	pStdErrFunc = pFunc;
	return true;
}


bool PythonCpp::EvalExpression( const char* strExpression )
{
	PyObject* global_dict = PyModule_GetDict( PyImport_AddModule( "__main__" ) );
	if ( !PyRun_String( strExpression, Py_single_input, global_dict, global_dict ) )
	{
		PyErr_Print( );
		PyErr_Clear( );
	}
	PyRun_SimpleString( "\n" );
	return true;
}
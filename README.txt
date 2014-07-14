	      == Python Cpp ==
A library that reflects C++ classes to Python
Copyright 2013, Eli Bukchin.


This is a WIP project!

So.. There are bugs.

Althow planned to be multiplatform it is tested with:

Architecture: 	64 bit
Compiler: 	Microsoft Visual C++ Compiler Nov 2013 CTP (v120_CTP_Nov2012)
Python: 	3.3.5 (Debug), 3.3 (Release)

Example of Usage:
--------------------------

#include <PythonCpp.h>

//////////////////  Class declerations //////////////////////
class B
{
public:
	void print() { printf("Hi I am B\n"); }
};

class Derived : public B
{
public:
	void	child() { printf("Child\n"); }
};

class A
{
public:
	void print() { printf("Hi I am A\n"); }

	static void otherprint()
	{
		printf("Other print A\n");
	}

	static A*	Get()
	{
		if ( !m_pInstance )
		{
			m_pInstance = new A;
		}
		return m_pInstance;
	}

	static void Release()
	{
		if ( m_pInstance )
		{
			delete m_pInstance;
		}
		m_pInstance = nullptr;
	}

	B*		GetB() { return b; }
	Derived*	GetDerived() { return d; }

	A()
	{
		b = new B();
		d = new Derived();
	}

private:
	static A* m_pInstance;
	B*	b;
	Derived* d;
};

A* A::m_pInstance = nullptr;
//////////////////  End Class declerations //////////////////////

// The following lines export to Python
/////////////////   Export to Python ///////////////////////////////
ADD_CLASS_TO_PYTHON( A )
{
	EXPORT_METHOD( print );
	EXPORT_METHOD( GetB );
	EXPORT_METHOD( GetDerived );
	EXPORT_STATIC_METHOD( otherprint );
	EXPORT_STATIC_METHOD( Get );
}

ADD_CLASS_TO_PYTHON( B )
{
	EXPORT_METHOD_DESC( print, "Fucking awesome" );
}

// Will export regular types of Derived, including const
ADD_CLASS_TO_PYTHON( Derived )
{
	SET_PARENT( B );
	EXPORT_METHOD( child );
}

// Will export references types of Derived, including const &
ADD_CLASS_REFERENCE_TO_PYTHON( Derived )
{
	SET_PARENT( B );
	EXPORT_METHOD( child );
}

// Will export Derived*, including const
ADD_CLASS_POINTER_TO_PYTHON( Derived )
{
	SET_PARENT( B );
	EXPORT_METHOD( child );
}
/////////////////  End Export to Python ///////////////////////////////


// to initialize python use:
PythonCpp::StartPython( "<your-module-name>" );

// to stop and release resources
PythonCpp::StopPython();

// to run a command
PythonCpp::EvalExpression( const char* strExpression );
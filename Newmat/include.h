//$$ include.h           include files required by various versions of C++

#ifndef INCLUDE_LIB
#define INCLUDE_LIB


#ifdef _WIN32
// Unterdrücke Warnung 4786. Diese besagt, dass gewisse Template-Expansionen
// längere Symbolbezeichner als 256 Zeichen erzeugen. Diese können vom Debugger
// nicht mehr dargestellt werden. 
#pragma warning( disable: 4786 )
// Konvertierung von 'double' in 'float', moeglicher Datenverlust
#pragma warning( disable: 4244 )
// Verkuerzung von 'const double' in 'float'
#pragma warning( disable: 4305 )
#endif

#define use_namespace                   // define name spaces

#define SETUP_C_SUBSCRIPTS              // allow element access via A[i][j]

// Activate just one of the following 3 statements

//#define SimulateExceptions              // use simulated exceptions
#define UseExceptions                   // use C++ exceptions
//#define DisableExceptions               // do not use exceptions


//#define TEMPS_DESTROYED_QUICKLY         // for compilers that delete
					// temporaries too quickly

//#define TEMPS_DESTROYED_QUICKLY_R       // the same thing but applied
					// to return from functions only

//#define DO_FREE_CHECK                   // check news and deletes balance

#define USING_DOUBLE                    // elements of type double
//#define USING_FLOAT                   // elements of type float

#define _STANDARD_                    // using standard library

//#define use_float_h                   // use float.h for precision data

//*********************** end of options set by user ********************



#include <cstdlib>
#ifdef WANT_STREAM
  #include <iostream>
  #include <iomanip>
#endif
#ifdef WANT_MATH
  #include <cmath>
#endif
#ifdef WANT_STRING
  #include <cstring>
#endif
#ifdef WANT_TIME
  #include <ctime>
#endif

namespace RBD_COMMON 
{
	#ifdef USING_FLOAT                      // set precision type to float
		typedef float Real;
		typedef double long_Real;
	#endif

	#ifdef USING_DOUBLE                     // set precision type to double
		typedef double Real;
		typedef long double long_Real;
	#endif
}

namespace RBD_COMMON {}
namespace RBD_LIBRARIES                 // access all my libraries
{
	using namespace RBD_COMMON;
}


#endif

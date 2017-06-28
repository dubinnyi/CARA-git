//$$ myexcept.h                                  Exception handling classes


// A set of classes to simulate exceptions in C++
//
//   Partially copied from Carlos Vidal s article in the C users  journal
//   September 1992, pp 19-28
//
//   Operations defined
//      Try {     }
//      Throw ( exception object )
//      ReThrow
//      Catch ( exception class ) {      }
//      CatchAll {      }
//      CatchAndThrow
//
//   All catch lists must end with a CatchAll or CatchAndThrow statement
//   but not both.
//
//   When exceptions are finally implemented replace Try, Throw(E), Rethrow,
//   Catch, CatchAll, CatchAndThrow by try, throw E, throw, catch,
//   catch(...), and {}.
//
//   All exception classes must be derived from Exception, have no non-static
//   variables and must include the statement
//
//      static unsigned long Select;
//
//   Any constructor in one of these exception classes must include
//
//      Select = Exception::Select;
//
//   For each exceptions class, EX_1, some .cpp file must include
//
//      unsigned long EX_1::Select;
//


#ifndef EXCEPTION_LIB
#define EXCEPTION_LIB

namespace RBD_COMMON 
{


	void Terminate();


	//********** classes for setting up exceptions and reporting ************//

	class Exception;

	class Tracer                             // linked list showing how
	{                                        // we got here
	   const char* entry;
	   Tracer* previous;
	public:
	   Tracer(const char*);
	   ~Tracer();
	   void ReName(const char*);
	   static void PrintTrace();             // for printing trace
	   static void AddTrace();               // insert trace in exception record
	   static Tracer* last;                  // points to Tracer list
	   friend class Exception;
	};


	class Exception                          // The base exception class
	{
	protected:
	   static char* what_error;              // error message
	   static int SoFar;                     // no. characters already entered
	   static int LastOne;                   // last location in error buffer
	public:
	   static void AddMessage(const char* a_what);
											 // messages about exception
	   static void AddInt(int value);        // integer to error message
	   static unsigned long Select;          // for identifying exception
	   Exception(const char* a_what = 0);
	   static const char* what() { return what_error; }
											 // for getting error message
	};


	inline Tracer::Tracer(const char* e)
	   : entry(e), previous(last) { last = this; }

	inline Tracer::~Tracer() { last = previous; }

	inline void Tracer::ReName(const char* e) { entry=e; }

	#define Try try
	#define Throw(E) throw E
	#define ReThrow throw
	#define Catch catch
	#define CatchAll catch(...)
	#define CatchAndThrow {}

	class Janitor                         // a dummy version
	{
	public:
	   virtual void CleanUp() {}
	   Janitor() {}
	   virtual ~Janitor() {}
	};

	//******************** FREE_CHECK and NEW_DELETE ***********************//

	#ifdef DO_FREE_CHECK                          // DO_FREE_CHECK
	// Routines for tracing whether new and delete calls are balanced

	class FreeCheck;

	class FreeCheckLink
	{
	protected:
	   FreeCheckLink* next;
	   void* ClassStore;
	   FreeCheckLink();
	   virtual void Report()=0;                   // print details of link
	   friend class FreeCheck;
	};

	class FCLClass : public FreeCheckLink         // for registering objects
	{
	   char* ClassName;
	   FCLClass(void* t, char* name);
	   void Report();
	   friend class FreeCheck;
	};

	class FCLRealArray : public FreeCheckLink     // for registering real arrays
	{
	   char* Operation;
	   int size;
	   FCLRealArray(void* t, char* o, int s);
	   void Report();
	   friend class FreeCheck;
	};

	class FCLIntArray : public FreeCheckLink     // for registering int arrays
	{
	   char* Operation;
	   int size;
	   FCLIntArray(void* t, char* o, int s);
	   void Report();
	   friend class FreeCheck;
	};


	class FreeCheck
	{
	   static FreeCheckLink* next;
	   static int BadDelete;
	public:
	   static void Register(void*, char*);
	   static void DeRegister(void*, char*);
	   static void RegisterR(void*, char*, int);
	   static void DeRegisterR(void*, char*, int);
	   static void RegisterI(void*, char*, int);
	   static void DeRegisterI(void*, char*, int);
	   static void Status();
	   friend class FreeCheckLink;
	   friend class FCLClass;
	   friend class FCLRealArray;
	   friend class FCLIntArray;
	};

	#define FREE_CHECK(Class)                                                  \
	public:                                                                    \
	   void* operator new(size_t size)                                         \
	   {                                                                       \
		  void* t = ::operator new(size); FreeCheck::Register(t,#Class);       \
		  return t;                                                            \
	   }                                                                       \
	   void operator delete(void* t)                                           \
	   { FreeCheck::DeRegister(t,#Class); ::operator delete(t); }


	#define MONITOR_REAL_NEW(Operation, Size, Pointer)                         \
		FreeCheck::RegisterR(Pointer, Operation, Size);
	#define MONITOR_INT_NEW(Operation, Size, Pointer)                          \
		FreeCheck::RegisterI(Pointer, Operation, Size);
	#define MONITOR_REAL_DELETE(Operation, Size, Pointer)                      \
		FreeCheck::DeRegisterR(Pointer, Operation, Size);
	#define MONITOR_INT_DELETE(Operation, Size, Pointer)                       \
		FreeCheck::DeRegisterI(Pointer, Operation, Size);

	#else                            // DO_FREE_CHECK not defined

	#define FREE_CHECK(Class) public:
	#define MONITOR_REAL_NEW(Operation, Size, Pointer) {}
	#define MONITOR_INT_NEW(Operation, Size, Pointer) {}
	#define MONITOR_REAL_DELETE(Operation, Size, Pointer) {}
	#define MONITOR_INT_DELETE(Operation, Size, Pointer) {}

	#endif                            // end of ! DO_FREE_CHECK


	//********************* derived exceptions ******************************//

	class Logic_error : public Exception
	{
	public:
	   static unsigned long Select;
	   Logic_error(const char* a_what = 0);
	};

	class Runtime_error : public Exception
	{
	public:
	   static unsigned long Select;
	   Runtime_error(const char* a_what = 0);
	};

	class Domain_error : public Logic_error
	{
	public:
	   static unsigned long Select;
	   Domain_error(const char* a_what = 0);
	};

	class Invalid_argument : public Logic_error
	{
	public:
	   static unsigned long Select;
	   Invalid_argument(const char* a_what = 0);
	};

	class Length_error : public Logic_error
	{
	public:
	   static unsigned long Select;
	   Length_error(const char* a_what = 0);
	};

	class Out_of_range : public Logic_error
	{
	public:
	   static unsigned long Select;
	   Out_of_range(const char* a_what = 0);
	};

	class Range_error : public Runtime_error
	{
	public:
	   static unsigned long Select;
	   Range_error(const char* a_what = 0);
	};

	class Overflow_error : public Runtime_error
	{
	public:
	   static unsigned long Select;
	   Overflow_error(const char* a_what = 0);
	};

	class Bad_alloc : public Exception
	{
	public:
	   static unsigned long Select;
	   Bad_alloc(const char* a_what = 0);
	};

}


#endif                            // end of EXCEPTION_LIB


// body file: myexcept.cpp




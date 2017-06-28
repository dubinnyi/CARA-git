//$$ precisio.h                          floating point constants

#ifndef PRECISION_LIB
#define PRECISION_LIB 0

#ifdef _WIN32

#include <limits>

namespace NEWMAT 
{

		
	class FloatingPointPrecision
	{
	public:
	   static const int Dig()              // number of decimal digits or precision
	   { return std::numeric_limits<Real>::digits10 ; }

	   static const Real Epsilon()         // smallest number such that 1+Eps!=Eps
		  { return std::numeric_limits<Real>::epsilon(); }

	   static const int Mantissa()         // bits in mantisa
		  { return std::numeric_limits<Real>::digits; }

	   static const Real Maximum()         // maximum value
		  { return std::numeric_limits<Real>::max(); }

	   static const int MaximumDecimalExponent()  // maximum decimal exponent
		  { return std::numeric_limits<Real>::max_exponent10; }

	   static const int MaximumExponent()  // maximum binary exponent
		  { return std::numeric_limits<Real>::max_exponent; }

	   static const Real LnMaximum()       // natural log of maximum
		  { return (Real)log(Maximum()); }

	   static const Real Minimum()         // minimum positive value
		  { return std::numeric_limits<Real>::min(); } 

	   static const int MinimumDecimalExponent() // minimum decimal exponent
		  { return std::numeric_limits<Real>::min_exponent10; }

	   static const int MinimumExponent()  // minimum binary exponent
		  { return std::numeric_limits<Real>::min_exponent; }

	   static const Real LnMinimum()       // natural log of minimum
		  { return (Real)log(Minimum()); }

	   static const int Radix()            // exponent radix
		  { return std::numeric_limits<Real>::radix; }

	   static const int Rounds()           // addition rounding (1 = does round)
	   {
		  return std::numeric_limits<Real>::round_style ==
			 std::round_to_nearest ? 1 : 0;
	   }

	};
}
#else // _WIN32

#include <float.h>

namespace NEWMAT 
{
	#ifdef USING_FLOAT

	class FloatingPointPrecision
	{
	public:
	   static int Dig()
		  { return FLT_DIG; }        // number of decimal digits or precision

	   static Real Epsilon()
		  { return FLT_EPSILON; }    // smallest number such that 1+Eps!=Eps

	   static int Mantissa()
		  { return FLT_MANT_DIG; }   // bits in mantisa

	   static Real Maximum()
		  { return FLT_MAX; }        // maximum value

	   static int MaximumDecimalExponent()
		  { return FLT_MAX_10_EXP; } // maximum decimal exponent

	   static int MaximumExponent()
		  { return FLT_MAX_EXP; }    // maximum binary exponent

	   static Real LnMaximum()
		  { return (Real)log(Maximum()); } // natural log of maximum

	   static Real Minimum()
		  { return FLT_MIN; }        // minimum positive value

	   static int MinimumDecimalExponent()
		  { return FLT_MIN_10_EXP; } // minimum decimal exponent

	   static int MinimumExponent()
		  { return FLT_MIN_EXP; }    // minimum binary exponent

	   static Real LnMinimum()
		  { return (Real)log(Minimum()); } // natural log of minimum

	   static int Radix()
		  { return FLT_RADIX; }      // exponent radix

	   static int Rounds()
		  { return FLT_ROUNDS; }     // addition rounding (1 = does round)

	};

	#endif                           // USING_FLOAT

	#ifdef USING_DOUBLE

	class FloatingPointPrecision
	{
	public:

	   static int Dig()
		  { return DBL_DIG; }        // number of decimal digits or precision

	   static Real Epsilon()
		  { return DBL_EPSILON; }    // smallest number such that 1+Eps!=Eps

	   static int Mantissa()
		  { return DBL_MANT_DIG; }   // bits in mantisa

	   static Real Maximum()
		  { return DBL_MAX; }        // maximum value

	   static int MaximumDecimalExponent()
		  { return DBL_MAX_10_EXP; } // maximum decimal exponent

	   static int MaximumExponent()
		  { return DBL_MAX_EXP; }    // maximum binary exponent

	   static Real LnMaximum()
		  { return (Real)log(Maximum()); } // natural log of maximum

	   static Real Minimum()
	   {
		   return DBL_MIN;
	   }

	   static int MinimumDecimalExponent()
		  { return DBL_MIN_10_EXP; } // minimum decimal exponent

	   static int MinimumExponent()
		  { return DBL_MIN_EXP; }    // minimum binary exponent

	   static Real LnMinimum()
		  { return (Real)log(Minimum()); } // natural log of minimum


	   static int Radix()
		  { return FLT_RADIX; }      // exponent radix

	   static int Rounds()
		  { return FLT_ROUNDS; }     // addition rounding (1 = does round)

	};

	#endif                             // USING_DOUBLE
}

#endif // _WIN32



#endif                                // PRECISION_LIB

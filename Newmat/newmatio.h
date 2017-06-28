//$$ newmatio.h           definition file for matrix package input/output

// Copyright (C) 1991,2,3,4: R B Davies

#ifndef NEWMATIO_LIB
#define NEWMATIO_LIB 0

#ifndef WANT_STREAM
#define WANT_STREAM
#endif

#include "newmat.h"

namespace NEWMAT 
{

/**************************** input/output *****************************/

	std::ostream& operator<<(std::ostream&, const BaseMatrix&);

	std::ostream& operator<<(std::ostream&, const GeneralMatrix&);

}



#endif

// body file: newmat9.cpp


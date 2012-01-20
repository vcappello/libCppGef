#ifndef FLOAT_CMP_H
#define FLOAT_CMP_H

#include <cmath>
#include <limits>

namespace cppgef
{

template< class T >	
bool float_equ(T a, T b)
{
	return std::fabs(a - b) < std::numeric_limits< T >::epsilon();
}

template< class T >	
bool float_not_equ(T a, T b)
{
	return !float_equ (a, b);
}

}

#endif // FLOAT_CMP_H

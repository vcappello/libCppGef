#ifndef MOVEPOLICY_H
#define MOVEPOLICY_H

#include <boost/shared_ptr.hpp>

#include <figure.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace cppgef
{

//////////////////////////////////////////////////////////////////////////////////////
	
class IMovePointPolicy
{
public:
	enum PointSequence
	{
		StartPoint,
		EndPoint,
		CurveStartPoint,
		CurveEndPoint,
		CurveStartControlPoint,
		CurveEndControlPoint
	};
	
public:
	virtual void moveTo(shared_ptr< Figure > figure, int dx, int dy) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////

class MoveStartPointPolicy : public IMovePointPolicy
{
public:
	void moveTo(shared_ptr< Figure > figure, int dx, int dy);
};

//////////////////////////////////////////////////////////////////////////////////////

class MoveEndPointPolicy : public IMovePointPolicy
{
public:
	void moveTo(shared_ptr< Figure > figure, int dx, int dy);
};

//////////////////////////////////////////////////////////////////////////////////////

class MoveCurveStartPointPolicy : public IMovePointPolicy
{
public:
	void moveTo(shared_ptr< Figure > figure, int dx, int dy);
};

//////////////////////////////////////////////////////////////////////////////////////

class MoveCurveEndPointPolicy : public IMovePointPolicy
{
public:
	void moveTo(shared_ptr< Figure > figure, int dx, int dy);
};

//////////////////////////////////////////////////////////////////////////////////////

class MoveCurveStartControlPointPolicy : public IMovePointPolicy
{
public:
	void moveTo(shared_ptr< Figure > figure, int dx, int dy);
};

//////////////////////////////////////////////////////////////////////////////////////

class MoveCurveEndControlPointPolicy : public IMovePointPolicy
{
public:
	void moveTo(shared_ptr< Figure > figure, int dx, int dy);
};

}

#endif // MOVEPOLICY_H

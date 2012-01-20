#include "move_point_policy_factory.h"

namespace cppgef
{

MovePointPolicyFactory* MovePointPolicyFactory::instance_ = 0;

MovePointPolicyFactory::MovePointPolicyFactory()
{
}

MovePointPolicyFactory::~MovePointPolicyFactory()
{
}

MovePointPolicyFactory* MovePointPolicyFactory::getInstance()
{
	if(instance_ == 0)
	{
		instance_ = new MovePointPolicyFactory();
	}
	return instance_;
}

void MovePointPolicyFactory::release()
{
	if(instance_)
	{
		delete instance_;
	}
	instance_ = 0;
}

shared_ptr< IMovePointPolicy > MovePointPolicyFactory::createPolicy(IMovePointPolicy::PointSequence sequence)
{
	switch (sequence)
	{
		case IMovePointPolicy::StartPoint:
			return shared_ptr< IMovePointPolicy >( new MoveStartPointPolicy() );
			break;
		case IMovePointPolicy::EndPoint:
			return shared_ptr< IMovePointPolicy >( new MoveEndPointPolicy() );
			break;
		case IMovePointPolicy::CurveStartPoint:
			return shared_ptr< IMovePointPolicy >( new MoveCurveStartPointPolicy );
			break;
		case IMovePointPolicy::CurveEndPoint:
			return shared_ptr< IMovePointPolicy >( new MoveCurveEndPointPolicy );
			break;
		case IMovePointPolicy::CurveStartControlPoint:
			return shared_ptr< IMovePointPolicy >( new MoveCurveStartControlPointPolicy );
			break;
		case IMovePointPolicy::CurveEndControlPoint:
			return shared_ptr< IMovePointPolicy >( new MoveCurveEndControlPointPolicy );
			break;
	}
	
	return shared_ptr< IMovePointPolicy >();
}

}

#ifndef MOVEPOINTPOLICYFACTORY_H
#define MOVEPOINTPOLICYFACTORY_H

#include <boost/shared_ptr.hpp>

#include <move_policy.h>

using boost::shared_ptr;

namespace cppgef
{

class MovePointPolicyFactory
{
private:	
	static MovePointPolicyFactory* instance_;

public:
	static MovePointPolicyFactory* getInstance();
	static void release();

	shared_ptr< IMovePointPolicy > createPolicy(IMovePointPolicy::PointSequence sequence);
	
private:
	MovePointPolicyFactory();
	virtual ~MovePointPolicyFactory();

};

}

#endif // MOVEPOINTPOLICYFACTORY_H

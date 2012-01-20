#ifndef ILAYOUTMANAGER_H
#define ILAYOUTMANAGER_H

#include <i_layout_constraint.h>

#include <boost/shared_ptr.hpp>

#include <rectangle.h>

using boost::shared_ptr;

namespace cppgef
{

class Figure; // Forward declaration

class ILayoutManager
{
public:
	ILayoutManager()
	{
	}
	virtual ~ILayoutManager()
	{
	}

	virtual shared_ptr< ILayoutConstraint > getChildConstraint(shared_ptr< Figure > figure) = 0;
	
	virtual void addChildConstraint(shared_ptr< Figure > figure, shared_ptr< ILayoutConstraint > constraint) = 0;
	virtual void eraseChildConstraint(shared_ptr< Figure > figure) = 0;
	
	virtual void applyConstraints(const Rectangle& rect) = 0;
};

}

#endif // ILAYOUTMANAGER_H

#ifndef VLAYOUTMANAGER_H
#define VLAYOUTMANAGER_H

#include <i_layout_manager.h> // Base class: cppgef::ILayoutManager

#include <vector>
#include <boost/shared_ptr.hpp>

#include <v_layout_constraint.h>
#include <rectangle.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace cppgef
{

class VLayoutManager : public cppgef::ILayoutManager
{
public:
	typedef std::vector< std::pair< shared_ptr<Figure>, shared_ptr<VLayoutConstraint> > > container_t;
	
public:
	VLayoutManager();
	virtual ~VLayoutManager();

public:
	virtual shared_ptr< ILayoutConstraint > getChildConstraint(shared_ptr< Figure > figure);
	
	virtual void addChildConstraint(shared_ptr<Figure> figure, shared_ptr<ILayoutConstraint> constraint);
	virtual void eraseChildConstraint(shared_ptr<Figure> figure);
	
	virtual void applyConstraints(const Rectangle& rect);
	
protected:
	container_t constraints_;
};

}

#endif // VLAYOUTMANAGER_H

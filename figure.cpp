#include "figure.h"

#include <boost/foreach.hpp>

namespace cppgef
{

Figure::Figure()
{
}

Figure::~Figure()
{
}

weak_ptr< Figure > Figure::getParent()
{
	return parent_;
}

shared_ptr< ILayoutManager > Figure::getLayoutManager()
{
	return layout_manager_;
}

void Figure::setLayoutManager(shared_ptr< ILayoutManager > value)
{
	layout_manager_ = value;
}

void Figure::addChild(shared_ptr<Figure> child)
{
	children_.push_back (child);
	child->parent_ = shared_from_this();
}

void Figure::addChild(shared_ptr< Figure > child, shared_ptr< ILayoutConstraint > constraint)
{
	Figure::addChild (child);
	
	if (layout_manager_)
		layout_manager_->addChildConstraint (child, constraint);
}

void Figure::insertChild(shared_ptr< Figure > child, size_t pos)
{
	if (pos >= children_.size())
		children_.push_back (child);
	else
		children_.insert (children_.begin() + pos, child);

	child->parent_ = shared_from_this();
}

void Figure::insertChild(shared_ptr< Figure > child, shared_ptr< ILayoutConstraint > constraint, size_t pos)
{
	Figure::insertChild (child, pos);

	if (layout_manager_)
		layout_manager_->addChildConstraint (child, constraint);
}

void Figure::eraseChild(shared_ptr<Figure> child)
{
	iterator child_itor = std::find (children_.begin(), children_.end(), child);
	
	if (child_itor != children_.end())
	{
		children_.erase (child_itor);
		
		if (layout_manager_)
			layout_manager_->eraseChildConstraint (child);		
	}
}

shared_ptr< Figure > Figure::getChild(int index)
{
	return children_.at (index);
}

int Figure::getChildIndex(shared_ptr< Figure > figure)
{
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (children_[i] == figure)
			return i;
	}
	
	return -1;
}

shared_ptr< IVisualStyle > Figure::getVisualStyle(const Glib::ustring& id)
{
	return shared_ptr< IVisualStyle >();
}

void Figure::paintChildren(Cairo::RefPtr< Cairo::Context > context)
{
	for (iterator i = children_.begin();
		i != children_.end();
		i++)
	{
		(*i)->paint (context);
	}
}

}




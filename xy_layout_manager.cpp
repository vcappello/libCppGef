#include "xy_layout_manager.h"

#include <figure.h>

namespace cppgef
{

XYLayoutManager::XYLayoutManager()
{
}

XYLayoutManager::~XYLayoutManager()
{
}

shared_ptr< ILayoutConstraint > XYLayoutManager::getChildConstraint(shared_ptr< Figure > figure)
{
	for (container_t::iterator item_itor = constraints_.begin(); item_itor != constraints_.end(); item_itor++)
	{
		if (item_itor->first == figure)
		{
			return (item_itor->second);
		}
	}	
	
	return shared_ptr< ILayoutConstraint >();
}

void XYLayoutManager::addChildConstraint(shared_ptr<Figure> figure, shared_ptr<ILayoutConstraint> constraint)
{
	constraints_.push_back (std::make_pair(figure, dynamic_pointer_cast<XYLayoutConstraint>(constraint)));
}

void XYLayoutManager::eraseChildConstraint(shared_ptr<Figure> figure)
{
	// TODO: use a different container for fast seek?
	for (container_t::iterator item_itor = constraints_.begin(); item_itor != constraints_.end(); item_itor++)
	{
		if (item_itor->first == figure)
		{
			constraints_.erase (item_itor);
			return;
		}
	}
}

void XYLayoutManager::applyConstraints(const Rectangle& rect)
{
	typedef container_t::iterator iterator_t;
	
	for (iterator_t i = constraints_.begin(); i != constraints_.end(); i++)
	{
		Rectangle child_rect = i->second->getBounds();
		child_rect.getLocation().setX (rect.getLocation().getX() + child_rect.getLocation().getX());
		child_rect.getLocation().setY (rect.getLocation().getY() + child_rect.getLocation().getY());

		i->first->fitRect (child_rect);
	}	
}

}


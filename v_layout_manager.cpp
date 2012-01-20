#include "v_layout_manager.h"

#include <figure.h>

namespace cppgef
{

VLayoutManager::VLayoutManager()
{
}

VLayoutManager::~VLayoutManager()
{
}

shared_ptr< ILayoutConstraint > VLayoutManager::getChildConstraint(shared_ptr< Figure > figure)
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

void VLayoutManager::addChildConstraint(shared_ptr<Figure> figure, shared_ptr<ILayoutConstraint> constraint)
{
	constraints_.push_back (std::make_pair(figure, dynamic_pointer_cast<VLayoutConstraint>(constraint)));
}

void VLayoutManager::eraseChildConstraint(shared_ptr<Figure> figure)
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

void VLayoutManager::applyConstraints(const Rectangle& rect)
{
	typedef container_t::iterator iterator_t;
	
	int vertical_fill_size = rect.getSize().getHeight();
	int fill_item_count = constraints_.size();
	
	for (iterator_t i = constraints_.begin(); i != constraints_.end(); i++)
	{
		Rectangle bounds = i->first->getOutlineRect();

		bounds.getLocation().setX (rect.getLocation().getX() + i->second->getPadLeft());
		
		if (i->second->getHorizontalConstraint() == VLayoutConstraint::HorizontalExpand)
		{
			bounds.getSize().setWidth (rect.getSize().getWidth() - (i->second->getPadLeft() + i->second->getPadRight()));
		}
		else
		{
			bounds.getSize().setWidth (i->second->getHorizontalSize());
		}	

		i->first->fitRect (bounds);

		vertical_fill_size -= i->second->getPadTop();
		vertical_fill_size -= i->second->getPadBottom();
		
		if (i->second->getVerticalConstraint() == VLayoutConstraint::VerticalShrink)
		{
			vertical_fill_size -= i->second->getVerticalSize();
			fill_item_count--;
		}
	}
	
	int y = rect.getLocation().getY();
	
	int fill_per_figure = 0;
	if (fill_item_count != 0)
		fill_per_figure = vertical_fill_size / fill_item_count;
		
	for (iterator_t i = constraints_.begin(); i != constraints_.end(); i++)
	{
		y += i->second->getPadTop();

		Rectangle bounds = i->first->getOutlineRect();

		bounds.getLocation().setY (y);
		
		if (i->second->getVerticalConstraint() == VLayoutConstraint::VerticalShrink)
		{
			bounds.getSize().setHeight (i->second->getVerticalSize());
			y += i->second->getVerticalSize();
		}
		else
		{
			bounds.getSize().setHeight (fill_per_figure);
			y += fill_per_figure ;
		}
		
		i->first->fitRect (bounds);

		y += i->second->getPadBottom();
	}
}

}


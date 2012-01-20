#include "resize_drag_tracker.h"

#include <command_manager.h>
#include <edit_part_factory.h>

namespace cppgef
{

ResizeDragTracker::ResizeDragTracker(shared_ptr< IContainerEditPart > parent, AbstractResizePolicy::ResizeDirection resize_direction) :
	parent_( parent ),
	resize_direction_( resize_direction ),
	dragging_( false )
{
}

ResizeDragTracker::~ResizeDragTracker()
{
}

void ResizeDragTracker::dragBegin(const Point& point)
{
	start_point_ = point;
	prev_point_ = point;
	dragging_ = true;
	
	resize_policies_.clear();
	selected_edit_parts_.clear();
	
	for (IContainerEditPart::iterator i = parent_->getSelectedChildrenBegin();
		i != parent_->getSelectedChildrenEnd();
		i++)
		{
			if ((*i)->isResizeSupported())
			{
				shared_ptr< IEditPart > clone_edit_part = EditPartFactory::getInstance()->createEditPart ((*i)->getModel());
				
				clone_edit_part->getFigure()->fitRect ((*i)->getFigure()->getOutlineRect());
				clone_edit_part->getFigure()->paintWithAlpha (0.5);
				
				selected_edit_parts_.push_back (clone_edit_part);
				
				resize_policies_[clone_edit_part] = AbstractResizePolicy::create (resize_direction_, clone_edit_part->getFigure()->getOutlineRect());
			}
		}	
}

void ResizeDragTracker::dragTo(const Point& point)
{
	int dx = point.getX() - prev_point_.getX();
	int dy = point.getY() - prev_point_.getY();
	
	typedef std::list< shared_ptr< IEditPart > >::iterator itor_t;
	for (itor_t i = selected_edit_parts_.begin();
		i != selected_edit_parts_.end();
		i++)
		{
			Rectangle bounds = (*i)->getFigure()->getOutlineRect();
			resize_policies_[*i]->resize (bounds, dx, dy);
			(*i)->getFigure()->fitRect (bounds);
		}
	
	prev_point_ = point;
}

void ResizeDragTracker::dragComplete(const Point& point)
{
	int dx = point.getX() - start_point_.getX();
	int dy = point.getY() - start_point_.getY();
	
	// If the final position is the start postition
	// does not execute any command
	if ((dx != 0) || (dy != 0))
	{
		shared_ptr< ICommand > cmd = parent_->createResizeCommand (resize_direction_, dx, dy);
	
		bool res = CommandManager::getInstance()->execute (cmd);
	}
	
	resize_policies_.clear();
	selected_edit_parts_.clear();
	dragging_ = false;
}

void ResizeDragTracker::dragCancel(const Point& point)
{
	resize_policies_.clear();
	selected_edit_parts_.clear();
	dragging_ = false;
}

void ResizeDragTracker::paint(Cairo::RefPtr<Cairo::Context> context)
{
	if (!dragging_)
	{
		parent_->paintSelectedDragTrackers (context);	
	}
	else
	{
		typedef std::list< shared_ptr< IEditPart > >::iterator itor_t;
		for (itor_t i = selected_edit_parts_.begin();
			i != selected_edit_parts_.end();
			i++)
			{
				(*i)->getFigure()->paint (context);
			}
	}		
}

}


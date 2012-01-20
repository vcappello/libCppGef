#include "move_drag_tracker.h"

#include <command_manager.h>
#include <edit_part_factory.h>
#include <diagram_edit_part.h>

namespace cppgef
{
	
MoveDragTracker::MoveDragTracker(shared_ptr< IContainerEditPart > parent, shared_ptr< IEditPart > active) :
	parent_( parent ),
	dragging_( false )
{
	// Remove from selection object with a different parent of the active object
	IContainerEditPart::children_container_t selected_items;
	
	// Store selected items because the next loop can change selection
	selected_items.insert (selected_items.begin(), 
		parent_->getSelectedChildrenBegin(),
		parent_->getSelectedChildrenEnd());

	for (IContainerEditPart::iterator i = selected_items.begin();
		i != selected_items.end();
		i++)
		{
			if ((*i)->getParent().lock() != active->getParent().lock())
			{
				(*i)->setSelected (false);
			}
		}
}

MoveDragTracker::~MoveDragTracker()
{
}

void MoveDragTracker::dragBegin (const Point& point)
{
	start_point_ = point;
	prev_point_ = point;
	dragging_ = true;
	
	selected_edit_parts_.clear();
	
	for (IContainerEditPart::iterator i = parent_->getSelectedChildrenBegin();
		i != parent_->getSelectedChildrenEnd();
		i++)
		{
			if ((*i)->isMoveSupported())
			{
				shared_ptr< IEditPart > clone_edit_part = EditPartFactory::getInstance()->createEditPart ((*i)->getModel());
				
				clone_edit_part->getFigure()->fitRect ((*i)->getFigure()->getOutlineRect());
				clone_edit_part->getFigure()->paintWithAlpha (0.5);
				
				selected_edit_parts_.push_back (clone_edit_part);
			}
		}	
	
}

void MoveDragTracker::dragTo (const Point& point)
{
	int dx = point.getX() - prev_point_.getX();
	int dy = point.getY() - prev_point_.getY();
	
	typedef std::list< shared_ptr< IEditPart > >::iterator itor_t;
	for (itor_t i = selected_edit_parts_.begin();
		i != selected_edit_parts_.end();
		i++)
		{
			Rectangle bounds = (*i)->getFigure()->getOutlineRect();
			bounds.getLocation().moveRel (dx, dy);

			(*i)->getFigure()->fitRect (bounds);
		}
	
	prev_point_ = point;
}

void MoveDragTracker::dragComplete (const Point& point)
{
	int dx = point.getX() - start_point_.getX();
	int dy = point.getY() - start_point_.getY();
	
	// If the final position is the start postition
	// does not execute any command
	if ((dx != 0) || (dy != 0))
	{
		shared_ptr< DiagramEditPart > diagram_edit_part = dynamic_pointer_cast< DiagramEditPart >(parent_);
		shared_ptr< IContainerEditPart > start_container = diagram_edit_part->findInnerContainer (start_point_);
		shared_ptr< IContainerEditPart > target_container = diagram_edit_part->findInnerContainer (point);
			
		shared_ptr< ICommand > cmd;
		
		if (start_container->isSelected())
			start_container = start_container->getParent().lock();

		// Target container can not be selected,
		// for avoid circular references, in this case
		// use the start container as target
		if (target_container->isSelected() || target_container->isAncestorSelected()) 
			target_container = start_container;
			
		if (start_container == target_container)
		{
			cmd = parent_->createMoveCommand (dx, dy);
		}
		else
		{

			Rectangle start_container_bounds = start_container->getClientFigure()->getOutlineRect();
			
			int sx = start_point_.getX() - start_container_bounds.getLocation().getX();
			int sy = start_point_.getY() - start_container_bounds.getLocation().getY();

			Rectangle target_container_bounds = target_container->getClientFigure()->getOutlineRect();

			int tx = point.getX() - target_container_bounds.getLocation().getX();
			int ty = point.getY() - target_container_bounds.getLocation().getY();
			
			int cdx = tx - sx;
			int cdy = ty - sy;
			
			cmd = parent_->createReparentCommand (
				dynamic_pointer_cast< ComplexModelElementBase >(target_container->getModel()), 
				cdx, cdy);
		}
		
		bool res = CommandManager::getInstance()->execute (cmd);
	}		
		
	selected_edit_parts_.clear();
		
	dragging_ = false;
}

void MoveDragTracker::dragCancel (const Point& point)
{
	selected_edit_parts_.clear();
	
	dragging_ = false;
}

void MoveDragTracker::paint(Cairo::RefPtr< Cairo::Context > context)
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


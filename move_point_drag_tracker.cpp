#include "move_point_drag_tracker.h"

#include <command_manager.h>
#include <edit_part_factory.h>
#include <diagram_edit_part.h>

namespace cppgef
{

MovePointDragTracker::MovePointDragTracker(shared_ptr< IContainerEditPart > parent, IMovePointPolicy::PointSequence sequence) :
	parent_( parent ),
	sequence_( sequence ),
	dragging_( false )
{
}

MovePointDragTracker::~MovePointDragTracker()
{
}

void MovePointDragTracker::dragBegin(const Point& point)
{
	start_point_ = point;
	prev_point_ = point;
	dragging_ = true;
	
	move_point_policy_ = MovePointPolicyFactory::getInstance()->createPolicy (sequence_);

	for (IContainerEditPart::iterator i = parent_->getSelectedChildrenBegin();
		i != parent_->getSelectedChildrenEnd();
		i++)
		{
			if ((*i)->isMovePointSupported())
			{
				shared_ptr< IEditPart > clone_edit_part = EditPartFactory::getInstance()->createEditPart ((*i)->getModel());
				
				clone_edit_part->getFigure()->fitRect ((*i)->getFigure()->getOutlineRect());
				clone_edit_part->getFigure()->paintWithAlpha (0.5);
				
				selected_edit_parts_.push_back (clone_edit_part);
			}
		}		
}

void MovePointDragTracker::dragTo(const Point& point)
{
	int dx = point.getX() - prev_point_.getX();
	int dy = point.getY() - prev_point_.getY();
	
	typedef std::list< shared_ptr< IEditPart > >::iterator itor_t;
	for (itor_t i = selected_edit_parts_.begin();
		i != selected_edit_parts_.end();
		i++)
		{
			move_point_policy_->moveTo ((*i)->getFigure(), dx, dy);
		}
	
	prev_point_ = point;	
}

void MovePointDragTracker::dragComplete(const Point& point)
{
	int dx = point.getX() - start_point_.getX();
	int dy = point.getY() - start_point_.getY();
	
	// If the final position is the start postition
	// does not execute any command
	if ((dx != 0) || (dy != 0))
	{
		shared_ptr< ICommand > cmd = parent_->createMovePointCommand (sequence_, dx, dy);
	
		bool res = CommandManager::getInstance()->execute (cmd);
	}
	
	move_point_policy_ = shared_ptr< IMovePointPolicy >();
	selected_edit_parts_.clear();
	dragging_ = false;	
}

void MovePointDragTracker::dragCancel(const Point& point)
{
	move_point_policy_ = shared_ptr< IMovePointPolicy >();
	selected_edit_parts_.clear();
	dragging_ = false;		
}

void MovePointDragTracker::paint(Cairo::RefPtr<Cairo::Context> context)
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


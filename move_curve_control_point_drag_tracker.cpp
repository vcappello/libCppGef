#include "move_curve_control_point_drag_tracker.h"

namespace cppgef
{

MoveCurveControlPointDragTracker::MoveCurveControlPointDragTracker(shared_ptr< IContainerEditPart > parent, IMovePointPolicy::PointSequence sequence, const Point& start_point) :
	MovePointDragTracker( parent, sequence ),
	start_point_( start_point )
{
	prev_point_ = start_point;
}

MoveCurveControlPointDragTracker::~MoveCurveControlPointDragTracker()
{
}

void MoveCurveControlPointDragTracker::paint(Cairo::RefPtr<Cairo::Context> context)
{
	MovePointDragTracker::paint (context);
	
	context->begin_new_path();
	context->move_to (start_point_.getX(), start_point_.getY());
	context->line_to (prev_point_.getX(), prev_point_.getY());
	
	context->set_source_rgba (1, 0, 0, 0.5);
	
	std::vector< double > dashes(2);
	dashes[0] = 2.0;
	dashes[1] = 2.0;

	context->set_dash (dashes, 0.0);
	
	context->stroke();
}

}


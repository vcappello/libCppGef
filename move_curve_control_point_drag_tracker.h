#ifndef MOVECURVECONTROLPOINTDRAGTRACKER_H
#define MOVECURVECONTROLPOINTDRAGTRACKER_H

#include <move_point_drag_tracker.h> // Base class: cppgef::MovePointDragTracker

namespace cppgef
{

class MoveCurveControlPointDragTracker : public cppgef::MovePointDragTracker
{
public:
	MoveCurveControlPointDragTracker(shared_ptr< IContainerEditPart > parent, IMovePointPolicy::PointSequence sequence, const Point& start_point);
	virtual ~MoveCurveControlPointDragTracker();

public:
	virtual void paint(Cairo::RefPtr<Cairo::Context> context);
	
protected:
	Point start_point_;
};

}

#endif // MOVECURVECONTROLPOINTDRAGTRACKER_H

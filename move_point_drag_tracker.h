#ifndef MOVEPOINTDRAGTRACKER_H
#define MOVEPOINTDRAGTRACKER_H

#include <idragtracker.h> // Base class: cppgef::IDragTracker

#include <boost/shared_ptr.hpp>

#include <i_container_edit_part.h>
#include <move_policy.h>
#include <move_point_policy_factory.h>

namespace cppgef
{

class MovePointDragTracker : public cppgef::IDragTracker
{
public:
	MovePointDragTracker(shared_ptr< IContainerEditPart > parent, IMovePointPolicy::PointSequence sequence);
	virtual ~MovePointDragTracker();

public:
	virtual void dragBegin(const Point& point);
	virtual void dragTo(const Point& point);
	virtual void dragComplete(const Point& point);
	virtual void dragCancel(const Point& point);
	
	virtual void paint(Cairo::RefPtr<Cairo::Context> context);
	
protected:
	shared_ptr< IContainerEditPart > parent_;
	IMovePointPolicy::PointSequence sequence_;
	Point start_point_;
	Point prev_point_;
	bool dragging_;	
	shared_ptr< IMovePointPolicy > move_point_policy_;
	
	std::list< shared_ptr< IEditPart > > selected_edit_parts_;
};

}

#endif // MOVEPOINTDRAGTRACKER_H

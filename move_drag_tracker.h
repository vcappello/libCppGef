#ifndef MOVEDRAGTRACKER_H
#define MOVEDRAGTRACKER_H

#include <idragtracker.h> // Base class: cppgef::IDragTracker

#include <boost/shared_ptr.hpp>

#include <i_container_edit_part.h>

namespace cppgef
{
	
class MoveDragTracker : public cppgef::IDragTracker
{
public:
	MoveDragTracker(shared_ptr< IContainerEditPart > parent, shared_ptr< IEditPart > active);
	virtual ~MoveDragTracker();

	void dragBegin (const Point& point);
	void dragTo (const Point& point);
	void dragComplete (const Point& point);
	void dragCancel (const Point& point);
	
	void paint(Cairo::RefPtr< Cairo::Context > context);
	
protected:
	shared_ptr< IContainerEditPart > parent_;
	Point start_point_;
	Point prev_point_;
	bool dragging_;
	
	std::list< shared_ptr< IEditPart > > selected_edit_parts_;
};

}

#endif // MOVEDRAGTRACKER_H

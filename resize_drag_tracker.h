#ifndef RESIZEDRAGTRACKER_H
#define RESIZEDRAGTRACKER_H

#include <idragtracker.h> // Base class: cppgef::IDragTracker

#include <map>

#include <resize_policy.h>
#include <i_container_edit_part.h>

using std::map;

namespace cppgef
{

class ResizeDragTracker : public cppgef::IDragTracker
{

public:
	ResizeDragTracker(shared_ptr< IContainerEditPart > parent, AbstractResizePolicy::ResizeDirection resize_direction);
	virtual ~ResizeDragTracker();

public:
	void dragBegin(const Point& point);
	void dragTo(const Point& point);
	void dragComplete(const Point& point);
	void dragCancel(const Point& point);
	
	void paint(Cairo::RefPtr<Cairo::Context> context);
	
protected:
	shared_ptr< IContainerEditPart > parent_;
	AbstractResizePolicy::ResizeDirection resize_direction_;
	map< shared_ptr< IEditPart >, shared_ptr< AbstractResizePolicy > > resize_policies_;
	Point start_point_;
	Point prev_point_;
	bool dragging_;	
	
	
	std::list< shared_ptr< IEditPart > > selected_edit_parts_;
};

}

#endif // RESIZEDRAGTRACKER_H

#ifndef SELECTIONDRAGTRACKER_H
#define SELECTIONDRAGTRACKER_H

#include "idragtracker.h" // Base class: cppgef::IDragTracker

#include <boost/shared_ptr.hpp>

#include <diagram_edit_part.h>

namespace cppgef
{

class SelectionDragTracker : public cppgef::IDragTracker
{
public:
	enum SelectionActionType
	{
		AddToSelection,
		NewSelection
	};

public:
	SelectionDragTracker(shared_ptr< DiagramEditPart > diagram_edit_part, SelectionActionType selection_action_type);
	virtual ~SelectionDragTracker();

public:
	void dragBegin(const Point& point);
	void dragTo(const Point& point);
	void dragComplete(const Point& point);
	void dragCancel(const Point& point);
	void paint(Cairo::RefPtr< Cairo::Context > context);
	
protected:
	shared_ptr< DiagramEditPart > diagram_edit_part_;
	SelectionActionType selection_action_type_;
	bool dragging_;
	Rectangle selected_rect_;
	Gdk::Color selection_border_color_;
	double selection_border_color_alpha_;
	Gdk::Color selection_fill_color_;	
	double selection_fill_color_alpha_;
};

}

#endif // SELECTIONDRAGTRACKER_H

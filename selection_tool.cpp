#include "selection_tool.h"

namespace cppgef
{

SelectionTool::SelectionTool()
{
}

SelectionTool::~SelectionTool()
{
}

shared_ptr<DiagramEditPart> SelectionTool::getDiagramEditPart()
{
	return diagram_edit_part_;
}

void SelectionTool::setDiagramEditPart(shared_ptr<DiagramEditPart> value)
{
	diagram_edit_part_ = value;
}

bool SelectionTool::buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor)
{
	if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1))
	{
		// Single-click start drag tracker action
		Point point(event->x, event->y);
		
		drag_tracker_ = diagram_edit_part_->queryDragTracker (point, KeyModifier( (GdkModifierType)(event->state) ));

		if (drag_tracker_)
			drag_tracker_->dragBegin (point);
	}
	else if (event->type == GDK_2BUTTON_PRESS)
	{
		// Double-click activate inplace editor
		Point point(event->x, event->y);
		
		shared_ptr< IInplaceEditor > editor = diagram_edit_part_->queryInplaceEditor (point);
		if (editor)
		{
			diagram_editor->activateInplaceEditor (editor, point);
		}
	}
	
	return true;
}

bool SelectionTool::motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor)
{
	if (drag_tracker_)
	{
		int x,y;
		GdkModifierType state;

		if (event->is_hint) 
		{
			gdk_window_get_pointer(event->window, &x, &y, &state);
		} 
		else 
		{
			x = event->x;
			y = event->y;
			state = static_cast< GdkModifierType >(event->state);
		} 

		if (state & GDK_BUTTON1_MASK) 
		{
			Point point(event->x, event->y);
			drag_tracker_->dragTo (point);
		}
	}
	
	return true;
}

bool SelectionTool::buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor)
{
	if ((event->type == GDK_BUTTON_RELEASE) && (event->button == 1))
	{
		if (drag_tracker_)
		{
			Point point(event->x, event->y);
			
			drag_tracker_->dragComplete (point);
			
			drag_tracker_.reset();
		}
	}
	
	return true;
}

bool SelectionTool::keyPress(GdkEventKey* event, IDiagramEditor* diagram_editor)
{
	return false;
}

bool SelectionTool::keyRelease(GdkEventKey* event, IDiagramEditor* diagram_editor)
{
	if (event->keyval == GDK_KEY_Escape)
	{
		drag_tracker_.reset();
		signal_action_cancel_.emit();

		return true;
	}

	return false;
}

#ifdef GTKMM_3
bool SelectionTool::draw(Cairo::RefPtr<Cairo::Context> context)
{
	if (drag_tracker_)
		drag_tracker_->paint (context);
	else if (diagram_edit_part_->getSelectedChildrenSize() > 0)
		diagram_edit_part_->paintSelectedDragTrackers (context);

	return true;
}
#else
bool SelectionTool::expose(GdkEventExpose* event, Cairo::RefPtr<Cairo::Context> context)
{
	if (drag_tracker_)
		drag_tracker_->paint (context);
	else if (diagram_edit_part_->getSelectedChildrenSize() > 0)
		diagram_edit_part_->paintSelectedDragTrackers (context);
		
	return true;
}
#endif

SelectionTool::signal_action_t SelectionTool::signalActionBegin()
{
	return signal_action_begin_;
}

SelectionTool::signal_action_t SelectionTool::signalActionComplete()
{
	return signal_action_complete_;
}

SelectionTool::signal_action_t SelectionTool::signalActionCancel()
{
	return signal_action_cancel_;
}

}


#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <itool.h> // Base class: cppgef::ITool

#include <boost/shared_ptr.hpp>
#include <gdkmm/color.h>

#include <rectangle.h>
#include <idragtracker.h>
#include <key_modifier.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace cppgef
{

class SelectionTool : public cppgef::ITool
{

public:
	SelectionTool();
	virtual ~SelectionTool();

public:
	virtual shared_ptr<DiagramEditPart> getDiagramEditPart();
	virtual void setDiagramEditPart(shared_ptr<DiagramEditPart> value);
	
	virtual bool buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor);
	virtual bool motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor);
	virtual bool buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor);
	
	virtual bool keyPress(GdkEventKey* event, IDiagramEditor* diagram_editor);
	virtual bool keyRelease(GdkEventKey* event, IDiagramEditor* diagram_editor);

#ifdef GTKMM_3
	virtual bool draw(Cairo::RefPtr<Cairo::Context> context);
#else
	virtual bool expose(GdkEventExpose* event, Cairo::RefPtr<Cairo::Context> context);
#endif

	signal_action_t signalActionBegin();
	signal_action_t signalActionComplete();
	signal_action_t signalActionCancel();
	
protected:
	shared_ptr< DiagramEditPart > diagram_edit_part_;
	shared_ptr< IDragTracker > drag_tracker_;
	
	signal_action_t signal_action_begin_;
	signal_action_t signal_action_complete_;
	signal_action_t signal_action_cancel_;
};

}

#endif // SELECTIONTOOL_H

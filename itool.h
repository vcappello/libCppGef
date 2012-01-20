#ifndef ITOOL_H
#define ITOOL_H

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>

#include <diagram_edit_part.h>
#include <i_diagram_editor.h>

using boost::shared_ptr;

namespace cppgef
{

class ITool
{
public:
	typedef sigc::signal< void > signal_action_t;
	
public:
	ITool() {}
	virtual ~ITool() {}

	virtual shared_ptr< DiagramEditPart > getDiagramEditPart() = 0;
	virtual void setDiagramEditPart(shared_ptr< DiagramEditPart > value) = 0;
	
	virtual bool buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor) = 0;
	virtual bool buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor) = 0;
	virtual bool motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor) = 0;
	
#ifdef GTKMM_3
	virtual bool draw(Cairo::RefPtr< Cairo::Context > context) = 0;
#else
	virtual bool expose(GdkEventExpose* event, Cairo::RefPtr< Cairo::Context > context) = 0;
#endif
	
	virtual signal_action_t signalActionBegin() = 0;
	virtual signal_action_t signalActionComplete() = 0;
	virtual signal_action_t signalActionCancel() = 0;
};

}

#endif // ITOOL_H

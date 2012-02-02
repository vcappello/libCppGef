#ifndef INSERTCURVETOOL_H
#define INSERTCURVETOOL_H

#include <itool.h> // Base class: cppgef::ITool

#include <point.h>
#include <boost/shared_ptr.hpp>

#include <edit_part_factory.h>
#include <curve_edit_part.h>
#include <curve_line.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace cppgef
{

class InsertCurveTool : public cppgef::ITool
{
public:
	InsertCurveTool();
	virtual ~InsertCurveTool();

	virtual shared_ptr< DiagramEditPart > getDiagramEditPart();
	virtual void setDiagramEditPart(shared_ptr< DiagramEditPart > value);
	
	virtual bool buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor);
	virtual bool buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor);
	virtual bool motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor);

	virtual bool keyPress(GdkEventKey* event, IDiagramEditor* diagram_editor);
	virtual bool keyRelease(GdkEventKey* event, IDiagramEditor* diagram_editor);

#ifdef GTKMM_3
	virtual bool draw(Cairo::RefPtr< Cairo::Context > context);
#else
	virtual bool expose(GdkEventExpose* event, Cairo::RefPtr< Cairo::Context > context);
#endif

	virtual signal_action_t signalActionBegin();
	virtual signal_action_t signalActionComplete();
	virtual signal_action_t signalActionCancel();

protected:
	enum INSERT_STATE
	{
		IS_SET_START_POINT,
		IS_SET_END_POINT,
		IS_SET_START_CONTROL_POINT,
		IS_SET_END_CONTROL_POINT
	};
	
protected:
	shared_ptr< DiagramEditPart > diagram_edit_part_;
	shared_ptr< IContainerEditPart > container_edit_part_;
	shared_ptr< CurveEditPart > edit_part_;
	Point start_point_;
	Point end_point_;
	Point start_control_point_;
	Point end_control_point_;
	INSERT_STATE insert_state_;
	
	signal_action_t signal_action_begin_;
	signal_action_t signal_action_complete_;
	signal_action_t signal_action_cancel_;
};

}

#endif // INSERTCURVETOOL_H

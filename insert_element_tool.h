#ifndef INSERTSHAPETOOL_H
#define INSERTSHAPETOOL_H

#include <itool.h> // Base class: cppgef::ITool

#include <list>
#include <point.h>
#include <boost/shared_ptr.hpp>

#include <edit_part_factory.h>
#include <i_edit_part.h>
#include <i_bounds_model_element.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace cppgef
{

/**
 * @class InsertElementTool
 * @author vincenzo.cappello
 * @date 01/12/2011
 * @file insert_element_tool.h
 * @brief Manage user events for insert elements in a diagram
 * 
 * The InsertElementTool can manage all class that implements 
 * IBoundsModelElement interface.
 * How to insert an element: move the mouse pointer to the desired
 * location, press the left mouse button and drag the mouse pointer
 * to the desired size, release the left mouse button.
 */
class InsertElementTool : public cppgef::ITool
{
public:
	InsertElementTool(shared_ptr< IBoundsModelElement > bounds_model_element);
	virtual ~InsertElementTool();

	std::string getClassType() const;
	void setClassType(const std::string& value);
	
	virtual shared_ptr< DiagramEditPart > getDiagramEditPart();
	virtual void setDiagramEditPart(shared_ptr< DiagramEditPart > value);
	
	virtual bool buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor);
	virtual bool buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor);
	virtual bool motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor);

#ifdef GTKMM_3
	virtual bool draw(Cairo::RefPtr< Cairo::Context > context);
#else
	virtual bool expose(GdkEventExpose* event, Cairo::RefPtr< Cairo::Context > context);
#endif

	virtual signal_action_t signalActionBegin();
	virtual signal_action_t signalActionComplete();
	virtual signal_action_t signalActionCancel();

protected:
	shared_ptr< IBoundsModelElement > bounds_model_element_;
	shared_ptr< DiagramEditPart > diagram_edit_part_;
	shared_ptr< IContainerEditPart > container_edit_part_;
	std::string class_type_;
	shared_ptr< IEditPart > edit_part_;
	Point start_point_;
	
	signal_action_t signal_action_begin_;
	signal_action_t signal_action_complete_;
	signal_action_t signal_action_cancel_;
};

}

#endif // INSERTSHAPETOOL_H

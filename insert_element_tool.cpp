#include "insert_element_tool.h"

#include <command_manager.h>

namespace cppgef
{

InsertElementTool::InsertElementTool(shared_ptr< IBoundsModelElement > bounds_model_element) :
	bounds_model_element_( bounds_model_element )
{
}

InsertElementTool::~InsertElementTool()
{
}

std::string InsertElementTool::getClassType() const
{
	return class_type_;
}

void InsertElementTool::setClassType(const std::string& value)
{
	class_type_ = value;
}

shared_ptr< DiagramEditPart > InsertElementTool::getDiagramEditPart()
{
	return diagram_edit_part_;
}

void InsertElementTool::setDiagramEditPart(shared_ptr< DiagramEditPart > value)
{
	diagram_edit_part_ = value;
}

bool InsertElementTool::buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor)
{
	if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1))
	{
		edit_part_ = EditPartFactory::getInstance()->createEditPart (class_type_);
		
		if (edit_part_)
		{
			start_point_ = Point(event->x, event->y);
			
			bounds_model_element_->setBounds (edit_part_->getFigure(), Rectangle( start_point_, start_point_ ));
			
			container_edit_part_ = diagram_edit_part_->findInnerContainer (start_point_);
		}
	}

	return true;
}

bool InsertElementTool::buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor)
{
	if (edit_part_)
	{
		Rectangle bounds( start_point_,
			Size(event->x - start_point_.getX(), event->y - start_point_.getY()));

		if (!bounds.getSize().isEmpty())
		{
			shared_ptr< ModelElementBase > element = edit_part_->getModel();
			
			Rectangle container_bounds = container_edit_part_->getClientFigure()->getOutlineRect();
			
			bounds.getLocation().moveRel (-container_bounds.getLocation().getX(),
				-container_bounds.getLocation().getY());
			
			bounds_model_element_->setBounds (element, bounds);
			
			shared_ptr< ICommand > cmd = container_edit_part_->createInsertModelElementCommand (element);
			
			if (cmd)
			{
				CommandManager::getInstance()->execute (cmd);
				
				diagram_editor->clearSelection();
			}
			
			signal_action_complete_.emit();
		}
		
		edit_part_.reset();
	}
	
	return true;
}

bool InsertElementTool::motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor)
{
	int x, y;
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

	if ((state & GDK_BUTTON1_MASK) && edit_part_) 
	{
		bounds_model_element_->setBounds (edit_part_->getFigure(), Rectangle( start_point_,
			Size(x - start_point_.getX(), y - start_point_.getY())));
	}
	
	return true;
}

bool InsertElementTool::keyPress(GdkEventKey* event, IDiagramEditor* diagram_editor)
{
	return false;
}

bool InsertElementTool::keyRelease(GdkEventKey* event, IDiagramEditor* diagram_editor)
{
	if (event->keyval == GDK_KEY_Escape)
	{
		edit_part_.reset();
		signal_action_cancel_.emit();

		return true;
	}

	return false;
}

#ifdef GTKMM_3
bool InsertElementTool::draw(Cairo::RefPtr< Cairo::Context > context)
{
	if (edit_part_)
	{
		edit_part_->getFigure()->paint (context);
	}

	return true;
}
#else
bool InsertElementTool::expose(GdkEventExpose* event, Cairo::RefPtr< Cairo::Context > context)
{
	if (edit_part_)
	{
		edit_part_->getFigure()->paint (context);
	}

	return true;
}
#endif

InsertElementTool::signal_action_t InsertElementTool::signalActionBegin()
{
	return signal_action_begin_;
}

InsertElementTool::signal_action_t InsertElementTool::signalActionComplete()
{
	return signal_action_complete_;
}

InsertElementTool::signal_action_t InsertElementTool::signalActionCancel()
{
	return signal_action_cancel_;
}

}

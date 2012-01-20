#include "insert_curve_tool.h"

#include <command_manager.h>
#include <bounds_curve.h>

namespace cppgef
{

InsertCurveTool::InsertCurveTool() :
	insert_state_( IS_SET_START_POINT )
{
}

InsertCurveTool::~InsertCurveTool()
{
}

shared_ptr< DiagramEditPart > InsertCurveTool::getDiagramEditPart()
{
	return diagram_edit_part_;
}

void InsertCurveTool::setDiagramEditPart(shared_ptr< DiagramEditPart > value)
{
	diagram_edit_part_ = value;
}

bool InsertCurveTool::buttonPress(GdkEventButton* event, IDiagramEditor* diagram_editor)
{
	if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1))
	{
		switch (insert_state_)
		{
			case IS_SET_START_POINT:
			{
				edit_part_ = dynamic_pointer_cast< CurveEditPart >(
					EditPartFactory::getInstance()->createEditPart (typeid(CurveLine).name()) );
				
				shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >( 
					edit_part_->getFigure() );
					
				if (edit_part_)
				{
					start_point_ = Point(event->x, event->y);
					end_point_ = start_point_;
					start_control_point_ = start_point_;
					end_control_point_ = start_point_;
					
					curve_figure->setStartPoint (start_point_);
					curve_figure->setEndPoint (end_point_);
					curve_figure->setStartControlPoint (start_control_point_);
					curve_figure->setEndControlPoint (end_control_point_);
					
					container_edit_part_ = diagram_edit_part_->findInnerContainer (start_point_);
					
					insert_state_ = IS_SET_END_POINT;
				}
			}
			break;
				
			case IS_SET_END_POINT:
			break;
			case IS_SET_START_CONTROL_POINT:
			break;
			case IS_SET_END_CONTROL_POINT:
			break;
		}
	}

	return true;
}

bool InsertCurveTool::buttonRelease(GdkEventButton* event, IDiagramEditor* diagram_editor)
{
	if (edit_part_)
	{
		shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >( 
			edit_part_->getFigure() );
		
		switch (insert_state_)
		{
			case IS_SET_START_POINT:
			break;
				
			case IS_SET_END_POINT:
				if (start_point_ == end_point_)
				{
					insert_state_ = IS_SET_START_POINT;
				}
				else
				{
					end_point_ = Point(event->x, event->y);
					end_control_point_ = end_point_;
					curve_figure->setEndPoint (end_control_point_);
					curve_figure->setEndControlPoint (end_point_);
					
					insert_state_ = IS_SET_START_CONTROL_POINT;
				}
			break;
				
			case IS_SET_START_CONTROL_POINT:
				start_control_point_ = Point(event->x, event->y);;
				curve_figure->setStartControlPoint (start_control_point_);
			
				insert_state_ = IS_SET_END_CONTROL_POINT;
			break;
				
			case IS_SET_END_CONTROL_POINT:
				end_control_point_ = Point(event->x, event->y);;
				curve_figure->setEndControlPoint (end_control_point_);

				if (start_point_ != end_point_)
				{
					Rectangle container_bounds = container_edit_part_->getClientFigure()->getOutlineRect();

					shared_ptr< CurveLine > curve_line = dynamic_pointer_cast< CurveLine >(edit_part_->getModel());
					
					curve_line->setStartPoint (Point(
						curve_figure->getStartPoint().getX() - container_bounds.getLocation().getX(),
						curve_figure->getStartPoint().getY() - container_bounds.getLocation().getY()));
						
					curve_line->setEndPoint (Point(
						curve_figure->getEndPoint().getX() - container_bounds.getLocation().getX(),
						curve_figure->getEndPoint().getY() - container_bounds.getLocation().getY()));

					curve_line->setStartControlPoint (Point(
						curve_figure->getStartControlPoint().getX() - container_bounds.getLocation().getX(),
						curve_figure->getStartControlPoint().getY() - container_bounds.getLocation().getY()));

					curve_line->setEndControlPoint (Point(
						curve_figure->getEndControlPoint().getX() - container_bounds.getLocation().getX(),
						curve_figure->getEndControlPoint().getY() - container_bounds.getLocation().getY()));

					shared_ptr< ICommand > cmd = container_edit_part_->createInsertModelElementCommand (curve_line);
					
					if (cmd)
					{
						CommandManager::getInstance()->execute (cmd);
						
						diagram_editor->clearSelection();
					}
					
					signal_action_complete_.emit();
				}
				
				edit_part_.reset();
				
			break;
		}
	
	}
	
	return true;
}

bool InsertCurveTool::motionNotify(GdkEventMotion* event, IDiagramEditor* diagram_editor)
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

	if (edit_part_) 
	{
		
		shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >( 
			edit_part_->getFigure() );
		
		switch (insert_state_)
		{
			case IS_SET_START_POINT:
			break;
				
			case IS_SET_END_POINT:
				end_point_ = Point(x, y);
				curve_figure->setEndPoint (end_point_);
			break;
				
			case IS_SET_START_CONTROL_POINT:
				start_control_point_ = Point(x, y);
				curve_figure->setStartControlPoint (start_control_point_);
			break;
				
			case IS_SET_END_CONTROL_POINT:
				end_control_point_ = Point(x, y);
				curve_figure->setEndControlPoint (end_control_point_);
			break;
		}
	}
	
	return true;
}

#ifdef GTKMM_3
bool InsertCurveTool::draw(Cairo::RefPtr< Cairo::Context > context)
#else
bool InsertCurveTool::expose(GdkEventExpose* event, Cairo::RefPtr< Cairo::Context > context)
#endif
{
	if (edit_part_)
	{
		shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >( 
			edit_part_->getFigure() );

		curve_figure->paint (context);
	
		edit_part_->drawControlLine (context, curve_figure->getStartPoint(), curve_figure->getStartControlPoint());
		edit_part_->drawControlLine (context, curve_figure->getEndPoint(), curve_figure->getEndControlPoint());
	}

	return true;
}

InsertCurveTool::signal_action_t InsertCurveTool::signalActionBegin()
{
	return signal_action_begin_;
}

InsertCurveTool::signal_action_t InsertCurveTool::signalActionComplete()
{
	return signal_action_complete_;
}

InsertCurveTool::signal_action_t InsertCurveTool::signalActionCancel()
{
	return signal_action_cancel_;
}

}


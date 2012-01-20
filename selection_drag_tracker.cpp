#include "selection_drag_tracker.h"

namespace cppgef
{

SelectionDragTracker::SelectionDragTracker(shared_ptr< DiagramEditPart > diagram_edit_part) :
	diagram_edit_part_( diagram_edit_part ),
	dragging_( false ),
	selection_border_color_( "DarkBlue" ),
	selection_border_color_alpha_(0.5),
	selection_fill_color_( "CornflowerBlue" ),
	selection_fill_color_alpha_(0.3)
{
}

SelectionDragTracker::~SelectionDragTracker()
{
}

void SelectionDragTracker::dragBegin(const Point& point)
{
	selected_rect_.setLocation (point);
	dragging_ = true;
}

void SelectionDragTracker::dragTo(const Point& point)
{
	selected_rect_.setSize (Size(point.getX() - selected_rect_.getLocation().getX(), 
		point.getY() - selected_rect_.getLocation().getY()));
}

void SelectionDragTracker::dragCancel(const Point& point)
{
	dragging_ = false;
}

void SelectionDragTracker::dragComplete(const Point& point)
{
	if (selected_rect_.getSize().isEmpty())
	{
		diagram_edit_part_->clearSelection();
		diagram_edit_part_->selectFromPoint (point);
	}
	else
	{
		diagram_edit_part_->clearSelection();
		diagram_edit_part_->selectFromRectangle (selected_rect_);
	}
	
	selected_rect_.setSize (Size(0,0));
	
	dragging_ = false;
}

void SelectionDragTracker::paint(Cairo::RefPtr< Cairo::Context > context)
{
	if (!selected_rect_.getSize().isEmpty())
	{
		context->set_line_width (1.0);
		
		context->begin_new_path();
		
		context->rectangle (selected_rect_.getLocation().getX() + 0.5, selected_rect_.getLocation().getY() + 0.5, 
			selected_rect_.getSize().getWidth(), selected_rect_.getSize().getHeight());
			
		context->set_source_rgba (selection_fill_color_.get_red_p(), 
			selection_fill_color_.get_green_p(),
			selection_fill_color_.get_blue_p(),
			selection_fill_color_alpha_);
			
		context->fill_preserve();
		
		std::vector< double > dashes(2);
		dashes[0] = 2.0;
		dashes[1] = 2.0;
    
		context->set_dash (dashes, 0.0);
	
		context->set_source_rgba (selection_border_color_.get_red_p(), 
			selection_border_color_.get_green_p(),
			selection_border_color_.get_blue_p(),
			selection_fill_color_alpha_);
			
		context->stroke();
	}	
	
	if (!dragging_)
		diagram_edit_part_->paintSelectedDragTrackers (context);
}

}


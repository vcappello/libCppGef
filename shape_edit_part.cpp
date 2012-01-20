#include "shape_edit_part.h"

#include <math.h>
#include <boost/foreach.hpp>

#include <resize_drag_tracker.h>
#include <move_shape_command.h>
#include <resize_shape_command.h>
#include <reparent_shape_command.h>
#include <set_line_color_command.h>
#include <set_line_width_command.h>
#include <set_fill_color_command.h>
#include <set_dash_style_command.h>
#include <delete_command.h>

#include <i_container_edit_part.h>
#include <diagram_edit_part.h>

const int RESIZE_DRAG_WIDTH=4;
const int RESIZE_DRAG_HEIGHT=4;

namespace cppgef
{

ShapeEditPart::ShapeEditPart() :
	selected_( false )
{
	resize_top_left_figure_ = createResizeDragTrackerFigure();
	resize_top_figure_ = createResizeDragTrackerFigure(); 
	resize_top_right_figure_ = createResizeDragTrackerFigure();
	resize_right_figure_ = createResizeDragTrackerFigure(); 
	resize_bottom_right_figure_ = createResizeDragTrackerFigure();
	resize_bottom_figure_ = createResizeDragTrackerFigure(); 
	resize_bottom_left_figure_ = createResizeDragTrackerFigure();
	resize_left_figure_ = createResizeDragTrackerFigure(); 
}

ShapeEditPart::~ShapeEditPart()
{
}

shared_ptr< ModelElementBase > ShapeEditPart::getModel()
{
	return shape_;
}
	
void ShapeEditPart::setModel(shared_ptr< ModelElementBase > value)
{
	shape_ = dynamic_pointer_cast< ShapeBase >(value);
	
	shape_bounds_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		shape_->signalBoundsChanged().connect (
			sigc::mem_fun(this, &ShapeEditPart::onShapeBoundsChanged)) ) );
		
	line_color_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		shape_->signalLineColorChanged().connect (
			sigc::mem_fun(this, &ShapeEditPart::onShapeLineColorChanged)) ) );
		
	line_width_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		shape_->signalLineWidthChanged().connect (
			sigc::mem_fun(this, &ShapeEditPart::onShapeLineWidthChanged)) ) );

	line_dash_style_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		shape_->signalLineDashStyleChanged().connect (
			sigc::mem_fun(this, &ShapeEditPart::onShapeLineDashStyleChanged)) ) );

	line_dash_style_offset_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		shape_->signalLineDashStyleOffsetChanged().connect (
			sigc::mem_fun(this, &ShapeEditPart::onShapeLineDashStyleOffsetChanged)) ) );
		
	fill_color_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		shape_->signalFillColorChanged().connect (
			sigc::mem_fun(this, &ShapeEditPart::onShapeFillColorChanged)) ) );
			
	setFigure (createFigure());
	
	updateResizeDragTrackerFigure();
}

weak_ptr< IContainerEditPart > ShapeEditPart::getParent()
{
	return parent_;
}

void ShapeEditPart::setParent(weak_ptr< IContainerEditPart > parent)
{
	parent_ = parent;
}

bool ShapeEditPart::isSelected() const
{
	return selected_;
}

void ShapeEditPart::setSelected(bool value)
{
	if (value != selected_)
	{
		selected_ = value;
		
		if (value)
		{
			signal_selection_add_.emit (shared_from_this());
		}
		else
		{
			signal_selection_remove_.emit (shared_from_this());
		}
	}
}

bool ShapeEditPart::isAncestorSelected() const
{
	// Search  for the root item
	if (shared_ptr< IContainerEditPart > root = parent_.lock())
	{
		if (root->isSelected())
			return true;
			
		while (shared_ptr< IContainerEditPart > root_parent = root->getParent().lock())
		{
			root = root_parent;
			
			if (root->isSelected())
				return true;			
		}
	}
	
	return false;	
}

bool ShapeEditPart::isMoveSupported() const
{
	return true;
}

bool ShapeEditPart::isResizeSupported() const
{
	return true;
}

bool ShapeEditPart::isMovePointSupported() const
{
	return false;
}

void ShapeEditPart::onShapeBoundsChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
			parent->setLayoutConstraint (shape_, shape_->getBounds());
		else
			getFigure()->fitRect (shape_->getBounds());
	}
	
	updateResizeDragTrackerFigure();
}

void ShapeEditPart::onShapeLineColorChanged()
{
	if (getFigure()->getVisualStyle("stroke-style"))
	{
		shared_ptr< StrokeStyle > stroke_style = dynamic_pointer_cast< StrokeStyle >(getFigure()->getVisualStyle("stroke-style"));
		
		if (stroke_style)
		{
			stroke_style->setColor (shape_->getLineColor());
		}
	}
}

void ShapeEditPart::onShapeLineWidthChanged()
{
	if (getFigure()->getVisualStyle("stroke-style"))
	{
		shared_ptr< StrokeStyle > stroke_style = dynamic_pointer_cast< StrokeStyle >(getFigure()->getVisualStyle("stroke-style"));
		
		if (stroke_style)
		{
			stroke_style->setLineWidth (shape_->getLineWidth());
		}
	}	
}

void ShapeEditPart::onShapeLineDashStyleChanged()
{
	if (getFigure()->getVisualStyle("stroke-style"))
	{
		shared_ptr< StrokeStyle > stroke_style = dynamic_pointer_cast< StrokeStyle >(getFigure()->getVisualStyle("stroke-style"));
		
		if (stroke_style)
		{
			stroke_style->setLineDashStyle (shape_->getLineDashStyle());
		}
	}	
}

void ShapeEditPart::onShapeLineDashStyleOffsetChanged()
{
	if (getFigure()->getVisualStyle("stroke-style"))
	{
		shared_ptr< StrokeStyle > stroke_style = dynamic_pointer_cast< StrokeStyle >(getFigure()->getVisualStyle("stroke-style"));
		
		if (stroke_style)
		{
			stroke_style->setLineDashStyleOffset (shape_->getLineDashStyleOffset());
		}
	}		
}

void ShapeEditPart::onShapeFillColorChanged()
{
	if (getFigure()->getVisualStyle("fill-style"))
	{
		shared_ptr< FillStyle > fill_style = dynamic_pointer_cast< FillStyle >(getFigure()->getVisualStyle("fill-style"));
		
		if (fill_style)
		{
			fill_style->setColor (shape_->getFillColor());
		}
	}
}

bool ShapeEditPart::selectFromPoint(const Point& point)
{
	if (getFigure()->isPointIn (point))
	{
		setSelected (true);
		return true;
	}
	
	return false;
}

bool ShapeEditPart::selectFromRectangle(const Rectangle& rectangle)
{
	if (getFigure()->isBoundsOut (rectangle))
	{
		setSelected (true);
		return true;
	}
	
	return false;
}

bool ShapeEditPart::queryStartMove(const Point& point)
{
	return getFigure()->isPointIn (point);
}

void ShapeEditPart::paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context)
{
	resize_top_left_figure_->paint (context);
	resize_top_figure_->paint (context);
	resize_top_right_figure_->paint (context);
	resize_right_figure_->paint (context);
	resize_bottom_right_figure_->paint (context);
	resize_bottom_figure_->paint (context);
	resize_bottom_left_figure_->paint (context);
	resize_left_figure_->paint (context);
}

shared_ptr< IDragTracker > ShapeEditPart::queryDragTracker(const Point& point)
{
	if (resize_top_left_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_TOP_LEFT));
		
	if (resize_top_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
		shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_TOP));

	if (resize_top_right_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_TOP_RIGHT));

	if (resize_right_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_RIGHT));

	if (resize_bottom_right_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_BOTTOM_RIGHT));

	if (resize_bottom_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_BOTTOM));

	if (resize_bottom_left_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_BOTTOM_LEFT));

	if (resize_left_figure_->getBounds().contains (point))
		return shared_ptr< ResizeDragTracker >(new ResizeDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), AbstractResizePolicy::RD_LEFT));
		
	return shared_ptr< IDragTracker >();
}

shared_ptr< IContainerEditPart > ShapeEditPart::findInnerContainer(const Point& point)
{
	return shared_ptr< IContainerEditPart >();
}

shared_ptr< ICommand > ShapeEditPart::createMoveCommand(int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< MoveShapeCommand >( new MoveShapeCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			shape_, dx, dy) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ResizeShapeCommand >( new ResizeShapeCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			shape_, resize_direction, dx, dy) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ReparentShapeCommand >( new ReparentShapeCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			shape_, 
			dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()), 
			new_parent, 
			dx, dy) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy)
{
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createSetLineColorCommand(const Color& color)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >( new SetLineColorCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
				shape_,
				color
			) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createSetLineWidthCommand(double width)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());

	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >( new SetLineWidthCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
				shape_,
				width
			) );
	}

	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createSetFillColorCommand(const Color& color)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >( new SetFillColorCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
				shape_,
				color
			) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > ShapeEditPart::createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command 
		shared_ptr< ICommand > cmd(
			new SetDashStyleCommand( diagram, shape_, dash_style, dash_offset ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > ShapeEditPart::createDeleteCommand()
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >(
			new DeleteCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
				dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()),
				shape_ ) );
	}
	
	return shared_ptr< ICommand >();	
}

ShapeEditPart::signal_selection_change_t ShapeEditPart::signalSelectionAdd()
{
	return signal_selection_add_;
}

ShapeEditPart::signal_selection_change_t ShapeEditPart::signalSelectionRemove()
{
	return signal_selection_remove_;
}

shared_ptr< DragTrackerFigure > ShapeEditPart::createResizeDragTrackerFigure()
{
	shared_ptr< DragTrackerFigure > resize_figure = shared_ptr< DragTrackerFigure >( new DragTrackerFigure() );
	resize_figure->getFillStyle()->setColor (Color("white"));
	resize_figure->getStrokeStyle()->setColor (Color("black"));
	
	return resize_figure;
}

void ShapeEditPart::updateResizeDragTrackerFigure()
{
	Rectangle bounds = getFigure()->getOutlineRect();
	
	resize_top_left_figure_->setBounds (Point( 
		bounds.getLeft(), 
		bounds.getTop()).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	resize_top_figure_->setBounds (Point( 
		bounds.getLeft() + (std::abs(bounds.getSize().getWidth()) / 2), 
		bounds.getTop()).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	resize_top_right_figure_->setBounds (Point( 
		bounds.getRight(), 
		bounds.getTop()).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	resize_right_figure_->setBounds (Point( 
		bounds.getRight(), 
		bounds.getTop() + (std::abs(bounds.getSize().getHeight()) / 2)).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	resize_bottom_right_figure_->setBounds (Point( 
		bounds.getRight(), 
		bounds.getBottom()).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	resize_bottom_figure_->setBounds (Point( 
		bounds.getLeft() + (std::abs(bounds.getSize().getWidth()) / 2), 
		bounds.getBottom()).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	resize_bottom_left_figure_->setBounds (Point( 
		bounds.getLeft(), 
		bounds.getBottom()).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));	
		
	resize_left_figure_->setBounds (Point( 
		bounds.getLeft(), 
		bounds.getTop() + (std::abs(bounds.getSize().getHeight()) / 2)).surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
}

}



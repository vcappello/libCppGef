#include "line_edit_part.h"

#include <geometry.h>

#include <move_line_command.h>
#include <reparent_line_command.h>
#include <move_line_point_command.h>
#include <set_line_color_command.h>
#include <set_line_width_command.h>
#include <set_dash_style_command.h>
#include <delete_command.h>

#include <i_container_edit_part.h>
#include <diagram_edit_part.h>

#include <move_point_drag_tracker.h>

const int RESIZE_DRAG_WIDTH=4;
const int RESIZE_DRAG_HEIGHT=4;

namespace cppgef
{

LineEditPart::LineEditPart() :
	selected_( false )
{
	move_start_point_figure_ = createMoveDragTrackerFigure();
	move_end_point_figure_ = createMoveDragTrackerFigure(); 
}

LineEditPart::~LineEditPart()
{
}

shared_ptr<ModelElementBase> LineEditPart::getModel()
{
	return simple_line_;
}

void LineEditPart::setModel(shared_ptr<ModelElementBase> value)
{
	simple_line_ = dynamic_pointer_cast< LineBase >(value);
	
	line_start_point_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		simple_line_->signalStartPointChanged().connect (
			sigc::mem_fun(this, &LineEditPart::onLineStartPointChanged)) ) );
		
	line_end_point_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		simple_line_->signalEndPointChanged().connect (
			sigc::mem_fun(this, &LineEditPart::onLineEndPointChanged)) ) );
			
	line_color_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		simple_line_->signalLineColorChanged().connect (
			sigc::mem_fun(this, &LineEditPart::onLineColorChanged)) ) );
	
	line_width_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		simple_line_->signalLineWidthChanged().connect (
			sigc::mem_fun(this, &LineEditPart::onLineWidthChanged)) ) );

	line_dash_style_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		simple_line_->signalLineDashStyleChanged().connect (
			sigc::mem_fun(this, &LineEditPart::onLineDashStyleChanged)) ) );

	line_dash_style_offset_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		simple_line_->signalLineDashStyleOffsetChanged().connect (
			sigc::mem_fun(this, &LineEditPart::onLineDashStyleOffsetChanged)) ) );
					
	setFigure (createFigure());
	
	updateMoveDragTrackerFigure();	
}

shared_ptr<Figure> LineEditPart::createFigure()
{
	shared_ptr< LineFigure > figure( new LineFigure());
	figure->setStartPoint (simple_line_->getStartPoint());
	figure->setEndPoint (simple_line_->getEndPoint());
	figure->getStrokeStyle()->setColor (simple_line_->getLineColor());
	figure->getStrokeStyle()->setLineWidth (simple_line_->getLineWidth());
	figure->getStrokeStyle()->setLineDashStyle (simple_line_->getLineDashStyle());
	figure->getStrokeStyle()->setLineDashStyleOffset (simple_line_->getLineDashStyleOffset());
	
	return figure;
}

weak_ptr<IContainerEditPart> LineEditPart::getParent()
{
	return parent_;
}

void LineEditPart::setParent(weak_ptr<IContainerEditPart> parent)
{
	parent_ = parent;
}

bool LineEditPart::isSelected() const
{
	return selected_;
}

void LineEditPart::setSelected(bool value)
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

bool LineEditPart::isAncestorSelected() const
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

bool LineEditPart::isMoveSupported() const
{
	return true;
}

bool LineEditPart::isResizeSupported() const
{
	return false;
}

bool LineEditPart::isMovePointSupported() const
{
	return true;
}

shared_ptr<ModelElementBase> LineEditPart::createModelElement()
{
	return shared_ptr< ModelElementBase >( new LineBase() );
}

bool LineEditPart::selectFromPoint(const Point& point)
{
	if (simple_line_figure_->isPointIn (point))
		{
			setSelected (true);
			return true;
		}
		
	return false;
}

bool LineEditPart::selectFromRectangle(const Rectangle& rectangle)
{
	if (simple_line_figure_->isBoundsOut (rectangle))
	{
		setSelected (true);
		return true;
	}
	
	return false;	
}

bool LineEditPart::queryStartMove(const Point& point)
{
	if (simple_line_figure_->isPointIn (point))
		{
			return true;
		}
		
	return false;	
}

void LineEditPart::paintSelectedDragTrackers(Cairo::RefPtr<Cairo::Context> context)
{
	move_start_point_figure_->paint (context);
	move_end_point_figure_->paint (context);
}

shared_ptr<IDragTracker> LineEditPart::queryDragTracker(const Point& point)
{
	if (move_start_point_figure_->getBounds().contains (point))
		return shared_ptr< MovePointDragTracker >(new MovePointDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), 
			IMovePointPolicy::StartPoint));
		
	if (move_end_point_figure_->getBounds().contains (point))
		return shared_ptr< MovePointDragTracker >(new MovePointDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), 
			IMovePointPolicy::EndPoint));
			
	return shared_ptr<IDragTracker>();
}

shared_ptr< IInplaceEditor > LineEditPart::queryInplaceEditor(const Point& point)
{
	return shared_ptr< IInplaceEditor >();
}

shared_ptr<ICommand> LineEditPart::createMoveCommand(int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< MoveLineCommand >( new MoveLineCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			simple_line_, dx, dy) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr<ICommand> LineEditPart::createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy)
{
	return shared_ptr< ICommand >();
}

shared_ptr<ICommand> LineEditPart::createReparentCommand(shared_ptr<ComplexModelElementBase> new_parent, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ReparentLineCommand >( new ReparentLineCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			simple_line_, 
			dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()), 
			new_parent, 
			dx, dy) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > LineEditPart::createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< MoveLinePointCommand >( new MoveLinePointCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			simple_line_, sequence, dx, dy) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > LineEditPart::createSetLineColorCommand(const Color& color)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< SetLineColorCommand >( new SetLineColorCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			simple_line_, color) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > LineEditPart::createSetLineWidthCommand(double width)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());

	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >( new SetLineWidthCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
			simple_line_, width) );
	}

	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > LineEditPart::createSetFillColorCommand(const Color& color)
{
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > LineEditPart::createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command 
		shared_ptr< ICommand > cmd(
			new SetDashStyleCommand( diagram, simple_line_, dash_style, dash_offset ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > LineEditPart::createDeleteCommand()
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >(
			new DeleteCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
				dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()),
				simple_line_ ) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr<IContainerEditPart> LineEditPart::findInnerContainer(const Point& point)
{
	return shared_ptr< IContainerEditPart >();
}

shared_ptr<Figure> LineEditPart::getFigure()
{
	return simple_line_figure_;
}

void LineEditPart::setFigure(shared_ptr<Figure> value)
{
	simple_line_figure_ = dynamic_pointer_cast< LineFigure >(value);
	
	simple_line_figure_->getStartPoint().signalXChanged().connect (
		sigc::mem_fun( this, &LineEditPart::onFigureStartPointChanged ));
		
	simple_line_figure_->getStartPoint().signalYChanged().connect (
		sigc::mem_fun( this, &LineEditPart::onFigureStartPointChanged ));
		
	simple_line_figure_->getEndPoint().signalXChanged().connect (
		sigc::mem_fun( this, &LineEditPart::onFigureEndPointChanged ));
		
	simple_line_figure_->getEndPoint().signalYChanged().connect (
		sigc::mem_fun( this, &LineEditPart::onFigureEndPointChanged ));
}

LineEditPart::signal_selection_change_t LineEditPart::signalSelectionAdd()
{
	return signal_selection_add_;
}

LineEditPart::signal_selection_change_t LineEditPart::signalSelectionRemove()
{
	return signal_selection_remove_;
}

shared_ptr< DragTrackerFigure > LineEditPart::createMoveDragTrackerFigure()
{
	shared_ptr< DragTrackerFigure > resize_figure = shared_ptr< DragTrackerFigure >( new DragTrackerFigure() );
	resize_figure->getFillStyle()->setColor (Color("white"));
	resize_figure->getStrokeStyle()->setColor (Color("black"));
	
	return resize_figure;
}

void LineEditPart::updateMoveDragTrackerFigure()
{
	Point sp = simple_line_figure_->getStartPoint();
	Point ep = simple_line_figure_->getEndPoint();
	
	move_start_point_figure_->setBounds (sp.surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
		
	move_end_point_figure_->setBounds (ep.surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
}

void LineEditPart::onLineStartPointChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
			parent->setLayoutConstraint (simple_line_, 
				Rectangle(simple_line_->getStartPoint(), simple_line_->getEndPoint()));
		else
			simple_line_figure_->setStartPoint (simple_line_->getStartPoint());
	}
	
	updateMoveDragTrackerFigure();
}

void LineEditPart::onLineEndPointChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
			parent->setLayoutConstraint (simple_line_, 
				Rectangle(simple_line_->getStartPoint(), simple_line_->getEndPoint()));
		else
			simple_line_figure_->setEndPoint (simple_line_->getEndPoint());
	}
	
	updateMoveDragTrackerFigure();	
}

void LineEditPart::onLineColorChanged()
{
	simple_line_figure_->getStrokeStyle()->setColor (simple_line_->getLineColor());
}

void LineEditPart::onLineWidthChanged()
{
	simple_line_figure_->getStrokeStyle()->setLineWidth (simple_line_->getLineWidth());
}

void LineEditPart::onLineDashStyleChanged()
{
	simple_line_figure_->getStrokeStyle()->setLineDashStyle (simple_line_->getLineDashStyle());
}

void LineEditPart::onLineDashStyleOffsetChanged()
{
	simple_line_figure_->getStrokeStyle()->setLineDashStyleOffset (simple_line_->getLineDashStyleOffset());
}
	
void LineEditPart::onFigureStartPointChanged()
{
	updateMoveDragTrackerFigure();	
}

void LineEditPart::onFigureEndPointChanged()
{
	updateMoveDragTrackerFigure();	
}

}


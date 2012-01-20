#include "curve_edit_part.h"

#include <geometry.h>

#include <move_curve_command.h>
#include <reparent_curve_command.h>
#include <move_curve_point_command.h>
#include <set_line_color_command.h>
#include <set_line_width_command.h>
#include <set_dash_style_command.h>
#include <delete_command.h>

#include <i_container_edit_part.h>
#include <diagram_edit_part.h>

#include <move_point_drag_tracker.h>
#include <move_curve_control_point_drag_tracker.h>

const int RESIZE_DRAG_WIDTH=4;
const int RESIZE_DRAG_HEIGHT=4;

namespace cppgef
{

CurveEditPart::CurveEditPart() :
	selected_( false )
{
	move_start_point_figure_ = createMoveDragTrackerFigure();
	move_end_point_figure_ = createMoveDragTrackerFigure(); 	
	move_start_control_point_figure_ = createMoveDragTrackerFigure();
	move_end_control_point_figure_ = createMoveDragTrackerFigure(); 	
}

CurveEditPart::~CurveEditPart()
{
}

shared_ptr<ModelElementBase> CurveEditPart::getModel()
{
	return curve_;
}

void CurveEditPart::setModel(shared_ptr<ModelElementBase> value)
{
	curve_ = dynamic_pointer_cast< CurveLine >(value);
	
	curve_start_point_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalStartPointChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveStartPointChanged)) ) );
		
	curve_end_point_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalEndPointChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveEndPointChanged)) ) );
			
	curve_start_control_point_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalStartControlPointChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveStartControlPointChanged)) ) );
		
	curve_end_control_point_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalEndControlPointChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveEndControlPointChanged)) ) );
			
	curve_color_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalLineColorChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveLineColorChanged)) ) );
			
	curve_line_width_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalLineWidthChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveLineWidthChanged)) ) );

	curve_line_dash_style_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalLineDashStyleChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveLineDashStyleChanged)) ) );

	curve_line_dash_style_offset_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		curve_->signalLineDashStyleOffsetChanged().connect (
			sigc::mem_fun(this, &CurveEditPart::onCurveLineDashStyleOffsetChanged)) ) );
			
	setFigure (createFigure());
	
	updateMoveDragTrackerFigure();	
}

shared_ptr<Figure> CurveEditPart::createFigure()
{
	shared_ptr< CurveFigure > figure( new CurveFigure());
	figure->setStartPoint (curve_->getStartPoint());
	figure->setEndPoint (curve_->getEndPoint());
	figure->setStartControlPoint (curve_->getStartControlPoint());
	figure->setEndControlPoint (curve_->getEndControlPoint());
	figure->getStrokeStyle()->setColor (curve_->getLineColor());
	figure->getStrokeStyle()->setLineWidth (curve_->getLineWidth());
	figure->getStrokeStyle()->setLineDashStyle (curve_->getLineDashStyle());
	figure->getStrokeStyle()->setLineDashStyleOffset (curve_->getLineDashStyleOffset());
	
	return figure;
}

weak_ptr<IContainerEditPart> CurveEditPart::getParent()
{
	return parent_;
}

void CurveEditPart::setParent(weak_ptr<IContainerEditPart> parent)
{
	parent_ = parent;
}

bool CurveEditPart::isSelected() const
{
	return selected_;
}

void CurveEditPart::setSelected(bool value)
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

bool CurveEditPart::isAncestorSelected() const
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

bool CurveEditPart::isMoveSupported() const
{
	return true;
}

bool CurveEditPart::isResizeSupported() const
{
	return false;
}

bool CurveEditPart::isMovePointSupported() const
{
	return true;
}

shared_ptr<ModelElementBase> CurveEditPart::createModelElement()
{
	return shared_ptr< ModelElementBase >( new CurveLine() );
}

bool CurveEditPart::selectFromPoint(const Point& point)
{
	if (curve_figure_->isPointIn (point)) 
	{
		setSelected (true);
		return true;
	}
		
	return false;
}

bool CurveEditPart::selectFromRectangle(const Rectangle& rectangle)
{
	if (curve_figure_->isBoundsOut (rectangle))
	{
		setSelected (true);
		return true;
	}
	
	return false;	
}

void CurveEditPart::drawControlLine(Cairo::RefPtr<Cairo::Context> context, const Point& start_point, const Point& end_point)
{
	context->begin_new_path();
	context->move_to (start_point.getX(), start_point.getY());
	context->line_to (end_point.getX(), end_point.getY());
	
	context->set_source_rgba (1, 0, 0, 0.5);
	
	std::vector< double > dashes(2);
	dashes[0] = 2.0;
	dashes[1] = 2.0;

	context->set_dash (dashes, 0.0);
	
	context->set_line_width (1.0);

	context->stroke();	
}

bool CurveEditPart::queryStartMove(const Point& point)
{
	if (curve_figure_->isPointIn (point))
	{
		return true;
	}
		
	return false;	
}

void CurveEditPart::paintSelectedDragTrackers(Cairo::RefPtr<Cairo::Context> context)
{
	move_start_point_figure_->paint (context);
	move_end_point_figure_->paint (context);
	move_start_control_point_figure_->paint (context);
	move_end_control_point_figure_->paint (context);
	
	drawControlLine (context, curve_figure_->getStartPoint(), curve_figure_->getStartControlPoint());
	drawControlLine (context, curve_figure_->getEndPoint(), curve_figure_->getEndControlPoint());
}

shared_ptr<IDragTracker> CurveEditPart::queryDragTracker(const Point& point)
{
	if (move_start_point_figure_->getBounds().contains (point))
		return shared_ptr< MovePointDragTracker >(new MovePointDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), 
			IMovePointPolicy::CurveStartPoint));
		
	if (move_end_point_figure_->getBounds().contains (point))
		return shared_ptr< MovePointDragTracker >(new MovePointDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), 
			IMovePointPolicy::CurveEndPoint));
			
	if (move_start_control_point_figure_->getBounds().contains (point))
		return shared_ptr< MoveCurveControlPointDragTracker >(new MoveCurveControlPointDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), 
			IMovePointPolicy::CurveStartControlPoint,
			curve_figure_->getStartPoint()));
		
	if (move_end_control_point_figure_->getBounds().contains (point))
		return shared_ptr< MoveCurveControlPointDragTracker >(new MoveCurveControlPointDragTracker(
			shared_ptr< IContainerEditPart >(DiagramEditPart::queryDiagramFromChild(shared_from_this())), 
			IMovePointPolicy::CurveEndControlPoint,
			curve_figure_->getEndPoint()));
			
	return shared_ptr<IDragTracker>();
}

shared_ptr< IInplaceEditor > CurveEditPart::queryInplaceEditor(const Point& point)
{
	return shared_ptr< IInplaceEditor >();
}

shared_ptr<ICommand> CurveEditPart::createMoveCommand(int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< MoveCurveCommand >( new MoveCurveCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			curve_, dx, dy) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr<ICommand> CurveEditPart::createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy)
{
	return shared_ptr< ICommand >();
}

shared_ptr<ICommand> CurveEditPart::createReparentCommand(shared_ptr<ComplexModelElementBase> new_parent, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ReparentCurveCommand >( new ReparentCurveCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			curve_, 
			dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()), 
			new_parent, 
			dx, dy) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > CurveEditPart::createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< MoveCurvePointCommand >( new MoveCurvePointCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			curve_, sequence, dx, dy) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > CurveEditPart::createSetLineColorCommand(const Color& color)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< SetLineColorCommand >( new SetLineColorCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			curve_, color) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > CurveEditPart::createSetLineWidthCommand(double width)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());

	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >( new SetLineWidthCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
			curve_, width) );
	}

	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > CurveEditPart::createSetFillColorCommand(const Color& color)
{
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > CurveEditPart::createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command 
		shared_ptr< ICommand > cmd(
			new SetDashStyleCommand( diagram, curve_, dash_style, dash_offset ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > CurveEditPart::createDeleteCommand()
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >(
			new DeleteCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
				dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()),
				curve_ ) );
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr<IContainerEditPart> CurveEditPart::findInnerContainer(const Point& point)
{
	return shared_ptr< IContainerEditPart >();
}

shared_ptr<Figure> CurveEditPart::getFigure()
{
	return curve_figure_;
}

void CurveEditPart::setFigure(shared_ptr<Figure> value)
{
	curve_figure_ = dynamic_pointer_cast< CurveFigure >(value);
	
	curve_figure_->getStartPoint().signalXChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureStartPointChanged));
		
	curve_figure_->getStartPoint().signalYChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureStartPointChanged));
		
	curve_figure_->getEndPoint().signalXChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureEndPointChanged));
		
	curve_figure_->getEndPoint().signalYChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureEndPointChanged));

	curve_figure_->getStartControlPoint().signalXChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureStartControlPointChanged));
		
	curve_figure_->getStartControlPoint().signalYChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureStartControlPointChanged));

	curve_figure_->getEndControlPoint().signalXChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureEndControlPointChanged));
		
	curve_figure_->getEndControlPoint().signalYChanged().connect (
		sigc::mem_fun(this, &CurveEditPart::onFigureEndControlPointChanged));
}

CurveEditPart::signal_selection_change_t CurveEditPart::signalSelectionAdd()
{
	return signal_selection_add_;
}

CurveEditPart::signal_selection_change_t CurveEditPart::signalSelectionRemove()
{
	return signal_selection_remove_;
}

shared_ptr< DragTrackerFigure > CurveEditPart::createMoveDragTrackerFigure()
{
	shared_ptr< DragTrackerFigure > resize_figure = shared_ptr< DragTrackerFigure >( new DragTrackerFigure() );
	resize_figure->getFillStyle()->setColor (Color("white"));
	resize_figure->getStrokeStyle()->setColor (Color("black"));
	
	return resize_figure;
}

void CurveEditPart::updateMoveDragTrackerFigure()
{
	Point sp = curve_figure_->getStartPoint();
	Point ep = curve_figure_->getEndPoint();
	Point scp = curve_figure_->getStartControlPoint();
	Point ecp = curve_figure_->getEndControlPoint();
	
	move_start_point_figure_->setBounds (sp.surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
	move_end_point_figure_->setBounds (ep.surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
	move_start_control_point_figure_->setBounds (scp.surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
	move_end_control_point_figure_->setBounds (ecp.surround (RESIZE_DRAG_WIDTH, RESIZE_DRAG_HEIGHT));
}

void CurveEditPart::onCurveStartPointChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
		{
			int dx = curve_->getStartControlPoint().getX() - curve_->getStartPoint().getX();
			int dy = curve_->getStartControlPoint().getY() - curve_->getStartPoint().getY();
			
			curve_figure_->getStartControlPoint().set (
				curve_figure_->getStartPoint().getX() + dx, 
				curve_figure_->getStartPoint().getY() + dy);
			
			parent->setLayoutConstraint (curve_, getCurveBoundsFromModel());
		}		
		else
			curve_figure_->setStartPoint (curve_->getStartPoint());
	}
	
	updateMoveDragTrackerFigure();
}

void CurveEditPart::onCurveEndPointChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
		{
			int dx = curve_->getEndControlPoint().getX() - curve_->getEndPoint().getX();
			int dy = curve_->getEndControlPoint().getY() - curve_->getEndPoint().getY();
			
			curve_figure_->getEndControlPoint().set (
				curve_figure_->getEndPoint().getX() + dx, 
				curve_figure_->getEndPoint().getY() + dy);
			
			parent->setLayoutConstraint (curve_, getCurveBoundsFromModel());
		}			
		else
			curve_figure_->setEndPoint (curve_->getEndPoint());
	}
	
	updateMoveDragTrackerFigure();	
}

void CurveEditPart::onCurveStartControlPointChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
		{
			int dx = curve_->getStartControlPoint().getX() - curve_->getStartPoint().getX();
			int dy = curve_->getStartControlPoint().getY() - curve_->getStartPoint().getY();
			
			curve_figure_->getStartControlPoint().set (
				curve_figure_->getStartPoint().getX() + dx, 
				curve_figure_->getStartPoint().getY() + dy);
			
			parent->setLayoutConstraint (curve_, getCurveBoundsFromModel());
		}
		else
			curve_figure_->setStartControlPoint (curve_->getStartControlPoint());
	}
	
	updateMoveDragTrackerFigure();
}

void CurveEditPart::onCurveEndControlPointChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
		{
			int dx = curve_->getEndControlPoint().getX() - curve_->getEndPoint().getX();
			int dy = curve_->getEndControlPoint().getY() - curve_->getEndPoint().getY();
			
			curve_figure_->getEndControlPoint().set (
				curve_figure_->getEndPoint().getX() + dx, 
				curve_figure_->getEndPoint().getY() + dy);
			
			parent->setLayoutConstraint (curve_, getCurveBoundsFromModel());
		}
		else
			curve_figure_->setEndControlPoint (curve_->getEndControlPoint());
	}
	
	updateMoveDragTrackerFigure();	
}

void CurveEditPart::onCurveLineColorChanged()
{
	curve_figure_->getStrokeStyle()->setColor (curve_->getLineColor());
}

void CurveEditPart::onCurveLineWidthChanged()
{
	curve_figure_->getStrokeStyle()->setLineWidth (curve_->getLineWidth());
}

void CurveEditPart::onCurveLineDashStyleChanged()
{
	curve_figure_->getStrokeStyle()->setLineDashStyle (curve_->getLineDashStyle());
}

void CurveEditPart::onCurveLineDashStyleOffsetChanged()
{
	curve_figure_->getStrokeStyle()->setLineDashStyleOffset (curve_->getLineDashStyleOffset());
}	

Rectangle CurveEditPart::getCurveBoundsFromModel()
{
	vector< Point > points;
	createBezierPoints (points,
		curve_->getStartPoint(), curve_->getStartControlPoint(), 
		curve_->getEndControlPoint(), curve_->getEndPoint());
		
	Rectangle curve_bounds = boundsPoints (points);	
	
	return curve_bounds;
}

void CurveEditPart::onFigureStartPointChanged()
{
	updateMoveDragTrackerFigure();	
}

void CurveEditPart::onFigureEndPointChanged()
{
	updateMoveDragTrackerFigure();	
}

void CurveEditPart::onFigureStartControlPointChanged()
{
	updateMoveDragTrackerFigure();	
}

void CurveEditPart::onFigureEndControlPointChanged()
{
	updateMoveDragTrackerFigure();	
}

}


#include "frame_edit_part.h"

#include <boost/foreach.hpp>
#include <algorithm>

#include <glibmm/i18n.h>

#include <command_manager.h>
#include <group_command.h>
#include <move_frame_command.h>
#include <resize_frame_command.h>
#include <reparent_frame_command.h>
#include <set_line_color_command.h>
#include <set_line_width_command.h>
#include <set_fill_color_command.h>
#include <set_dash_style_command.h>
#include <insert_model_element_command.h>
#include <edit_text_command.h>
#include <delete_command.h>

#include <selection_drag_tracker.h>
#include <move_drag_tracker.h>
#include <resize_drag_tracker.h>

#include <v_layout_manager.h>
#include <v_layout_constraint.h>
#include <xy_layout_manager.h>
#include <xy_layout_constraint.h>
#include <rounded_rectangle_figure.h>
#include <diagram_edit_part.h>

const int RESIZE_DRAG_WIDTH=4;
const int RESIZE_DRAG_HEIGHT=4;

const int LABEL_INDEX = 0;
const int LINE_INDEX = 1;
const int CONTAINER_INDEX = 2;

namespace cppgef
{

FrameEditPart::FrameEditPart() :
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

FrameEditPart::~FrameEditPart()
{
}

weak_ptr< IContainerEditPart > FrameEditPart::getParent()
{
	return parent_;
}

void FrameEditPart::setParent(weak_ptr< IContainerEditPart > parent)
{
	parent_ = parent;
}

shared_ptr< ModelElementBase > FrameEditPart::getModel()
{
	return frame_;
}
	
void FrameEditPart::setModel(shared_ptr< ModelElementBase > value)
{
	frame_ = dynamic_pointer_cast< Frame >(value);
	
	// Connect model signals
	title_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalTitleChanged().connect (
			sigc::mem_fun( this, &FrameEditPart::onModelTitleChanged )) ) );
			
	model_bounds_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalBoundsChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onModelBoundsChanged)) ) );
			
	child_added_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalChildAdded().connect (
			sigc::mem_fun( this, &FrameEditPart::onModelChildAdded )) ) );
			
	child_erased_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalChildErased().connect (
			sigc::mem_fun( this, &FrameEditPart::onModelChildErased )) ) );
	
	line_color_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalLineColorChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onFrameLineColorChanged)) ) );
		
	fill_color_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalFillColorChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onFrameFillColorChanged)) ) );
	
	line_width_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalLineWidthChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onFrameLineWidthChanged)) ) );

	line_dash_style_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalLineDashStyleChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onFrameLineDashStyleChanged)) ) );

	line_dash_style_offset_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		frame_->signalLineDashStyleOffsetChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onFrameLineDashStyleOffsetChanged)) ) );
			
	// Initialize view
	setFigure (createFigure());
	
	// Create children EditPart from Model children
	for (Frame::iterator child = frame_->getChildrenBegin();
		child != frame_->getChildrenEnd();
		child++)
	{
		addChild (*child);
	}
	
	updateResizeDragTrackerFigure();	
}

shared_ptr<Figure> FrameEditPart::getFigure()
{
	return figure_;
}

void FrameEditPart::setFigure(shared_ptr<Figure> value)
{
	figure_ = dynamic_pointer_cast< GroupFigure >( value );
	label_figure_ = dynamic_pointer_cast< LabelFigure >( figure_->getChild (LABEL_INDEX) );
	line_figure_ = dynamic_pointer_cast< LineFigure >( figure_->getChild (LINE_INDEX) );
	client_figure_ = dynamic_pointer_cast< GroupFigure >( figure_->getChild (CONTAINER_INDEX) );
	
	figure_->getBounds().signalRectangleChanged().connect (
			sigc::mem_fun(this, &FrameEditPart::onFigureBoundsChanged));
			
	label_figure_->signalTextApplied().connect (
			sigc::mem_fun(this, &FrameEditPart::onLabelTextApplied));
}

bool FrameEditPart::isSelected() const
{
	return selected_;
}

void FrameEditPart::setSelected(bool value)
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

bool FrameEditPart::isAncestorSelected() const
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

bool FrameEditPart::isMoveSupported() const
{
	return true;
}

bool FrameEditPart::isResizeSupported() const
{
	return true;
}

bool FrameEditPart::isMovePointSupported() const
{
	return false;
}

shared_ptr<ModelElementBase> FrameEditPart::createModelElement()
{
	return shared_ptr< ModelElementBase >(new Frame() );
}

shared_ptr< Figure > FrameEditPart::createFigure()
{
	shared_ptr< GroupFigure > group_figure( new GroupFigure() );
	group_figure->setLayoutManager (shared_ptr< ILayoutManager >( new VLayoutManager() ));
	
	// Title - Index 0
	shared_ptr< LabelFigure > label_figure( new LabelFigure() );
	label_figure->getTextStyle()->setColor (Gdk::Color("black"));
	label_figure->setText (frame_->getTitle());
	
	group_figure->addChild (label_figure, 
		shared_ptr< ILayoutConstraint >( new VLayoutConstraint(VLayoutConstraint::HorizontalExpand, 24, 6, 6, 6, 6)));

	// Line title separator - Index 1
	shared_ptr< LineFigure > line_figure( new LineFigure() );
	line_figure->getStrokeStyle()->setColor (frame_->getLineColor());
	line_figure->getStrokeStyle()->setLineWidth (frame_->getLineWidth());
	line_figure->getStrokeStyle()->setLineDashStyle (frame_->getLineDashStyle());
	line_figure->getStrokeStyle()->setLineDashStyleOffset (frame_->getLineDashStyleOffset());

	group_figure->addChild (line_figure, 
		shared_ptr< ILayoutConstraint >( new VLayoutConstraint(VLayoutConstraint::HorizontalExpand, 0, 0, 0, 0, 0)));
	
	// Client area - Index 2
	shared_ptr< GroupFigure > client_figure( new GroupFigure() );
	client_figure->setLayoutManager (shared_ptr< ILayoutManager >( new XYLayoutManager() ));

	group_figure->addChild (client_figure, 
		shared_ptr< ILayoutConstraint >( new VLayoutConstraint(VLayoutConstraint::HorizontalExpand, VLayoutConstraint::VerticalFill, 0, 0, 0, 20)));

	// Border
	shared_ptr< RoundedRectangleFigure > background_figure( new RoundedRectangleFigure() );
	background_figure->getStrokeStyle()->setColor (frame_->getLineColor());
	background_figure->getStrokeStyle()->setLineWidth (frame_->getLineWidth());
	background_figure->getStrokeStyle()->setLineDashStyle (frame_->getLineDashStyle());
	background_figure->getStrokeStyle()->setLineDashStyleOffset (frame_->getLineDashStyleOffset());
	background_figure->getFillStyle()->setColor (frame_->getFillColor());
		
	group_figure->setBackgroundFigure (background_figure);
	
	group_figure->setBounds (frame_->getBounds());
	
	return group_figure;
}

shared_ptr< Figure > FrameEditPart::getClientFigure()
{
	return client_figure_;
}

void FrameEditPart::onModelTitleChanged()
{
	label_figure_->setText (frame_->getTitle());
}

void FrameEditPart::onModelBoundsChanged()
{
	shared_ptr< IContainerEditPart > parent = parent_.lock();
	
	if (parent)
	{
		if (parent->hasLayoutManager())
			parent->setLayoutConstraint (frame_, frame_->getBounds());
		else
			figure_->setBounds (frame_->getBounds());
	}
	
	updateResizeDragTrackerFigure();
}

void FrameEditPart::onModelChildAdded(shared_ptr< ModelElementBase > element)
{
	addChild (element);
}

void FrameEditPart::onModelChildErased(shared_ptr< ModelElementBase > element)
{
	// Search the edit part child from the source model
	for (iterator child_itor = children_.begin();
		child_itor != children_.end();
		child_itor++)
	{
		if ((*child_itor)->getModel() == element)
		{
			// Erase child view
			client_figure_->eraseChild ((*child_itor)->getFigure());
			
			// Erase child from selection
			if ((*child_itor)->isSelected())
				(*child_itor)->setSelected (false);
				
			// Erase child edit part from children
			children_.erase (child_itor);
			
			return;
		}
	}	
}

shared_ptr< IContainerEditPart > FrameEditPart::findInnerContainer(const Point& point)
{
	shared_ptr< IContainerEditPart > container;
	
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)
	{
		container = (*itor)->findInnerContainer (point);
		if (container)
			return container;
	}	
	
	if (figure_->getBounds().contains (point))
		return shared_from_this();
		
	return container;
}

shared_ptr< ICommand > FrameEditPart::createMoveCommand(int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< MoveFrameCommand >( new MoveFrameCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			frame_, dx, dy) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ResizeFrameCommand >( new ResizeFrameCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			frame_, resize_direction, dx, dy) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ReparentFrameCommand >( new ReparentFrameCommand (
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()), 
			frame_, 
			dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()), 
			new_parent, 
			dx, dy) );
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy)
{
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createInsertModelElementCommand(shared_ptr< ModelElementBase > element)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command and execute it
		shared_ptr< InsertModelElementCommand > cmd( new InsertModelElementCommand( diagram, frame_, element ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createSetLineColorCommand(const Color& color)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command 
		shared_ptr< ICommand > cmd(
			new SetLineColorCommand( diagram, frame_, color ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createSetLineWidthCommand(double width)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());

	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());

		// Create the new command
		shared_ptr< ICommand > cmd(
			new SetLineWidthCommand( diagram, frame_, width ) );

		return cmd;
	}

	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createSetFillColorCommand(const Color& color)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command 
		shared_ptr< ICommand > cmd(
			new SetFillColorCommand( diagram, frame_, color ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > FrameEditPart::createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset)
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< Diagram > diagram = dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel());
		
		// Create the new command 
		shared_ptr< ICommand > cmd(
			new SetDashStyleCommand( diagram, frame_, dash_style, dash_offset ) );
		
		return cmd;
	}
	
	return shared_ptr< ICommand >();	
}

shared_ptr< ICommand > FrameEditPart::createDeleteCommand()
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		return shared_ptr< ICommand >(
			new DeleteCommand(
				dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
				dynamic_pointer_cast< ComplexModelElementBase >(parent_.lock()->getModel()),
				frame_ ) );
	}
	
	return shared_ptr< ICommand >();	
}

void FrameEditPart::clearSelection()
{
	while (!selected_children_.empty())
	{
		// Set only selected to false, the selected child is effectively removed
		// when the selection_remove signal is caught by onChildSelectionRemove
		(*selected_children_.begin())->setSelected (false);
	}
}

void FrameEditPart::selectAll()
{
	for (iterator itor = children_.begin();
		itor != children_.end();
		itor++)
	{
		(*itor)->setSelected (true);

		// If the child is a container invoke selectAll()
		shared_ptr< IContainerEditPart > container = dynamic_pointer_cast< IContainerEditPart >( *itor );
		if (container)
			container->selectAll();
	}
}

shared_ptr< IEditPart > FrameEditPart::getChildAt(int index)
{
	return children_.at (index);
}

bool FrameEditPart::selectFromPoint(const Point& point)
{
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)
	{
		if ((*itor)->selectFromPoint (point))
			return true;
	}
	
	if (figure_->getBackgroundFigure()->isPointIn (point))
	{
		setSelected (true);
		return true;
	}
	
	return false;
}

bool FrameEditPart::selectFromRectangle(const Rectangle& rectangle)
{
	bool res = false;
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, children_)
	{
		res |= edit_part->selectFromRectangle (rectangle);
	}
	
	if (figure_->getBackgroundFigure()->isBoundsOut (rectangle))
	{
		setSelected (true);
		return true;
	}
	
	return res;
}

bool FrameEditPart::toggleSelectionFromPoint(const Point& point)
{
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)
	{
		if ((*itor)->toggleSelectionFromPoint (point))
			return true;
	}

	bool selected = isSelected();

	if (figure_->getBackgroundFigure()->isPointIn (point))
	{
		setSelected (!isSelected());
	}

	return isSelected() != selected;
}

bool FrameEditPart::toggleSelectionFromRectangle(const Rectangle& rectangle)
{
	bool res = false;
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, children_)
	{
		res |= edit_part->toggleSelectionFromRectangle (rectangle);
	}

	bool selected = isSelected();

	if (figure_->getBackgroundFigure()->isBoundsOut (rectangle))
	{
		setSelected (!isSelected());
	}

	return res || (isSelected() != selected);
}

bool FrameEditPart::queryStartMove(const Point& point)
{
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, children_)
	{
		if (edit_part->queryStartMove (point))
		{
			return true;
		}
	}
	
	return figure_->getBackgroundFigure()->isPointIn (point);
}

shared_ptr< IDragTracker > FrameEditPart::queryDragTracker(const Point& point, const KeyModifier& key_modifier)
{
	// 1. point is on a selected child drag tacker
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		shared_ptr< IDragTracker > drag_tracker = edit_part->queryDragTracker (point, key_modifier);
		
		if (drag_tracker)
		{
			return drag_tracker;
		}
	}
	
	// 2. point is on a selected child: start move
	// Selected object are also on the selected_children_ of the parent
	// DiagramEditPart, so this loop never find a DragTracker
	// TODO: remove this loop?
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		if (edit_part->queryStartMove (point))
		{
			return shared_ptr< IDragTracker >(new MoveDragTracker( DiagramEditPart::queryDiagramFromChild(shared_from_this()), edit_part ));
		}
	}
	
	// 3. point is on a not selected child: select child, then start move
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)	
	{
		if ((*itor)->queryStartMove (point))
		{
			shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild(shared_from_this());
			
			if (!key_modifier.isShift())
				diagram_edit_part->clearSelection();

			// Use selectFromPoint because the selected item can be a child of a child
			(*itor)->selectFromPoint (point);
			
			return shared_ptr< IDragTracker >(new MoveDragTracker( diagram_edit_part, *(diagram_edit_part->getSelectedChildrenBegin()) ));
		}
	}
	
	// 4. point is on a frame drag tacker
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
	
	// 5. 
	return shared_ptr< IDragTracker >();
}

void FrameEditPart::paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context)
{
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		edit_part->paintSelectedDragTrackers (context);
	}	
	
	if (selected_)
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
}

shared_ptr< IInplaceEditor > FrameEditPart::queryInplaceEditor(const Point& point)
{
	// Frame caption
	if (selected_ && label_figure_->getOutlineRect().contains (point))
	{
		return shared_ptr< IInplaceEditor >( 
			new TextInplaceEditor( label_figure_ ) );
	}
	
	// Frame child
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)	
	{
		shared_ptr< IInplaceEditor > editor = (*itor)->queryInplaceEditor (point);
		
		if (editor)
		{
			clearSelection();
			
			return editor;
		}
	}	
	
	return shared_ptr< IInplaceEditor >();
}

FrameEditPart::iterator FrameEditPart::getChildrenBegin()
{
	return children_.begin();
}

FrameEditPart::const_iterator FrameEditPart::getChildrenBegin() const
{
	return children_.begin();
}

FrameEditPart::iterator FrameEditPart::getChildrenEnd()
{
	return children_.end();
}

FrameEditPart::const_iterator FrameEditPart::getChildrenEnd() const
{
	return children_.end();
}

FrameEditPart::size_type FrameEditPart::getChildrenSize() const
{
	return children_.size();
}

FrameEditPart::iterator FrameEditPart::getSelectedChildrenBegin()
{
	return selected_children_.begin();
}

FrameEditPart::const_iterator FrameEditPart::getSelectedChildrenBegin() const
{
	return selected_children_.begin();
}

FrameEditPart::iterator FrameEditPart::getSelectedChildrenEnd()
{
	return selected_children_.end();
}

FrameEditPart::const_iterator FrameEditPart::getSelectedChildrenEnd() const
{
	return selected_children_.end();
}

FrameEditPart::size_type FrameEditPart::getSelectedChildrenSize() const
{
	return selected_children_.size();
}

bool FrameEditPart::hasLayoutManager() const
{
	return true;
}

void FrameEditPart::setLayoutConstraint(shared_ptr< ModelElementBase > element, const Rectangle& bounds)
{
	// Search the edit part child from the source model
	for (iterator child_itor = children_.begin();
		child_itor != children_.end();
		child_itor++)
	{
		if ((*child_itor)->getModel() == element)
		{
			shared_ptr< GroupFigure > client_figure = dynamic_pointer_cast< GroupFigure >(getClientFigure());
			shared_ptr< XYLayoutConstraint > xy_constraint = dynamic_pointer_cast< XYLayoutConstraint >(client_figure->getLayoutManager()->getChildConstraint((*child_itor)->getFigure()));
			xy_constraint->setBounds (bounds);
			client_figure->getLayoutManager()->applyConstraints (client_figure->getBounds());
	
			return;
		}
	}
}

FrameEditPart::signal_selection_change_t FrameEditPart::signalSelectionAdd()
{
	return signal_selection_add_;
}

FrameEditPart::signal_selection_change_t FrameEditPart::signalSelectionRemove()
{
	return signal_selection_remove_;
}

void FrameEditPart::onChildSelectionAdd(shared_ptr< IEditPart > edit_part)
{
	// Notify parent
	signal_selection_add_.emit (edit_part);
	
	selected_children_.push_back (edit_part);
}

void FrameEditPart::onChildSelectionRemove(shared_ptr< IEditPart > edit_part)
{
	// Notify parent
	signal_selection_remove_.emit (edit_part);
	
	iterator itor = 
		std::find (selected_children_.begin(), selected_children_.end(), edit_part);
	
	if (itor != selected_children_.end())
	{
		selected_children_.erase (itor);
	}
}

void FrameEditPart::onFigureBoundsChanged()
{
	updateResizeDragTrackerFigure();
}

void FrameEditPart::onLabelTextApplied()
{
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< EditTextCommand > cmd( new EditTextCommand(
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
			frame_,
			label_figure_->getText()) );
		
		CommandManager::getInstance()->execute (cmd);
	}	
}

void FrameEditPart::onFrameLineColorChanged()
{
	figure_->getBackgroundFigure()->getStrokeStyle()->setColor (frame_->getLineColor());
	line_figure_->getStrokeStyle()->setColor (frame_->getLineColor());
}

void FrameEditPart::onFrameFillColorChanged()
{
	figure_->getBackgroundFigure()->getFillStyle()->setColor (frame_->getFillColor());
}

void FrameEditPart::onFrameLineWidthChanged()
{
	figure_->getBackgroundFigure()->getStrokeStyle()->setLineWidth (frame_->getLineWidth());
	line_figure_->getStrokeStyle()->setLineWidth (frame_->getLineWidth());
}

void FrameEditPart::onFrameLineDashStyleChanged()
{
	figure_->getBackgroundFigure()->getStrokeStyle()->setLineDashStyle (frame_->getLineDashStyle());
	line_figure_->getStrokeStyle()->setLineDashStyle (frame_->getLineDashStyle());
}

void FrameEditPart::onFrameLineDashStyleOffsetChanged()
{
	figure_->getBackgroundFigure()->getStrokeStyle()->setLineDashStyleOffset (frame_->getLineDashStyleOffset());
	line_figure_->getStrokeStyle()->setLineDashStyleOffset (frame_->getLineDashStyleOffset());
}
	
void FrameEditPart::fillSelectedShapeList(std::list< shared_ptr< ModelElementBase > >& vector)
{
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		vector.push_back (edit_part->getModel());
	}	
}

void FrameEditPart::addChild(shared_ptr< ModelElementBase > element)
{
	// Add the child edit part
	shared_ptr< IEditPart > edit_part = 
		EditPartFactory::getInstance()->createEditPart (element);
		
	edit_part->setParent (shared_from_this());

	shared_ptr< GroupFigure > client_figure = dynamic_pointer_cast< GroupFigure >(getClientFigure());
	
	// Get index from model element
	int index = -1;

	if (!children_.empty())
	{
		int element_index = frame_->getChildIndex (element);
		index = 0;
		// Search for a child edit part relative to a model element
		// with the index less than the index of given model element
		for (int i = children_.size() - 1; i >= 0; i--)
		{
			if (frame_->getChildIndex (children_[i]->getModel()) < element_index)
			{
				index = i + 1;
				break;
			}
		}

	}

	if (index == -1 || index >= children_.size())
	{
		children_.push_back (edit_part);

		// Add the child view
		client_figure->addChild (edit_part->getFigure(),
			shared_ptr< ILayoutConstraint >( new XYLayoutConstraint(edit_part->getFigure()->getOutlineRect())));
	}
	else
	{
		shared_ptr< IEditPart > old_child_at_pos = children_[index];
		
		children_.insert (children_.begin() + index, edit_part);

		// Add the child view before the next view
		int figure_index = client_figure->getChildIndex (old_child_at_pos->getFigure());
		client_figure->insertChild (edit_part->getFigure(),
			shared_ptr< ILayoutConstraint >( new XYLayoutConstraint(edit_part->getFigure()->getOutlineRect())),
			figure_index);
	}
	
	// TODO: implement applyConstraints() on Figure
	client_figure->getLayoutManager()->applyConstraints (client_figure->getBounds());
	
	edit_part->signalSelectionAdd().connect (
		sigc::mem_fun(this, &FrameEditPart::onChildSelectionAdd));
		
	edit_part->signalSelectionRemove().connect (
		sigc::mem_fun(this, &FrameEditPart::onChildSelectionRemove));	
}

shared_ptr< DragTrackerFigure > FrameEditPart::createResizeDragTrackerFigure()
{
	shared_ptr< DragTrackerFigure > resize_figure = shared_ptr< DragTrackerFigure >( new DragTrackerFigure() );
	resize_figure->getFillStyle()->setColor (Color("white"));
	resize_figure->getStrokeStyle()->setColor (Color("black"));
	
	return resize_figure;
}

void FrameEditPart::updateResizeDragTrackerFigure()
{
	Rectangle bounds = figure_->getBounds();
	
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


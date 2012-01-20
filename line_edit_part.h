#ifndef LINEEDITPART_H
#define LINEEDITPART_H

#include <i_edit_part.h> // Base class: cppgef::IEditPart

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>

#include <line_base.h>
#include <line_figure.h>

#include <drag_tracker_figure.h>

#include <signal_disconnect.h>
#include <idragtracker.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using boost::weak_ptr;
using std::auto_ptr;

namespace cppgef
{

class LineEditPart : public cppgef::IEditPart, 
					 public boost::enable_shared_from_this< LineEditPart >
{

public:
	LineEditPart();
	virtual ~LineEditPart();

public:
	virtual shared_ptr< Figure > createFigure();
	virtual shared_ptr< ModelElementBase > createModelElement();
	
	virtual shared_ptr< ICommand > createMoveCommand(int dx, int dy);
	virtual shared_ptr< ICommand > createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy);
	virtual shared_ptr< ICommand > createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual shared_ptr< ICommand > createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy);
	virtual shared_ptr< ICommand > createSetLineColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetLineWidthCommand(double width);
	virtual shared_ptr< ICommand > createSetFillColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset);	
	virtual shared_ptr< ICommand > createDeleteCommand();
	
	virtual shared_ptr< IContainerEditPart > findInnerContainer(const Point& point);
	
	virtual shared_ptr< Figure > getFigure();
	virtual void setFigure(shared_ptr< Figure > value);
	
	virtual shared_ptr< ModelElementBase > getModel();
	virtual void setModel(shared_ptr< ModelElementBase > value);
	
	virtual weak_ptr< IContainerEditPart > getParent();
	virtual void setParent(weak_ptr< IContainerEditPart > parent);
	
	virtual bool isSelected() const;
	virtual void setSelected(bool value);

	virtual bool isAncestorSelected() const;
	
	bool isMoveSupported() const;
	bool isResizeSupported() const;
	bool isMovePointSupported() const;
	
	virtual void paintSelectedDragTrackers(Cairo::RefPtr<Cairo::Context> context);
	
	virtual shared_ptr<IDragTracker> queryDragTracker(const Point& point);
	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point);
	
	virtual bool queryStartMove(const Point& point);
	
	virtual bool selectFromPoint(const Point& point);
	virtual bool selectFromRectangle(const Rectangle& rectangle);
	
	virtual signal_selection_change_t signalSelectionAdd();
	virtual signal_selection_change_t signalSelectionRemove();
	
protected:
	shared_ptr< LineBase > simple_line_;
	bool selected_;
	weak_ptr< IContainerEditPart > parent_;
	shared_ptr< LineFigure > simple_line_figure_;
	
	shared_ptr< DragTrackerFigure > move_start_point_figure_;
	shared_ptr< DragTrackerFigure > move_end_point_figure_;
	
	signal_selection_change_t signal_selection_add_;
	signal_selection_change_t signal_selection_remove_;
	
	auto_ptr< SignalDisconnect > line_start_point_changed_disconnect;
	auto_ptr< SignalDisconnect > line_end_point_changed_disconnect;
	auto_ptr< SignalDisconnect > line_color_changed_disconnect;
	auto_ptr< SignalDisconnect > line_width_changed_disconnect;
	auto_ptr< SignalDisconnect > line_dash_style_changed_disconnect;
	auto_ptr< SignalDisconnect > line_dash_style_offset_changed_disconnect;
	
protected:
	shared_ptr< DragTrackerFigure > createMoveDragTrackerFigure();
	void updateMoveDragTrackerFigure();
	
	void onLineStartPointChanged();
	void onLineEndPointChanged();
	void onLineColorChanged();
	void onLineWidthChanged();
	void onLineDashStyleChanged();
	void onLineDashStyleOffsetChanged();
	
	void onFigureStartPointChanged();
	void onFigureEndPointChanged();
};

}

#endif // LINEEDITPART_H

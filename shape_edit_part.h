#ifndef SHAPEEDITPART_H
#define SHAPEEDITPART_H

#include <i_edit_part.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>

#include <shape_base.h> // Base model

#include <drag_tracker_figure.h>

#include <signal_disconnect.h>
#include <idragtracker.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using boost::weak_ptr;
using std::auto_ptr;

namespace cppgef
{

// TODO: rename to ShapeEditPartBase
class ShapeEditPart : public IEditPart, 
					  public boost::enable_shared_from_this< ShapeEditPart >
{
public:
	ShapeEditPart();
	virtual ~ShapeEditPart();

	shared_ptr< ModelElementBase > getModel();
	void setModel(shared_ptr< ModelElementBase > model_element);

	weak_ptr< IContainerEditPart > getParent();
	void setParent(weak_ptr< IContainerEditPart > parent);
	
	bool isSelected() const;
	void setSelected(bool value);
	
	bool isAncestorSelected() const;
	
	bool isMoveSupported() const;
	bool isResizeSupported() const;
	bool isMovePointSupported() const;
	
	bool selectFromPoint(const Point& point);
	bool selectFromRectangle(const Rectangle& rectangle);
	
	bool queryStartMove(const Point& point);
	
	void paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context);
	shared_ptr< IDragTracker > queryDragTracker(const Point& point);
	
	shared_ptr< IContainerEditPart > findInnerContainer(const Point& point);
	
	virtual shared_ptr< ICommand > createMoveCommand(int dx, int dy);
	virtual shared_ptr< ICommand > createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy);
	virtual shared_ptr< ICommand > createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual shared_ptr< ICommand > createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy);
	virtual shared_ptr< ICommand > createSetLineColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetLineWidthCommand(double width);
	virtual shared_ptr< ICommand > createSetFillColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset);	
	virtual shared_ptr< ICommand > createDeleteCommand();
	
	signal_selection_change_t signalSelectionAdd();
	signal_selection_change_t signalSelectionRemove();
	
protected:
	shared_ptr< ShapeBase > shape_;
	bool selected_;
	weak_ptr< IContainerEditPart > parent_;
	
	shared_ptr< DragTrackerFigure > resize_top_left_figure_;
	shared_ptr< DragTrackerFigure > resize_top_figure_;
	shared_ptr< DragTrackerFigure > resize_top_right_figure_;
	shared_ptr< DragTrackerFigure > resize_right_figure_;
	shared_ptr< DragTrackerFigure > resize_bottom_right_figure_;
	shared_ptr< DragTrackerFigure > resize_bottom_figure_;
	shared_ptr< DragTrackerFigure > resize_bottom_left_figure_;
	shared_ptr< DragTrackerFigure > resize_left_figure_;
	
	signal_selection_change_t signal_selection_add_;
	signal_selection_change_t signal_selection_remove_;
	
	auto_ptr< SignalDisconnect > shape_bounds_changed_disconnect;
	auto_ptr< SignalDisconnect > line_color_changed_disconnect;
	auto_ptr< SignalDisconnect > line_width_changed_disconnect;
	auto_ptr< SignalDisconnect > line_dash_style_changed_disconnect;
	auto_ptr< SignalDisconnect > line_dash_style_offset_changed_disconnect;
	auto_ptr< SignalDisconnect > fill_color_changed_disconnect;
	
protected:
	void onShapeBoundsChanged();
	void onShapeLineColorChanged();
	void onShapeLineWidthChanged();
	void onShapeLineDashStyleChanged();
	void onShapeLineDashStyleOffsetChanged();
	void onShapeFillColorChanged();
	
	shared_ptr< DragTrackerFigure > createResizeDragTrackerFigure();
	void updateResizeDragTrackerFigure();
};

}

#endif // SHAPEEDITPART_H

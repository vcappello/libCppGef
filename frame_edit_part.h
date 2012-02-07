#ifndef FRAMEEDITPART_H
#define FRAMEEDITPART_H

#include <i_container_edit_part.h> // Base class: cppgef::IContainerEditPart

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <list>
#include <memory>

#include <frame.h> // Model
#include <group_figure.h> // View
#include <rectangle_figure.h> 
#include <label_figure.h> 
#include <line_figure.h>

#include <signal_disconnect.h>
#include <edit_part_factory.h>
#include <model_element_base.h>
#include <drag_tracker_figure.h>
#include <idragtracker.h>
#include <settings.h>
#include <text_inplace_editor.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using std::auto_ptr;

namespace cppgef
{

class FrameEditPart : public cppgef::IContainerEditPart, 
					  public boost::enable_shared_from_this< FrameEditPart >
{
public:
	FrameEditPart();
	virtual ~FrameEditPart();

	weak_ptr< IContainerEditPart > getParent();
	void setParent(weak_ptr< IContainerEditPart > parent);

	shared_ptr< ModelElementBase > getModel();
	void setModel(shared_ptr< ModelElementBase > value);
	
	shared_ptr< Figure > getFigure();
	void setFigure(shared_ptr< Figure > value) ;
	
	bool isSelected() const;
	void setSelected(bool value);
	
	bool isAncestorSelected() const;
	
	bool isMoveSupported() const;
	bool isResizeSupported() const;
	bool isMovePointSupported() const;
	
	shared_ptr< ModelElementBase > createModelElement();
	
	shared_ptr< Figure > createFigure();

	shared_ptr< Figure > getClientFigure();
	
	shared_ptr< IContainerEditPart > findInnerContainer(const Point& point);

	virtual shared_ptr< ICommand > createMoveCommand(int dx, int dy);
	virtual shared_ptr< ICommand > createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy);
	virtual shared_ptr< ICommand > createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual shared_ptr< ICommand > createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy);
	virtual shared_ptr< ICommand > createInsertModelElementCommand(shared_ptr< ModelElementBase > element);
	virtual shared_ptr< ICommand > createSetLineColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetLineWidthCommand(double width);
	virtual shared_ptr< ICommand > createSetFillColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset);	
	virtual shared_ptr< ICommand > createDeleteCommand();
	
	virtual bool selectFromPoint(const Point& point);
	virtual bool selectFromRectangle(const Rectangle& rectangle);

	virtual bool toggleSelectionFromPoint(const Point& point);
	virtual bool toggleSelectionFromRectangle(const Rectangle& rectangle);
	
	bool queryStartMove(const Point& point);
	
	shared_ptr< IDragTracker > queryDragTracker(const Point& point, const KeyModifier& key_modifier);
	void paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context);
	
	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point);
	
	virtual void clearSelection();
	
	/**
	 * @brief Select all children
	 */
	virtual void selectAll();

	virtual shared_ptr< IEditPart > getChildAt(int index);
	
	virtual iterator getChildrenBegin();
	virtual const_iterator getChildrenBegin() const;
	virtual iterator getChildrenEnd();
	virtual const_iterator getChildrenEnd() const;
	
	virtual size_type getChildrenSize() const;
	
	virtual iterator getSelectedChildrenBegin();
	virtual const_iterator getSelectedChildrenBegin() const;
	virtual iterator getSelectedChildrenEnd();
	virtual const_iterator getSelectedChildrenEnd() const;
	
	virtual size_type getSelectedChildrenSize() const;

	virtual bool hasLayoutManager() const;
	virtual void setLayoutConstraint(shared_ptr< ModelElementBase > element, const Rectangle& bounds);

	signal_selection_change_t signalSelectionAdd();
	signal_selection_change_t signalSelectionRemove();
	
protected:
	shared_ptr< Frame > frame_;
	shared_ptr< GroupFigure > figure_;
	shared_ptr< LabelFigure > label_figure_;
	shared_ptr< LineFigure > line_figure_;
	shared_ptr< GroupFigure > client_figure_;
	weak_ptr< IContainerEditPart > parent_;
	bool selected_;
	
	shared_ptr< DragTrackerFigure > resize_top_left_figure_;
	shared_ptr< DragTrackerFigure > resize_top_figure_;
	shared_ptr< DragTrackerFigure > resize_top_right_figure_;
	shared_ptr< DragTrackerFigure > resize_right_figure_;
	shared_ptr< DragTrackerFigure > resize_bottom_right_figure_;
	shared_ptr< DragTrackerFigure > resize_bottom_figure_;
	shared_ptr< DragTrackerFigure > resize_bottom_left_figure_;
	shared_ptr< DragTrackerFigure > resize_left_figure_;
	
	children_container_t children_;
	children_container_t selected_children_;
	
	auto_ptr< SignalDisconnect > title_changed_disconnect;
	auto_ptr< SignalDisconnect > model_bounds_changed_disconnect;
	auto_ptr< SignalDisconnect > child_added_disconnect;
	auto_ptr< SignalDisconnect > child_erased_disconnect;
	auto_ptr< SignalDisconnect > line_color_changed_disconnect;
	auto_ptr< SignalDisconnect > line_width_changed_disconnect;
	auto_ptr< SignalDisconnect > line_dash_style_changed_disconnect;
	auto_ptr< SignalDisconnect > line_dash_style_offset_changed_disconnect;
	auto_ptr< SignalDisconnect > fill_color_changed_disconnect;
	
	signal_selection_change_t signal_selection_add_;
	signal_selection_change_t signal_selection_remove_;
	
protected:
	void onModelTitleChanged();
	void onModelBoundsChanged();
	void onModelChildAdded(shared_ptr< ModelElementBase > element);
	void onModelChildErased(shared_ptr< ModelElementBase > element);
	
	void onChildSelectionAdd(shared_ptr< IEditPart > shape_edit_part);
	void onChildSelectionRemove(shared_ptr< IEditPart > shape_edit_part);
	
	void onFigureBoundsChanged();
	void onLabelTextApplied();
	
	void onFrameLineColorChanged();
	void onFrameLineWidthChanged();
	void onFrameLineDashStyleChanged();
	void onFrameLineDashStyleOffsetChanged();
	void onFrameFillColorChanged();
	
	void fillSelectedShapeList(std::list< shared_ptr< ModelElementBase > >& vector);
	void addChild(shared_ptr< ModelElementBase > element);
	
	shared_ptr< DragTrackerFigure > createResizeDragTrackerFigure();
	void updateResizeDragTrackerFigure();
};

}

#endif // FRAMEEDITPART_H

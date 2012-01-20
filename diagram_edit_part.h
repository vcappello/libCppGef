#ifndef DIAGRAMEDITPART_H
#define DIAGRAMEDITPART_H

#include <i_container_edit_part.h> // Base class

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <list>
#include <memory>

#include <diagram.h> // Model
#include <rectangle_figure.h> // View

#include <signal_disconnect.h>
#include <model_element_base.h>
#include <sort_vector.h>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;
using std::auto_ptr;

namespace cppgef
{

/**
 * @class DiagramEditPart
 * @author vincenzo cappello
 * @date 05/12/2010
 * @file diagram_edit_part.h
 * 
 * @brief Controller for Diagram object
 */
class DiagramEditPart : public IContainerEditPart, 
						public boost::enable_shared_from_this< DiagramEditPart >
{
public:
	DiagramEditPart();
	virtual ~DiagramEditPart();

	/** DiagramEditPart is the root EditPart, getParent() return null */
	weak_ptr< IContainerEditPart > getParent();
	
	/** DiagramEditPart is the root EditPart, this function do nothing */
	void setParent(weak_ptr< IContainerEditPart > parent);

	/** @copydoc IEditPart::getModel() */
	shared_ptr< ModelElementBase > getModel();
	
	/** @copydoc IEditPart::setModel() */
	void setModel(shared_ptr< ModelElementBase > value);
	
	/** @copydoc IEditPart::getFigure() */
	shared_ptr< Figure > getFigure();
	
	/** @copydoc IEditPart::setFigure() */
	void setFigure(shared_ptr< Figure > value) ;
	
	/** @copydoc IEditPart::isSelected() */
	bool isSelected() const;
	
	/** @copydoc IEditPart::setSelected() */
	void setSelected(bool value);
	
	bool isAncestorSelected() const;
	
	bool isMoveSupported() const;
	bool isResizeSupported() const;
	bool isMovePointSupported() const;
	
	/**
	 * @brief Create the Diagram model element
	 * @return a new Diagram
	 */
	shared_ptr< ModelElementBase > createModelElement();
	
	/**
	 * @brief Create the Diagram view
	 * @return the view, a white rectangle
	 */
	shared_ptr< Figure > createFigure();

	shared_ptr< Figure > getClientFigure();
	
	/**
	 * @brief Find the innser container at given point
	 * 
	 * Search on all children container the inner container at given point.
	 * If a container exist, the function return this object.
	 * If no container was found the function return this DiagramEditPart.
	 * 
	 * @param point point to user for search
	 * @return the inner container
	 */
	shared_ptr< IContainerEditPart > findInnerContainer(const Point& point);
	
	/**
	 * @brief Create the ICommand for move selected children
	 * @param dx horizontal location increment
	 * @param dy vertical location increment
	 * @return the ICommand
	 */
	shared_ptr< ICommand > createMoveCommand(int dx, int dy);
	
	/**
	 * @brief Create the ICommand for resize selected children
	 * @param resize_direction the resize direction, see AbstractResizePolicy::ResizeDirection
	 * @param dx horizontal size increment
	 * @param dy vertical size increment
	 * @return the ICommand
	 */
	shared_ptr< ICommand > createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy);
	
	virtual shared_ptr< ICommand > createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual shared_ptr< ICommand > createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy);
	virtual shared_ptr< ICommand > createInsertModelElementCommand(shared_ptr< ModelElementBase > element);
	virtual shared_ptr< ICommand > createSetLineColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetLineWidthCommand(double width);
	virtual shared_ptr< ICommand > createSetFillColorCommand(const Color& color);
	virtual shared_ptr< ICommand > createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset);	
	virtual shared_ptr< ICommand > createDeleteCommand();
	
	/**
	 * @brief Add the object at given point to the selection
	 * 
	 * If an object was found it is added to the selection set.
	 * Only one object is selected, if more than one object share the
	 * same point, the selected object is the top-most
	 * 
	 * @param point location used for select objects
	 * 
	 * @return true if an object was found
	 */
	bool selectFromPoint(const Point& point);
	
	/**
	 * @brief Add all objects contained in the given rectangle to the selection
	 * 
	 * All found objects are added to the selection set.
	 * 
	 * @param rectangle rectangle used for select objects
	 * 
	 * @return true if at least one object was found
	 */
	bool selectFromRectangle(const Rectangle& rectangle);
	
	/**
	 * @brief Can not move a diagram
	 * @return always false
	 */
	bool queryStartMove(const Point& point);
	
	shared_ptr< IDragTracker > queryDragTracker(const Point& point);
	void paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context);
	
	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point);
	
	/**
	 * @brief Remove all objects from selection
	 */
	virtual void clearSelection();
	
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
	
	/**
	 * @brief Load a diagram from file
	 * @param file_name the file name
	 * @return true on success
	 */
	bool open(const std::string& file_name);

	/**
	 * @brief Save the Diagram
	 * @param file_name the file name to use for save
	 * @return true if the file was successfully saved
	 */
	bool save(const std::string& file_name);
	
	void querySelectedChildrenNoChild(children_container_t& container);

	bool isSelectNewChild() const;
	void setSelectNewChild(bool value);
	
public:	
	static shared_ptr< DiagramEditPart > queryDiagramFromChild(shared_ptr< IEditPart > child);
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< RectangleFigure > rectangle_figure_;
	bool selected_;
	bool select_new_child_;
	
	children_container_t children_;
	children_container_t selected_children_;
	
	auto_ptr< SignalDisconnect > title_changed_disconnect;
	auto_ptr< SignalDisconnect > child_added_disconnect;
	auto_ptr< SignalDisconnect > child_erased_disconnect;
	
	signal_selection_change_t signal_selection_add_;
	signal_selection_change_t signal_selection_remove_;
	
protected:
	void onModelTitleChanged();
	void onModelChildAdded(shared_ptr< ModelElementBase > element);
	void onModelChildErased(shared_ptr< ModelElementBase > element);
	
	void onChildSelectionAdd(shared_ptr< IEditPart > shape_edit_part);
	void onChildSelectionRemove(shared_ptr< IEditPart > shape_edit_part);
	
	void fillSelectedShapeList(std::list< shared_ptr< ModelElementBase > >& vector);
	void addChild(shared_ptr< ModelElementBase > element);
};

}

#endif // DIAGRAMEDITPART_H

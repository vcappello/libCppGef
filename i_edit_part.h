#ifndef IEDITPART_H
#define IEDITPART_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include <model_element_base.h>
#include <figure.h>
#include <idragtracker.h>
#include <resize_policy.h>
#include <move_policy.h>
#include <i_command.h>
#include <i_inplace_editor.h>
#include <color.h>

using boost::shared_ptr;

namespace cppgef
{

class IContainerEditPart; // Forward declaration

/**
 * @class EditPart
 * @author vincenzo cappello
 * @date 05/12/2010
 * @file edit_part.h
 * 
 * @brief Interface for edit part, the controller
 */
class IEditPart
{
public:
	typedef sigc::signal< void, shared_ptr< IEditPart > > signal_selection_change_t;

public:
	IEditPart() {}
	virtual ~IEditPart() {}

	/**
	 * @brief The IContainerEditPart that containes this EditPart
	 * 
	 * @return The parent or null for the root EditPart
	 */
	virtual weak_ptr< IContainerEditPart > getParent() = 0;
	
	/**
	 * @brief The IContainerEditPart that containes this EditPart
	 * 
	 * @param parent the parent EditPart
	 */
	virtual void setParent(weak_ptr< IContainerEditPart > parent) = 0;
	
	/**
	 * @brief Return the model referenced by the EditPart
	 * @return the model element
	 */
	virtual shared_ptr< ModelElementBase > getModel() = 0;
	
	/**
	 * @brief Set the model to reference
	 * @param value the model element
	 */
	virtual void setModel(shared_ptr< ModelElementBase > value) = 0;
	
	/**
	 * @brief Return the view referenced by the EditPart
	 * @return the view
	 */	
	virtual shared_ptr< Figure > getFigure() = 0;
	
	/**
	 * @brief Return the view to reference
	 * @param value the view
	 */	
	virtual void setFigure(shared_ptr< Figure > value) = 0;
	
	/**
	 * @brief Create a new model element for the EditPart
	 * @return the new model element instance
	 */
	virtual shared_ptr< ModelElementBase > createModelElement() = 0;
	
	/**
	 * @brief Create a new figure for the EditPart
	 * @return the new figure
	 */
	virtual shared_ptr< Figure > createFigure() = 0;
	
	virtual bool isSelected() const = 0;
	virtual void setSelected(bool value) = 0;
	
	virtual bool isAncestorSelected() const = 0;
	
	virtual bool isMoveSupported() const = 0;
	virtual bool isResizeSupported() const = 0;
	virtual bool isMovePointSupported() const = 0;
	
	virtual bool selectFromPoint(const Point& point) = 0;
	virtual bool selectFromRectangle(const Rectangle& rectangle) = 0;
	
	virtual bool queryStartMove(const Point& point) = 0;
	
	virtual shared_ptr< IDragTracker > queryDragTracker(const Point& point) = 0;
	virtual void paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context) = 0;
	
	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point) = 0;
	
	virtual shared_ptr< IContainerEditPart > findInnerContainer(const Point& point) = 0;
	
	virtual shared_ptr< ICommand > createMoveCommand(int dx, int dy) = 0;
	virtual shared_ptr< ICommand > createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy) = 0;
	virtual shared_ptr< ICommand > createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy) = 0;
	virtual shared_ptr< ICommand > createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy) = 0;
	virtual shared_ptr< ICommand > createSetLineColorCommand(const Color& color) = 0;
	virtual shared_ptr< ICommand > createSetLineWidthCommand(double color) = 0;
	virtual shared_ptr< ICommand > createSetFillColorCommand(const Color& color) = 0;
	virtual shared_ptr< ICommand > createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset) = 0;
	virtual shared_ptr< ICommand > createDeleteCommand() = 0;
	
	virtual signal_selection_change_t signalSelectionAdd() = 0;
	virtual signal_selection_change_t signalSelectionRemove() = 0;
};

}

#endif // IEDITPART_H

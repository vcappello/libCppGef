#include "diagram_edit_part.h"

#include <boost/foreach.hpp>
#include <algorithm>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <glibmm/i18n.h>

#include <command_manager.h>
#include <save_diagram_command.h>
#include <insert_model_element_command.h>
#include <group_command.h>
#include <selection_drag_tracker.h>
#include <move_drag_tracker.h>
#include <edit_part_factory.h>

namespace cppgef
{

DiagramEditPart::DiagramEditPart() :
	selected_( false ),
	select_new_child_( false )
{
}

DiagramEditPart::~DiagramEditPart()
{
}

weak_ptr< IContainerEditPart > DiagramEditPart::getParent()
{
	return weak_ptr< IContainerEditPart >();
}

void DiagramEditPart::setParent(weak_ptr< IContainerEditPart > parent)
{
	// Diagram has no parent
}

shared_ptr< ModelElementBase > DiagramEditPart::getModel()
{
	return diagram_;
}
	
void DiagramEditPart::setModel(shared_ptr< ModelElementBase > value)
{
	diagram_ = dynamic_pointer_cast< Diagram >(value);
	
	// Connect model signals
	title_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		diagram_->signalTitleChanged().connect (
			sigc::mem_fun( this, &DiagramEditPart::onModelTitleChanged )) ) );
			
	child_added_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		diagram_->signalChildAdded().connect (
			sigc::mem_fun( this, &DiagramEditPart::onModelChildAdded )) ) );
			
	child_erased_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		diagram_->signalChildErased().connect (
			sigc::mem_fun( this, &DiagramEditPart::onModelChildErased )) ) );
	
	// Initialize view
	rectangle_figure_ = dynamic_pointer_cast< RectangleFigure >( createFigure() );
	
	// Create children EditPart from Model children
	for (Diagram::iterator shape = diagram_->getChildrenBegin();
		shape != diagram_->getChildrenEnd();
		shape++)
	{
		addChild (*shape);
	}
}

shared_ptr<Figure> DiagramEditPart::getFigure()
{
	return rectangle_figure_;
}

void DiagramEditPart::setFigure(shared_ptr<Figure> value)
{
	rectangle_figure_ = dynamic_pointer_cast< RectangleFigure >(value);
}

bool DiagramEditPart::isSelected() const
{
	return selected_;
}

void DiagramEditPart::setSelected(bool value)
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

bool DiagramEditPart::isAncestorSelected() const
{
	return false;
}

bool DiagramEditPart::isMoveSupported() const
{
	return false;
}

bool DiagramEditPart::isResizeSupported() const
{
	return false;
}

bool DiagramEditPart::isMovePointSupported() const
{
	return false;
}

shared_ptr<ModelElementBase> DiagramEditPart::createModelElement()
{
	return shared_ptr< ModelElementBase >(new Diagram() );
}

shared_ptr< Figure > DiagramEditPart::createFigure()
{
	shared_ptr< RectangleFigure > rectangle_figure( new RectangleFigure() );
	rectangle_figure->getFillStyle()->setColor (Color("white"));
	rectangle_figure->getStrokeStyle()->setColor (Color("white"));
	
	return rectangle_figure;
}

shared_ptr< Figure > DiagramEditPart::getClientFigure()
{
	return rectangle_figure_;
}

bool DiagramEditPart::open(const std::string& file_name)
{
	std::ifstream ifs(file_name.c_str());
	
	if (ifs.good())
	{
		boost::archive::text_iarchive ia(ifs);
		Diagram* diagram = new Diagram();
		ia >> *diagram;

		setModel (shared_ptr< Diagram >( diagram ));

		return true;
	}
	else
	{
		throw Error(
			Glib::ustring::compose(_("Fail to access file '%1'"), file_name));
	}
	
	return false;	
}

bool DiagramEditPart::save(const std::string& file_name)
{
	shared_ptr< SaveDiagramCommand > cmd( new SaveDiagramCommand(diagram_, file_name) );
	bool res = CommandManager::getInstance()->execute (cmd);
	
	return res;
}

void DiagramEditPart::querySelectedChildrenNoChild(children_container_t& container)
{
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		if (std::find (
			selected_children_.begin(),
			selected_children_.end(),
			edit_part->getParent().lock()) == selected_children_.end())
		{
			container.push_back (edit_part);
		}
	}	
}

bool DiagramEditPart::isSelectNewChild() const
{
	return select_new_child_;
}

void DiagramEditPart::setSelectNewChild(bool value)
{
	select_new_child_ = value;
}

void DiagramEditPart::onModelTitleChanged()
{
	//TODO: do something!
}

void DiagramEditPart::onModelChildAdded(shared_ptr< ModelElementBase > element)
{
	addChild (element);
}

void DiagramEditPart::onModelChildErased(shared_ptr< ModelElementBase > element)
{
	// Search the edit part child from the source model
	for (iterator child_itor = children_.begin();
		child_itor != children_.end();
		child_itor++)
	{
		if ((*child_itor)->getModel() == element)
		{
			// Erase child from selection
			if ((*child_itor)->isSelected())
				(*child_itor)->setSelected (false);

			// Erase child view
			rectangle_figure_->eraseChild ((*child_itor)->getFigure());
			
			// Erase child edit part from children
			children_.erase (child_itor);
			
			return;
		}
	}	
}

bool DiagramEditPart::hasLayoutManager() const
{
	return false;
}

void DiagramEditPart::setLayoutConstraint(shared_ptr< ModelElementBase > element, const Rectangle& bounds)
{
	
}

shared_ptr< IContainerEditPart > DiagramEditPart::findInnerContainer(const Point& point)
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
	
	return shared_from_this();
}

shared_ptr< ICommand > DiagramEditPart::createMoveCommand(int dx, int dy)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createMoveCommand (dx, dy);
		
		if (cmd)
			commands.push_back (cmd);
	}
	
	return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );	
}

shared_ptr< ICommand > DiagramEditPart::createResizeCommand(AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createResizeCommand (resize_direction, dx, dy);
		
		if (cmd)
			commands.push_back (cmd);
	}

	return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );
}

shared_ptr< ICommand > DiagramEditPart::createReparentCommand(shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createReparentCommand (new_parent, dx, dy);
		
		if (cmd)
			commands.push_back (cmd);
	}

	return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );
}

shared_ptr< ICommand > DiagramEditPart::createMovePointCommand(IMovePointPolicy::PointSequence sequence, int dx, int dy)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createMovePointCommand (sequence, dx, dy);
		
		if (cmd)
			commands.push_back (cmd);
	}

	return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );	
}

shared_ptr< ICommand > DiagramEditPart::createInsertModelElementCommand(shared_ptr< ModelElementBase > element)
{
	// Create the new command and execute it
	shared_ptr< InsertModelElementCommand > cmd( new InsertModelElementCommand( diagram_, element ) );
	
	return cmd;
}

shared_ptr< ICommand > DiagramEditPart::createSetLineColorCommand(const Color& color)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createSetLineColorCommand (color);
		
		if (cmd)
			commands.push_back (cmd);
	}

	if (commands.size() > 0)
		return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );	
		
	return shared_ptr< ICommand >(); 
}

shared_ptr< ICommand > DiagramEditPart::createSetLineWidthCommand(double width)
{
	std::list< shared_ptr< ICommand > > commands;

	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createSetLineWidthCommand (width);

		if (cmd)
			commands.push_back (cmd);
	}

	if (commands.size() > 0)
		return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );

	return shared_ptr< ICommand >();
}

shared_ptr< ICommand > DiagramEditPart::createSetFillColorCommand(const Color& color)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createSetFillColorCommand (color);
		
		if (cmd)
			commands.push_back (cmd);
	}

	if (commands.size() > 0)
		return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );	
		
	return shared_ptr< ICommand >(); 
}

shared_ptr< ICommand > DiagramEditPart::createSetDashStyleCommand(const std::vector< double > dash_style, double dash_offset)
{
	std::list< shared_ptr< ICommand > > commands;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		shared_ptr< ICommand > cmd = child->createSetDashStyleCommand (dash_style, dash_offset);
		
		if (cmd)
			commands.push_back (cmd);
	}

	if (commands.size() > 0)
		return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );	
		
	return shared_ptr< ICommand >(); 	
}

shared_ptr< ICommand > DiagramEditPart::createDeleteCommand()
{
	std::list< shared_ptr< ICommand > > commands;

	// Create a vector of selected items sorted by index
	SortVector< shared_ptr< IEditPart >, int > sorted_selection;
	
	BOOST_FOREACH(shared_ptr< IEditPart > child, selected_children_)
	{
		// If the child parent is selected do not add child
		if (std::find (
			selected_children_.begin(),
			selected_children_.end(),
			child->getParent().lock()) == selected_children_.end())
		{		
			SortItem< shared_ptr< IEditPart >, int > new_item;
			new_item.item = child;
			new_item.key = diagram_->getChildIndex (child->getModel());
			
			sorted_selection.items.push_back (new_item);
		}
	}	
	
	// Sort selection by index
	sorted_selection.sort();
	
	// Create delete commands
	for (size_t i = 0; i < sorted_selection.items.size(); i++)
	{
		shared_ptr< ICommand > cmd = sorted_selection.items[i].item->createDeleteCommand();
		
		if (cmd)
			commands.push_back (cmd);
	}
	
	if (commands.size() > 0)
	{
		clearSelection();		
		
		return shared_ptr< GroupCommand >(new GroupCommand (diagram_, commands) );	
	}
	
	return shared_ptr< ICommand >(); 
}

void DiagramEditPart::clearSelection()
{
	while (!selected_children_.empty())
	{
		// Set only selected to false, the selected child is effectively removed
		// when the selection_remove signal is caught by onChildSelectionRemove
		(*selected_children_.begin())->setSelected (false);
	}
}

void DiagramEditPart::selectAll()
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

shared_ptr< IEditPart > DiagramEditPart::getChildAt(int index)
{
	return children_.at (index);
}

bool DiagramEditPart::selectFromPoint(const Point& point)
{
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)
	{
		if ((*itor)->selectFromPoint (point))
			return true;
	}
	
	return false;
}

bool DiagramEditPart::selectFromRectangle(const Rectangle& rectangle)
{
	bool res = false;
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, children_)
	{
		res |= edit_part->selectFromRectangle (rectangle);
	}
	
	return res;
}

bool DiagramEditPart::queryStartMove(const Point& point)
{
	return false;
}

shared_ptr< IDragTracker > DiagramEditPart::queryDragTracker(const Point& point)
{
	// 1. point is on a selected child drag tacker
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		shared_ptr< IDragTracker > drag_tracker = edit_part->queryDragTracker (point);
		
		if (drag_tracker)
		{
			return drag_tracker;
		}
	}
	
	// 2. point is on a selected child: start move
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		if (edit_part->queryStartMove (point))
		{
			return shared_ptr< IDragTracker >(new MoveDragTracker( shared_from_this(), edit_part ));
		}
	}
	
	// 3. point is on a not selected child: select child - start move
	for (reverse_iterator itor = children_.rbegin();
		itor != children_.rend();
		itor++)	
	{
		if ((*itor)->queryStartMove (point))
		{
			clearSelection();
			
			// Use selectFromPoint because the selected item can be a child of a child
			(*itor)->selectFromPoint (point);
			
			return shared_ptr< IDragTracker >(new MoveDragTracker( shared_from_this(), *(selected_children_.begin()) ));
		}
	}
	
	// 4. else start new selection
	return shared_ptr< IDragTracker >(new SelectionDragTracker( shared_from_this() ));
}

void DiagramEditPart::paintSelectedDragTrackers(Cairo::RefPtr< Cairo::Context > context)
{
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		edit_part->paintSelectedDragTrackers (context);
	}	
}

shared_ptr< IInplaceEditor > DiagramEditPart::queryInplaceEditor(const Point& point)
{
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

DiagramEditPart::iterator DiagramEditPart::getChildrenBegin()
{
	return children_.begin();
}

DiagramEditPart::const_iterator DiagramEditPart::getChildrenBegin() const
{
	return children_.begin();
}

DiagramEditPart::iterator DiagramEditPart::getChildrenEnd()
{
	return children_.end();
}

DiagramEditPart::const_iterator DiagramEditPart::getChildrenEnd() const
{
	return children_.end();
}

DiagramEditPart::size_type DiagramEditPart::getChildrenSize() const
{
	return children_.size();
}

DiagramEditPart::iterator DiagramEditPart::getSelectedChildrenBegin()
{
	return selected_children_.begin();
}

DiagramEditPart::const_iterator DiagramEditPart::getSelectedChildrenBegin() const
{
	return selected_children_.begin();
}

DiagramEditPart::iterator DiagramEditPart::getSelectedChildrenEnd()
{
	return selected_children_.end();
}

DiagramEditPart::const_iterator DiagramEditPart::getSelectedChildrenEnd() const
{
	return selected_children_.end();
}

DiagramEditPart::size_type DiagramEditPart::getSelectedChildrenSize() const
{
	return selected_children_.size();
}

DiagramEditPart::signal_selection_change_t DiagramEditPart::signalSelectionAdd()
{
	return signal_selection_add_;
}

DiagramEditPart::signal_selection_change_t DiagramEditPart::signalSelectionRemove()
{
	return signal_selection_remove_;
}

void DiagramEditPart::onChildSelectionAdd(shared_ptr< IEditPart > edit_part)
{
	selected_children_.push_back (edit_part);
	
	signal_selection_add_.emit (edit_part);
}

void DiagramEditPart::onChildSelectionRemove(shared_ptr< IEditPart > edit_part)
{
	iterator itor = 
		std::find (selected_children_.begin(), selected_children_.end(), edit_part);
	
	if (itor != selected_children_.end())
	{
		selected_children_.erase (itor);
	}
	
	signal_selection_remove_.emit (edit_part);
}

void DiagramEditPart::fillSelectedShapeList(std::list< shared_ptr< ModelElementBase > >& vector)
{
	BOOST_FOREACH(shared_ptr< IEditPart > edit_part, selected_children_)
	{
		vector.push_back (edit_part->getModel());
	}	
}

void DiagramEditPart::addChild(shared_ptr< ModelElementBase > element)
{
	// Add the child edit part
	shared_ptr< IEditPart > edit_part = 
		EditPartFactory::getInstance()->createEditPart (element);
		
	edit_part->setParent (shared_from_this());
	
	int index = diagram_->getChildIndex (element);

	// Test if the new element is to be added at the 
	// end of the children list or in the middle of the list
	if (index == -1 || index >= children_.size())
	{
		// Add the element at the end of the list
		children_.push_back (edit_part);

		// Add the child view
		rectangle_figure_->addChild (edit_part->getFigure());
	}
	else
	{
		// TODO: Assume that the index of the model element is equal
		// to the index of the controller
		shared_ptr< IEditPart > old_child_at_pos = children_[index];

		children_.insert (children_.begin() + index, edit_part);

		// Add the child view before the next view
		int figure_index = rectangle_figure_->getChildIndex (old_child_at_pos->getFigure());
		rectangle_figure_->insertChild (edit_part->getFigure(), figure_index);
	}
	
	edit_part->signalSelectionAdd().connect (
		sigc::mem_fun(this, &DiagramEditPart::onChildSelectionAdd));
		
	edit_part->signalSelectionRemove().connect (
		sigc::mem_fun(this, &DiagramEditPart::onChildSelectionRemove));	
		
	if (select_new_child_)
	{
		edit_part->setSelected (true);
	}
}

shared_ptr< DiagramEditPart > DiagramEditPart::queryDiagramFromChild(shared_ptr< IEditPart > child)
{
	// Search  for the root item
	if (shared_ptr< IContainerEditPart > root = child->getParent().lock())
	{
		while (shared_ptr< IContainerEditPart > root_parent = root->getParent().lock())
		{
			root = root_parent;
		}
		
		return dynamic_pointer_cast< DiagramEditPart >(root);
	}
	
	// The child is the DiagramEditPart
	return dynamic_pointer_cast< DiagramEditPart >( child );
}

}




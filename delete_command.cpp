#include "delete_command.h"

namespace cppgef
{

DeleteCommand::DeleteCommand(shared_ptr< Diagram > diagram, shared_ptr< ComplexModelElementBase > container, shared_ptr< ModelElementBase > element) :
	diagram_( diagram ),
	container_( container ),
	element_( element )
{
	index_ = container_->getChildIndex (element);
}

DeleteCommand::~DeleteCommand()
{
}

shared_ptr<Diagram> DeleteCommand::getDiagram()
{
	return diagram_;
}

bool DeleteCommand::allowUndo() const
{
	return true;
}

bool DeleteCommand::execute()
{
	container_->eraseChild (element_);
	
	return true;
}

bool DeleteCommand::unexecute()
{
	container_->insertChild (element_, index_);
	
	return true;
}

}


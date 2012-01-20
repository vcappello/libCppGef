#include "insert_model_element_command.h"

namespace cppgef
{

InsertModelElementCommand::InsertModelElementCommand(shared_ptr< Diagram > diagram, shared_ptr< ModelElementBase > element) :
	diagram_( diagram ),
	container_( diagram ),
	element_( element )
{
}

InsertModelElementCommand::InsertModelElementCommand(shared_ptr< Diagram > diagram, shared_ptr< ComplexModelElementBase > container, shared_ptr< ModelElementBase > element) :
	diagram_( diagram ),
	container_( container ),
	element_( element )
{
}

InsertModelElementCommand::~InsertModelElementCommand()
{
}

shared_ptr< Diagram > InsertModelElementCommand::getDiagram()
{
	return diagram_;
}

bool InsertModelElementCommand::allowUndo() const
{
	return true;
}

bool InsertModelElementCommand::execute()
{
	container_->addChild (element_);
	
	return true;
}

bool InsertModelElementCommand::unexecute()
{
	container_->eraseChild (element_);
	
	return true;
}

}


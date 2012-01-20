#include "group_command.h"

#include <boost/foreach.hpp>

namespace cppgef
{

GroupCommand::GroupCommand(shared_ptr< Diagram > diagram, const std::list< shared_ptr< ICommand > >& children) :
	diagram_( diagram ),
	children_( children )
{
}

GroupCommand::~GroupCommand()
{
}

shared_ptr<Diagram> GroupCommand::getDiagram()
{
	return diagram_;
}

bool GroupCommand::allowUndo() const
{
	return true;
}

bool GroupCommand::execute()
{
	BOOST_FOREACH(shared_ptr< ICommand > cmd, children_)
	{
		cmd->execute();
	}
	
	return true;
}

bool GroupCommand::unexecute()
{
	BOOST_FOREACH(shared_ptr< ICommand > cmd, children_)
	{
		cmd->unexecute();
	}
	
	return true;
}

}


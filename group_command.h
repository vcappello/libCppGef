#ifndef GROUPCOMMAND_H
#define GROUPCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <list>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace cppgef
{

class GroupCommand : public cppgef::ICommand
{

public:
	GroupCommand(shared_ptr< Diagram > diagram, const std::list< shared_ptr< ICommand > >& children);
	virtual ~GroupCommand();

public:
	shared_ptr<Diagram> getDiagram();
	bool allowUndo() const;
	bool execute();
	bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	std::list< shared_ptr< ICommand > > children_;
};

}

#endif // GROUPCOMMAND_H

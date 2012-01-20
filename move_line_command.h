#ifndef MOVELINECOMMAND_H
#define MOVELINECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <line_base.h>

namespace cppgef
{

class MoveLineCommand : public cppgef::ICommand
{
public:
	MoveLineCommand(shared_ptr< Diagram > diagram, shared_ptr< LineBase > simple_line, int dx, int dy);
	virtual ~MoveLineCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< LineBase > simple_line_;
	int dx_;
	int dy_;
};

}

#endif // MOVELINECOMMAND_H

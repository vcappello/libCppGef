#ifndef MOVELINEPOINTCOMMAND_H
#define MOVELINEPOINTCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <move_policy.h>
#include <line_base.h>

namespace cppgef
{

class MoveLinePointCommand : public cppgef::ICommand
{
public:
	MoveLinePointCommand(shared_ptr< Diagram > diagram, shared_ptr< LineBase > simple_line, IMovePointPolicy::PointSequence sequence, int dx, int dy);
	virtual ~MoveLinePointCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	
	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< LineBase > simple_line_;	
	IMovePointPolicy::PointSequence sequence_;
	int dx_;
	int dy_;	
};

}

#endif // MOVELINEPOINTCOMMAND_H

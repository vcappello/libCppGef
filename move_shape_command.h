#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

namespace cppgef
{

class MoveShapeCommand : public cppgef::ICommand
{

public:
	MoveShapeCommand(shared_ptr< Diagram > diagram, shared_ptr< ShapeBase > shape, int dx, int dy);
	virtual ~MoveShapeCommand();

public:
	shared_ptr<Diagram> getDiagram();
	bool allowUndo() const;
	bool execute();
	bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ShapeBase > shape_;	
	int dx_;
	int dy_;
};

}

#endif // MOVECOMMAND_H

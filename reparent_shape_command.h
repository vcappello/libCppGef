#ifndef REPARENTSHAPECOMMAND_H
#define REPARENTSHAPECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

namespace cppgef
{

class ReparentShapeCommand : public cppgef::ICommand
{

public:
	ReparentShapeCommand(shared_ptr< Diagram > diagram, shared_ptr< ShapeBase > shape, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual ~ReparentShapeCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	
	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ShapeBase > shape_;
	shared_ptr< ComplexModelElementBase > old_parent_;
	shared_ptr< ComplexModelElementBase > new_parent_;
	int dx_;
	int dy_;
	size_t index_;
};

}

#endif // REPARENTSHAPECOMMAND_H

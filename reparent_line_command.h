#ifndef REPARENTLINECOMMAND_H
#define REPARENTLINECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <line_base.h>
#include <complex_model_element_base.h>

namespace cppgef
{

class ReparentLineCommand : public cppgef::ICommand
{
public:
	ReparentLineCommand(shared_ptr< Diagram > diagram, shared_ptr< LineBase > simple_line, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual ~ReparentLineCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< LineBase > simple_line_;
	shared_ptr< ComplexModelElementBase > old_parent_;
	shared_ptr< ComplexModelElementBase > new_parent_;
	int dx_;
	int dy_;
	size_t index_;
};

}

#endif // REPARENTLINECOMMAND_H

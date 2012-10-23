#ifndef REPARENTCURVECOMMAND_H
#define REPARENTCURVECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <curve_line.h>
#include <complex_model_element_base.h>

namespace cppgef
{

class ReparentCurveCommand : public cppgef::ICommand
{
public:
	ReparentCurveCommand(shared_ptr< Diagram > diagram, shared_ptr< CurveLine > curve, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual ~ReparentCurveCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< CurveLine > curve_;
	shared_ptr< ComplexModelElementBase > old_parent_;
	shared_ptr< ComplexModelElementBase > new_parent_;
	int dx_;
	int dy_;
	size_t index_;
};

}

#endif // REPARENTCURVECOMMAND_H

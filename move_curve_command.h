#ifndef MOVECURVECOMMAND_H
#define MOVECURVECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <curve_line.h>

namespace cppgef
{

class MoveCurveCommand : public cppgef::ICommand
{
public:
	MoveCurveCommand(shared_ptr< Diagram > diagram, shared_ptr< CurveLine > curve, int dx, int dy);
	virtual ~MoveCurveCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< CurveLine > curve_;
	int dx_;
	int dy_;

};

}

#endif // MOVECURVECOMMAND_H

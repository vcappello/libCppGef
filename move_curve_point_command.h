#ifndef MOVECURVEPOINTCOMMAND_H
#define MOVECURVEPOINTCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <move_policy.h>
#include <curve_line.h>

namespace cppgef
{

class MoveCurvePointCommand : public cppgef::ICommand
{
public:
	MoveCurvePointCommand(shared_ptr< Diagram > diagram, shared_ptr< CurveLine > curve, IMovePointPolicy::PointSequence sequence, int dx, int dy);
	virtual ~MoveCurvePointCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	
	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< CurveLine > curve_;	
	IMovePointPolicy::PointSequence sequence_;
	int dx_;
	int dy_;
};

}

#endif // MOVECURVEPOINTCOMMAND_H

#ifndef MOVEFRAMECOMMAND_H
#define MOVEFRAMECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <frame.h>

namespace cppgef
{

class MoveFrameCommand : public cppgef::ICommand
{

public:
	MoveFrameCommand(shared_ptr< Diagram > diagram, const shared_ptr< Frame > frame, int dx, int dy);
	virtual ~MoveFrameCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	virtual bool allowUndo() const;
	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< Frame > frame_;
	int dx_;
	int dy_;
};

}

#endif // MOVEFRAMECOMMAND_H

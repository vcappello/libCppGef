#ifndef REPARENTFRAMECOMMAND_H
#define REPARENTFRAMECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <frame.h>

namespace cppgef
{

class ReparentFrameCommand : public cppgef::ICommand
{

public:
	ReparentFrameCommand(shared_ptr< Diagram > diagram, shared_ptr< Frame > frame, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy);
	virtual ~ReparentFrameCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	virtual bool allowUndo() const;
	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< Frame > frame_;
	shared_ptr< ComplexModelElementBase > old_parent_;
	shared_ptr< ComplexModelElementBase > new_parent_;
	int dx_;
	int dy_;
	size_t index_;
};

}

#endif // REPARENTFRAMECOMMAND_H

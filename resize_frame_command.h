#ifndef RESIZEFRAMECOMMAND_H
#define RESIZEFRAMECOMMAND_H

#include "i_command.h" // Base class: cppgef::ICommand

#include <boost/shared_ptr.hpp>

#include <resize_policy.h>
#include <frame.h>

using boost::shared_ptr;

namespace cppgef
{

class ResizeFrameCommand : public cppgef::ICommand
{

public:
	ResizeFrameCommand(shared_ptr< Diagram > diagram, shared_ptr< Frame > frame, AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy);
	virtual ~ResizeFrameCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	virtual bool allowUndo() const;
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< Frame> frame_;
	shared_ptr< AbstractResizePolicy > resize_policy_;	
	AbstractResizePolicy::ResizeDirection resize_direction_;
	int dx_;
	int dy_;
};

}

#endif // RESIZEFRAMECOMMAND_H

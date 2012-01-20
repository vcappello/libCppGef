#ifndef RESIZECOMMAND_H
#define RESIZECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <boost/shared_ptr.hpp>

#include <resize_policy.h>

using boost::shared_ptr;

namespace cppgef
{

class ResizeShapeCommand : public cppgef::ICommand
{

public:
	ResizeShapeCommand(shared_ptr< Diagram > diagram, shared_ptr< ShapeBase > shape, AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy);
	virtual ~ResizeShapeCommand();

public:
	shared_ptr<Diagram> getDiagram();
	bool allowUndo() const;
	bool execute();
	bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ShapeBase > shape_;
	shared_ptr< AbstractResizePolicy > resize_policy_;	
	AbstractResizePolicy::ResizeDirection resize_direction_;
	int dx_;
	int dy_;
};

}

#endif // RESIZECOMMAND_H

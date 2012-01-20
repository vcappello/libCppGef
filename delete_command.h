#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <boost/shared_ptr.hpp>

#include <diagram.h>
#include <model_element_base.h>
#include <complex_model_element_base.h>

namespace cppgef
{

class DeleteCommand : public cppgef::ICommand
{

public:
	DeleteCommand(shared_ptr< Diagram > diagram, shared_ptr< ComplexModelElementBase > container, shared_ptr< ModelElementBase > element);
	virtual ~DeleteCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ComplexModelElementBase > container_;
	shared_ptr< ModelElementBase > element_;
	size_t index_;
};

}

#endif // DELETECOMMAND_H

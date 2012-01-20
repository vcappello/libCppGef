#ifndef SETLINECOLORCOMMAND_H
#define SETLINECOLORCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <i_support_line_style.h>

namespace cppgef
{

class SetLineColorCommand : public cppgef::ICommand
{

public:
	SetLineColorCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportLineStyle > element_, const Color& color);
	virtual ~SetLineColorCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ISupportLineStyle > element_;
	Color old_color_;
	Color new_color_;
};

}

#endif // SETLINECOLORCOMMAND_H

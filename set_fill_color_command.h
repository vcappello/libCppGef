#ifndef SETSHAPEFILLCOLORCOMMAND_H
#define SETSHAPEFILLCOLORCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <i_support_fill_style.h>

namespace cppgef
{

class SetFillColorCommand : public cppgef::ICommand
{
public:
	SetFillColorCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportFillStyle > element, const Color& color);
	virtual ~SetFillColorCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	
	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ISupportFillStyle > element_;
	Color old_color_;
	Color new_color_;	
};

}

#endif // SETSHAPEFILLCOLORCOMMAND_H

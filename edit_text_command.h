#ifndef EDITTEXTCOMMAND_H
#define EDITTEXTCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <i_text_element.h>

namespace cppgef
{

class EditTextCommand : public cppgef::ICommand
{
public:
	EditTextCommand(shared_ptr< Diagram > diagram, shared_ptr< ITextElement> text_element, const Glib::ustring& new_value);
	virtual ~EditTextCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ITextElement > text_element_;
	Glib::ustring new_value_;
	Glib::ustring old_value_;
};

}

#endif // EDITTEXTCOMMAND_H

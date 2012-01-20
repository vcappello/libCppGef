#ifndef CLIPBOARDCOPYCOMMAND_H
#define CLIPBOARDCOPYCOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <vector>
#include <stdexcept>

#include <diagram_edit_part.h>
#include <clipboard_manager.h>

namespace cppgef
{

class ClipboardCopyCommand : public cppgef::ICommand
{

public:
	ClipboardCopyCommand(shared_ptr<Diagram> diagram, shared_ptr< DiagramEditPart > diagram_editpart);
	virtual ~ClipboardCopyCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	
	virtual bool allowUndo() const;
	
	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr<Diagram> diagram_;
};

}

#endif // CLIPBOARDCOPYCOMMAND_H

#ifndef CLIPBOARDPASTECOMMAND_H
#define CLIPBOARDPASTECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <clipboard_manager.h>

namespace cppgef
{

class ClipboardPasteCommand : public cppgef::ICommand
{

public:
	ClipboardPasteCommand(shared_ptr<Diagram> diagram);
	virtual ~ClipboardPasteCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();

	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();
	
protected:
	shared_ptr<Diagram> diagram_;
	std::vector< shared_ptr< ModelElementBase > > items_;
	bool first_execute_;
	
protected:
	void onClipboardReceived(const Gtk::SelectionData& selection_data);
};

}

#endif // CLIPBOARDPASTECOMMAND_H

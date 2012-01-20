#ifndef IINPLACEEDITOR_H
#define IINPLACEEDITOR_H

#include <gtkmm.h>

#include <rectangle.h>

namespace cppgef
{

class IInplaceEditor
{
public:
	typedef sigc::signal< void > signal_stop_edit_t;
	
public:
	IInplaceEditor()
	{
	}
	virtual ~IInplaceEditor()
	{
	}

	virtual Rectangle getBounds() const = 0;

	virtual Gtk::Widget* getWidget() = 0;
	
	virtual Gtk::Widget* createWidget() = 0;
	
	virtual void apply() = 0;
	
	virtual void executeUndo() = 0;
	virtual void executeRedo() = 0;
	virtual void executeCut() = 0;
	virtual void executeCopy() = 0;
	virtual void executePaste() = 0;
	
	virtual signal_stop_edit_t signalStopEdit() = 0;
};

}

#endif // IINPLACEEDITOR_H

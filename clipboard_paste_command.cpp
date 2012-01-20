#include "clipboard_paste_command.h"

#include <sstream>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

const std::string CLIPBOARD_TARGET = "libcppgef";

namespace cppgef
{

ClipboardPasteCommand::ClipboardPasteCommand(shared_ptr<Diagram> diagram) :
	diagram_( diagram ),
	first_execute_( true )
{
}

ClipboardPasteCommand::~ClipboardPasteCommand()
{
}

shared_ptr<Diagram> ClipboardPasteCommand::getDiagram()
{
	return diagram_;
}

bool ClipboardPasteCommand::allowUndo() const
{
	return true;
}

bool ClipboardPasteCommand::execute()
{
	if (first_execute_)
	{
		ClipboardManager::getInstance()->paste (sigc::mem_fun(*this, &ClipboardPasteCommand::onClipboardReceived));
	}
	else
	{
		for (size_t i = 0; i < items_.size(); i++)
		{
			diagram_->addChild (items_[i]);
		}
	}
	return true;
}

bool ClipboardPasteCommand::unexecute()
{
	for (size_t i = 0; i < items_.size(); i++)
	{
		diagram_->eraseChild (items_[i]);
	}
	
	return true;
}

void ClipboardPasteCommand::onClipboardReceived(const Gtk::SelectionData& selection_data)
{
	const std::string target = selection_data.get_target();

	//It should always be this, because that' what we asked for when calling
	//request_contents().
	if(target == ClipboardManager::getInstance()->getClipboardTarget())
	{
		std::string clipboard_data = selection_data.get_data_as_string();
		
		if (!clipboard_data.empty())
		{
			std::stringstream ifs(clipboard_data);
			
			boost::archive::text_iarchive ia(ifs);
			ia >> items_;
			
			for (size_t i = 0; i < items_.size(); i++)
			{
				diagram_->addChild (items_[i]);
			}			
			
			first_execute_ = false;
		}
	}	
}

}


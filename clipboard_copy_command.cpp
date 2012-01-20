#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "clipboard_copy_command.h"

#include <sstream>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace cppgef
{

ClipboardCopyCommand::ClipboardCopyCommand(shared_ptr<Diagram> diagram, shared_ptr< DiagramEditPart > diagram_editpart) :
	diagram_( diagram )
{
	std::vector< shared_ptr< ModelElementBase > > items;
	
	IContainerEditPart::children_container_t selected_items;
	diagram_editpart->querySelectedChildrenNoChild (selected_items);
	
	// Store all items for serialization
	for (DiagramEditPart::iterator edit_part_itor = selected_items.begin();
		edit_part_itor != selected_items.end();
		edit_part_itor++)
	{	
		items.push_back ((*edit_part_itor)->getModel());
	}
	
	// Serialize and set clipboard data
	try
	{
		std::stringstream ofs;
		boost::archive::text_oarchive oa(ofs);

		oa << items;

		ClipboardManager::getInstance()->setCopyData (ofs.str());
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception on copy command: " << e.what() << std::endl;
	}

}

ClipboardCopyCommand::~ClipboardCopyCommand()
{
}

shared_ptr<Diagram> ClipboardCopyCommand::getDiagram()
{
	return diagram_;
}

bool ClipboardCopyCommand::allowUndo() const
{
	return false;
}

bool ClipboardCopyCommand::execute()
{
	ClipboardManager::getInstance()->copy();

	return true;
}

bool ClipboardCopyCommand::unexecute()
{
	return true;
}

}


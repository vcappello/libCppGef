#include "save_diagram_command.h"

#include <fstream>
#include <glibmm/i18n.h>

#include <command_manager.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

namespace cppgef
{

SaveDiagramCommand::SaveDiagramCommand(shared_ptr< Diagram > diagram, const std::string& file_name) :
	diagram_( diagram ),
	file_name_( file_name )
{
}

SaveDiagramCommand::~SaveDiagramCommand()
{
}

shared_ptr< Diagram > SaveDiagramCommand::getDiagram()
{
	return diagram_;
}

bool SaveDiagramCommand::allowUndo() const
{
	return false;
}

bool SaveDiagramCommand::execute()
{
	std::ofstream ofs(file_name_.c_str());
	
	if (ofs.good())
	{
		boost::archive::xml_oarchive oa(ofs);
		oa << boost::serialization::make_nvp ("Diagram", *diagram_);
		
		CommandManager::getInstance()->setSaveBookmark (diagram_);

		return true;
	}
	else
	{
		throw Error(
			Glib::ustring::compose(_("Fail to access file '%1'"), file_name_));
	}
	
	return false;
}

bool SaveDiagramCommand::unexecute()
{
	return false;
}

}


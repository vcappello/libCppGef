#ifndef SAVEDIAGRAMCOMMAND_H
#define SAVEDIAGRAMCOMMAND_H

#include "i_command.h" // Base class: cppgef::ICommand

#include <boost/shared_ptr.hpp>

#include <diagram.h>
#include <error.h>

using boost::shared_ptr;

namespace cppgef
{

class SaveDiagramCommand : public cppgef::ICommand
{

public:
	SaveDiagramCommand(shared_ptr< Diagram > diagram, const std::string& file_name);
	virtual ~SaveDiagramCommand();

	shared_ptr< Diagram > getDiagram();

public:
	bool allowUndo() const;
	bool execute();
	bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	std::string file_name_;
};

}

#endif // SAVEDIAGRAMCOMMAND_H

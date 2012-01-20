#ifndef SETDASHSTYLECOMMAND_H
#define SETDASHSTYLECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <i_support_line_style.h>

namespace cppgef
{

class SetDashStyleCommand : public cppgef::ICommand
{

public:
	SetDashStyleCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportLineStyle > element, const std::vector< double >& dash_style, double dash_offset);
	virtual ~SetDashStyleCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ISupportLineStyle > element_;
	std::vector< double > old_dash_style_;
	double old_dash_offset_;
	std::vector< double > new_dash_style_;
	double new_dash_offset_;
};

}

#endif // SETDASHSTYLECOMMAND_H

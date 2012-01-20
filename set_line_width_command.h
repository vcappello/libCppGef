/*
 * set_line_width_command.h
 *
 *  Created on: Nov 10, 2011
 *      Author: vincenzo
 */

#ifndef SET_LINE_WIDTH_COMMAND_H_
#define SET_LINE_WIDTH_COMMAND_H_

#include <i_command.h>

#include <i_support_line_style.h>

namespace cppgef
{

class SetLineWidthCommand: public cppgef::ICommand
{
public:
	SetLineWidthCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportLineStyle > element, double width);
	virtual ~SetLineWidthCommand();

public:
	virtual shared_ptr<Diagram> getDiagram();
	virtual bool allowUndo() const;

	virtual bool execute();
	virtual bool unexecute();

protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ISupportLineStyle > element_;
	double old_width_;
	double new_width_;
};

} /* namespace cppgef */

#endif /* SET_LINE_WIDTH_COMMAND_H_ */

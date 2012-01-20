/*
 * set_line_width_command.cpp
 *
 *  Created on: Nov 10, 2011
 *      Author: vincenzo
 */

#include "set_line_width_command.h"

namespace cppgef
{

SetLineWidthCommand::SetLineWidthCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportLineStyle > element, double width) :
	diagram_( diagram ),
	element_( element ),
	old_width_( element->getLineWidth() ),
	new_width_( width )
{
}

SetLineWidthCommand::~SetLineWidthCommand()
{
}

shared_ptr<Diagram> SetLineWidthCommand::getDiagram()
{
	return diagram_;
}

bool SetLineWidthCommand::allowUndo() const
{
	return true;
}

bool SetLineWidthCommand::execute()
{
	element_->setLineWidth (new_width_);

	return true;
}

bool SetLineWidthCommand::unexecute()
{
	element_->setLineWidth (old_width_);

	return true;
}

} /* namespace cppgef */

#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <boost/shared_ptr.hpp>

#include <diagram.h>

using boost::shared_ptr;

namespace cppgef
{

/**
 * @class ICommand
 * @author vincenzo.cappello
 * @date 12/14/10
 * @file icommand.h
 * 
 * @brief Base class for commands
 */
class ICommand
{

public:
	ICommand()
	{
	}
	virtual ~ICommand()
	{
	}

	/**
	 * @brief The diagram used by the command
	 *
	 * The cppgef::CommandManager class store the command on
	 * its stack according to this properties.
	 *
	 * @return the diagram
	 */
	virtual shared_ptr< Diagram > getDiagram() = 0;
	
	/**
	 * @brief The command allow undo/redo
	 * 
	 * If the command allow undo/redo the cppgef::CommandManager class
	 * after execute the command store it in the undo stack.
	 *
	 * @return true if the command allow undo/redo
	 */
	virtual bool allowUndo() const = 0;

	/**
	 * @brief Execute the command
	 * 
	 * Called on first execution and on redo
	 * 
	 * @return command result, true for success
	 */
	virtual bool execute() = 0;
	
	/**
	 * @brief Unexecute the command
	 * 
	 * Called on undo
	 * 
	 * @return command result, true for success
	 */
	virtual bool unexecute() = 0;
};

}

#endif // ICOMMAND_H

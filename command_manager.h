#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <stack>
#include <map>
#include <boost/shared_ptr.hpp>
#include <gtkmm.h>

#include <i_command.h>

using boost::shared_ptr;

namespace cppgef
{

/**
 * @class CommandManager
 * @author vincenzo.cappello
 * @date 12/03/10
 * @file command_manager.h
 * 
 * @brief Manage command execution
 * 
 * This is a singleton object, use static member getInstance() for retrieve
 * the unique object.
 * 
 * The CommandManager store all commands separately for each document, and allow
 * to undo and redo commands.
 * 
 * When a Diagram is opened use CommandManager::openStack() for start a new undo/redo session,
 * when the Diagram is closed use CommandManager::closeStack() for close the undo/redo session.
 */
class CommandManager
{
private:
	typedef std::stack< shared_ptr< ICommand > > cmd_stack_t;
	
public:
	typedef sigc::signal< void, shared_ptr< Diagram > > signal_command_execute_t;
	typedef sigc::signal< void, shared_ptr< Diagram > > signal_command_undo_t;
	typedef sigc::signal< void, shared_ptr< Diagram > > signal_command_redo_t;
	typedef sigc::signal< void, shared_ptr< Diagram > > signal_saved_state_t;

private:
	static CommandManager* instance_;

public:
	/**
	 * @brief The singleton instance
	 * @return the singleton instance
	 */
	static CommandManager* getInstance();
	
	/**
	 * @brief Release the singleton object
	 */
	static void release();

	/**
	 * @brief Execute a command
	 * 
	 * If the command allow undo, after execution it was pushed
	 * in the undo stack. If the redo stack is not empty it was cleared.
	 * Emits the signalCommandExecute() signal
	 * 
	 * @param cmd the command to execute
	 * @return command result
	 */
	bool execute(shared_ptr< ICommand > cmd);
	
	/**
	 * @brief Store a command for undo/redo if the command allow
	 * undo, but does not execute the command
	 * @param cmd the command to store
	 */
	void storeForUndoRedo(shared_ptr< ICommand > cmd);
	
	/**
	 * @brief Undo last command
	 * 
	 * Pop last command from the undo stack and unexecute it,
	 * after push the command in the redo stack.
	 * Emits the signalCommandUndo() signal
	 * 
	 * @param diagram the diagram to use
	 * @return command result
	 */
	bool undo(shared_ptr< Diagram > diagram);
	
	/**
	 * @brief Redo last undo command
	 * 
	 * Pop last command from redo stack and execute it,
	 * after push the command in the undo stack
	 * Emits the signalCommandRedo() signal
	 * 
	 * @param diagram the diagram to use
	 * @return command result
	 */
	bool redo(shared_ptr< Diagram > diagram);
	
	/**
	 * @brief The undo stack empty status
	 * @param diagram the diagram to use
	 * @return true if the undo stack is empty
	 */
	bool isUndoStackEmpty(shared_ptr< Diagram > diagram) const;
	
	/**
	 * @brief  The redo stack empty status
	 * @param diagram the diagram to use
	 * @return true if the redo stack is empty
	 */
	bool isRedoStackEmpty(shared_ptr< Diagram > diagram) const;
	
	/**
	 * @brief Start a new undo/redo session for a diagram
	 * @param diagram the diagram to use
	 */
	void openStack(shared_ptr< Diagram > diagram);
	
	/**
	 * @brief Close an undo/redo session for a diagram
	 * @param diagram the diagram to use
	 */
	void closeStack(shared_ptr< Diagram > diagram);

	/**
	 * @brief Set the saved state for given diagram
	 *
	 * After calling undo or redo when the save bookmark is reached
	 * the signal signalSavedState() was emitted.
	 * The call to setSaveBookmark emits the signalSavedState() signal.
	 *
	 * @param diagram the diagram to use
	 */
	void setSaveBookmark(shared_ptr< Diagram > diagram);

	bool isReachedSavedState(shared_ptr< Diagram > diagram);

	signal_command_execute_t signalCommandExecute();
	signal_command_undo_t signalCommandUndo();
	signal_command_redo_t signalCommandRedo();
	signal_saved_state_t signalReachedSavedState();
	
private:
	std::map< shared_ptr< Diagram >, shared_ptr< cmd_stack_t > > undo_stack_;
	std::map< shared_ptr< Diagram >, shared_ptr< cmd_stack_t > > redo_stack_;
	std::map< shared_ptr< Diagram >, shared_ptr< ICommand > > save_bookmarks_;
	
	signal_command_execute_t signal_command_execute_;
	signal_command_undo_t signal_command_undo_;
	signal_command_redo_t signal_command_redo_;
	signal_saved_state_t signal_saved_state_;

private:
	CommandManager();
	virtual ~CommandManager();
};

}

#endif // COMMANDMANAGER_H

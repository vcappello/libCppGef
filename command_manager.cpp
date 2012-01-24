#include "command_manager.h"

namespace cppgef
{

CommandManager* CommandManager::instance_ = 0;

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

CommandManager* CommandManager::getInstance()
{
	if(instance_ == 0)
	{
		instance_ = new CommandManager();
	}
	return instance_;
}

void CommandManager::release()
{
	if(instance_)
	{
		delete instance_;
	}
	instance_ = 0;
}

bool CommandManager::execute(shared_ptr< ICommand > cmd)
{
	bool result = cmd->execute();
	
	if (cmd->allowUndo())
	{
		undo_stack_[cmd->getDiagram()]->push (cmd);

		// Clear redo stack
		while (!redo_stack_[cmd->getDiagram()]->empty())
		{
			redo_stack_[cmd->getDiagram()]->pop();
		}		
	}
	
	signal_command_execute_.emit (cmd->getDiagram());

	return result;
}

void CommandManager::storeForUndoRedo(shared_ptr< ICommand > cmd)
{
	if (cmd->allowUndo())
	{
		undo_stack_[cmd->getDiagram()]->push (cmd);

		// Clear redo stack
		while (!redo_stack_[cmd->getDiagram()]->empty())
		{
			redo_stack_[cmd->getDiagram()]->pop();
		}		
	}	
}

bool CommandManager::undo(shared_ptr< Diagram > diagram)
{
	if (undo_stack_[diagram]->empty())
		return false;
		
	shared_ptr< ICommand > cmd = undo_stack_[diagram]->top();
	undo_stack_[diagram]->pop();
	
	bool result = cmd->unexecute();
	
	redo_stack_[diagram]->push (cmd);
	
	signal_command_undo_.emit (diagram);

	// Check if we reach the saved state
	if (isReachedSavedState (diagram))
		signal_saved_state_.emit (diagram);

	return result;
}

bool CommandManager::redo(shared_ptr< Diagram > diagram)
{
	if (redo_stack_[diagram]->empty())
		return false;
		
	shared_ptr< ICommand > cmd = redo_stack_[diagram]->top();
	redo_stack_[diagram]->pop();
	
	bool result = cmd->execute();
	
	undo_stack_[diagram]->push (cmd);	

	signal_command_redo_.emit (diagram);
	
	// Check if we reach the saved state
	if (isReachedSavedState (diagram))
		signal_saved_state_.emit (diagram);

	return result;
}	

bool CommandManager::isUndoStackEmpty(shared_ptr< Diagram > diagram) const
{
	return undo_stack_.find(diagram)->second->empty();
}

bool CommandManager::isRedoStackEmpty(shared_ptr< Diagram > diagram) const
{
	return redo_stack_.find(diagram)->second->empty();
}

void CommandManager::openStack(shared_ptr< Diagram > diagram)
{
	undo_stack_[diagram] = shared_ptr< cmd_stack_t >( new cmd_stack_t() );
	redo_stack_[diagram] = shared_ptr< cmd_stack_t >( new cmd_stack_t() );
}

void CommandManager::closeStack(shared_ptr< Diagram > diagram)
{
	undo_stack_.erase (diagram);
	redo_stack_.erase (diagram);
	save_bookmarks_.erase (diagram);
}

void CommandManager::setSaveBookmark(shared_ptr< Diagram > diagram)
{
	if (!undo_stack_[diagram]->empty())
	{
		save_bookmarks_[diagram] = undo_stack_[diagram]->top();
	}
	else
	{
		if (save_bookmarks_.find (diagram) != save_bookmarks_.end())
		{
			save_bookmarks_.erase (diagram);
		}
	}

	signal_saved_state_.emit (diagram);
}

CommandManager::signal_command_execute_t CommandManager::signalCommandExecute()
{
	return signal_command_execute_;
}

CommandManager::signal_command_undo_t CommandManager::signalCommandUndo()
{
	return signal_command_undo_;
}

CommandManager::signal_command_redo_t CommandManager::signalCommandRedo()
{
	return signal_command_redo_;
}

CommandManager::signal_saved_state_t CommandManager::signalReachedSavedState()
{
	return signal_saved_state_;
}

bool CommandManager::isReachedSavedState(shared_ptr< Diagram > diagram)
{
	if (save_bookmarks_.find (diagram) != save_bookmarks_.end())
	{
		if (!undo_stack_[diagram]->empty() && save_bookmarks_[diagram] == undo_stack_[diagram]->top())
			return true;
	}
	else
	{
		if (undo_stack_[diagram]->empty())
			return true;
	}

	return false;
}

}

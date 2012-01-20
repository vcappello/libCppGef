#ifndef INSERTSHAPECOMMAND_H
#define INSERTSHAPECOMMAND_H

#include <i_command.h> // Base class: cppgef::ICommand

#include <boost/shared_ptr.hpp>

#include <diagram.h>
#include <model_element_base.h>
#include <complex_model_element_base.h>

namespace cppgef
{

/**
 * @class InsertShapeCommand
 * @author vincenzo.cappello
 * @date 12/14/10
 * @file insert_shape_command.h
 * 
 * @brief Add a ModelElementBase to a diagram
 */
class InsertModelElementCommand : public cppgef::ICommand
{

public:
	/**
	 * @brief Create the command
	 * @param diagram the diagram container
	 * @param element the child ModelElementBase
	 */
	InsertModelElementCommand(shared_ptr< Diagram > diagram, shared_ptr< ModelElementBase > element);
	
	InsertModelElementCommand(shared_ptr< Diagram > diagram, shared_ptr< ComplexModelElementBase > container, shared_ptr< ModelElementBase > element);

	virtual ~InsertModelElementCommand();

	shared_ptr< Diagram > getDiagram();

public:
	/**
	 * @brief This command allow undo
	 * @return true
	 */
	virtual bool allowUndo() const;
	
	/**
	 * @brief Add the element to the diagram
	 * @return 
	 */
	virtual bool execute();
	
	/**
	 * @brief Erase the element to the diagram
	 * @return 
	 */
	virtual bool unexecute();
	
protected:
	shared_ptr< Diagram > diagram_;
	shared_ptr< ComplexModelElementBase > container_;
	shared_ptr< ModelElementBase > element_;
};

}

#endif // INSERTSHAPECOMMAND_H

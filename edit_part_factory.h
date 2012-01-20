#ifndef EDITPARTFACTORY_H
#define EDITPARTFACTORY_H

#include <typeinfo>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

#include <i_edit_part.h>
#include <model_element_base.h>
#include <model_element_type.h>
#include <error.h>

using boost::shared_ptr;

namespace cppgef
{

template< typename class_type >
shared_ptr< IEditPart > createObject()
{
	return shared_ptr< IEditPart >( new class_type() );
}

/**
 * @class EditPartFactory
 * @author vincenzo cappello
 * @date 05/12/2010
 * @file edit_part_factory.h
 * 
 * @brief Factory for edit part objects
 * 
 * Create EditPart object for the relative ModelElement.
 * 
 * This is a singleton object, use static memebr getInstance() for retrieve 
 * the unique object.
 */
class EditPartFactory
{
public:
	typedef shared_ptr< IEditPart > (*create_object_func)();
	
	typedef std::map< std::string, create_object_func > registry_t;
	
	typedef std::map< std::string, shared_ptr< ModelElementType > > registered_types_t;
	typedef registered_types_t::iterator iterator;
	typedef registered_types_t::const_iterator const_iterator;
	
public:
	/**
	 * @brief The singleton instance
	 * @return the singleton instance
	 */
	static EditPartFactory* getInstance();
	
	/**
	 * @brief Release the singleton object
	 */
	static void release();

	/**
	 * @brief Register a new EditPart factory
	 * 
	 * Example:
	 * @code 
	 * registerFactory (shared_ptr< ModelElementType >(
	 *   new ModelElementType( typeid(Shape).name(), Glib::ustring("Shape") )), 
	 *   &createObject< ShapeEditPart >);
	 * @endcode
	 * 
	 * Throw an Error if the model_element_type is already registered
	 * 
	 * @param model_element_type the type descriptor of the model element to register
	 * @param factory_func factory function
	 */
	void registerFactory(shared_ptr< ModelElementType > model_ement_type, create_object_func factory_func);
	
	/**
	 * @brief Unregister a previously registered EditPart factory
	 * 
	 * Throw an Error if the type_name is not registered
	 * 
	 * @param type_name the name of the type of a model element to unregister
	 */
	void unregisterFactory(const std::string& type_name);
	
	/**
	 * @brief Create a new EditPart for the given model element
	 * 
	 * The EditPart returned reference the given model_element.
	 * 
	 * Throw an Error if a factory for the model_element type is not registered
	 * 
	 * @param model_element a model element
	 * @return the new EditPart object
	 */
	shared_ptr< IEditPart > createEditPart(shared_ptr< ModelElementBase > model_element);

	/**
	 * @brief Create a new EditPart for the given type
	 * 
	 * The EditPart returned reference a new empty model element.
	 * 
	 * Throw an Error if a factory for the model_element_type is not registered
	 * 
	 * @param type_name the name of the type
	 * @return  the new EditPart object
	 */
	shared_ptr< IEditPart > createEditPart(const std::string& type_name);

	/**
	 * @brief Test if a factory for a model element type is already registered
	 * @param type_name the name of the type of a model element
	 * @return true if the factory is registered
	 */
	bool existFactory(const std::string& type_name) const;

	/**
	 * @brief Return a ModelElementType from a type name
	 * 
	 * Throw an Error if the given type name is not registered
	 * 
	 * @param type_name the type name
	 * @return the ModelElementType
	 */
	shared_ptr< ModelElementType > getModelElementType(const std::string& type_name);
	
	iterator getTypesBegin();
	const_iterator getTypesBegin() const;
	iterator getTypesEnd();
	const_iterator getTypesEnd() const;
	
private:
	static EditPartFactory* instance_;
	registry_t registry_;
	registered_types_t registered_types_;
	
private:
	EditPartFactory();
	virtual ~EditPartFactory();
	
	Glib::RefPtr< Gdk::Pixbuf > loadPixbuf(const std::string& file_name);
};

}

#endif // EDITPARTFACTORY_H

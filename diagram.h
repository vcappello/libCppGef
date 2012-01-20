#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <model_element_base.h> // Base class
#include <complex_model_element_base.h>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <glibmm/ustring.h>
#include <sigc++/sigc++.h>

#include <shape_base.h>
#include <error.h>

using std::vector;
using boost::shared_ptr;

namespace cppgef
{

/**
 * @class Diagram
 * @author vincenzo.cappello
 * @date 12/03/10
 * @file diagram.h
 * 
 * @brief The root model element
 */
class Diagram : public cppgef::ComplexModelElementBase
{
public:
	Diagram();
	virtual ~Diagram();

	/**
	 * @brief Diagram title
	 * 
	 * The signalTitleChange was raised
	 * 
	 * @param value the new diagram title
	 */
	void setTitle(const Glib::ustring& value);
	
	/**
	 * @brief Diagram title
	 * @return the diagram title
	 */
	Glib::ustring getTitle() const;
	
	/**
	 * @name Signals 
	 */
	/*@{*/
	
	/**
	 * @brief Signal emitted when the Title property change value
	 * @return the signal
	 */
	signal_property_changed_t signalTitleChanged();
	
	/*@}*/

protected:
	Glib::ustring title_;
	
	signal_property_changed_t signal_title_changed_;
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // DIAGRAM_H

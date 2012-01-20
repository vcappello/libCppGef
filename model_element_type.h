#ifndef MODELELEMENTTYPE_H
#define MODELELEMENTTYPE_H

#include <string>
#include <glibmm/ustring.h>
#include <gdkmm/pixbuf.h>
#include <boost/shared_ptr.hpp>

#include <itool.h>
#include <i_bounds_model_element.h>

using boost::shared_ptr;

namespace cppgef
{

template< typename class_type >
shared_ptr< ITool > createInsertTool()
{
	return shared_ptr< ITool >( new class_type() );
}


/**
 * @class ModelElementType
 * @author vincenzo.cappello
 * @date 12/15/10
 * @file model_element_type.h
 * 
 * @brief Describe the type of a model element
 * 
 * Used by EditPartFactory
 */
class ModelElementType
{
public:
	typedef shared_ptr< ITool > (*create_tool_func)();

public:
	ModelElementType(const std::string& class_type, bool root=false);
	ModelElementType(const std::string& class_type, const Glib::ustring& display_name, bool root=false);
	ModelElementType(const std::string& class_type, const Glib::ustring& display_name, Glib::RefPtr<Gdk::Pixbuf> display_image, bool root=false);
	ModelElementType(const std::string& class_type, const Glib::ustring& display_name, Glib::RefPtr<Gdk::Pixbuf> display_image, shared_ptr< IBoundsModelElement > bounds_model_element, bool root=false);
	ModelElementType(const std::string& class_type, const Glib::ustring& display_name, Glib::RefPtr<Gdk::Pixbuf> display_image, create_tool_func factory_func, bool root=false);
	virtual ~ModelElementType();
	
	std::string getClassType() const;
	Glib::ustring getDisplayName() const;
	Glib::RefPtr<Gdk::Pixbuf> getDisplayImage() const;
	bool isRoot() const;
	
	virtual shared_ptr< ITool > createInsertTool();
	
	bool operator== (const ModelElementType& rhs) const;
	
protected:
	std::string class_type_;
	Glib::ustring display_name_;
	Glib::RefPtr<Gdk::Pixbuf> display_image_;
	bool root_;
	shared_ptr< IBoundsModelElement > bounds_model_element_;
	create_tool_func factory_func_;
};

}

#endif // MODELELEMENTTYPE_H

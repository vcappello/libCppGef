#include "model_element_type.h"

#include <gtkmm.h>

#include <insert_element_tool.h>

namespace cppgef
{

ModelElementType::ModelElementType(const std::string& class_type, bool root) :
	class_type_( class_type ),
	display_name_( class_type ),
	root_( root ),
	factory_func_( 0 )
{
	Gtk::Invisible w;
	
#ifdef GTKMM_3
	display_image_ = w.render_icon_pixbuf (Gtk::Stock::MISSING_IMAGE, Gtk::ICON_SIZE_LARGE_TOOLBAR);
#else
	display_image_ = w.render_icon(Gtk::Stock::MISSING_IMAGE, Gtk::ICON_SIZE_LARGE_TOOLBAR);
#endif	
}

ModelElementType::ModelElementType(const std::string& class_type, const Glib::ustring& display_name, bool root) :
	class_type_( class_type ),
	display_name_( display_name ),
	root_( root ),
	factory_func_( 0 )
{
	Gtk::Invisible w;
#ifdef GTKMM_3
	display_image_ = w.render_icon_pixbuf (Gtk::Stock::MISSING_IMAGE, Gtk::ICON_SIZE_LARGE_TOOLBAR);
#else
	display_image_ = w.render_icon(Gtk::Stock::MISSING_IMAGE, Gtk::ICON_SIZE_LARGE_TOOLBAR);
#endif	
}

ModelElementType::ModelElementType(const std::string& class_type, const Glib::ustring& display_name, Glib::RefPtr<Gdk::Pixbuf> display_image, bool root) :
	class_type_( class_type ),
	display_name_( display_name ),
	display_image_( display_image ),
	root_( root ),
	factory_func_( 0 )
{
}

ModelElementType::ModelElementType(const std::string& class_type, const Glib::ustring& display_name, Glib::RefPtr<Gdk::Pixbuf> display_image, shared_ptr< IBoundsModelElement > bounds_model_element, bool root) :
	class_type_( class_type ),
	display_name_( display_name ),
	display_image_( display_image ),
	root_( root ),
	bounds_model_element_( bounds_model_element ),
	factory_func_( 0 )
{
}

ModelElementType::ModelElementType(const std::string& class_type, const Glib::ustring& display_name, Glib::RefPtr<Gdk::Pixbuf> display_image, create_tool_func factory_func, bool root) :
	class_type_( class_type ),
	display_name_( display_name ),
	display_image_( display_image ),
	root_( root ),
	factory_func_( factory_func )
{
	
}

ModelElementType::~ModelElementType()
{
}

std::string ModelElementType::getClassType() const
{
	return class_type_;
}

Glib::RefPtr<Gdk::Pixbuf> ModelElementType::getDisplayImage() const
{
	return display_image_;
}

Glib::ustring ModelElementType::getDisplayName() const
{
	return display_name_;
}

bool ModelElementType::isRoot() const
{
	return root_;
}

shared_ptr< ITool > ModelElementType::createInsertTool()
{
	if (bounds_model_element_)
	{
		shared_ptr< InsertElementTool > tool( new InsertElementTool(bounds_model_element_) );
		tool->setClassType (class_type_);
		
		return tool;
	}
	else
	{
		shared_ptr< ITool > tool( factory_func_() );
		
		return tool;
	}
	
	return shared_ptr< ITool >();
}

bool ModelElementType::operator== (const ModelElementType& rhs) const
{
	return class_type_ == rhs.class_type_;
}

}




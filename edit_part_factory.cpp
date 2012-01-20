#include "edit_part_factory.h"

#include <glibmm/i18n.h>

#include <diagram.h>
#include <diagram_edit_part.h>
#include <rectangle_shape.h>
#include <rectangle_shape_edit_part.h>
#include <ellipse_shape.h>
#include <ellipse_shape_edit_part.h>
#include <frame.h>
#include <frame_edit_part.h>
#include <line_base.h>
#include <line_edit_part.h>
#include <curve_line.h>
#include <curve_edit_part.h>
#include <text_shape.h>
#include <text_edit_part.h>

#include <bounds_shape.h>
#include <bounds_frame.h>
#include <bounds_line.h>
#include <bounds_curve.h>
#include <insert_curve_tool.h>

#include <iostream>

namespace cppgef
{

EditPartFactory* EditPartFactory::instance_ = 0;

EditPartFactory::EditPartFactory()
{
	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(RectangleShape).name(), 
			Glib::ustring(_("Rectangle")),
			loadPixbuf ("draw-rectangle.png"),
			shared_ptr< IBoundsModelElement >( new BoundsShape() ) )), 
		&createObject< RectangleShapeEditPart >);

	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(EllipseShape).name(), 
			Glib::ustring(_("Ellipse")),
			loadPixbuf ("draw-ellipse.png"),
			shared_ptr< IBoundsModelElement >( new BoundsShape() ) )), 
		&createObject< EllipseShapeEditPart >);
		
	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(Frame).name(), 
			Glib::ustring(_("Frame")),
			loadPixbuf ("draw-rectangle.png"),
			shared_ptr< IBoundsModelElement >( new BoundsFrame() ) )), 
		&createObject< FrameEditPart >);
		
	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(LineBase).name(), 
			Glib::ustring(_("Line")),
			loadPixbuf ("draw-line.png"),
			shared_ptr< IBoundsModelElement >( new BoundsLine() ) )), 
		&createObject< LineEditPart >);
		
	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(CurveLine).name(), 
			Glib::ustring(_("Curve")),
			loadPixbuf ("draw-curve.png"),
			&createInsertTool< InsertCurveTool > )), 
		&createObject< CurveEditPart >);

	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(TextShape).name(), 
			Glib::ustring(_("Text")),
			loadPixbuf ("draw-text.png"),
			shared_ptr< IBoundsModelElement >( new BoundsShape() ) )), 
		&createObject< TextEditPart >);
		
	registerFactory (shared_ptr< ModelElementType >(
		new ModelElementType( typeid(Diagram).name(), Glib::ustring(_("Diagram")), true)), 
		&createObject< DiagramEditPart >);
}

EditPartFactory::~EditPartFactory()
{
}

EditPartFactory* EditPartFactory::getInstance()
{
	if(instance_ == 0)
	{
		instance_ = new EditPartFactory();
	}
	return instance_;
}

void EditPartFactory::release()
{
	if(instance_)
	{
		delete instance_;
	}
	instance_ = 0;
}

void EditPartFactory::registerFactory(shared_ptr< ModelElementType > model_element_type, create_object_func factory_func)
{
	if (registry_.find(model_element_type->getClassType()) != registry_.end())
	{
		throw Error(
			Glib::ustring::compose(_("Can not register type '%1', a factory for this type is already registered."), model_element_type->getClassType()));
	}
	
	registry_[model_element_type->getClassType()] = factory_func;
	registered_types_[model_element_type->getClassType()] = model_element_type;
}

void EditPartFactory::unregisterFactory(const std::string& type_name)
{
	if (registry_.find(type_name) == registry_.end())
	{
		throw Error(
			Glib::ustring::compose(_("Can not unregister type '%1', a factory for this type is not registered."), type_name));
	}
	
	registry_.erase (type_name);
	registered_types_.erase (type_name);
}

shared_ptr< IEditPart > EditPartFactory::createEditPart(shared_ptr< ModelElementBase > model_element)
{
	std::map< std::string, create_object_func >::iterator iter = registry_.find (typeid(*model_element).name());

	if (iter == registry_.end())
	{
		throw Error(
			Glib::ustring::compose(_("Can not create an EditPart for type '%1', a factory for this type is not registered."), typeid(*model_element).name()));
	}
	
	shared_ptr< IEditPart > edit_part = ((*iter).second)();
	edit_part->setModel (model_element);
	
	return edit_part;
}

shared_ptr< IEditPart > EditPartFactory::createEditPart(const std::string& type_name)
{
	std::map< std::string, create_object_func >::iterator iter = registry_.find (type_name);

	if (iter == registry_.end())
	{
		throw Error(
			Glib::ustring::compose(_("Can not create an EditPart for type '%1', a factory for this type is not registered."), type_name));
	}

	shared_ptr< IEditPart > edit_part = ((*iter).second)();
	edit_part->setModel (edit_part->createModelElement());
	
	return edit_part;	
}

bool EditPartFactory::existFactory(const std::string& type_name) const
{
	if (registry_.find(type_name) == registry_.end())
		return false;

	return true;
}

shared_ptr< ModelElementType > EditPartFactory::getModelElementType(const std::string& type_name)
{
	if (registered_types_.find(type_name) == registered_types_.end())
	{
		throw Error(
			Glib::ustring::compose(_("Can not retrieve the ModelElementType for type '%1', a factory for this type is not registered."), type_name));
	}
	
	return registered_types_[type_name];
}

EditPartFactory::iterator EditPartFactory::getTypesBegin()
{
	return registered_types_.begin();
}

EditPartFactory::const_iterator EditPartFactory::getTypesBegin() const
{
	return registered_types_.begin();
}

EditPartFactory::iterator EditPartFactory::getTypesEnd()
{
	return registered_types_.end();
}

EditPartFactory::const_iterator EditPartFactory::getTypesEnd() const
{
	return registered_types_.end();
}

Glib::RefPtr<Gdk::Pixbuf> EditPartFactory::loadPixbuf(const std::string& file_name)
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;

	try
	{
		pixbuf = Gdk::Pixbuf::create_from_file (file_name);
	}
	catch (const Glib::Exception& err)
	{
		Gtk::Image stock_image;

#ifdef GTKMM_3
		pixbuf = stock_image.render_icon_pixbuf (Gtk::StockID("gtk-missing-image"), Gtk::ICON_SIZE_SMALL_TOOLBAR);
#else
		pixbuf = stock_image.render_icon (Gtk::StockID("gtk-missing-image"), Gtk::ICON_SIZE_SMALL_TOOLBAR);
#endif

	}
	
	return pixbuf;
}

}



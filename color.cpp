/*
 * color.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: vincenzo
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "color.h"

#include <boost/serialization/nvp.hpp>

#include <sstream>

template
void cppgef::Color::save(boost::archive::text_oarchive& ar, const unsigned int version) const;

template
void  cppgef::Color::load(boost::archive::text_iarchive& ar, const unsigned int version);

template
void cppgef::Color::save(boost::archive::xml_oarchive& ar, const unsigned int version) const;

template
void  cppgef::Color::load(boost::archive::xml_iarchive& ar, const unsigned int version);

const double MAX_CHANNEL_VALUE = 65535;

namespace cppgef
{

Color::Color() :
	color_()
#ifndef GTKMM_3
	,alpha_( 1.0 )
#endif
{
}

Color::Color(const Color& rhs) :
	color_( rhs.color_ )
#ifndef GTKMM_3
	,alpha_( rhs.alpha_ )
#endif
{
	
}

Color::Color(const Glib::ustring& color) :
	color_( color )
#ifndef GTKMM_3
	,alpha_( 1.0 )
#endif
{
}

#ifdef GTKMM_3
Color::Color(const Gdk::RGBA& color) :
	color_( color )
{

}
#else
Color::Color(const Gdk::Color& color) :
	color_( color )
{

}
#endif

Color::~Color()
{
}

////////////////////////////////////////////////////////////////////
#ifdef GTKMM_3

void Color::set(const Glib::ustring& color)
{
	color_.set (color);
}

void Color::setFromDialog(const Gtk::ColorSelectionDialog& dialog)
{
	color_ = dialog.get_color_selection()->get_current_rgba();
}

void Color::setRGB(double r, double g, double b)
{
	color_.set_rgba (r, g, b, 1.0);
}

void Color::setRGBA(double r, double g, double b, double a)
{
	color_.set_rgba (r, g, b, a);
}

double Color::getR() const
{
	return color_.get_red();
}

void Color::setR(double value)
{
	color_.set_red (value);
}

double Color::getG() const
{
	return color_.get_green();
}

void Color::setG(double value)
{
	color_.set_green (value);
}

double Color::getB() const
{
	return color_.get_blue();
}

void Color::setB(double value)
{
	color_.set_blue (value);
}

double Color::getA() const
{
	return color_.get_alpha();
}

void Color::setA(double value)
{
	color_.set_alpha (value);
}

Gdk::Color Color::getGdkColor() const
{
	Gdk::Color c;

	c.set_rgb_p (color_.get_red(), color_.get_green(), color_.get_blue());
	return c;
}

Glib::ustring Color::toString() const
{
	std::stringstream ss;
	
	ss << std::hex << color_.get_red();
	ss << std::hex << color_.get_green();
	ss << std::hex << color_.get_blue();
	ss << std::hex << color_.get_alpha();
	
	return ss.str();
}

void Color::useInCairo(Cairo::RefPtr< Cairo::Context > context)
{
	context->set_source_rgba (color_.get_red(), color_.get_green(), color_.get_blue(), color_.get_alpha());
}

void Color::useInCairoRev(Cairo::RefPtr< Cairo::Context > context)
{
	context->set_source_rgba (color_.get_blue(), color_.get_green(), color_.get_red(), color_.get_alpha());
}

template<class Archive>
void Color::save(Archive& ar, const unsigned int version) const 
{
	double r = color_.get_red();
	double g = color_.get_green();
	double b = color_.get_blue();
	double a = color_.get_alpha();

	ar & BOOST_SERIALIZATION_NVP(r);
	ar & BOOST_SERIALIZATION_NVP(g);
	ar & BOOST_SERIALIZATION_NVP(b);
	ar & BOOST_SERIALIZATION_NVP(a);
}

template<class Archive>
void Color::load(Archive& ar, const unsigned int version) 
{
	double r = 0;
	double g = 0;
	double b = 0;
	double a = 0;

	ar & BOOST_SERIALIZATION_NVP(r);
	ar & BOOST_SERIALIZATION_NVP(g);
	ar & BOOST_SERIALIZATION_NVP(b);
	ar & BOOST_SERIALIZATION_NVP(a);
	
	color_.set_rgba (r, g, b, a);
}

bool Color::operator== (const Color& rhs)
{
	return color_ == rhs.color_;
}

bool Color::operator!= (const Color& rhs)
{
	return color_ != rhs.color_;
}

const Color& Color::operator= (const Color& rhs)
{
	color_ = rhs.color_;
	return *this;
}

////////////////////////////////////////////////////////////////////
#else

void Color::set(const Glib::ustring& color)
{
	color_.set (color);
	alpha_ = 1;
}
	
void Color::setFromDialog(const Gtk::ColorSelectionDialog& dialog)
{
	color_ = dialog.get_color_selection()->get_current_color();
	alpha_ = (double)dialog.get_color_selection()->get_current_alpha() / MAX_CHANNEL_VALUE;
}

void Color::setRGB(double r, double g, double b)
{
	color_.set_rgb_p (r, g, b);
	alpha_ = 1;
}

void Color::setRGBA(double r, double g, double b, double a)
{
	color_.set_rgb_p (r, g, b);
	alpha_ = a;
}

double Color::getR() const
{
	return color_.get_red_p();
}

void Color::setR(double value)
{
	color_.set_red (value * MAX_CHANNEL_VALUE);
}

double Color::getG() const
{
	return color_.get_green_p();
}

void Color::setG(double value)
{
	color_.set_green (value * MAX_CHANNEL_VALUE);
}

double Color::getB() const
{
	return color_.get_blue_p();
}

void Color::setB(double value)
{
	color_.set_blue (value * MAX_CHANNEL_VALUE);
}

double Color::getA() const
{
	return alpha_;
}

void Color::setA(double value)
{
	alpha_ = value;
}

Gdk::Color Color::getGdkColor() const
{
	return color_;
}

Glib::ustring Color::toString() const
{
	std::stringstream ss;
	
	ss << std::hex << color_.get_red_p();
	ss << std::hex << color_.get_green_p();
	ss << std::hex << color_.get_blue_p();
	ss << std::hex << alpha_;
	
	return ss.str();
}

void Color::useInCairo(Cairo::RefPtr< Cairo::Context > context)
{
	context->set_source_rgba (color_.get_red_p(), color_.get_green_p(), color_.get_blue_p(), alpha_);
}

void Color::useInCairoRev(Cairo::RefPtr< Cairo::Context > context)
{
	context->set_source_rgba (color_.get_blue_p(), color_.get_green_p(), color_.get_red_p(), alpha_);
}

template<class Archive>
void Color::save(Archive& ar, const unsigned int version) const 
{
	double r = color_.get_red_p();
	double g = color_.get_green_p();
	double b = color_.get_blue_p();
	double a = alpha_;

	ar & BOOST_SERIALIZATION_NVP(r);
	ar & BOOST_SERIALIZATION_NVP(g);
	ar & BOOST_SERIALIZATION_NVP(b);
	ar & BOOST_SERIALIZATION_NVP(a);
}

template<class Archive>
void Color::load(Archive& ar, const unsigned int version) 
{
	double r = 0;
	double g = 0;
	double b = 0;
	double a = 0;

	ar & BOOST_SERIALIZATION_NVP(r);
	ar & BOOST_SERIALIZATION_NVP(g);
	ar & BOOST_SERIALIZATION_NVP(b);
	ar & BOOST_SERIALIZATION_NVP(a);
	
	setRGBA (r, g, b, a);
}

bool Color::operator== (const Color& rhs)
{
	return color_ == rhs.color_ && float_equ (alpha_, rhs.alpha_);
}

bool Color::operator!= (const Color& rhs)
{
	return color_ != rhs.color_ || float_not_equ (alpha_, rhs.alpha_);
}

const Color& Color::operator= (const Color& rhs)
{
	color_ = rhs.color_;
	alpha_ = rhs.alpha_;
	
	return *this;
}

#endif

} /* namespace cppgef */

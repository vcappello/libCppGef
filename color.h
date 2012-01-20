/*
 * color.h
 *
 *  Created on: Oct 30, 2011
 *      Author: vincenzo
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <gtkmm.h>

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>

#include <float_cmp.h>

namespace cppgef
{

class Color
{
public:
	Color();
	Color(const Color& rhs);
	Color(const Glib::ustring& color);

#ifdef GTKMM_3
	Color(const Gdk::RGBA& color);
#else
	Color(const Gdk::Color& color);
#endif

	virtual ~Color();

	void set(const Glib::ustring& color);

	void setFromDialog(const Gtk::ColorSelectionDialog& dialog);

	void setRGB(double r, double g, double b);
	void setRGBA(double r, double g, double b, double a);

	double getR() const;
	void setR(double value);

	double getG() const;
	void setG(double value);

	double getB() const;
	void setB(double value);

	double getA() const;
	void setA(double value);

	Gdk::Color getGdkColor() const;

	Glib::ustring toString() const;

	void useInCairo(Cairo::RefPtr< Cairo::Context > context);
	void useInCairoRev(Cairo::RefPtr< Cairo::Context > context);

	bool operator== (const Color& rhs);
	bool operator!= (const Color& rhs);

	const Color& operator= (const Color& rhs);
	
protected:

#ifdef GTKMM_3
	Gdk::RGBA color_;
#else
	Gdk::Color color_;
	double alpha_;
#endif

private:
	// Serialization
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const;

	template<class Archive>
	void load(Archive& ar, const unsigned int version);

	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

} /* namespace cppgef */
#endif /* COLOR_H_ */

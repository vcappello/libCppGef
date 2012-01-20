#ifndef FRAME_H
#define FRAME_H

#include <complex_model_element_base.h>
#include <i_text_element.h>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <sigc++/sigc++.h>
#include <algorithm>

#include <error.h>
#include <color.h>
#include <settings.h>
#include <rectangle.h>
#include <float_cmp.h>
#include <i_support_line_style.h>
#include <i_support_fill_style.h>

using boost::shared_ptr;
using std::vector;

namespace cppgef
{

class Frame : public cppgef::ComplexModelElementBase,
			  public cppgef::ITextElement,
			  public cppgef::ISupportLineStyle,
			  public cppgef::ISupportFillStyle

{
public:
	Frame();
	virtual ~Frame();
	
	Rectangle& getBounds();
	void setBounds(const Rectangle& value);

	void setTitle(const Glib::ustring& value);
	Glib::ustring getTitle() const;
	
	cppgef::Color getLineColor() const;
	void setLineColor(const cppgef::Color& value);

	double getLineWidth() const;
	void setLineWidth(double value);
	
	std::vector< double > getLineDashStyle() const;
	void setLineDashStyle(const std::vector< double >& value);
	
	double getLineDashStyleOffset() const;
	void setLineDashStyleOffset(double value);
	
	cppgef::Color getFillColor() const;
	void setFillColor(const cppgef::Color& value);
	
	Glib::ustring getText() const;
	void setText(const Glib::ustring& value);
	
	signal_property_changed_t signalBoundsChanged();
	signal_property_changed_t signalTitleChanged();
	signal_property_changed_t signalLineColorChanged();
	signal_property_changed_t signalFillColorChanged();
	signal_property_changed_t signalLineWidthChanged();
	signal_property_changed_t signalLineDashStyleChanged();
	signal_property_changed_t signalLineDashStyleOffsetChanged();
	
protected:
	Rectangle bounds_;
	Glib::ustring title_;
	cppgef::Color line_color_;
	double line_width_;
	std::vector< double > line_dash_style_;
	double line_dash_style_offset_;
	cppgef::Color fill_color_;
	
	// Signal instances
	signal_property_changed_t signal_bounds_changed_;
	signal_property_changed_t signal_title_changed_;
	signal_property_changed_t signal_line_color_changed_;
	signal_property_changed_t signal_line_width_changed_;
	signal_property_changed_t signal_line_dash_style_changed_;
	signal_property_changed_t signal_line_dash_style_offset_changed_;
	signal_property_changed_t signal_fill_color_changed_;

protected:
	void onBoundsRectangleChanged();

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // FRAME_H

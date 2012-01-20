#ifndef SHAPE_H
#define SHAPE_H

#include <model_element_base.h> // Base class

#include <boost/serialization/access.hpp>
#include <algorithm>

#include <color.h>
#include <rectangle.h>
#include <settings.h>
#include <float_cmp.h>
#include <i_support_line_style.h>
#include <i_support_fill_style.h>

namespace cppgef
{

class ShapeBase : public cppgef::ModelElementBase,
				  public cppgef::ISupportLineStyle,
				  public cppgef::ISupportFillStyle
{
public:
	ShapeBase();
	virtual ~ShapeBase();

	Rectangle& getBounds();
	void setBounds(const Rectangle& value);
	
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
	
	signal_property_changed_t signalBoundsChanged();
	signal_property_changed_t signalLineColorChanged();
	signal_property_changed_t signalLineWidthChanged();
	signal_property_changed_t signalLineDashStyleChanged();
	signal_property_changed_t signalLineDashStyleOffsetChanged();
	signal_property_changed_t signalFillColorChanged();
	
protected:
	Rectangle bounds_;
	cppgef::Color line_color_;
	double line_width_;
	std::vector< double > line_dash_style_;
	double line_dash_style_offset_;
	cppgef::Color fill_color_;

	// Signal instances
	signal_property_changed_t signal_bounds_changed_;
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

#endif // SHAPE_H

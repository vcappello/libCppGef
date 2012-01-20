#ifndef LINEBASE_H
#define LINEBASE_H

#include <model_element_base.h> // Base class: cppgef::IModelElement

#include <boost/serialization/access.hpp>
#include <algorithm>

#include <color.h>
#include <point.h>
#include <settings.h>
#include <float_cmp.h>
#include <i_support_line_style.h>

namespace cppgef
{

/**
 * @class LineBase
 * @author vincenzo.cappello
 * @brief Simple line model
 */
class LineBase : public cppgef::ModelElementBase,
				 public cppgef::ISupportLineStyle
{
public:
	LineBase();
	virtual ~LineBase();
	
	Point& getStartPoint();
	void setStartPoint(const Point& value);
	
	Point& getEndPoint();
	void setEndPoint(const Point& value);

	cppgef::Color getLineColor() const;
	void setLineColor(const cppgef::Color& value);
	
	double getLineWidth() const;
	void setLineWidth(double value);
	
	std::vector< double > getLineDashStyle() const;
	void setLineDashStyle(const std::vector< double >& value);
	
	double getLineDashStyleOffset() const;
	void setLineDashStyleOffset(double value);
	
	signal_property_changed_t signalStartPointChanged();
	signal_property_changed_t signalEndPointChanged();
	signal_property_changed_t signalLineColorChanged();
	signal_property_changed_t signalLineWidthChanged();
	signal_property_changed_t signalLineDashStyleChanged();
	signal_property_changed_t signalLineDashStyleOffsetChanged();
	
protected:
	Point start_point_;
	Point end_point_;
	cppgef::Color line_color_;
	double line_width_;
	std::vector< double > line_dash_style_;
	double line_dash_style_offset_;
	
	// Signal instances
	signal_property_changed_t signal_start_point_changed_;
	signal_property_changed_t signal_end_point_changed_;
	signal_property_changed_t signal_line_color_changed_;
	signal_property_changed_t signal_line_width_changed_;
	signal_property_changed_t signal_line_dash_style_changed_;
	signal_property_changed_t signal_line_dash_style_offset_changed_;
	
protected:
	void onStartPointChanged();
	void onEndPointChanged();
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // LINEBASE_H

/*
 * i_support_line_style.h
 *
 *  Created on: Nov 10, 2011
 *      Author: vincenzo
 */

#ifndef I_SUPPORT_LINE_STYLE_H_
#define I_SUPPORT_LINE_STYLE_H_

#include <vector>

#include <color.h>

namespace cppgef
{

class ISupportLineStyle
{
public:
	ISupportLineStyle()
	{
	}
	virtual ~ISupportLineStyle()
	{
	}

	virtual cppgef::Color getLineColor() const = 0;
	virtual void setLineColor(const cppgef::Color& value) = 0;

	virtual double getLineWidth() const = 0;
	virtual void setLineWidth(double value) = 0;

	virtual std::vector< double > getLineDashStyle() const = 0;
	virtual void setLineDashStyle(const std::vector< double >& value) = 0;

	virtual double getLineDashStyleOffset() const = 0;
	virtual void setLineDashStyleOffset(double value) = 0;

};

}

#endif /* I_SUPPORT_LINE_STYLE_H_ */

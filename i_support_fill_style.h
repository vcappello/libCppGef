/*
 * i_support_fill_style.h
 *
 *  Created on: Nov 10, 2011
 *      Author: vincenzo
 */

#ifndef I_SUPPORT_FILL_STYLE_H_
#define I_SUPPORT_FILL_STYLE_H_

#include <vector>

#include <color.h>

namespace cppgef
{

class ISupportFillStyle
{
public:
	ISupportFillStyle()
	{
	}
	virtual ~ISupportFillStyle()
	{
	}

	virtual cppgef::Color getFillColor() const = 0;
	virtual void setFillColor(const cppgef::Color& value) = 0;
};

}

#endif /* I_SUPPORT_FILL_STYLE_H_ */

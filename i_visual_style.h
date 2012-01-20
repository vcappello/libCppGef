#ifndef IVISUALSTYLE_H
#define IVISUALSTYLE_H

#include <cairomm/context.h>

namespace cppgef
{

class IVisualStyle
{

public:
	IVisualStyle()
	{
	}
	virtual ~IVisualStyle()
	{
	}

	virtual void apply(Cairo::RefPtr< Cairo::Context > context) = 0;
};

}

#endif // IVISUALSTYLE_H

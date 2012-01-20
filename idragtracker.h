#ifndef IDRAGTRACKER_H
#define IDRAGTRACKER_H

#include <point.h>
#include <cairomm/context.h>

namespace cppgef
{

class IDragTracker
{

public:
	IDragTracker()
	{
	}
	virtual ~IDragTracker()
	{
	}

	virtual void dragBegin (const Point& point) = 0;
	virtual void dragTo (const Point& point) = 0;
	virtual void dragComplete (const Point& point) = 0;
	virtual void dragCancel (const Point& point) = 0;
	virtual void paint (Cairo::RefPtr< Cairo::Context > context) = 0;
};

}

#endif // IDRAGTRACKER_H

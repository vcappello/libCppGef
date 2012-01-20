#ifndef SIGNALDISCONNECT_H
#define SIGNALDISCONNECT_H

#include <sigc++/connection.h>

namespace cppgef
{

class SignalDisconnect
{
public:
	SignalDisconnect(const sigc::connection& connection);
	virtual ~SignalDisconnect();

protected:
	sigc::connection connection_;
};

}

#endif // SIGNALDISCONNECT_H

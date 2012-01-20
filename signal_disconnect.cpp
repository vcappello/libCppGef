#include "signal_disconnect.h"

namespace cppgef
{

SignalDisconnect::SignalDisconnect(const sigc::connection& connection) :
	connection_( connection )
{
}

SignalDisconnect::~SignalDisconnect()
{
	connection_.disconnect();
}

}


#include "error.h"

namespace cppgef
{

Error::Error(const Glib::ustring& message) :
	std::runtime_error( message )
{
}

Error::~Error() throw()
{
}

}




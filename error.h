#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>
#include <glibmm/ustring.h>

namespace cppgef
{

/**
 * @class Error
 * @author vincenzo.cappello
 * @date 12/15/10
 * @file error.h
 * 
 * @brief Generic error
 */
class Error : public std::runtime_error
{
public:
	Error(const Glib::ustring& message);
	virtual ~Error() throw();
};

}

#endif // ERROR_H

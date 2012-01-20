#ifndef USTRING_SERIALIZATION_H_
#define USTRING_SERIALIZATION_H_

#include <boost/serialization/utility.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/array.hpp> 
#include <boost/serialization/string.hpp>

#include <glibmm/ustring.h>

// Add serialization capabilities to Glib::ustring

BOOST_SERIALIZATION_SPLIT_FREE(Glib::ustring)

namespace boost
{
	
namespace serialization
{

// Glib::ustring
template< class Archive >
inline void save(
    Archive& ar,
    const Glib::ustring& t,
    const unsigned int /* file_version */
)
{
	ar << t.raw();
}

template< class Archive >
inline void load(
    Archive& ar,
    Glib::ustring& t,
    const unsigned int /* file_version */
)
{
	std::string buffer;
	ar >> buffer;
	
	t = buffer;
}

}

}

#endif // USTRING_SERIALIZATION_H_


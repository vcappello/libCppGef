/*
 * key_modifier.h
 *
 *  Created on: Feb 2, 2012
 *      Author: vincenzo
 */

#ifndef KEY_MODIFIER_H_
#define KEY_MODIFIER_H_

#include <gtkmm.h>

namespace cppgef
{

class KeyModifier
{
public:
	KeyModifier(GdkModifierType modifier_type);
	virtual ~KeyModifier();

	bool isShift() const;
	bool isLock() const;
	bool isControl() const;
	bool isAlt() const;

protected:
	GdkModifierType modifier_type_;
};

} /* namespace cppgef */
#endif /* KEY_MODIFIER_H_ */

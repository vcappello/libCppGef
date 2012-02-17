/*
 * key_modifier.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: vincenzo
 */

#include "key_modifier.h"

namespace cppgef
{

KeyModifier::KeyModifier(GdkModifierType modifier_type) :
	modifier_type_( modifier_type )
{
}

KeyModifier::~KeyModifier()
{
}

bool KeyModifier::isShift() const
{
	return modifier_type_ & GDK_SHIFT_MASK;
}

bool KeyModifier::isLock() const
{
	return modifier_type_ & GDK_LOCK_MASK;
}

bool KeyModifier::isControl() const
{
	return modifier_type_ & GDK_CONTROL_MASK;
}

bool KeyModifier::isAlt() const
{
	return modifier_type_ & GDK_MOD1_MASK;
}

} /* namespace cppgef */

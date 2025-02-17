/**
 * @file Vector2D.cpp
 * @brief Implementation of a the Vector2D class.
 *
 * @details Most of the implementation can be found in Vector2D.h. Most of the implementation is simply (standard math).
 */

#include <util/Vector2D.h>

Vector2D Vector2D::normalize() {
	float magnitudeStored = magnitude();
	if (magnitudeStored > 0.0f) {
		x /= magnitudeStored;
		y /= magnitudeStored;
	}

	return *this;
}
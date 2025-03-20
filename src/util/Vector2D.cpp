/**
 * @file Vector2D.cpp
 * @brief Implementation of the Vector2D class.
 *
 * This file contains the implementation of the Vector2D class methods.
 */

#include <util/Vector2D.h>

 /**
  * @brief Normalizes the vector.
  *
  * This function modifies the current vector by dividing its components by its magnitude,
  * effectively converting it into a unit vector (a vector with a magnitude of 1).
  * If the magnitude is zero, the function does nothing.
  *
  * @return The normalized vector.
  */
Vector2D Vector2D::normalize() {
    float magnitudeStored = magnitude();
    if (magnitudeStored > 0.0f) {
        x /= magnitudeStored;
        y /= magnitudeStored;
    }
    return *this;
}
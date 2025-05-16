/***********************************************************************
 * Header File:
 *    PHYSICS
 * Author:
 *    Matt Benson
 * Summary:
 *    Laws of motion, effects of gravity, etc.
 ************************************************************************/

#pragma once

#include "acceleration.h"
#include "position.h"
#include "angle.h"
#include "velocity.h"
#define _USE_MATH_DEFINES 
#include <math.h>
#include <cassert>  // for ASSERT 
#include <cmath>    // for abs

/**********************************************************
* ACCELERATION GET ALTITUDE
* Return the altitude of a point above the earth's surface
*********************************************************/
double getAltitude(const Position& posElement);

/**********************************************************
* ACCELERATION GET GRAVITY
* Equation: a = g_0 (R_e / (r_e + h)) ^ 2
 *********************************************************/
Acceleration getGravity(const Position& posElement);

/**********************************************************
* VELOCITY UPDATE VELOCITY
* Update the current velocity
*********************************************************/
Velocity& updateVelocity(Velocity& velocity, const Acceleration& acceleration, double time);

/**********************************************************
* POSITION UPDATE POSITION
* Update the current position
*********************************************************/
Position& updatePosition(Position& pos, const Velocity& vel, const Acceleration& acceleration, double time);

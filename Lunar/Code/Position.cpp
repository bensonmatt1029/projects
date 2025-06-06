/***********************************************************************
 * Source File:
 *    POSITION
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#include "position.h"
#include "velocity.h"
#include "acceleration.h"


 /******************************************
  * POINT : ADD
  * Update point based on the distance formula
  *   s = s_0 + vt + 1/2 a t^2
  *****************************************/
void Position::add(const Acceleration& a, const Velocity& v, double t)
{
    x += (v.getDX() * t) + (0.5 * a.getDDX() * (t * t));
    y += (v.getDY() * t) + (0.5 * a.getDDY() * (t * t));
}

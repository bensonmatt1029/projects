/***********************************************************************
 * Source File:
 *    Airplane
 * Author:
 *    Matt Benson
 * Summary:
 *    All the information about the airplane
 ************************************************************************/

#include "airplane.h"
#include "acceleration.h"

 /***************************************************************
  * RESET
  * Reset the plane and its position to start the game over
  ***************************************************************/
void Airplane :: reset(const Position & posUpperRight)
{
   // straight up
   angle.setUp();

   // the velocity is random
   velocity.setDX(random(-40.0, -35.0));
   velocity.setDY(random(-1.0, 1.0  ));

   // the position is at the right side of the screen
   pos.setMetersX(posUpperRight.getMetersX() - 1.0);
   pos.setMetersY(random(posUpperRight.getMetersY() * 0.35, posUpperRight.getMetersY() * 0.5));

   // status is playing
   status = PLAYING;

   // fill 'er up with fuel
   fuel = FUEL_MAX;
}

/***************************************************************
 * DRAW
 * Draw the plane on the screen
 ***************************************************************/
void Airplane :: draw(const Thrust & thrust, ogstream & gout) const
{
   // draw lander on the screen.
   gout.drawShip(pos, angle.getRadians(), false);
}

/***************************************************************
 * INPUT
 * Accept input from the Pilot
 ***************************************************************/
Acceleration Airplane :: input(const Thrust& thrust, double gravity)
{
   Acceleration a;

   // Always apply gravity
   a.addDDY(gravity);

   // Do nothing if dead or out of fuel
   if (!isFlying() || fuel <= 0.0)
      return a;

   // Rotate left/right (i.e., pitch up/down)
   if (thrust.isClock()) // pitch right
   {
      angle.add(0.05);   // smoother turning
      fuel -= FUEL_ROTATE;
   }
   if (thrust.isCounter()) // pitch left
   {
      angle.add(-0.05);
      fuel -= FUEL_ROTATE;
   }

   // Apply forward/main thrust in direction of pitch
   if (thrust.isMain())
   {
      double power = PLANE_THRUST / PLANE_WEIGHT;
      double radians = angle.getRadians();
      a.addDDX(-sin(radians) * power); // forward X
      a.addDDY(cos(radians) * power);  // forward Y
      fuel -= FUEL_MAIN_THRUST;
   }

   if (fuel < 0.0)
      fuel = 0.0;

   return a;
}

/******************************************************************
 * COAST
 * What happens when we coast?
 *******************************************************************/
void Airplane :: coast(Acceleration & acceleration, double time)
{
   pos.add(acceleration, velocity, time); // update position
   velocity.add(acceleration, time);      // update velocity
}

/******************************************************************
 * APPLY AERODYNAMICS
 * Forces applied to the plane.
 *******************************************************************/
Acceleration Airplane::applyAerodynamics()
{
   Acceleration a;

   if (!isFlying())
      return a;

   double speed = velocity.getSpeed();
   if (speed == 0.0)
      return a;

   double angleOfAttack = angle.getRadians();
   double velAngle = atan2(velocity.getDY(), velocity.getDX());

   double lift = 0.5 * AIR_DENSITY * speed * speed * DRAG_AREA * LIFT;
   double drag = 0.5 * AIR_DENSITY * speed * speed * DRAG_AREA * DRAG;

   double accLift = lift / PLANE_WEIGHT;
   double accDrag = drag / PLANE_WEIGHT;

   // Lift is perpendicular to direction of motion
   a.addDDX(accLift * cos(velAngle + M_PI_2));
   a.addDDY(accLift * sin(velAngle + M_PI_2));

   // Drag is opposite direction of velocity
   a.addDDX(-accDrag * cos(velAngle));
   a.addDDY(-accDrag * sin(velAngle));

   return a;
}

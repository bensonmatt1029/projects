/***********************************************************************
 * Source File:
 *    LANDER
 * Author:
 *    Matt Benson
 * Summary:
 *    All the information about the lunar lander
 ************************************************************************/

#include "lander.h"
#include "acceleration.h"

 /***************************************************************
  * RESET
  * Reset the lander and its position to start the game over
  ***************************************************************/
void Lander :: reset(const Position & posUpperRight)
{
   // straight up
   angle.setUp();

   // the velocity is random
   velocity.setDX(random(-10.0, -4.0));
   velocity.setDY(random(-2.0, 2.0  ));

   // the position is at the right side of the screen
   pos.setX(posUpperRight.getX() - 1.0);
   pos.setY(random(posUpperRight.getY() * 0.75, posUpperRight.getY() * 0.95));

   // status is playing
   status = PLAYING;

   // fill 'er up with fuel
   fuel = FUEL_MAX;
}

/***************************************************************
 * DRAW
 * Draw the lander on the screen
 ***************************************************************/
void Lander :: draw(const Thrust & thrust, ogstream & gout) const
{
   // draw lander on the screen.
   gout.drawLander(pos, angle.getRadians());

   // only fire the engines if we are still flying
   if (isFlying() && fuel > 0.0)
   {
      gout.drawLanderFlames(pos, angle.getRadians(),
         thrust.isMain(), thrust.isClock(), thrust.isCounter());
   }
}

/***************************************************************
 * INPUT
 * Accept input from the Neil Armstrong
 ***************************************************************/
Acceleration Lander :: input(const Thrust& thrust, double gravity)
{
   // Acceleration due to gravity
   Acceleration a;

   // add gravity
   a.addDDY(gravity);

   // are we out of gas?
   if (fuel == 0.0)
      return a;

   // main engines
   if (thrust.isMain())
   {
      double power = (LANDER_THRUST / LANDER_WEIGHT);
      a.addDDX(-sin(angle.getRadians()) * power);
      a.addDDY(cos(angle.getRadians()) * power);
      fuel -= FUEL_MAIN_THRUST;
   }

   // clockwise
   if (thrust.isClock())
   {
      angle.add(0.1);
      fuel -= FUEL_ROTATE;
   }
   // counter clockwise
   if (thrust.isCounter())
   {
      angle.add(-0.1);
      fuel -= FUEL_ROTATE;
   }

   if (fuel < 0.0)
      fuel = 0.0;

   return a;
}

/******************************************************************
 * COAST
 * What happens when we coast?
 *******************************************************************/
void Lander :: coast(Acceleration & acceleration, double time)
{
   pos.add(acceleration, velocity, time); // update position
   velocity.add(acceleration, time);      // update velocity
}


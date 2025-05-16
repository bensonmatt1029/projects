/***********************************************************************
 * Source File:
 *    Ship
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about the Ship
 ************************************************************************/

#include "ship.h"

 /**********************************
  * Ship
  * Create a Ship
  **********************************/
Ship::Ship() : Whole(4000)
{
   // Setup for the ship
   pos.setPixelsX(-450.0);
   pos.setPixelsY(450.0);
   angle.setDegrees(0.0);
   velocity.setDX(0.0);
   velocity.setDY(-2000.0);
   angularVelocity = 0.0;
   dead = false;
   radius = 6.0 * this->pos.getZoom();
   thrust = false;
}

/**********************************
* Ship Input
* Move the Ship
**********************************/
void Ship::input(const Interface& ui, std::list<Satellite*>& satellites)
{
   angle.rotate((ui.isRight() ? 0.1 : 0.0) + (ui.isLeft() ? -0.1 : 0.0));

   if (ui.isDown())
   {
      Velocity vel;
      vel.set(angle, 30.0);
      velocity += vel;
      thrust = true;
   }
   else
   {
      thrust = false; 
   }
   // Handle firing projectiles when the space key is pressed
   if (ui.isSpace())
   {
      // Velocity for the projectile
      Velocity vBullet(9000.0, angle);
      vBullet += velocity;  // Add ship's velocity to the projectile

      // Position for the projectile: 19 pixels in front of the ship
      Position posShipFront = pos;  // Start at the ship's position
      posShipFront.addMetersX(19.0 * cos(angle.getRadians()));  // Offset in the X-direction
      posShipFront.addMetersY(19.0 * sin(angle.getRadians()));  // Offset in the Y-direction

      // Create and add the projectile
      Satellite* pBullet = new Projectile(*this, vBullet);
      satellites.push_back(pBullet);
   }
}

/**********************************
* Ship Destroy
* Destroy the Ship
**********************************/
void Ship::destroy(std::list <Satellite*>& satellites)
{
   for (double degrees = 0.0; degrees <= 360.0; degrees += 90.0)
      satellites.push_back(new Fragment(*this, Angle(degrees)));
}

/**********************************
* Ship Move
* Move the Ship using the input
**********************************/
void Ship::move(double timeDilation, const Interface& ui, std::list<Satellite*>& satellites)
{
   // First, handle input (thrust, rotation, firing bullets)
   input(ui, satellites);

   // Skip if the ship is dead
   if (isDead()) return;

   // Calculate gravity's effect on the ship
   Acceleration aGravity = getGravity(pos);

   // Update velocity due to gravity
   updateVelocity(velocity, aGravity, timeDilation); // Gravity influences velocity

   // Update position based on velocity (including time dilation)
   updatePosition(pos, velocity, aGravity, timeDilation); // Update position

   // Apply thrust if the main engine is on (direct thrust application)
   if (ui.isDown()) // If thrust is active (checked in input())
   {
      Acceleration mainThrust(3.0, angle); // Create thrust in direction of angle (without using Thrust class)
      velocity.add(mainThrust, timeDilation); // Add thrust to velocity over time
   }

   // Handle rotation based on input (clockwise or counterclockwise)
   angle.rotate(angularVelocity * timeDilation); // Apply angular velocity to rotation
}

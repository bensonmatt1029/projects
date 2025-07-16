/***********************************************************************
 * Header File:
 *    Airplane
 * Author:
 *    Matt Benson
 * Summary:
 *    All the information about the airplane
 ************************************************************************/

#pragma once

#include "position.h"  // for POINT
#include "ground.h"
#include "velocity.h"  // for VELOCITY
#include "thrust.h"    // for THRUST
#include "uiDraw.h"    // for DRAW* and RANDOM
#include "angle.h"     // for ANGLE

 /****************************************************************
  * Airplane CONSTANTS
  * All the constants pertaining to the airplane itself.
  ***************************************************************/

#define PLANE_WIDTH     27.61      // width of the lander
#define MAX_SPEED       35.118       // maximum speed to land
#define PLANE_WEIGHT    41140.00  // weight of the lander in KG
#define PLANE_THRUST    107000.00  // thrust to main engine in N
#define FUEL_MAIN_THRUST 10.0      // fuel consumed by main thruster
#define FUEL_ROTATE      1.0       // fuel consumed by the rotation thrusters
#define FUEL_MAX         5000.0    // total amount of fuel
#define LIFT             1.2
#define DRAG             0.3
#define DRAG_AREA        26.4
#define AIR_DENSITY      1.225


enum Status { PLAYING, SAFE, DEAD };

class TestLander;

/****************************************************************
 * AIRPLANE
 * The position and status of the plane
 ***************************************************************/
class Airplane
{
   
public:
   // to create a plane, we need to know something about the board
   Airplane(const Position& posUpperRight) : status(PLAYING) { reset(posUpperRight); }

   // reset the plane and its position
   void reset(const Position& posUpperRight);

   // get the status of the plane
   bool     isDead()         const { return status == DEAD;      }
   bool     isLanded()       const { return status == SAFE;      }
   bool     isFlying()       const { return status == PLAYING;   }
   Position getPosition()    const { return pos;                 }
   double   getSpeed()       const { return velocity.getSpeed(); }
   int      getFuel()        const { return (int)fuel;           }
   int      getWidth()       const { return PLANE_WIDTH;        }
   double   getMaxSpeed()    const { return MAX_SPEED;           }

   // draw the plane on the screen
   void draw(const Thrust & thrust, ogstream & gout) const;

   void setPosition(const Position& p) { pos = p; }
 
   // handle input
   Acceleration input(const Thrust & thrust, double gravity);

   // what happens when we coast?
   void coast(Acceleration & acceleration, double time);

   // the forces applied to the plane
   Acceleration applyAerodynamics(); 

   // straighten the plane and put it on the ground
   void land() 
   { 
      angle.setUp(); 
      status = SAFE;
   }

   // we are dead. Draw the plane upside down
   void crash() 
   { 
      angle.setDown(); 
      status = DEAD; 
   }

private:
   Status   status;      // are we dead or not?
   Position pos;         // the location of the plane
   Velocity velocity;    // the speed of the plane
   Angle    angle;       // the orientation of the plane
   double   fuel;        // amount of fuel remaining
};

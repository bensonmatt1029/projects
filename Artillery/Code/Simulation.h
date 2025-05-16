/**********************************************************************
 * Header File:
 *    SIMULATION
 * Author:
 *    Daniel Malasky and Matt Benson
 * Summary:
 *    Execute one simulation of a projectile being fired.
 ************************************************************************/

#pragma once
#include "position.h"
#include "ground.h"      // for GROUND
#include "howitzer.h"    // for HOWITZER
#include "projectile.h"  // for PROJECTILE
#include "uiInteract.h"  // for INTERFACE

using namespace std;

 /*********************************************
  * Simulation
  * Execute one simulation of a projectile being fired.
  *********************************************/
class Simulator
{
public:
   Simulator(const Position & posUpperRight) :
      ground(posUpperRight)
   {
      howitzer.generatePosition(posUpperRight);
      ground.reset(howitzer.getPosition());
      this->posUpperRight = posUpperRight;
   }

   // display stuff on the screen
   void display();

   // update the simulator for each frame
   void update(const Interface* pUI);

   // handle gameplay rules
   void gameplay(const Interface* pUI);

private:
   Ground ground;
   Howitzer howitzer;
   Projectile projectile;
   Position posUpperRight;
};

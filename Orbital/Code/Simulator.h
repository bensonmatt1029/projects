/***********************************************************************
 * Header File:
 *    Simulator : A class to run the orbit simulator
 * Author:
 *    Matt Benson
 * Summary:
 *    Provides a convenient interface to run the orbital simulator
 ************************************************************************/

#pragma once

#include "uiInteract.h" // for INTERFACE
#include "uiDraw.h"     // for RANDOM and DRAW*
#include "position.h"   // for POINT
#include "star.h"       // for STAR
#include "satellite.h"  // for SATELLITE *
#include "gps.h"        // for GPS
#include "sputnik.h"    // for SPUTNIK
#include "ship.h"       // for DREAM CHASER
#include "crewDragon.h" // for DRAGON
#include "starlink.h"   // for STARLINK
#include "hubble.h"     // for HUBBLE
#include "Test.h"       // for test
#include "physics.h"    // for physics calculations
#include <list>         // for LIST

using namespace std;

/*************************************************************************
 * Simulator
 * A class to handle the orbital logic
 *************************************************************************/
class Simulator
{
public:
   Simulator(const Position ptUpperRight);

   ~Simulator();

   // handle input, updates, and graphics
   void input(const Interface& pUI);
   void move();
   void draw(ogstream& gout);

private:
   list<Satellite*> satellites;    // collection of satellites in orbit
   Star stars[200];
   Position ptUpperRight;
   Position ptEarth;
   double timeDilation;
   double angleEarth;
   Thrust thrust;
   Projectile* proj;
};

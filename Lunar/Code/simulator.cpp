/**********************************************************************
 * Lunar Lander
 * Matt Benson
 * Lunar Lander simulation. This is the Game class and main()
 **********************************************************************/

#include "position.h"     // everything should have a point
#include "angle.h"        // angle of the lander
#include "uiInteract.h"   // for INTERFACE
#include "uiDraw.h"       // for RANDOM and DRAW*
#include "ground.h"       // for GROUND
#include "test.h"         // for the unit tests
#include <cmath>          // for SQRT
#include <cassert>        // for ASSERT
#include "star.h"         // for stars
#include "lander.h"       // for lander
#include "acceleration.h" // for acceleration
#include <vector>         // for star
using namespace std;

#define GRAVITY -1.625    // for acceleration
#define TIME 0.1          // for speed of gameplay

/*************************************************************************
 * SIMULATOR
 * Everything pertaining to the simulator.
 *************************************************************************/
class Simulator
{
public:
   // set up the simulator
   Simulator(const Position& posUpperRight)
      : ground(posUpperRight), lander(posUpperRight)
   {
      // Create stars
      for (int i = 0; i < 50; i++)
      {
         Star star;
         star.reset(posUpperRight.getX(), posUpperRight.getY());
         stars.push_back(star);
      }
   }

   // display stuff on the screen
   void display();

   // update the simulator for each frame
   void update(const Interface* pUI);

   // handle gameplay rules
   void gameplay(const Interface* pUI);

private:
   Ground ground;
   Lander lander;
   Thrust thrust;
   vector<Star> stars; // List of stars in the simulation
};

/**********************************************************
 * DISPLAY
 * Draw on the screen
 **********************************************************/
void Simulator::display()
{
   ogstream gout;

   // draw 50 stars
   for (auto& star : stars)
   {
      star.draw(gout);
   }
   
   // draw the ground
   ground.draw(gout);

   // draw the lander
   lander.draw(thrust, gout);

   gout = Position(20, 350);  // set position of messages
   gout.setf(ios::fixed);     // for double precision
   
   // display the fuel, altitude, and speed
   gout << "Fuel: "     << lander.getFuel()
        << endl;
   gout.precision(0);
   gout << "Altitude: " << floor(ground.getElevation(lander.getPosition()))
        << endl;
   gout.precision(2);
   gout << "Speed: "    << lander.getSpeed()
        << endl;

   // Display landing message
   if (lander.isLanded())
   {
      gout = Position(190, 300);
      gout << "The Eagle has landed! ";
   }

   // Display death message
   if (lander.isDead())
   {
      gout = Position(190, 300);
      gout << "Houston, we have a problem... ";
   }
}

/**********************************************************
 * UPDATE
 * Update the simulator state for each frame
 **********************************************************/
void Simulator::update(const Interface* pUI)
{
   // Update the thrust based on user input
   thrust.set(pUI);

   // Calculate the acceleration based on thrust, input, and gravity
   Acceleration acceleration = lander.input(thrust, GRAVITY);

   // Update the lander's position based on its acceleration and the time step
   if (lander.isFlying())
   {
      lander.coast(acceleration, TIME);
   }
}

/************************************************
 * GAMEPLAY
 * Handle the gameplay rules based on user input,
 * lander position, and lander speed
 ************************************************/
void Simulator::gameplay(const Interface* pUI)
{
   // Check for collision with the ground
   if (ground.hitGround(lander.getPosition(), lander.getWidth()))
   {
      lander.crash();
   }
   // Check for collision with the landing pad
   else if (ground.onPlatform(lander.getPosition(), lander.getWidth()))
   {
      // If you land at a safe speed
      if (lander.getSpeed() < lander.getMaxSpeed())
      {
         lander.land();
      }
      // If you land too fast
      else
      {
         lander.crash();
      }
   }

   // Reset the game from anywhere by pressing space
   if (pUI->isSpace())
   {
      Position posUpperRight(400, 400);  
      lander.reset(posUpperRight);
      ground.reset();
      for (auto& star : stars)
      {
         star.reset(400, 400); 
      }
   }
}

/*************************************
 * CALLBACK
 * Handle one frame of the simulator
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL. 
   Simulator* pSimulator = (Simulator*)p;

   // Update the simulator state
   pSimulator->update(pUI);

   // Check gameplay rules
   pSimulator->gameplay(pUI);

   // Draw the updated game state
   pSimulator->display();
}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my LM type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Run the unit tests
   testRunner();

   // Initialize OpenGL
   Position posUpperRight(400, 400);
   Interface ui("Lunar Lander", posUpperRight);

   // Initialize the game class
   Simulator simulator(posUpperRight);

   // set everything into action
   ui.run(callBack, (void*)&simulator);

   return 0;
}

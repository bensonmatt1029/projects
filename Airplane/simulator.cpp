/**********************************************************************
 * Airplane Simulator
 * Matt Benson
 * Airplane simulation. This is the Game class and main()
 **********************************************************************/

#include "position.h"     // everything should have a point
#include "angle.h"        // angle of the lander
#include "uiInteract.h"   // for INTERFACE
#include "uiDraw.h"       // for RANDOM and DRAW*
#include "ground.h"       // for GROUND
#include "test.h"         // for the unit tests
#include <cmath>          // for SQRT
#include <cassert>        // for ASSERT
#include "airplane.h"       // for lander
#include "acceleration.h" // for acceleration
#include "cloud.h"
#include <vector>         // for star
using namespace std;

#define GRAVITY -1.625    // for acceleration
#define TIME 0.1          // for speed of gameplay

enum class GameState
{
   PLAYING,
   PAUSED
};

/*************************************************************************
 * SIMULATOR
 * Everything pertaining to the simulator.
 *************************************************************************/
class Simulator
{
public:
   // set up the simulator
   Simulator(const Position& posUpperRight)
      : ground(posUpperRight), plane(posUpperRight)
   {
      clouds.push_back(Cloud(100, 850, 40));
      clouds.push_back(Cloud(200, 900, 65));
      clouds.push_back(Cloud(300, 950, 50));
      clouds.push_back(Cloud(400, 850, 60));
      clouds.push_back(Cloud(500, 900, 65));
      clouds.push_back(Cloud(600, 850, 50));
      clouds.push_back(Cloud(700, 850, 50));
      clouds.push_back(Cloud(800, 900, 65));
      clouds.push_back(Cloud(900, 820, 50));
   }

   // display stuff on the screen
   void display();

   // update the simulator for each frame
   void update(const Interface* pUI);

   // handle gameplay rules
   void gameplay(const Interface* pUI);

   GameState getGameState() const { return gameState; }

private:
   Ground ground;
   Airplane plane;
   Thrust thrust;
   vector<Cloud> clouds;
   GameState gameState = GameState::PLAYING;
};

/**********************************************************
 * DISPLAY
 * Draw on the screen
 **********************************************************/
void Simulator::display()
{
   ogstream gout;

   // Draw clouds first (in the sky)
   for (const Cloud& cloud : clouds)
   {
      cloud.draw(gout);
   }

   // draw the ground (runway with stripes)
   ground.draw(gout);

   // draw the control tower
   gout.drawControlTower(Position(30.0, 250.0), 100.0);

   // draw the plane/lander
   plane.draw(thrust, gout);

   gout.setPosition(Position(20, 980));  // set position of messages
   gout.setf(ios::fixed);     // for double precision

   // display the fuel, altitude, and speed
   gout << "Fuel: " << plane.getFuel()
      << endl;
   gout.precision(0);
   gout << "Altitude: " << floor(ground.getElevation(plane.getPosition()))
      << endl;
   gout.precision(2);
   gout << "Speed: " << plane.getSpeed()
      << endl;

   // Display landing message
   if (plane.isLanded())
   {
      gout.setPosition(Position(190, 300));
      gout << "The Eagle has landed! ";
   }

   // Display death message
   if (plane.isDead())
   {
      gout.setPosition(Position(190, 300));
      gout << "Houston, we have a problem... ";
   }

   if (gameState == GameState::PAUSED)
   {
      gout.setPosition(Position(350, 500));
      gout << "=== MENU ===" << endl
         << "P - Resume" << endl
         << "R - Reset" << endl
         << "Esc - Exit";
      return; // Don't draw the rest while paused
   }
}

/**********************************************************
 * UPDATE
 * Update the simulator state for each frame
 **********************************************************/
void Simulator::update(const Interface* pUI)
{
   // Get thrust/gravity acceleration
   thrust.set(pUI);
   Acceleration a1 = plane.input(thrust, GRAVITY);

   // Get aerodynamic acceleration
   Acceleration a2 = plane.applyAerodynamics();

   // Combine the two
   a1.addDDX(a2.getDDX());
   a1.addDDY(a2.getDDY());

   // Move the plane
   if (plane.isFlying())
   {
      plane.coast(a1, TIME);
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
   if (ground.hitGround(plane.getPosition(), plane.getWidth()))
   {
      plane.crash();
   }
   // Check for collision with the runway
   else if (ground.onPlatform(plane.getPosition(), plane.getWidth()))
   {
      // If you land at a safe speed
      if (plane.getSpeed() < plane.getMaxSpeed())
      {
         plane.land();
      }
      // If you land too fast
      else
      {
         plane.crash();
      }
   }

   if (pUI->isP())
      Simulator::gameState = (gameState == GameState::PLAYING) ? GameState::PAUSED : GameState::PLAYING;

   if (gameState == GameState::PAUSED)
   {
      if (pUI->isR())
      {
         plane.reset(Position(1000, 1000));
         ground.reset();
         gameState = GameState::PLAYING;
      }

      if (pUI->isEsc())
      {
         exit(0); // Exit the simulator
      }

      return;
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

   // Only update if not paused
   if (pSimulator->getGameState() == GameState::PLAYING)
   {
      pSimulator->update(pUI);
   }

   pSimulator->gameplay(pUI);
   pSimulator->display();
}

double Position::metersFromPixels = 40.0;

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
   Position ptUpperRight;
   ptUpperRight.setZoom(1.0);
   ptUpperRight.setPixelsX(1000.0);
   ptUpperRight.setPixelsY(1000.0);
   Interface ui(0, NULL,
      "Airplane Simulator",   /* name on the window */
      ptUpperRight);

   // Initialize the game class
   Simulator simulator(ptUpperRight);

   // set everything into action
   ui.run(callBack, (void*)&simulator);


   return 0;
}

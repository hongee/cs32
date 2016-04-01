// main.cpp

#include "globals.h"
#include "robot.h"
#include "player.h"
#include "arena.h"
#include "game.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

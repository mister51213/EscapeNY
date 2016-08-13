#pragma once

#include "Camera.h"
#include "GameView.h"
#include "Input.h"
#include "Actor.h"
#include "Overlay.h"
#include "Includes.h"
#include "Utilities.h"

class ISubGame
{
public:
	// Use Initialize to initialize anything that needs initializing.
	virtual void Initialize(
		Graphics *pGraphics, 
		class Game *const pGame,
		Camera *const pCamera) = 0;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	virtual void UpdateScene( const Input &InputRef, Camera *const pCamera) = 0;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) = 0;

};

// TODO: Discuss freedom/role of sub-games
/*Depending on how much control you want each sub-game to have
It can either store their own master draw list, or have a
virtual std::vector<Actor *> GetMasterDrawList()const = 0;

This would keep Game in control of rendering instead of each sub-game.
So instead of a RenderFrame function you would just have a
GetMasterDrawList function, which game would call to get the current
list of actor pointers and send to GameView::Render.

I personally feel that keeping the control in the sub-game would give
more flexibility during testing.  Eventually, we are going to want to
control other aspects like which shader to use.  All the tests will be
able to be done independantly to eventually make it into the final game.

I believe the role of the sub-games, is to be able to test new features
before implementing them into the final product.  So, while we are
testing, we can see what types of classes need to be made and how to 
restructure the current classes to make interaction between classes simpler.

Final thoughts, keep in mind:
How is your current feature going to fit into the final project?
How is it going to interact with other parts of the game?

I'll explain how the maze game answers those questions. 
First, let me describe the features that are implemented:
	* Camera is pointing downward in a "top-down" view, which IS something
	  you said it would be when we fist discussed making the game.  Having
	  it follow the player is just the result of the mechanics of a maze
	  type game.  If you want full view of the level, you'd just have to 
	  move the camera up higher, and as far as the camera not following
	  the player, that can be eliminated in final.
	* Interaction between input and single player actor
	* Game logic, player interacts with game world by detecting if player
	  has reached the end of the maze.
	  ( Almost like collision detection, feature needs work because it's 
		too sensitive in where the player needs to be to be considered in 
		the end cell )
	* Interaction between with the overlay, when player reaches the end of
	  the maze, a message appears asking if you want to regenerate the 
	  maze.  ( This feature needs work, to much game logic in overlay )

As you can see, the maze game has a few features that will directly help
with the final game and as pointed out, it shows that there is some 
functionality that needs to be re-examined as well.  

Now that I have maze generation, I can work on trying to get the walls to
be cars, work on being able to interact with those cars and make them 
move...so collision detection and inter-class communication 
(between player and the car that needs to be pushed), probably using the board
as the intermediary, since the board should know where all the game objects
are that need interaction.
*/


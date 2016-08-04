/////////////////////////////////////////////////////////////////
// Class name: Actor
// Function: Parent class that holds all information related 
//           to gameplay for a specific object (location, etc.).
// Example:  MovingObject->Car->Mercedes
/////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

struct ModelSpecs_W
{
    XMFLOAT3 position, orientation, scale;
};

struct ModelSpecs_L
{
    XMFLOAT3 center, size, orientation;
};

enum eModType{CUBE, CUBE_TEXTURED, PLANE, SPHERE, POLYGON};

class Actor {





};
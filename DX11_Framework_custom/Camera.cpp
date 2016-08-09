/*****************************************************************************************
 *This camera class will to let DirectX 11 know from where and also how we are viewing   *
 *the scene. The camera class will keep track of where the camera is and its current     *
 *rotation. It will use the position and rotation information to generate a view matrix  *
 *which will be passed into the HLSL shader for rendering.                               *
 *                                                                                       *
 *****************************************************************************************/
#include "CameraALT.h"

// The constructor will initialize the position and rotation of the camera to be at the 
// origin of the scene.

CameraALT::CameraALT()
{
}

CameraALT::~CameraALT()
{
}

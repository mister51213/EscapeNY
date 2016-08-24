#pragma once

#include "ILightDirection.h"
#include "ILightPoint.h"
#include "ILightSpot.h"

//////////////////////////////////////////////////////////////
// If set only contains one type of light, then just return	//
// nullptr for the functions for the other lights, will		//
// have to test for nullptr when getting lights.			//
//															//
// Each concrete child of ILightSet can store a				//
// vector<unique_ptr<ILight*>>, but will have to keep tabs	//
// of which elements of the vector are the different types	//
// of lights.												//
//															//
// The best way to use this class would be to initialize	//
// the light types consecutively, for example; initialize	//
// all directional lights and add them to the set, then		//
// initialize all the point lights and add them to the set, //
// then initialize all the spot lights and add them to the	//
// set.														//
//															//
// By doing it this way, you can calculate where in the		//
// vector each type's list begins, and just return the		//
// pointer to that location in the array.					//
//////////////////////////////////////////////////////////////


class ILightSet
{
public:
	//////////////////////////////////////////////////////////////
	// Use these functions to get the number of each light type //
	// in this light set.										//
	//////////////////////////////////////////////////////////////

	virtual int GetLightDirectionCount()const = 0;
	virtual int GetLightPointCount()const = 0;
	virtual int GetLightSpotCount()const = 0;

	//////////////////////////////////////////////////////////////
	// Use these functions to get a single light from the set   //
	//////////////////////////////////////////////////////////////

	// Implement this function by returning indexed directional light
	virtual ILightDirection *GetDirectionalLight( int Index )const = 0;
	// Implement this function by returning indexed point light
	virtual ILightPoint *GetPointLight( int Index )const = 0;
	// Implement this function by returning indexed spot light
	virtual ILightSpot *GetSpotLight( int Index )const = 0;

	//////////////////////////////////////////////////////////////
	// Use these functions to get a pointer to the first light	// 
	// of the requested type in the set.						//
	//////////////////////////////////////////////////////////////

	// Implement this function by returning all directional lights
	virtual ILightDirection *GetDirectionalLights()const = 0;
	// Implement this function by returning all point lights
	virtual ILightPoint *GetPointLights()const = 0;
	// Implement this function by returning all spot lights
	virtual ILightSpot *GetSpotLights()const = 0;


};


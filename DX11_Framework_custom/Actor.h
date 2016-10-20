/////////////////////////////////////////////////////////////////
// Class name: Actor
// Function: Parent class that holds all information related 
//           to gameplay for a specific object (location, etc.).
// Example:  MovingObject->Car->Mercedes
/////////////////////////////////////////////////////////////////
#pragma once

#include "Includes.h"
#include "Utilities.h"
#include "Model.h"
#include "Input.h"
#include "Physics.h"

class Actor
{
public:
    Actor() = default;

    Actor(
        const ModelSpecs_W &worldSpecs,
        eTexture tex,
        const ModelSpecs_L &localSpecs,
        eModType modType = CUBE_TEXTURED);

    ///////////////
    // ACCESSORS //
    ///////////////
    eModType GetModelType()const { return m_modType; }
    eTexture GetTexIndex() const { return m_texIndex; }
    const ModelSpecs_W &GetWorldSpecs() const { return m_worldSpecs; }
    const ModelSpecs_L &GetLocalSpecs() const { return m_localSpecs; }

    // TODO: move these into a child class, they are redundant for most
    // If they are redundant, wouldn't you want them in parent, so you 
    // don't have to keep remaking the same function?
    const XMFLOAT3 &GetPosition()const { return m_worldSpecs.position; }
    const XMFLOAT3 &GetRotation()const { return m_worldSpecs.orientation; }

    //////////////////////////////////////
    // GAME LOGIC AND PHYSICS FUNCTIONS //
    //////////////////////////////////////
    // TODO: CHANGE THIS TO TAKE REAL TIME!!!!!!!!!!
    void UpdateState(eState state, float deltaT = 0.0f);

    // This doesn't need to be implemented by most objects in the scene, 
    // maybe multiple inhertance would serve our purposes, have the
    // non controllable characters inherit from Actor and have a 
    // Actor_Controllable child of Actor, but parent to Actor_Player and other
    // controllable actors.
    virtual void GetInput(const Input& pInput, int randI = 0.f, float randF = 0.f) {}

    void MovePID(float deltaT = 0.0f);
    
protected:
    void Move(const float time);
    void DoGravity(const float deltaT);

protected:
    eModType m_modType;
    eTexture m_texIndex;

    ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;

    eState m_state;

	////////////////////////////////
    // PHYSICS / MOVEMENT RELATED //
    ////////////////////////////////
	//PhysAttributes physics;
	Physics m_physics;

    //XMFLOAT3 m_velocity = {0.f, 0.0f, 0.f};
    //// "downward" acceleration on the Y axis
    //float m_gravity = -9.8; // units are meters/second^2
    //// Given no mass, drag, area, or fluid density info, use an arbitrary value for 
    //// terminal velocity; would need to calculate this for more realistic physics.
    //float terminalVelocity = 50.0f;

    //XMFLOAT3 m_posError;
    //XMFLOAT3 m_target = { 0.0f,0.0f,0.0f };
};
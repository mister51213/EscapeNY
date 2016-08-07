#pragma once
#include "Algorithm.h"
#include "Utilities.h"

class Algorithm_Spiral3D : public Algorithm
{
public: Algorithm_Spiral3D(Game* pGame, std::shared_ptr<Input> pInput):
    Algorithm(pGame, pInput)
{}

        vector<Actor_NPC> MakePatternNPC(int numActors)
        {
            vector<Actor_NPC> actorsSUB;
            actorsSUB.reserve(numActors);
            ModelSpecs_W specs = { { 1.f, -30.f, 1.f }, { 0.f,0.f,0.f }, { 1.2f,0.5f,1.3f } };

            float spacing = 1080.f / numActors;

            float x = 1;
            float y = -50;
            float z = 1;

            float increment = 100.f;

            int tex = 0;
            for (int i = 0; i < numActors; i++)
            {
                float angleStep = static_cast<float>(i)*spacing;

                x = sin(angleStep*radian) * 30;
                z = cos(angleStep*radian) * 30;
                y += .5;

                specs.position.x = x;
                specs.position.y = y;
                specs.position.z = z;
               
                actorsSUB.push_back(Actor_NPC(m_pInput, specs, (eTexture)tex, ModelSpecs_L()));

                x = sin(angleStep*radian) * increment;
                z = cos(angleStep*radian) * increment;
                increment -= 20.f;

                if(tex<4)
                {
                    tex++;
                }
                else if (tex == 4)
                {
                    tex = 0;
                }
            }

            // Cast child actors to parent in order to return polymorphically
            // vector<Actor> actorsSub_Casted;
            //for each (Actor_NPC npc in actorsSUB)
            //{
            //    actorsSub_Casted.push_back(npc);
            //}
            //return actorsSub_Casted;
            return actorsSUB;
        }

        void SetData() {}
};
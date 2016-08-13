#pragma once
#include "Algorithm.h"

class Algorithm_Grid3D : public Algorithm
{
public: Algorithm_Grid3D(/*std::shared_ptr<Input> pInput*/):
    Algorithm(nullptr/*, pInput*/){}

        vector<Actor_NPC> MakePatternNPC(int numActors)
        {
            int m_numColumns;
            int m_numRows;
            int m_numZ;

            if (numActors != 0 && numActors % 25 == 0)
            {
                m_numColumns = numActors / 25;
                m_numRows = numActors / 25;
                m_numZ = numActors / 25;
            }
            else
            {
                numActors -= (numActors % 25);
                m_numColumns = numActors / 25;
                m_numRows = numActors / 25;
                m_numZ = numActors / 25;
            }
            float spacing = 1.f;
            vector<Actor_NPC> actorsSUB;
            actorsSUB.reserve(numActors);

            ModelSpecs_W specs = { { 0.f, 0.f, 0.f }, { 0.f,0.f,0.f }, { .3f,.4f,.5f } };
            for (float i = 0; i < m_numColumns; i++)
            {
                for (float j = 0; j < m_numRows; j++)
                {
                    for (float k = 0; k < m_numZ; k++)
                    {
                        specs.position.x = i*spacing;
                        specs.position.y = j*spacing;
                        specs.position.z = k*spacing;
                        specs.scale.x += .01f;
                        specs.scale.y += .01f;
                        specs.scale.z += .01f;

                        int index = actorsSUB.size();
                        actorsSUB.push_back(Actor_NPC(/*m_pInput, */specs, Water3, ModelSpecs_L(), CUSTOM_MESH));
                        spacing += .5f;
                    }
                }
            }

            // Cast child actors to parent in order to return polymorphically
            //vector<Actor> actorsSub_Casted;
            //for each (Actor_NPC npc in actorsSUB)
            //{
            //    actorsSub_Casted.push_back(npc);
            //}
            //return actorsSub_Casted;
            return actorsSUB;
        }

       void SetData() {}
};
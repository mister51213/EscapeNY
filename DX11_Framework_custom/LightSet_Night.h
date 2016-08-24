#pragma once
#include "ILightSet.h"
class LightSet_Night:
	public ILightSet
{
public:
	LightSet_Night();
	~LightSet_Night();

	bool Initialize();

	int GetLightDirectionCount()const override;
	int GetLightPointCount()const override;
	int GetLightSpotCount()const override;

	ILightDirection *GetDirectionalLight( int Index )const override;
	ILightPoint *GetPointLight( int Index )const override;
	ILightSpot *GetSpotLight( int Index )const override;

	ILightDirection *GetDirectionalLights()const override;
	ILightPoint *GetPointLights()const override;
	ILightSpot *GetSpotLights()const override;

private:
	int m_directionalCount, m_pointCount, m_spotCount;
	int m_directionalIndex, m_pointIndex, m_spotIndex;
	std::vector<ILight *> m_pLightList;
	std::unique_ptr<ILightDirection> m_pMoonLight;
	std::unique_ptr<ILightPoint> m_pLanterns;

};


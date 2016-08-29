Texture2D textures[2];
SamplerState samp;


struct LightTemplate
{
	float4 color;
	float3 position;
	float coneAngle;
	float3 direction;
	float padding;
};

cbuffer LightBuffer
{
	float4 g_ambientColor;
	int g_lightCount;
	float3 g_padding;
	LightTemplate g_lights;
}

struct PixelBuffer
{
	float4 position : SV_Position;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
	float4 worldPixelPosition : POSITION;
	float3 tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float3 normal : NORMAL;
};

float4 main(PixelBuffer input):SV_Target
{
	float4 texColor = textures[0].Sample(samp, input.texcoord);
	float4 nMapColor = textures[1].Sample(samp, input.texcoord);

	float3 nMap = (nMapColor.rgb * 2.f) - float3(1.f, 1.f, 1.f);
	
	float3x3 tanMatrix = float3x3(input.tangent, input.Binormal, input.normal);
	float3 lightTan = mul(nMap, tanMatrix);

	float intensity = dot(g_lights.direction, lightTan);
	float4 color = g_ambientColor * texColor;
	if(color.r > .5f)
	{
		color = float4(.1f, .1f, .1f, .1f);
	}
	if(intensity > 0.f)
	{
		color = saturate(color + (texColor * (g_lights.color * intensity)));
	}
    	//return color;

        /******************* SPOTLIGHT **************************/
   	float4 ambientColor = texColor*float4( 0.13f, 0.13f, 0.13f, 1.0f );

    // TODO: transform light direction first

    // Calculate the vector of pixel to light
    float3 surfToLight = input.worldPixelPosition.xyz - g_lights.position;
    float3 surfToLightN = normalize(surfToLight);


    float DP = saturate(dot(surfToLightN, g_lights.direction));
    
    float4 finalColor;

    //float cAngle = 1.0f / 180.0f;

    if (DP < 0.9f) // CUTOFF RANGE
    {
        finalColor = ambientColor;
    }
    else
    {
    // Calculate the amount of light on this pixel.
        float intensityL = DP;
        // / length(surfToLight)*10.0f;
    finalColor = saturate( color * intensityL + ambientColor);
    }

    return finalColor;
}
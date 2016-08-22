/***********************************************************/
/**  VARS NOT CONTROLLED BY ENGINE YET             *********/
/***********************************************************/

float4 diffuseColor = { 0.8f, 0.8f, 0.8f, 0.0f };    // NEW
float3 lightPosition = { 0.0f, 2.0f, 0.0f };   // NEW
float lightRange = 0.9f;    // NEW

// modelfile, just for effectedit
string XFile = "paving.x";   // model

/***********************************************************/
/**       VARS CONTROLLED BY ENGINE                *********/
/***********************************************************/

float4x4    World         : WORLD;
float4x4    WorldInvTransp: WORLDINVTRANSP;
float4x4    ViewProj      : VIEWPROJECTION;

float4      AmbientColor;
float       AmbientIntensity;

float4      MatAmb : MATERIALAMBIENT; 
float4      MatDiff: MATERIALDIFFUSE; 
            
texture     Tex0 < string name = "hungary009.jpg"; >;

/***********************************************************/
/**          SAMPLER STATES FOR TEXTURING          *********/
/***********************************************************/

sampler2D textureSampler = sampler_state
{
    Texture         = (Tex0);
                                 
    MinFilter       = ANISOTROPIC;
    MagFilter       = LINEAR;
    MipFilter       = LINEAR;    
    MaxAnisotropy   = 4;            
};

struct VS_INPUT
{
    float4    Pos      : POSITION0;
    float4    Normal   : NORMAL0;
    float2    TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos     : POSITION;
    float3 Normal  : TEXCOORD1;
    float2 TexCoord: TEXCOORD2;
    float3 LightPos: TEXCOORD3; 
    float3 Att     : TEXCOORD4;
};

/***********************************************************/
/**          THE VERTEXSHADER        PROGRAM       *********/
/***********************************************************/

VS_OUTPUT VS_function(VS_INPUT input)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4 worldPosition = mul(input.Pos, World);
    Out.Pos = mul(worldPosition, ViewProj);

    float4 normal = mul(input.Normal, WorldInvTransp);
    Out.Normal = normal;
    Out.TexCoord = input.TexCoord;

    Out.LightPos = lightPosition.xyz - worldPosition;   
    Out.LightPos = normalize(Out.LightPos);           

    Out.Att = Out.LightPos * lightRange;

    return Out;
}

/***********************************************************/
/**          THE PIXELSHADER        PROGRAM        *********/
/***********************************************************/

float4 PS_function(VS_OUTPUT input): COLOR0
{
    float4 textureColor = tex2D(textureSampler, input.TexCoord);

    float lightIntensity = saturate(dot(input.Normal, input.LightPos));    
    float4 color = diffuseColor * lightIntensity;          

    float4 amb = AmbientColor * AmbientIntensity * MatAmb;
    float4 attenuation = mul(input.Att, input.Att);

    return saturate((color + amb) * textureColor) * MatDiff * (1-attenuation); 
}

/***********************************************************/
/**          OPAUQE SHADER, REGULAR MESHES         *********/
/***********************************************************/

technique OpaqueShader
{
    pass P0              
    {
        AlphaBlendEnable    = FALSE;
                              
        VertexShader = compile vs_2_0 VS_function();
        PixelShader = compile ps_2_0 PS_function();
    }  
}

/***********************************************************/
/**   BLENDING SHADER, MESHES WITH BLENDED TEXTURES  *******/
/***********************************************************/

technique BlendingShader
{
    pass P0              
    {
        AlphaBlendEnable    = TRUE;
        SrcBlend            = SRCALPHA;
        DestBlend           = INVSRCALPHA;
        AlphaOp[0]          = SelectArg1;
        AlphaArg1[0]        = Texture;
                               
        VertexShader = compile vs_2_0 VS_function();
        PixelShader = compile ps_2_0 PS_function();
    }  
}
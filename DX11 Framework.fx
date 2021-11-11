//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Texture and Samplers
//--------------------------------------------------------------------------------------
Texture2D texDiffuse : register(t0);
Texture2D texSpecular : register(t1);
Texture2D texNormal : register(t2);
SamplerState samLinear : register(s0);


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    float Time;

    float3 LightVecW;

    float4 DiffuseMtrl;
    float4 DiffuseLight;

    float4 AmbientMtrl;
    float4 AmbientLight;

    float4 SpecularMtrl;
    float4 SpecularLight;

    float SpecularPower;

    float3 EyePosW;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 NormalW : NORMAL;
    float3 PosW : POSITION;
    float2 TexCoord : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float2 TexCoord : TEXCOORD, float3 NormalL : NORMAL )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    // Normal stuff
    output.NormalW = mul(float4(NormalL, 0.0f), World).xyz;

    output.Pos = mul( Pos, World);
    output.PosW = output.Pos.xyz;
    
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    
    output.TexCoord = TexCoord;
    //output.Color = Color;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    input.NormalW = normalize(input.NormalW);
    // Get the vector towards the position of the camera and the position of the vertices.
    float3 toEye = normalize(EyePosW - input.PosW.xyz);


    // Get the reflection from the normal to where the light is.
    float3 r = reflect(-LightVecW, input.NormalW);

    // Check angle of the reflection towards the camera position.
    float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
    float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;


    // How much to brighten/darken an object.
    float diffuseAmount = max(dot(LightVecW, input.NormalW), 0.0f);

    // ambient light.
    float4 ambientDiffuse = AmbientMtrl * AmbientLight;

    //	output.Color = Color * (1 - diffuseAmount) + DiffuseMtrl * diffuseAmount;

    float4 textureColour = texDiffuse.Sample(samLinear, input.TexCoord);

    float4 color;

    color.rgb = ambientDiffuse + (diffuseAmount * textureColour) + specular;
    color.a = DiffuseMtrl.a;


    return color;
}

//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

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
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float3 Normal : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float3 NormalL : NORMAL, float4 Color : COLOR )
{
    VS_OUTPUT output = (VS_OUTPUT)0;


    output.Pos = mul( Pos, World);
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    

	float3 normalW = mul(float4(NormalL, 0.0f), World).xyz;
	normalW = normalize(normalW);
    
    float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
	
    float4 ambientDiffuse = AmbientMtrl * AmbientLight;

//	output.Color = Color * (1 - diffuseAmount) + DiffuseMtrl * diffuseAmount;
    
	output.Color.rgb = ambientDiffuse + diffuseAmount * Color;
	output.Color.a = DiffuseMtrl.a;
    
    
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}

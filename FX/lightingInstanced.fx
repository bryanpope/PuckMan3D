//***************************************************************************************
// lighting.fx by Super Rob (C) 2014 No Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "lightingHelper.fx"

DepthStencilState DSS
{
	DepthEnable = false;
	DepthWriteMask = Zero;

	StencilEnable = false;
	StencilReadMask = 0;
	StencilWriteMask = 0;

	FrontFaceStencilFunc =  Always;
	FrontFaceStencilPass = Incr;
	FrontFaceStencilFail = Keep;

	BackFaceStencilFunc = Always;
	BackFaceStencilPass = Incr;
	BackFaceStencilFail = Keep;

};

SamplerState mySampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;

	BorderColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
};

Texture2D gDiffuseMap;

cbuffer cbPerObject
{
	float4x4 gWVP;
	float4x4 gW;
	float4x4 gViewProj;
	float4x4 gInvTransposeW;
	Material gMaterial;
};

cbuffer cbPerFrame
{
	float4 gAmbientLight;
	PointLight gPointLight[3];
	float3 gEyePos;
	SpotLight gSpotLight;
};

cbuffer cbFixed
{
	float4 fogColour = float4(0.5f, 0.5f, 0.5f, 1.0f);
	float startFogDist = 0.0f;
	float maxFogDist = 75.0f;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float3 NormalL  : NORMAL;
    float2 Tex : TEXCOORD;
	row_major float4x4 World  : WORLD;
	float4 Color    : COLOR;
	uint InstanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float2 Tex : TEXCOORD;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float4 Color   : COLOR;
};

VertexOut VS(VertexIn vin, uniform bool gUseInstanced)
{
	VertexOut vout;
	
	if (gUseInstanced)
	{
		// Transform to world space space.
		vout.PosW = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
		vout.PosW = mul(float4(vout.PosW, 1.0f), gW).xyz;

		vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);

		// Transform to homogeneous clip space.
		vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);
	}
	else
	{
		vout.NormalW = mul(vin.NormalL, (float3x3)gInvTransposeW);

		vout.PosW = (float3)mul(float4(vin.PosL, 1.0f), gW);

		// Transform to homogeneous clip space.
		vout.PosH = mul(float4(vin.PosL, 1.0f), gWVP);
	}

	// Output vertex attributes for interpolation across triangle.
	//vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	vout.Tex = vin.Tex;

	vout.Color = vin.Color;

    return vout;
}

float4 PS(VertexOut pin, uniform bool gUseTexture, uniform bool gUseSpotLight) : SV_Target
{

	Material mat;
	if (gUseTexture)
	{
		mat.Diffuse = gDiffuseMap.Sample(mySampler, pin.Tex);
		mat.Specular = float4(0.75f, 0.75f, 0.75f, 1000.0f);
	}
	else
	{
		mat = gMaterial;
		mat.Diffuse = pin.Color;
	}

	pin.NormalW = normalize(pin.NormalW);
	float3 toEye = gEyePos - pin.PosW;
	float dist = length(toEye);
	toEye = normalize(toEye);

	float4 d, s;
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		ComputePointLight(mat, gPointLight[i], pin.PosW, pin.NormalW, toEye, d, s);
		diffuse += d;
		specular += s;
	}

	if (gUseSpotLight)
	{
		ComputeSpotLight(mat, gSpotLight, pin.PosW, pin.NormalW, toEye, d, s);

		diffuse += d;
		specular += s;
	}

	float4 litColour = gAmbientLight * mat.Diffuse + diffuse + specular;
	//float4 litColour = gAmbientLight * mat.Diffuse;
	//float4 litColour = float4(1.0f, 0.0f, 0.0f, 1.0f);
	litColour.a = mat.Diffuse.a;

	//float rat = saturate((dist - startFogDist) / maxFogDist);
	//litColour = lerp(litColour, fogColour, rat);

	float4 oC;
	CRTshader(pin.PosH, litColour, oC);

	//return litColour;
	return oC;
}

technique11 TestTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS(false) ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(true, true) ) );
		
    }
}

technique11 LitMatTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(false, false)));

	}
}

technique11 LitMatTechInstanced
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(false, false)));

	}
}

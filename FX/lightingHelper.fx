struct PointLight
{
	float4 lightColour;
	float3 pos;
	float range;

	float3 att;
	float pad;
};

struct SpotLight
{
	float4 lightColour;
	float3 pos;
	float range;

	float3 direction;
	float spot;

	float3 att;
	float pad;	
};

/*struct Material
{
	//float4 ambient;
	float4 specular; // how shiny it is (usually is grayscale)
	float4 diffuse; // we are going to use for ambient
};*/
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};


//Note: toEye must be normalized
void ComputePointLight(Material mat, PointLight l, float3 pos, 
					   float3 normal, float3 toEye, out float4 diffuse,
					   out float4 specular)
{
	diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	specular = float4(0.0f, 0.0f,0.0f, 1.0f);

	float3 lightVec = pos - l.pos;
	float d = length(lightVec);

	if(d > l.range)
	{
		return;
	}

	lightVec /= d;

	float kd = dot(-lightVec, normal);
	if(kd > 0)
	{
		diffuse = (kd * l.lightColour) * mat.Diffuse;
		
		float3 r = reflect(lightVec, normal);

		float ks = pow(max(dot(toEye, r), 0.0f), mat.Specular.w);

		specular = (ks * l.lightColour) * mat.Specular;
	}

	float attenuation = 1.0f / (l.att.x + d * l.att.y + pow(d,2) * l.att.z);
	diffuse *= attenuation;
	specular *= attenuation;
}

void ComputeSpotLight(Material mat, SpotLight light, float3 pos, 
					  float3 normal, float3 toEye, out float4 diffuse,
					  out float4 specular)
{
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	PointLight temp;
	temp.lightColour = light.lightColour;
	temp.pos = light.pos;
	temp.range = light.range;
	temp.att = light.att;

	ComputePointLight(mat, temp, pos, normal, toEye, diffuse, specular);

	float3 lightVec = pos - light.pos;
	lightVec = normalize(lightVec);

	float spotFactor = pow(max(dot(lightVec, light.direction), 0.0f), light.spot);

	diffuse *= spotFactor;
	specular *= spotFactor;
}

//Note: toEye must be normalized
void ComputePointLightToon(Material mat, PointLight l, float3 pos, 
					   float3 normal, float3 toEye, out float4 diffuse,
					   out float4 specular)
{
	diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	specular = float4(0.0f, 0.0f,0.0f, 1.0f);

	float3 lightVec = pos - l.pos;
	float d = length(lightVec);

	if(d > l.range)
	{
		return;
	}

	lightVec /= d;

	float kd = dot(-lightVec, normal);
	if(kd <= 0.0f)
	{
		kd = 0.4f;
	}
	else if(kd <= 0.5)
	{
		kd = 0.6f;
	}
	else
	{
		kd = 1.0f;
	}

		
		diffuse = (kd * l.lightColour) * mat.Diffuse;
		
		float3 r = reflect(lightVec, normal);

		float ks = pow(max(dot(toEye, r), 0.0f), mat.Specular.w);

		if(ks <= 0.1f)
	{
		kd = 0.0f;
	}
	else if(ks <= 0.8)
	{
		kd = 0.5f;
	}
	else
	{
		kd = 0.8f;
	}

		specular = (ks * l.lightColour) * mat.Specular;
	

	float attenuation = 1.0f / (l.att.x + d * l.att.y + pow(d,2) * l.att.z);
	diffuse *= attenuation;
	specular *= attenuation;
}

void ComputeSpotLightToon(Material mat, SpotLight light, float3 pos, 
					  float3 normal, float3 toEye, out float4 diffuse,
					  out float4 specular)
{
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	PointLight temp;
	temp.lightColour = light.lightColour;
	temp.pos = light.pos;
	temp.range = light.range;
	temp.att = light.att;

	ComputePointLightToon(mat, temp, pos, normal, toEye, diffuse, specular);

	float3 lightVec = pos - light.pos;
	lightVec = normalize(lightVec);

	float spotFactor = pow(max(dot(lightVec, light.direction), 0.0f), light.spot);

	if(spotFactor < 0.2f)
	{
		spotFactor = 0.0f;
	}
	else if(spotFactor < 0.8f)
	{
		spotFactor = 0.5f;
	}
	else
	{
		spotFactor = 1.0f;
	}

	diffuse *= spotFactor;
	specular *= spotFactor;
}

void CRTshader(float4 pixelPos, float4 inColour, out float4 outColour)
{
	//outColour = inColour;
	//return;

	float vertForce1 = 0.80f;
	float vertForce2 = 0.70f;
	// R (1, vF1, vF2, 1), G (vF2, 1, vF1, 1), B (vF1, vF2, 1, 1)
	float4 muls;

	float brightness = 27;		// -200 - 200
	float contrast = 2.1;		// -2 - 20
	float scanColour = 0.75f;

	float2 ps = pixelPos.xy - float2(0.5, 0.5);
	uint pp = (uint)ps.x % 3;
	
	outColour = float4(0, 0, 0, 1);

	if (pp == 1)
	{
		muls = float4(1, vertForce1, vertForce2, 1);
	}
	else if (pp == 2)
	{
		muls = float4(vertForce2, 1, vertForce1, 1);
	}
	else
	{
		muls = float4(vertForce1, vertForce2, 1, 1);
	}

	if ((uint)ps.y % 3 == 0)
	{
		muls *= float4(scanColour, scanColour, scanColour, 1);
	}

	outColour = inColour * muls;

	outColour += (brightness / 255);
	outColour = outColour - contrast * (outColour - 1.0) * outColour * (outColour - 0.5);
}
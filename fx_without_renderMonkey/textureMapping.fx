float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjectionMatrix;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT testShader_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	Output.mTexCoord = Input.mTexCoord;

	return Output;
}

texture gDiffuseMap;

sampler2D DiffuseSampler = sampler_state
{
	Texture = (gDiffuseMap);
};

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

float4 testShader_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);

	return albedo;
}

technique testShader
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 testShader_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 testShader_Pass_0_Pixel_Shader_ps_main();
	}
}
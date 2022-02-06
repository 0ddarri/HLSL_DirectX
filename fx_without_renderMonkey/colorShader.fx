float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjectionMatrix;

struct VS_INPUT
{
	float4 mPosition : POSITION;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	return Output;
}

float4 ps_main() : COLOR
{
	return float4(1.0f,0.0f,0.0f,0.0f);
}

technique textureMapping
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
}
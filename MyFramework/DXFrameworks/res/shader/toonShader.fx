float4x4 gWorldViewProjectionMatrix;

float4x4 gInvWorld;

float4 gWorldLightPosition;

float4 gLightColor;

struct VS_INPUT
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
    float3 mDiffuse : TEXCOORD1;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.mPosition = mul(Input.mPosition, gWorldViewProjectionMatrix);

    float4 objectLightPosition = mul(gWorldLightPosition, gInvWorld);

    float3 lightDir = normalize(Input.mPosition.xyz - objectLightPosition.xyz);

    Output.mDiffuse = dot(-lightDir, Input.mNormal);
    return Output;
}

struct PS_INPUT
{
    float3 mDiffuse : TEXCOORD1;
};

float4 ps_main(PS_INPUT Input) : COLOR
{
    float3 diffuse = saturate(Input.mDiffuse);

    diffuse = ceil(diffuse * 5) / 5.0f;

    return float4(diffuse * gLightColor.rgb, 1);
}

technique toonShader
{
    pass pass_0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
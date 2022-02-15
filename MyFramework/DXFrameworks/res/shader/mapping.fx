float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjectionMatrix;

float4 gWorldLightPosition;

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

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.mPosition = mul(Input.mPosition, gWorldMatrix);
    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

    Output.mTexCoord = Input.mTexCoord;

    return Output;
}

struct PS_INPUT
{
    float2 mTexCoord : TEXCOORD0;
};

texture gDiffuseTexture;
sampler2D gDiffuseSampler = sampler_state
{
    Texture = (gDiffuseTexture);
};

float4 ps_main(PS_INPUT Input) : COLOR
{
    float4 albedo = tex2D(gDiffuseSampler, Input.mTexCoord);
    return albedo.rgba;
}

technique texturemapping
{
    pass pass_0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
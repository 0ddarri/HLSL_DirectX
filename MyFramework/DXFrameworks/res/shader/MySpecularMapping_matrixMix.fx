// world, view, projection 행렬을 통합해서 계산한 specular mapping

float4x4 gWorldViewProjectionMatrix;
float4x4 gInvWorld;

float4 gWorldLightPosition;

float4 gWorldCameraPosition;

float4 gLightColor;

struct VS_INPUT
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
    float2 mUV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
    float2 mUV : TEXCOORD0;
    float3 mDiffuse : TEXCOORD1;
    float3 mReflection : TEXCOORD2;
    float3 mViewDir : TEXCOORD3;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.mPosition = mul(Input.mPosition, gWorldViewProjectionMatrix);

    float4 objectLightPos = mul(gWorldLightPosition, gInvWorld);
    
    float3 lightDir = Input.mPosition.xyz - objectLightPos.xyz;
    lightDir = normalize(lightDir);

    Output.mDiffuse = dot(-lightDir, Input.mNormal);

    Output.mReflection = normalize(reflect(lightDir, Input.mNormal));

    float4 objectCameraPos = mul(gWorldCameraPosition, gInvWorld);

    Output.mViewDir = Input.mPosition.xyz - objectCameraPos.xyz;

    Output.mUV = Input.mUV;

    return Output;
}

texture DiffuseMap;
sampler2D DiffuseSampler
{
    Texture = (DiffuseMap);
};

texture SpecularMap;
sampler2D SpecularSampler
{
    Texture = (SpecularMap);
};

struct PS_INPUT
{
    float2 mUV : TEXCOORD0;
    float3 mDiffuse : TEXCOORD1;
    float3 mReflection : TEXCOORD2;
    float3 mViewDir : TEXCOORD3;
};

float4 ps_main(PS_INPUT Input) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mUV);
    float3 diffuse = albedo.rgb * gLightColor.rgb * saturate(Input.mDiffuse);

    float3 reflection = normalize(Input.mReflection);
    float3 viewDir = normalize(Input.mViewDir);

    float3 specular = 0;
    if(diffuse.x > 0)
    {
        specular = saturate(dot(-viewDir, reflection));
        specular = pow(specular, 20.0f);

        float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
        specular *= specularIntensity.rgb * gLightColor.rgb;
    }

    float3 ambient = float3(0.1f,0.1f,0.1f) * albedo.rgb;

    return float4(diffuse + specular + ambient, 1);
}

technique specularMapping
{
    pass pass_0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
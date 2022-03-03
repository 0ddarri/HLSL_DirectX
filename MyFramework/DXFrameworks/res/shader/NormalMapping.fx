float4x4 gWorldViewProjectionMatrix;
float4x4 gWorldMatrix;

float4 gWorldCameraPosition;
float4 gWorldLightPosition;

struct VS_INPUT
{
    float4 mPosition : POSITION;
    float2 mUV : TEXCOORD0;
    float3 mNormal : NORMAL;
    float3 mTangent : TANGENT;
    float3 mBinormal : BINORMAL;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;

    float2 mUV : TEXCOORD0; // UV Mapping
    float3 mViewDir : TEXCOORD1;
    float3 mLightDir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.mPosition = mul(Input.mPosition, gWorldViewProjectionMatrix);
    Output.mUV = Input.mUV;

    float4 worldPosition = mul(Input.mPosition, gWorldMatrix);
    float3 viewDir = worldPosition.xyz - gWorldCameraPosition.xyz;
    Output.mViewDir = normalize(viewDir);

    float3 lightDir = worldPosition.xyz - gWorldLightPosition.xyz;
    Output.mLightDir = normalize(lightDir);

    float3 worldNormal = mul(Input.mNormal, (float3x3)gWorldMatrix);
    Output.N = normalize(worldNormal);

    float3 worldTangent = mul(Input.mTangent, (float3x3)gWorldMatrix);
    Output.T = normalize(worldTangent);

    float3 worldBinormal = mul(Input.mBinormal, (float3x3)gWorldMatrix);
    Output.B = normalize(worldBinormal);

    return Output;
}

struct PS_INPUT
{
    float2 mUV : TEXCOORD0;
    float3 mViewDir : TEXCOORD1;
    float3 mLightDir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

texture DiffuseMap;
sampler2D DiffuseSampler
{
    Texture = DiffuseMap;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

texture SpecularMap;
sampler2D SpecularSampler
{
    Texture = SpecularMap;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

texture NormalMap;
sampler2D NormalSampler
{
    Texture = NormalMap;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

float4 gLightColor;
float gSpecularPower;

float4 ps_main(PS_INPUT Input) : COLOR
{
    float3 tangentNormal = tex2D(NormalSampler, Input.mUV).xyz;
    tangentNormal = normalize(tangentNormal * 2 - 1);

    float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
    TBN = transpose(TBN);
    float3 worldNormal = mul(TBN, tangentNormal);

    float3 lightDir = normalize(Input.mLightDir);
    float3 diffuse = saturate(dot(worldNormal, -lightDir));

    float4 albedo = tex2D(DiffuseSampler, Input.mUV);
    diffuse = albedo.rgb * gLightColor * diffuse;

    float3 specular = 0;
    if(diffuse.x > 0)
    {
        float3 reflection = reflect(lightDir, worldNormal);
        float3 viewDir = normalize(Input.mViewDir);

        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, gSpecularPower);

        float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
        specular *= specularIntensity.xyz * gLightColor;
    }

    float3 ambient = float3(0.1f,0.1f,0.1f);

    return float4(ambient + diffuse + specular, 1);
}

technique normalMap
{
    pass pass_0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
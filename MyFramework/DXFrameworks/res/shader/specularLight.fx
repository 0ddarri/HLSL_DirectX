float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjectionMatrix;

float4 gWorldLightPosition;
float4 gWorldCameraPosition;

float gSpecularPower;

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
    float3 mDiffuse : TEXCOORD1;
    float3 mReflection : TEXCOORD2;
    float3 mViewDir : TEXCOORD3;
};

struct VS_INPUT
{
    float4 mPosiiton : POSITION;
    float3 mNormal : NORMAL;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.mPosition = mul(Input.mPosiiton, gWorldMatrix);

    float3 lightDir = Output.mPosition.xyz - gWorldLightPosition.xyz; // 정점좌표 월드좌표일 때 빛 방향벡터 계산
    lightDir = normalize(lightDir);

    float3 worldNormal = mul(Input.mNormal, (float3x3)gWorldMatrix); // 월드노말벡터 계산
    worldNormal = normalize(worldNormal);

    Output.mDiffuse = dot(-lightDir, worldNormal); // 내적으로 디퓨즈세기 구함

    Output.mReflection = reflect(lightDir, worldNormal);

    Output.mViewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);

    Output.mPosition = mul(Output.mPosition, gViewMatrix);
    Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

    return Output;
}

struct PS_INPUT
{
    float4 mPosition : POSITION;
    float3 mDiffuse : TEXCOORD1;
    float3 mReflection : TEXCOORD2;
    float3 mViewDir : TEXCOORD3;
};

float4 ps_main(PS_INPUT Input) : COLOR
{
    float3 diffuse = saturate(Input.mDiffuse); // 0에서 1사이 값으로

    float3 viewDir = normalize(Input.mViewDir);
    float3 reflection = normalize(Input.mReflection);

    float3 specular = 0;
    if(diffuse.x > 0)
    {
        specular = dot(-viewDir, reflection);
        specular = saturate(specular);
        specular = pow(specular, gSpecularPower);
    }

    float3 ambient = float3(0.1f,0.1f,0.1f);
    return float4(ambient + specular + diffuse, 1);
}

technique lighting
{
    pass pass_0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
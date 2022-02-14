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
   return float4(1.0f,0.0f,0.0f,1.0f);
}

// technique : 렌더링 방식을 정의하는 모든 정보를 캡슐화해서 가지고 있다.
// 여러개의 패스를 가지고 있을 수 있다
// pass : 모델 또는 오브젝트의 최종 렌더링이 추가적인 처리를 거쳐서 통합될 필요가 있을 때 사용
// 방향광을 고려한 패스와 지점광을 고려한 패스가 따로 있고 이를 통합해 처리할 수도 있다.
technique technique1
{
   pass pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

// vs_2_0, ps_2_0 : hlsl의 버전 느낌
/*
DirectX9
 - shader 모델 1, 2, 3 사용 가능
 - vs_2_0, ps_2_0, vs_3_0, ps_3_0
DirectX10
 - shader 모델 4 사용 가능
DirectX11
 - shader 모델 5 사용 가능
DirectX12
 - shader 모델 6 사용 가능

*/
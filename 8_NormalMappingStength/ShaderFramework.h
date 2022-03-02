//**********************************************************************
//
// ShaderFramework.h
// 
// 쉐이더 데모를 위한 C스타일의 초간단 프레임워크입니다.
// (실제 게임을 코딩하실 때는 절대 이렇게 프레임워크를
// 작성하시면 안됩니다. -_-)
//
// Author: Pope Kim
//
//**********************************************************************


#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

// ---------- 선언 ------------------------------------
#define WIN_WIDTH		800
#define WIN_HEIGHT		600

#define PI 3.14159265f
#define FOV (PI/4.0f) // 시야각
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT) // 화면 종횡비
#define NEAR_PLANE 1 // 근접 평면
#define FAR_PLANE 10000 // 원거리 평면

// ---------------- 함수 프로토타입 ------------------------

// 메시지 처리기 관련
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ProcessInput(HWND hWnd, WPARAM keyPress);

// 초기화 과련
bool InitEverything(HWND hWnd);
bool InitD3D(HWND hWnd);
bool LoadAssets();
LPD3DXEFFECT LoadShader(const char * filename);
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename);
LPD3DXMESH LoadModel(const char * filename);

// 게임루프 관련
void PlayDemo();
void Update();

// 렌더링 관련
void RenderFrame();
void RenderScene();
void RenderInfo();

// 뒷정리 관련
void Cleanup();

LPD3DXMESH gpSphere = NULL;
LPD3DXEFFECT gpSpecularShader = NULL;

LPDIRECT3DTEXTURE9 stoneDiffuse;
LPDIRECT3DTEXTURE9 stoneSpecular;
LPDIRECT3DTEXTURE9 stoneNormal;

D3DXVECTOR4 gWorldLightPosition(700.0f, 500.0f, 500.0f, 1.0f);
D3DXVECTOR4 gWorldCameraPosition(0, 0, -150, 1);
D3DXVECTOR4 gLightColor(1, 1, 1, 1);

FLOAT gSpecularPower = 20.0f;

float gNormalStrength = 0.0f;

float gRotationY = 0.0f;

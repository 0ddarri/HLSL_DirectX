//**********************************************************************
//
// ShaderFramework.h
// 
// ���̴� ���� ���� C��Ÿ���� �ʰ��� �����ӿ�ũ�Դϴ�.
// (���� ������ �ڵ��Ͻ� ���� ���� �̷��� �����ӿ�ũ��
// �ۼ��Ͻø� �ȵ˴ϴ�. -_-)
//
// Author: Pope Kim
//
//**********************************************************************


#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

// ---------- ���� ------------------------------------
#define WIN_WIDTH		800
#define WIN_HEIGHT		600

#define PI 3.14159265f
#define FOV (PI/4.0f) // �þ߰�
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT) // ȭ�� ��Ⱦ��
#define NEAR_PLANE 1 // ���� ���
#define FAR_PLANE 10000 // ���Ÿ� ���

// ---------------- �Լ� ������Ÿ�� ------------------------

// �޽��� ó���� ����
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ProcessInput(HWND hWnd, WPARAM keyPress);

// �ʱ�ȭ ����
bool InitEverything(HWND hWnd);
bool InitD3D(HWND hWnd);
bool LoadAssets();
LPD3DXEFFECT LoadShader(const char * filename);
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename);
LPD3DXMESH LoadModel(const char * filename);

// ���ӷ��� ����
void PlayDemo();
void Update();

// ������ ����
void RenderFrame();
void RenderScene();
void RenderInfo();

// ������ ����
void Cleanup();

LPD3DXMESH gpTeapot = NULL;
LPD3DXEFFECT gpEnvironmentShader = NULL;

LPDIRECT3DTEXTURE9 stoneDiffuse;
LPDIRECT3DTEXTURE9 stoneSpecular;
LPDIRECT3DTEXTURE9 stoneNormal;
LPDIRECT3DCUBETEXTURE9 snowEnv;

D3DXVECTOR4 gWorldLightPosition(700.0f, 500.0f, 500.0f, 1.0f);
D3DXVECTOR4 gWorldCameraPosition(0, 0, -150, 1);
D3DXVECTOR4 gLightColor(1, 1, 1, 1);

FLOAT gSpecularPower = 20.0f;

float gNormalStrength = 0.0f;

float gRotationY = 0.0f;
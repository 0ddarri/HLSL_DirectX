//**********************************************************************
//
// ShaderFramework.cpp
// 
// 쉐이더 데모를 위한 C스타일의 초간단 프레임워크입니다.
// (실제 게임을 코딩하실 때는 절대 이렇게 프레임워크를
// 작성하시면 안됩니다. -_-)
//
// Author: Pope Kim
//
//**********************************************************************

#include "ShaderFramework.h"
#include <stdio.h>


//----------------------------------------------------------------------
// 전역변수
//----------------------------------------------------------------------

// D3D 관련
LPDIRECT3D9             gpD3D = NULL;				// D3D
LPDIRECT3DDEVICE9       gpD3DDevice = NULL;			// D3D 장치

// 폰트
ID3DXFont*              gpFont = NULL;

// 모델

// 쉐이더

// 텍스처

// 프로그램 이름
const char*				gAppName = "초간단 쉐이더 데모 프레임워크";


//-----------------------------------------------------------------------
// 프로그램 진입점/메시지 루프
//-----------------------------------------------------------------------

// 진입점
//INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
int main()
{
	// 윈도우 클래스를 등록한다.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		gAppName, NULL };
	RegisterClassEx(&wc);

	// 프로그램 창을 생성한다.
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindow(gAppName, gAppName,
		style, CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	// Client Rect 크기가 WIN_WIDTH, WIN_HEIGHT와 같도록 크기를 조정한다. (창 크기에 타이틀 바 및 경계선이 포함됨)
	POINT ptDiff;
	RECT rcClient, rcWindow;

	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd, rcWindow.left, rcWindow.top, WIN_WIDTH + ptDiff.x, WIN_HEIGHT + ptDiff.y, TRUE);

	// 창을 띄운다
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// D3D를 비롯한 모든 것을 초기화한다.
	if (!InitEverything(hWnd))
		PostQuitMessage(1); // 초기화에 실패하면 프로그램 종료

	// 메시지 루프
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) // WM_QUIT : 데모를 종료하라는 윈도우 메세지
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // 메시지가 없으면 게임을 업데이트하고 장면을 그린다
		{
			PlayDemo();
		}
	}

	UnregisterClass(gAppName, wc.hInstance); // 데모를 종료할 때가 되면 윈도우 클래스 등록을 해제하고 프로그램 끝냄
	return 0;
}

// 메시지 처리기
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		ProcessInput(hWnd, wParam); // 키보드 입력 처리
		break;

	case WM_DESTROY: // 창이 닫힐 때
		Cleanup(); // 자원 해제
		PostQuitMessage(0); // 프로그램 종료 메세지 보냄
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 키보드 입력처리
void ProcessInput(HWND hWnd, WPARAM keyPress)
{
	switch (keyPress)
	{
		// ESC 키가 눌리면 프로그램을 종료한다.
	case VK_ESCAPE:
		PostMessage(hWnd, WM_DESTROY, 0L, 0L);
		break;

	case VK_F1:
	{
		gRotationX += 0.04;
	}
	break;
	case VK_F2:
	{
		gRotationY += 0.04;
	}
	break;
	case VK_F3:
	{
		gRotationZ += 0.04;
	}
	break;
	}
}

//------------------------------------------------------------
// 게임루프
//------------------------------------------------------------
void PlayDemo()
{
	Update();
	RenderFrame();
}

// 게임로직 업데이트
void Update()
{
	if (gRotationX > 2 * PI)
	{
		gRotationX -= 2 * PI;
	}
	if (gRotationY > 2 * PI)
	{
		gRotationY -= 2 * PI;
	}
	if (gRotationZ > 2 * PI)
	{
		gRotationZ -= 2 * PI;
	}
}

//------------------------------------------------------------
// 렌더링
//------------------------------------------------------------

void RenderFrame()
{
	D3DCOLOR bgColour = 0xFF0000FF;	// 배경색상 - 파랑

	gpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), bgColour, 1.0f, 0);

	gpD3DDevice->BeginScene();
	{
		RenderScene();				// 3D 물체등을 그린다.
		RenderInfo();				// 디버그 정보 등을 출력한다.
	}
	gpD3DDevice->EndScene();

	gpD3DDevice->Present(NULL, NULL, NULL, NULL);
}


// 3D 물체등을 그린다.
void RenderScene()
{
	// 뷰 행렬
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vEyePos(0.0f, 0.0f, -200.0f); // 카메라 위치
	D3DXVECTOR3 vLookPos(0.0f, 0.0f, 0.0f); // 카메라가 바라보는 위치
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 카메라 위쪽을 가리키는 벡터
	D3DXMatrixLookAtLH(&matView, &vEyePos, &vLookPos, &vUpVec); // 뷰행렬 만들기

	// 투영행렬
	D3DXMATRIXA16 matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, 
		ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	// 월드행렬
	D3DXMATRIXA16 matWorldX;
	D3DXMATRIXA16 matWorldY;
	D3DXMATRIXA16 matWorldZ;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixRotationX(&matWorldX, gRotationX);
	D3DXMatrixRotationY(&matWorldY, gRotationY);
	D3DXMatrixRotationZ(&matWorldZ, gRotationZ);

	matWorld = matWorldX * matWorldY * matWorldZ;

	gpTextureMappingShader->SetMatrix("gWorldMatrix", &matWorld);
	gpTextureMappingShader->SetMatrix("gViewMatrix", &matView);
	gpTextureMappingShader->SetMatrix("gProjectionMatrix", &matProjection);
	gpTextureMappingShader->SetTexture("DiffuseMap_Tex", gpEarthTexture);

	UINT numPasses = 0;
	gpTextureMappingShader->Begin(&numPasses, NULL);
	for (UINT i = 0; i < numPasses; i++)
	{
		gpTextureMappingShader->BeginPass(i);
		gpSphere->DrawSubset(0);
		gpTextureMappingShader->EndPass();
	}
	gpTextureMappingShader->End();

}

// 디버그 정보 등을 출력.
void RenderInfo()
{
	// 텍스트 색상
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// 텍스트를 출력할 위치
	RECT rct;
	rct.left = 5;
	rct.right = WIN_WIDTH / 3;
	rct.top = 5;
	rct.bottom = WIN_HEIGHT / 3;

	// 키 입력 정보를 출력
	gpFont->DrawText(NULL, "데모 프레임워크\n\nESC: 데모종료", -1, &rct, 0, fontColor);
}

//------------------------------------------------------------
// 초기화 코드
//------------------------------------------------------------
bool InitEverything(HWND hWnd)
{
	// D3D를 초기화
	if (!InitD3D(hWnd))
	{
		return false;
	}

	// 모델, 쉐이더, 텍스처등을 로딩
	if (!LoadAssets())
	{
		return false;
	}

	// 폰트를 로딩
	if (FAILED(D3DXCreateFont(gpD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		"Arial", &gpFont)))
	{
		return false;
	}

	return true;
}

// D3D 객체 및 장치 초기화
bool InitD3D(HWND hWnd)
{
	// D3D 객체
	gpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!gpD3D)
	{
		return false;
	}

	// D3D장치를 생성하는데 필요한 구조체를 채워넣는다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = WIN_WIDTH; // 백버퍼 너비
	d3dpp.BackBufferHeight = WIN_HEIGHT; // 백버퍼 높이
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 백버퍼 포맷
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 백버퍼 스왑할 때 효과
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8; // 깊이/스텐실 버퍼 포맷
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // 모니터 주사율과 백버퍼를 스왑하는 빈도간의 관계
//									▲ : 모니터가 수직동기 될때마다 백버퍼 스왑 / D3DPRESENT IMMEDIATE : 수직동기를 기다리지 않고 바로 스왑 -> 프레임이 찢겨보일수있음

	// D3D장치를 생성한다.
	if (FAILED(gpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &gpD3DDevice)))
	{
		return false;
	}

	return true;
}

bool LoadAssets()
{
	// 텍스처 로딩
	gpEarthTexture = LoadTexture("Resources/Texture/Earth.jpg");
	// 쉐이더 로딩
	gpTextureMappingShader = LoadShader("Resources/Shader/texturemapping.fx");
	if (!gpTextureMappingShader) return false;
	// 모델 로딩
	gpSphere = LoadModel("Resources/Model/sphere.x");
	if (!gpSphere) return false;
	return true;
}

// 쉐이더 로딩
LPD3DXEFFECT LoadShader(const char * filename)
{
	LPD3DXEFFECT ret = NULL;

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	// 쉐이더 파일 로딩 및 컴파일
	D3DXCreateEffectFromFile(gpD3DDevice, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// 쉐이더 로딩에 실패한 경우 output창에 쉐이더
	// 컴파일 에러를 출력한다.
	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

// 모델 로딩
LPD3DXMESH LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, gpD3DDevice, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// 텍스처 로딩
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(gpD3DDevice, filename, &ret)))
	{
		OutputDebugString("텍스처 로딩 실패: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}
//------------------------------------------------------------
// 뒷정리 코드.
//------------------------------------------------------------

void Cleanup()
{
	// 폰트를 release 한다.
	if (gpFont)
	{
		gpFont->Release();
		gpFont = NULL;
	}

	// 모델을 release 한다.
	gpSphere->Release();
	gpSphere = NULL;
	// 쉐이더를 release 한다.
	gpTextureMappingShader->Release();
	gpTextureMappingShader = NULL;
	// 텍스처를 release 한다.
	gpEarthTexture->Release();
	gpEarthTexture = NULL;

	// D3D를 release 한다.
	if (gpD3DDevice)
	{
		gpD3DDevice->Release();
		gpD3DDevice = NULL;
	}

	if (gpD3D)
	{
		gpD3D->Release();
		gpD3D = NULL;
	}
}


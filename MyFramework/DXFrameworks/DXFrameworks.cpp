//--------------------------------------------------------------------------------------
// File: DXFrameworks.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "resource.h"
#include "Resources.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Util.h"

Mesh* mesh = new Mesh();
Shader* shader = new Shader();
Texture* texture = new Texture();

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
    mesh->Initialize();
    shader->LoadShaders();
    texture->LoadTextures();
    Util::GetIns()->SwitchInitialize();
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
    return S_OK;
}

float yRotation = 0.0f;

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    Util::GetIns()->SwitchShader();
    mesh->Update(fElapsedTime);
    yRotation += fElapsedTime;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

    // Render the scene
    if (SUCCEEDED(pd3dDevice->BeginScene()))
    {
        D3DXMATRIXA16 matView;
        D3DXVECTOR3 vEyePos(0.0f, 0.0f, -200.0f); // 카메라 위치
        D3DXVECTOR3 vLookPos(0.0f, 0.0f, 0.0f); // 카메라가 바라보는 위치
        D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 카메라 위쪽을 가리키는 벡터
        D3DXMatrixLookAtLH(&matView, &vEyePos, &vLookPos, &vUpVec); // 뷰행렬 만들기

        D3DXMATRIXA16 matProjection;
        D3DXMatrixPerspectiveFovLH(&matProjection, FOV,
            ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

        //--------------------------------------------------//
        D3DXMATRIXA16 matWorld;
        D3DXMatrixIdentity(&matWorld);

        D3DXMATRIX mTranslate;
        D3DXMatrixTranslation(&mTranslate, 0, 0, 0);

        D3DXMATRIX mScale;
        D3DXMatrixScaling(&mScale, 1, 1, 1);
        D3DXMATRIX mEulerAngle;

        D3DXMATRIX mRotX;
        D3DXMATRIX mRotY;
        D3DXMATRIX mRotZ;
        D3DXMatrixRotationX(&mRotX, D3DXToRadian(0));
        D3DXMatrixRotationY(&mRotY, yRotation);
        D3DXMatrixRotationZ(&mRotZ, D3DXToRadian(0));
        mEulerAngle = mRotZ * mRotX * mRotY;
    
        matWorld = mEulerAngle * mScale * mTranslate;

        DEVICE->SetTransform(D3DTS_WORLD, &matWorld);
        DEVICE->SetTransform(D3DTS_VIEW, &matView);
        DEVICE->SetTransform(D3DTS_PROJECTION, &matProjection);
        //--------------------------------------------------//
        D3DXVECTOR4 eyePos(vEyePos);
        //--------------------------------------------------//
        D3DXMATRIXA16 matWorldView;
        D3DXMATRIXA16 matWorldViewProj;
        D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
        D3DXMatrixMultiply(&matWorldViewProj, &matWorldView, &matProjection);
        //--------------------------------------------------//
        D3DXVECTOR4 gWorldLightPosition(500, 500, -500, 1);
        D3DXVECTOR4 gWorldCameraPosition(vEyePos);
        D3DXVECTOR4 gLightColor(1, 1, 1, 1);
        //--------------------------------------------------//

        D3DXMATRIXA16 matInvWorld;
        D3DXMatrixTranspose(&matInvWorld, &matWorld);

        LPD3DXEFFECT effect;
        switch (Util::GetIns()->num)
        {
        case 1:
        {
            effect = shader->colorShader;
            effect->SetMatrix((D3DXHANDLE)"gWorldMatrix", &matWorld);
            effect->SetMatrix((D3DXHANDLE)"gViewMatrix", &matView);
            effect->SetMatrix((D3DXHANDLE)"gProjectionMatrix", &matProjection);
        }
        break;
        case 2:
        {
            effect = shader->textureMappingShader;
            effect->SetMatrix((D3DXHANDLE)"gWorldMatrix", &matWorld);
            effect->SetMatrix((D3DXHANDLE)"gViewMatrix", &matView);
            effect->SetMatrix((D3DXHANDLE)"gProjectionMatrix", &matProjection);

            effect->SetTexture((D3DXHANDLE)"gDiffuseTexture", texture->earthDiffuse);
        }
        break;
        case 3:
        {
            effect = shader->lightShader;
            effect->SetMatrix((D3DXHANDLE)"gWorldMatrix", &matWorld);
            effect->SetMatrix((D3DXHANDLE)"gViewMatrix", &matView);
            effect->SetMatrix((D3DXHANDLE)"gProjectionMatrix", &matProjection);
            
            effect->SetVector((D3DXHANDLE)"gWorldLightPosition", &gWorldLightPosition);
        }
        break;
        case 4:
        {
            effect = shader->specularlightShader;
            effect->SetMatrix((D3DXHANDLE)"gWorldMatrix", &matWorld);
            effect->SetMatrix((D3DXHANDLE)"gViewMatrix", &matView);
            effect->SetMatrix((D3DXHANDLE)"gProjectionMatrix", &matProjection);

            effect->SetVector((D3DXHANDLE)"gWorldLightPosition", &gWorldLightPosition);
            effect->SetVector((D3DXHANDLE)"gWorldCameraPosition", &eyePos);
            effect->SetFloat((D3DXHANDLE)"gSpecularPower", 20.0f);
        }
        break;
        case 5:
        {
            effect = shader->specularMappingShader;

            effect->SetMatrix((D3DXHANDLE)"gWorldViewProjectionMatrix", &matWorldViewProj);
            effect->SetMatrix((D3DXHANDLE)"gInvWorld", &matInvWorld);

            effect->SetVector((D3DXHANDLE)"gWorldLightPosition", &gWorldLightPosition);
            effect->SetVector((D3DXHANDLE)"gWorldCameraPosition", &gWorldCameraPosition);
            effect->SetVector((D3DXHANDLE)"gLightColor", &gLightColor);

            effect->SetTexture((D3DXHANDLE)"DiffuseMap", texture->brickDiffuse);
            effect->SetTexture((D3DXHANDLE)"SpecularMap", texture->brickSpecular);

        }
        break;
        case 6:
        {
            effect = shader->toonShader;

            effect->SetMatrix((D3DXHANDLE)"gWorldViewProjectionMatrix", &matWorldViewProj);
            effect->SetMatrix((D3DXHANDLE)"gInvWorld", &matInvWorld);

            effect->SetVector((D3DXHANDLE)"gWorldLightPosition", &gWorldLightPosition);
            effect->SetVector((D3DXHANDLE)"gLightColor", &gLightColor);
        }
        break;
        case 7:
        {
            effect = shader->normalMappingShader;

            effect->SetMatrix((D3DXHANDLE)"gWorldViewProjectionMatrix", &matWorldViewProj);
            effect->SetMatrix((D3DXHANDLE)"gWorldMatrix", &matWorld);
            effect->SetVector((D3DXHANDLE)"gWorldLightPosition", &gWorldLightPosition);
            effect->SetVector((D3DXHANDLE)"gWorldCameraPosition", &gWorldCameraPosition);
            effect->SetVector((D3DXHANDLE)"gLightColor", &gLightColor);

            effect->SetTexture((D3DXHANDLE)"DiffuseMap", texture->brickDiffuse);
            effect->SetTexture((D3DXHANDLE)"SpecularMap", texture->brickSpecular);
            effect->SetTexture((D3DXHANDLE)"NormalMap", texture->brickNormal);

            effect->SetFloat((D3DXHANDLE)"gSpecularPower", 20.0f);
        }
        break;
        default:
            break;
        }

        UINT passnum;
        effect->Begin(&passnum, NULL);
        for (UINT i = 0; i < passnum; i++)
        {
            effect->BeginPass(i);
            mesh->Render();
            effect->EndPass();
        }
        effect->End();
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
    mesh->Release();
    shader->Release();
    texture->Release();
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
//INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
int main()
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // TODO: Perform any application-level initialization here

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"DXFrameworks" );
    DXUTCreateDevice( true, WIN_WIDTH, WIN_HEIGHT);

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here

    return DXUTGetExitCode();
}



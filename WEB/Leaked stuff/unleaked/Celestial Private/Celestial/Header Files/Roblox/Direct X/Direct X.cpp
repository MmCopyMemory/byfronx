#pragma once

#include <Windows.h>
#include "Direct X.h"
#include <GUIFramework/GUIFramework.h>
#include <GUI/GUI.h>

#include <GUIFramework/Fonts/Akira Expanded Demo.c>
#include <GUIFramework/Fonts/fa-solid-900.c>
#include <GUIFramework/Fonts/Poppins-SemiBold.c>

#include <GUIFramework/FontStore.h>
#include <Renderer/Renderer.h>
#include <thread>
#include <Roblox/Globals/Globals.h>
#include <TlHelp32.h>
#include <iostream>

Celestial::GUI GGUI;
Celestial::Renderer Render;

int menu_bind[] = {
VK_INSERT,
VK_F1,
VK_F2,
VK_F3,
VK_F4,
VK_F5,
VK_F6,
VK_F7,
VK_F8,
VK_F9,
VK_F10,
VK_F11,
VK_F12,
VK_RSHIFT,
VK_LSHIFT,
VK_RCONTROL,
VK_LCONTROL,
};


// Data
ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
ID3D11Texture2D* g_pd3dTexture = NULL;

DWORD_PTR* D3D11SwapChain_VTABLE;
DWORD_PTR* D3D11Device_VTABLE;
DWORD_PTR* D3D11Context_VTABLE;

/*ImGui Data*/
bool Shown = true;
bool First = true;

typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* D3D11DrawHook) (ID3D11DeviceContext* pContext, UINT VertexCount, UINT StartVertexLocation);
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* D3D11DrawInstancedHook) (ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall* D3D11DrawIndexedInstancedHook) (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall* D3D11DrawInstancedIndirectHook) (ID3D11DeviceContext* pContext, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void(__stdcall* D3D11DrawIndexedInstancedIndirectHook) (ID3D11DeviceContext* pContext, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void(__stdcall* D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews);
typedef void(__stdcall* D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);

D3D11PresentHook phookD3D11Present = NULL;
D3D11DrawHook phookD3D11Draw = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11DrawInstancedHook phookD3D11DrawInstanced = NULL;
D3D11DrawIndexedInstancedHook phookD3D11DrawIndexedInstanced = NULL;
D3D11DrawInstancedIndirectHook phookD3D11DrawInstancedIndirect = NULL;
D3D11DrawIndexedInstancedIndirectHook phookD3D11DrawIndexedInstancedIndirect = NULL;
D3D11PSSetShaderResourcesHook phookD3D11PSSetShaderResources = NULL;
D3D11CreateQueryHook phookD3D11CreateQuery = NULL;

//vertex
ID3D11Buffer* veBuffer;
UINT Stride = 0;
UINT veBufferOffset = 0;
D3D11_BUFFER_DESC vedesc;

//index
ID3D11Buffer* inBuffer;
DXGI_FORMAT inFormat;
UINT        inOffset;
D3D11_BUFFER_DESC indesc;

//shader
ID3D11PixelShader* psRed = NULL;
ID3D11PixelShader* psGreen = NULL;

//wh
char* state;
ID3D11RasterizerState* rwState;
ID3D11RasterizerState* rsState;

enum eDepthState
{
	ENABLED,
	DISABLED,
	READ_NO_WRITE,
	NO_READ_NO_WRITE,
	_DEPTH_COUNT
};


ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

//pssetshaderresources
UINT pssrStartSlot;
D3D11_SHADER_RESOURCE_VIEW_DESC  Descr;
ID3D11ShaderResourceView* ShaderResourceView;

typedef HRESULT(__stdcall* HookPresent) (IDXGISwapChain* D3D11SwapChain, UINT SyncInterval, UINT Flags);
HookPresent PRESENT_hook;
typedef HRESULT(__stdcall* resize) (IDXGISwapChain* this_ptr, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);
resize RESIZE_hook;
WNDPROC orig;
LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }
extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//generate shader func
HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
{
	char szCast[] = "struct VS_OUT"
		"{"
		" float4 Position : SV_Position;"
		" float4 Color : COLOR0;"
		"};"

		"float4 main( VS_OUT input ) : SV_Target"
		"{"
		" float4 fake;"
		" fake.a = 1.0f;"
		" fake.r = %f;"
		" fake.g = %f;"
		" fake.b = %f;"
		" return fake;"
		"}";
	ID3D10Blob* pBlob;
	char szPixelShader[1000];

	sprintf_s(szPixelShader, szCast, r, g, b);

	ID3DBlob* d3dErrorMsgBlob;

	HRESULT hr = D3DCompile(szPixelShader, sizeof(szPixelShader), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &pBlob, &d3dErrorMsgBlob);

	if (FAILED(hr))
		return hr;

	hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

LRESULT __stdcall WindowProc(const HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, message, w_param, l_param);

	return CallWindowProc(orig, hwnd, message, w_param, l_param);
}

// https://stackoverflow.com/questions/14029626/getting-a-specific-window-name-in-c
// Added a struct for easier return and passing of the PID
// One note. Fuck wstring with GetWindowTextW.
struct Window
{
	DWORD PID;
	HWND Handle;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	Window& pid = *(Window*)(lParam);

	DWORD WinId;
	GetWindowThreadProcessId(hwnd, &WinId);

	if (WinId == pid.PID)
		return FALSE;

	return TRUE;
}

// Find the main window of the program, returns 0 if not found
HWND FindMainWindow()
{
	Window ToFind = { GetCurrentProcessId(), 0 };

	EnumWindows(EnumWindowsProc, (LPARAM)&ToFind);

	return ToFind.Handle;
}

HWND main_window = 0;
HWND GetMainWindow()
{
	// Set the proper scaling
	if (!main_window)
		main_window = FindMainWindow();

	return main_window;
}


HRESULT __stdcall PRESENT(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags)
{

	if (First)
	{
		swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&g_pd3dDevice));
		swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&g_pd3dTexture));

		orig = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(FindWindow(NULL, L"Roblox"), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc)));

		g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

		//create depthstencilstate
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		g_pd3dDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::ENABLED)]);

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		g_pd3dDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::DISABLED)]);

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;
		g_pd3dDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		g_pd3dDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::READ_NO_WRITE)]);

		//wireframe
		D3D11_RASTERIZER_DESC rwDesc;
		g_pd3dDeviceContext->RSGetState(&rwState); // retrieve the current state
		rwState->GetDesc(&rwDesc);    // get the desc of the state
		rwDesc.FillMode = D3D11_FILL_WIREFRAME;
		rwDesc.CullMode = D3D11_CULL_NONE;
		// create a whole new rasterizer state
		g_pd3dDevice->CreateRasterizerState(&rwDesc, &rwState);

		//solid
		D3D11_RASTERIZER_DESC rsDesc;
		g_pd3dDeviceContext->RSGetState(&rsState); // retrieve the current state
		rsState->GetDesc(&rsDesc);    // get the desc of the state
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_BACK;
		// create a whole new rasterizer state
		g_pd3dDevice->CreateRasterizerState(&rsDesc, &rsState);

		g_pd3dDevice->CreateRenderTargetView(g_pd3dTexture, nullptr, &g_mainRenderTargetView);
		g_pd3dTexture->Release();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = NULL;
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Button] = ImColor(210, 115, 138);
		colors[ImGuiCol_ButtonActive] = ImColor(210, 115, 138);
		colors[ImGuiCol_ButtonHovered] = ImColor(210, 115, 138);
		colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30);
		colors[ImGuiCol_FrameBgActive] = ImColor(30, 30, 30);
		colors[ImGuiCol_FrameBgHovered] = ImColor(30, 30, 30);
		colors[ImGuiCol_Header] = ImColor(30, 30, 30);
		colors[ImGuiCol_HeaderActive] = ImColor(30, 30, 30);
		colors[ImGuiCol_HeaderHovered] = ImColor(30, 30, 30);

		Celestial::Fonts::TitleFont = io.Fonts->AddFontFromMemoryTTF(MainTitle, sizeof(MainTitle), 20);
		Celestial::Fonts::CheckFont = io.Fonts->AddFontFromMemoryTTF(Poppins, sizeof(Poppins), 15);
		Celestial::Fonts::SmallPoppins = io.Fonts->AddFontFromMemoryTTF(Poppins, sizeof(Poppins), 13);

		Celestial::Fonts::RenderFont = io.Fonts->AddFontFromMemoryTTF(Poppins, sizeof(Poppins), 11);
		Celestial::Fonts::BigRenderFont = io.Fonts->AddFontFromMemoryTTF(MainTitle, sizeof(MainTitle), 17);

		Celestial::Fonts::SmallTitle = io.Fonts->AddFontFromMemoryTTF(MainTitle, sizeof(MainTitle), 9);
		static const ImWchar icons_ranges[] = { 0x21, 0xf8ff, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		Celestial::Fonts::IconFont = io.Fonts->AddFontFromMemoryTTF(Solid, sizeof(Solid), 15.0f, &icons_config, icons_ranges);


		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(FindWindow(0, L"Roblox"));
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		First = false;
	}

	if (g_mainRenderTargetView == nullptr)
	{
		swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&g_pd3dTexture));
		g_pd3dDevice->CreateRenderTargetView(g_pd3dTexture, nullptr, &g_mainRenderTargetView);

		g_pd3dTexture->Release();
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	if (GetAsyncKeyState(menu_bind[Celestial::Globals::MenuKey]) & 1)
	{
		Shown = !Shown;
	}

	ImGui::NewFrame();

	__try {

		if (RenderSettings.Enabled)
		{
			ImGui::PushFont(Celestial::Fonts::RenderFont);
			Render.Hook();
			ImGui::PopFont();
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		if (RenderSettings.Enabled)
		{
			ImGui::PushFont(Celestial::Fonts::RenderFont);
			Render.Hook();
			ImGui::PopFont();
		}
	}


	if (Shown)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
		bgDrawList->AddRectFilled(ImVec2(0, 0), ImVec2(viewport->Size.x, viewport->Size.y), ImColor(0.0f, 0.0f, 0.0f, 0.5f));
		GGUI.Load();
	}

	ImGui::Render();

	//ImGui::GetIO().MouseDrawCursor = Shown;


	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);

	// Scale fixes
	float scale = ImGui_ImplWin32_GetDpiScaleForHwnd(GetMainWindow());
	scale = scale ? scale : 1.0f;  // If somehow 0?

	ImDrawData* data = ImGui::GetDrawData();
	for (int i = 0; i < data->CmdListsCount; i++)
	{
		for (auto& obj : data->CmdLists[i]->VtxBuffer)
		{
			obj.pos.x *= scale;
			obj.pos.y *= scale;
		}

		for (auto& obj : data->CmdLists[i]->CmdBuffer)
		{
			obj.ClipRect.x *= scale;
			obj.ClipRect.y *= scale;
			obj.ClipRect.z *= scale;
			obj.ClipRect.w *= scale;
		}
	}

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return PRESENT_hook(swap_chain, sync_interval, flags);
}

void SetDepthStencilState(eDepthState aState)
{
	g_pd3dDeviceContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
}

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	//if (GetAsyncKeyState(VK_F9) & 1)
		//Log("DrawIndexed called");

	//get stride & vdesc.ByteWidth

	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	if (veBuffer)
		veBuffer->GetDesc(&vedesc);
	if (veBuffer != NULL) { veBuffer->Release(); veBuffer = NULL; }

	//get indesc.ByteWidth
	pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
	if (inBuffer)
		inBuffer->GetDesc(&indesc);
	if (inBuffer != NULL) { inBuffer->Release(); inBuffer = NULL; }

	//wallhack example
	if (Stride == 44)
	{
		SetDepthStencilState(DISABLED);
		pContext->PSSetShader(psRed, NULL, NULL);
		phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
		pContext->PSSetShader(psGreen, NULL, NULL);
		//if (pssrStartSlot == 1) //if black screen, find correct pssrStartSlot
		SetDepthStencilState(READ_NO_WRITE);
	}

	return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

//==========================================================================================================================

void __stdcall hookD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	pssrStartSlot = StartSlot;

	for (UINT j = 0; j < NumViews; j++)
	{
		//Resources loop
		ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[j];
		if (pShaderResView)
		{
			pShaderResView->GetDesc(&Descr);
			//Descr.Buffer.NumElements;
			//Descr.Format;

			if (
				(Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFER)
				||
				(Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFEREX)
				)
			{
				continue;//Skip buffer resources
			}

		}// if( pShaderResView )
	}// for

	/*
	//alternative wallhack example for f'up games
	if (Descr.Format == 56)
	{
		pContext->PSSetShader(psRed, NULL, NULL);
		SetDepthStencilState(DISABLED);
	}
	else
		if(pssrStartSlot == 1) //if black screen, find correct pssrStartSlot
		SetDepthStencilState(READ_NO_WRITE);
	*/
	return phookD3D11PSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);
}

//==========================================================================================================================

void __stdcall hookD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery)
{
	//Disable Occlusion which prevents rendering player models through certain objects (used by wallhack to see models through walls at all distances, REDUCES FPS)
	if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
	{
		D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
		(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
		(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;

		return phookD3D11CreateQuery(pDevice, &oqueryDesc, ppQuery);
	}

	return phookD3D11CreateQuery(pDevice, pQueryDesc, ppQuery);
}


//==========================================================================================================================

void __stdcall hookD3D11Draw(ID3D11DeviceContext* pContext, UINT VertexCount, UINT StartVertexLocation)
{

	return phookD3D11Draw(pContext, VertexCount, StartVertexLocation);
}

//==========================================================================================================================

void __stdcall hookD3D11DrawInstanced(ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{

	return phookD3D11DrawInstanced(pContext, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

//==========================================================================================================================

void __stdcall hookD3D11DrawIndexedInstanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	return phookD3D11DrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

//==========================================================================================================================

void __stdcall hookD3D11DrawInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs)
{

	return phookD3D11DrawInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
}

//==========================================================================================================================

void __stdcall hookD3D11DrawIndexedInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs)
{

	return phookD3D11DrawIndexedInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
}

HRESULT __stdcall RESIZE(IDXGISwapChain* this_ptr, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags)
{
	if (g_mainRenderTargetView)
	{
		g_mainRenderTargetView->Release();
		g_mainRenderTargetView = nullptr;
	}
	return RESIZE_hook(this_ptr, buffer_count, width, height, new_format, swap_chain_flags);
}


Celestial::DirectX::DirectX()
{
	const std::uintptr_t base_address = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("DiscordHook.dll"));

	HMODULE hDXGIDLL = 0;
	do {
		hDXGIDLL = GetModuleHandleA("dxgi.dll");
		Sleep(500);
	} while (!hDXGIDLL);

	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

	if (base_address)
	{
		PRESENT_hook = *reinterpret_cast<decltype(PRESENT_hook)*>(base_address + 0xB3ED4);
		*reinterpret_cast<void**>(base_address + 0xB3ED4) = reinterpret_cast<void*>(&PRESENT);

		RESIZE_hook = *reinterpret_cast<decltype(RESIZE_hook)*>(base_address + 0xB3EE8);
		*reinterpret_cast<void**>(base_address + 0xB3EE8) = reinterpret_cast<void*>(&RESIZE);

	}
	else
	{

		D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
		D3D_FEATURE_LEVEL obtainedLevel;
		ID3D11Device* d3dDevice = nullptr;
		ID3D11DeviceContext* d3dContext = nullptr;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext))) {
		}


		std::uintptr_t* vt_swapchain;
		memcpy(&vt_swapchain, reinterpret_cast<LPVOID>(g_pSwapChain), sizeof(std::uintptr_t));

		DWORD old_protection;
		VirtualProtect(vt_swapchain, sizeof(std::uintptr_t), PAGE_EXECUTE_READWRITE, &old_protection);

		PRESENT_hook = reinterpret_cast<decltype(PRESENT_hook)>(vt_swapchain[8]);
		RESIZE_hook = reinterpret_cast<decltype(RESIZE_hook)>(vt_swapchain[13]);
		//phookD3D11Draw = reinterpret_cast<decltype(phookD3D11Draw)>(vt_swapchain[13]);
		//phookD3D11DrawIndexed = reinterpret_cast<decltype(phookD3D11DrawIndexed)>(vt_swapchain[12]);

		vt_swapchain[8] = reinterpret_cast<std::uintptr_t>(&PRESENT);
		vt_swapchain[13] = reinterpret_cast<std::uintptr_t>(&RESIZE);

		VirtualProtect(vt_swapchain, sizeof(std::uintptr_t), old_protection, &old_protection);
	}

}

#include "Graphics.h"
#include "CustomException.h"
#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd{ sizeof(sd) };

	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, nullptr, 0, D3D11_SDK_VERSION, &sd, &_pSwapChain,
		&_pDevice, nullptr, &_pDeviceCtx);

	if (!SUCCEEDED(hr))
		throw std::exception("Faild creating the DX objects.");

	ID3D11Resource* pBackBuffer = nullptr;

	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

	if (!SUCCEEDED(hr))
		throw std::exception("Faild creating the DX objects.");

	hr = _pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pTarget);

	if (!SUCCEEDED(hr))
		throw std::exception("Faild creating the render object.");

	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	SafeRelease(_pDevice);
	SafeRelease(_pDeviceCtx);
	SafeRelease(_pSwapChain);
	SafeRelease(_pTarget);
}

void Graphics::EndFrame()
{
	_pSwapChain->Present(1u, 0u);
}

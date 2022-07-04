#pragma once
#include <d3d11.h>

template <class _Ty>
void SafeRelease(_Ty& obj)
{
	if (obj != nullptr)
	{
		obj->Release();

		obj = nullptr;
	}
}

class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	Graphics(const Graphics&) = delete;
	Graphics operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept
	{
		const float colors[] = { r, g, b, 1.0f };
		_pDeviceCtx->ClearRenderTargetView(_pTarget, colors);
	}

private:
	ID3D11Device* _pDevice = nullptr;
	IDXGISwapChain* _pSwapChain = nullptr;
	ID3D11DeviceContext* _pDeviceCtx = nullptr;
	ID3D11RenderTargetView* _pTarget = nullptr;
};
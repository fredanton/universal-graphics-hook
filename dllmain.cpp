#include "ug.h"

using namespace::ug;
static std::once_flag g_isInitialized;

#pragma region D3D9
static LPDIRECT3DDEVICE9 g_pd3d9Device = nullptr;

typedef HRESULT(__stdcall* d3d9Present)(LPDIRECT3DDEVICE9);
typedef HRESULT(__stdcall* d3d9EndScene)(LPDIRECT3DDEVICE9);

static d3d9Present g_od3d9Present = nullptr;
static d3d9Present g_od3d9EndScene = nullptr;

HRESULT __stdcall hkd3d9Present
(
	LPDIRECT3DDEVICE9 pDevice
)
{
	static bool init = false;
	if (!init)
	{
		g_pd3d9Device = pDevice;
		init = true;
	}

	return g_od3d9Present(pDevice);
}

HRESULT __stdcall hkd3d9EndScene
(
	LPDIRECT3DDEVICE9 pDevice
)
{
	return g_od3d9EndScene(pDevice);
}
#pragma endregion

#pragma region D3D10
static IDXGISwapChain* g_pd3d10SwapChain = nullptr;
static ID3D10Device* g_pd3d10Device = nullptr;

typedef HRESULT(__stdcall* d3d10Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* d3d10DrawIndexed) (UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef HRESULT(__stdcall* d3d10ClearRenderTargetView)(ID3D10RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

d3d10Present g_od3d10Present = nullptr;
d3d10DrawIndexed g_od3d10DrawIndexed = nullptr;
d3d10ClearRenderTargetView g_od3d10ClearRenderTargetView = nullptr;

HRESULT __stdcall hkd3d10Present
(
	IDXGISwapChain* pSwapChain,
	UINT SyncInterval,
	UINT Flags
)
{
	std::call_once(g_isInitialized, [&]() {
		g_pd3d10SwapChain = pSwapChain;
		pSwapChain->GetDevice(__uuidof(g_pd3d10Device), reinterpret_cast<void**>(&g_pd3d10Device));
		});

	return g_od3d10Present(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall hkd3d10DrawIndexed
(
	UINT IndexCount,
	UINT StartIndexLocation,
	INT BaseVertexLocation
)
{
	return g_od3d10DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

HRESULT __stdcall hkd3d10ClearRenderTargetView
(
	ID3D10RenderTargetView* pRenderTargetView,
	const FLOAT ColorRGBA[4]
)
{
	return g_od3d10ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}
#pragma endregion

#pragma region D3D11
static IDXGISwapChain* g_pd3d11SwapChain = nullptr;
static ID3D11Device* g_pd3d11Device = nullptr;
static ID3D11DeviceContext* g_pd3d11Context = nullptr;

typedef HRESULT(__stdcall* d3d11Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* d3d11DrawIndexed) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef HRESULT(__stdcall* d3d11ClearRenderTargetView) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

d3d11Present g_od3d11Present = nullptr;
d3d11DrawIndexed g_od3d11DrawIndexed = nullptr;
d3d11ClearRenderTargetView g_od3d11ClearRenderTargetView = nullptr;

HRESULT __stdcall hkd3d11Present
(
	IDXGISwapChain* pSwapChain,
	UINT SyncInterval,
	UINT Flags
)
{
	std::call_once(g_isInitialized, [&]() {
		g_pd3d11SwapChain = pSwapChain;
		pSwapChain->GetDevice(__uuidof(g_pd3d11Device), reinterpret_cast<void**>(&g_pd3d11Device));
		g_pd3d11Device->GetImmediateContext(&g_pd3d11Context);
		});

	return g_od3d11Present(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall hkd3d11DrawIndexed
(
	ID3D11DeviceContext* pContext,
	UINT IndexCount,
	UINT StartIndexLocation,
	INT BaseVertexLocation
)
{
	return g_od3d11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

HRESULT __stdcall hkd3d11ClearRenderTargetView
(
	ID3D11DeviceContext* pContext,
	ID3D11RenderTargetView* pRenderTargetView,
	const FLOAT ColorRGBA[4]
)
{
	return g_od3d11ClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);
}
#pragma endregion

#pragma region D3D12
static IDXGISwapChain* g_pd3d12SwapChain = nullptr;
static ID3D12Device* g_pd3d12Device = nullptr;

typedef HRESULT(__stdcall* d3d12Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* d3d12DrawInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef HRESULT(__stdcall* d3d12DrawIndexedInstanced)(ID3D12GraphicsCommandList* dCommandList, UINT IndexCount, UINT InstanceCount, UINT StartIndex, INT BaseVertex);

d3d12Present g_od3d12Present = nullptr;
d3d12DrawInstanced g_od3d12DrawInstanced = nullptr;
d3d12DrawIndexedInstanced g_od3d12DrawIndexedInstanced = nullptr;

HRESULT __stdcall hkd3d12Present
(
	IDXGISwapChain* pSwapChain,
	UINT SyncInterval,
	UINT Flags
)
{
	std::call_once(g_isInitialized, [&]() {
		g_pd3d12SwapChain = pSwapChain;
		pSwapChain->GetDevice(__uuidof(g_pd3d12Device), reinterpret_cast<void**>(&g_pd3d12Device));
		});

	return g_od3d12Present(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall hkd3d12DrawInstanced
(
	ID3D12GraphicsCommandList* dCommandList,
	UINT VertexCountPerInstance,
	UINT InstanceCount,
	UINT StartVertexLocation,
	UINT StartInstanceLocation
)
{
	return g_od3d12DrawInstanced(dCommandList, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

HRESULT __stdcall hkd3d12DrawIndexedInstanced
(
	ID3D12GraphicsCommandList* dCommandList,
	UINT IndexCount,
	UINT InstanceCount,
	UINT StartIndex,
	INT BaseVertex
)
{
	return g_od3d12DrawIndexedInstanced(dCommandList, IndexCount, InstanceCount, StartIndex, BaseVertex);
}
#pragma endregion

#pragma region OpenGL
typedef HRESULT(__stdcall* oglBegin)(GLenum glenum);
typedef HRESULT(__stdcall* oglEnd)(void);
typedef HRESULT(__stdcall* oglClear)(GLbitfield glbitfield);
typedef HRESULT(__stdcall* oglVertex3fv)(const GLfloat* v);
typedef HRESULT(__stdcall* oglVertex3f)(GLfloat x, GLfloat y, GLfloat z);
typedef HRESULT(__stdcall* oglEnable)(GLenum glenum);

oglBegin g_oglBegin = nullptr;
oglEnd g_oglEnd = nullptr;
oglClear g_oglClear = nullptr;
oglVertex3fv g_oglVertex3fv = nullptr;
oglVertex3f g_oglVertex3f = nullptr;
oglEnable g_oglEnable = nullptr;

HRESULT __stdcall hkglBegin(GLenum mode)
{
	return g_oglBegin(mode);
}

HRESULT __stdcall hkglEnd(void)
{
	return g_oglEnd();
}

HRESULT __stdcall hkglVertex3fv(GLfloat* v)
{
	return g_oglVertex3fv(v);
}

HRESULT __stdcall hkglVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	return g_oglVertex3f(x, y, z);
}

HRESULT __stdcall hkglClear(GLbitfield mask)
{
	return g_oglClear(mask);
}

HRESULT __stdcall hkglEnable(GLenum cap)
{
	return g_oglEnable(cap);
}
#pragma endregion

#pragma region Vulkan
typedef HRESULT(__stdcall* vkDrawIndexed) (VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
typedef HRESULT(__stdcall* vkDrawIndexedIndirect) (VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
typedef HRESULT(__stdcall* vkBindVertexBuffers) (VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);

vkDrawIndexed g_ovkDrawIndexed = nullptr;
vkDrawIndexedIndirect g_ovkDrawIndexedIndirect = nullptr;
vkBindVertexBuffers g_ovkBindVertexBuffers = nullptr;

HRESULT __stdcall hkvkDrawIndexed
(
	VkCommandBuffer commandBuffer,
	uint32_t indexCount,
	uint32_t instanceCount,
	uint32_t firstIndex,
	int32_t vertexOffset,
	uint32_t firstInstance
)
{
	return g_ovkDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

HRESULT __stdcall hkvkDrawIndexedIndirect
(
	VkCommandBuffer commandBuffer,
	VkBuffer buffer,
	VkDeviceSize offset,
	uint32_t drawCount,
	uint32_t stride
)
{
	return g_ovkDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

HRESULT __stdcall hkvkBindVertexBuffers
(
	VkCommandBuffer commandBuffer,
	uint32_t firstBinding,
	uint32_t bindingCount,
	const VkBuffer* pBuffers,
	const VkDeviceSize* pOffsets
)
{
	return g_ovkBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
#pragma endregion

int thread()
{
	if (init() == Success)
	{
		GraphicsType render_type = getRenderType();

		switch (render_type) {
		case D3D9:
			bind(17, (void**)& g_od3d9Present, hkd3d9Present);
			bind(42, (void**)& g_od3d9EndScene, hkd3d9EndScene);
			break;
		case D3D10:
			bind(8, (void**)& g_od3d10Present, hkd3d10Present);
			bind(73, (void**)& g_od3d10DrawIndexed, hkd3d10DrawIndexed);
			bind(111, (void**)& g_od3d10ClearRenderTargetView, hkd3d10ClearRenderTargetView);
			break;
		case D3D11:
			bind(8, (void**)& g_od3d11Present, hkd3d11Present);
			bind(73, (void**)& g_od3d11DrawIndexed, hkd3d11DrawIndexed);
			bind(111, (void**)& g_od3d11ClearRenderTargetView, hkd3d11ClearRenderTargetView);
			break;
		case D3D12:
			bind(140, (void**)& g_od3d12Present, hkd3d12Present);
			//bind(84, (void**)& hkd3d12DrawInstanced, hkd3d12DrawInstanced);
			//bind(85, (void**)& hkd3d12DrawIndexedInstanced, hkd3d12DrawIndexedInstanced);
			break;
		case GDI:
			// Not implemented	
			break;
		case GDIPLUS:
			// Not implemented
			break;
		case OpenGL:
			bind(321, (void**)& g_oglVertex3fv, hkglVertex3fv);
			bind(320, (void**)& g_oglVertex3f, hkglVertex3f);
			bind(4, (void**)& g_oglBegin, hkglBegin);
			bind(74, (void**)& g_oglEnd, hkglEnd);
			bind(10, (void**)& g_oglClear, hkglClear);
			bind(72, (void**)& g_oglEnable, hkglEnable);
			break;
		case Vulkan:
			bind(106, (void**)& g_ovkDrawIndexed, hkvkDrawIndexed);
			bind(108, (void**)& g_ovkDrawIndexedIndirect, hkvkDrawIndexedIndirect);
			bind(104, (void**)& g_ovkBindVertexBuffers, hkvkBindVertexBuffers);
			break;
		}
	}

	do {
		Sleep(100);
	} while (!(GetAsyncKeyState(VK_END) & 0x1));

	exit(-1);

	return 0;
}

BOOL WINAPI DllMain
(
	HINSTANCE hInstance,
	DWORD dwReason,
	LPVOID lpReserved
)
{
	DisableThreadLibraryCalls(hInstance);

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, 0, NULL);
		break;

		return TRUE;
	}
}

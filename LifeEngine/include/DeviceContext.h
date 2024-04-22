#pragma once
#include "Prerequisites.h"

class
	DeviceContext {
public:
	DeviceContext() = default;
	~DeviceContext() { SAFE_RELEASE(m_deviceContext) };

	void
		init();

	void
		update();

	void
		render();

	void
		destroy();

	//Draw indexed, non-instanced primitives.
	void
		DrawIndexed(unsigned int IndexCount,
			unsigned int StartIndexLocation,
			unsigned int BaseVertexLocation);

	//Set an array of sampler states to the pixel shader pipeline stage.
	void
		PSSetSamplers(unsigned int StartSlot,
			unsigned int NumSamplers,
			ID3D11SamplerState* const* ppSamplers);

	//Bind an array of shader resources to the pixel shader stage.
	void
		PSSetShaderResources(unsigned int StartSlot,
			unsigned int NumViews,
			ID3D11ShaderResourceView* const* ppShaderResourceViews);

	//Sets the constant buffers used by the pixel shader pipeline stage.
	void
		PSSetConstantBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppConstantBuffers);

	//Sets a pixel shader to the device.
	void
		PSSetShader(ID3D11PixelShader* pPixelShader,
			ID3D11ClassInstance* const* ppClassInstances,
			unsigned int NumClassInstances);

	//Sets the constant buffers used by the vertex shader pipeline stage.
	void
		VSSetConstantBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppConstantBuffers);

	//Set a vertex shader to the device.
	void
		VSSetShader(ID3D11VertexShader* pVertexShader,
			ID3D11ClassInstance* const* ppClassInstances,
			unsigned int NumClassInstances);

	//Clears the depth-stencil resource.
	void
		ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
			unsigned int ClearFlags,
			float Depth,
			unsigned int Stencil);

	//Set all the elements in a render target to one value.
	void
		ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
			const float ColorRGBA[4]);

	//Bind one or more render targets atomically and the depth-stencil buffer
	//to the output-merger stage.
	void
		OMSetRenderTargets(unsigned int NumViews,
			ID3D11RenderTargetView* const* ppRenderTargetViews,
			ID3D11DepthStencilView* pDepthStencilView);

	//Bind an array of viewports to the rasterizer stage of the pipeline.
	void
		RSSetViewports(unsigned int NumViewports,
			const D3D11_VIEWPORT* pViewports);

	//Bind an input-layout object to the input-assembler stage.
	void
		IASetInputLayout(ID3D11InputLayout* pInputLayout);

	//Bind an array of vertex buffers to the input-assembler stage.
	void
		IASetVertexBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppVertexBuffers,
			const unsigned int* pStrides,
			const unsigned int* pOffsets);

	//Bind an index buffer to the input-assembler stage.
	void
		IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
			DXGI_FORMAT Format,
			unsigned int Offset);

	//Bind information about the primitive type, and data order that describes 
	//input data for the input assembler stage.
	void
		IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

	//The CPU copies data from memory to a subresource created in non-mappable memory.
	void
		UpdateSubresource(ID3D11Resource* pDstResource,
			unsigned int DstSubresource,
			const D3D11_BOX* pDstBox,
			const void* pSrcData,
			unsigned int SrcRowPitch,
			unsigned int SrcDepthPitch);

public:
	//The ID3D11DeviceContext interface represents a device context 
	//which generates rendering commands.
	ID3D11DeviceContext* m_deviceContext = nullptr;
};

#include "InputLayout.h"
#include "Device.h"
#include "DeviceContext.h"

void InputLayout::init(Device device,
	                   std::vector<D3D11_INPUT_ELEMENT_DESC> Layout,
	                   ID3DBlob* VertexShaderData)
{
	if (Layout.size() == 0)
	{
		WARNING("ERROR: InputLayout::init : Error in data from params [CHECK FOR std::vector<D3D11_INPUT_ELEMENT_DESC> Layout]\n");
		exit(1);
	}
	else if (VertexShaderData == nullptr)
	{
		WARNING("ERROR: InputLayout::init : Error in data from params [CHECK FOR ID3DBlob* VertexShaderData]\n");
		exit(1);
	}
	//Create the resource
	device.CreateInputLayout(Layout.data(),
		Layout.size(),
		VertexShaderData->GetBufferPointer(),
		VertexShaderData->GetBufferSize(),
		&m_inputLayout);
}

void InputLayout::update()
{
}

void
InputLayout::render(DeviceContext& deviceContext)
{
	deviceContext.IASetInputLayout(m_inputLayout);
}

void InputLayout::destroy()
{
	SAFE_RELEASE(m_inputLayout);
}
#include "SamplerState.h"
#include "Device.h"
#include "DeviceContext.h"

void SamplerState::init(Device device)
{
	if (device.m_device == nullptr)
	{
		WARNING("ERROR: SampplerState::init : Error in data from params [CHECK FOR Device device] \n");
		exit(1);
	}

    HRESULT hr = S_OK;
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
    hr = device.CreateSamplerState(&sampDesc, &m_sampler);
    if (FAILED(hr))
    {
        WARNING("ERROR: SamplerState::init : Error in data from method [CHECK FOR CreateSamplerState()]\n");
        exit(1);
    }
}

void SamplerState::update()
{
}

void SamplerState::render(DeviceContext& deviceContext, 
                          unsigned int StartSlot, 
                          unsigned int NumSamplers)
{
    deviceContext.PSSetSamplers(StartSlot, NumSamplers, &m_sampler);
}

void SamplerState::destroy()
{
    SAFE_RELEASE(m_sampler);
}



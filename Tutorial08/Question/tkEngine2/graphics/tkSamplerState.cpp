/*!
 * @brief	�T���v���X�e�[�g
 */

#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/tkEngine.h"
#include "tkEngine2/graphics/tkSamplerState.h"

namespace tkEngine2{
	/*!
	 * @brief	�T���v���X�e�[�g���쐬�B
	 */
	bool CSamplerState::Create(const D3D11_SAMPLER_DESC& desc)
	{
		Release();
		HRESULT hr = Engine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
		if (FAILED(hr)) {
			TK_LOG("Failed CSamplerState::Create\n");
			return false;
		}
		return true;
	}
}
/*!
 * @brief	�����_�����O�R���e�L�X�g�B
 */

#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/graphics/tkRenderContext.h"
#include "tkEngine2/graphics/tkRenderTarget.h"

namespace tkEngine2{
	void CRenderContext::Init(ID3D11DeviceContext* pD3DDeviceContext)
	{
		TK_ASSERT(pD3DDeviceContext != nullptr, "pD3DDeviceContext��NULL�ł��B�쐬���Ă���Ă�łˁB");
		m_pD3DDeviceContext = pD3DDeviceContext;
	}
	void CRenderContext::OMSetRenderTargets(unsigned int NumViews, CRenderTarget* renderTarget)
	{
		TK_ASSERT(NumViews <= MRT_MAX, "NumViews is invalid");
		
		ZeroMemory(m_renderTargetViews, sizeof(m_renderTargetViews));
		m_depthStencilView = nullptr;
		if (renderTarget != nullptr) {
			m_depthStencilView = renderTarget[0].GetDepthStencilView();
			for (unsigned int i = 0; i < NumViews; i++) {
				m_renderTargetViews[i] = renderTarget[i].GetRenderTargetView();
			}
		}
		m_pD3DDeviceContext->OMSetRenderTargets(NumViews, m_renderTargetViews, m_depthStencilView);
		m_numRenderTargetView = NumViews;
	}
}
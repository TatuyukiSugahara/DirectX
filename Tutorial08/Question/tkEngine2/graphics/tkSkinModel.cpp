/*!
 *@brief	�X�L�����f���B
 */

#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/graphics/tkSkinModel.h"
#include "tkEngine2/graphics/tkSkinModelData.h"
#include "tkEngine2/tkEngine.h"

namespace tkEngine2{
	CSkinModel::CSkinModel()
	{
	}
	CSkinModel::~CSkinModel()
	{
	}
	void CSkinModel::Init(CSkinModelData& modelData)
	{
		m_skinModelData = &modelData;
		m_cb.Create(NULL, sizeof(SVSConstantBuffer));
	}
	
	void CSkinModel::Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale)
	{
		CMatrix mScale, mTrans, mRot;
		mScale.MakeScaling(scale);
		mRot.MakeRotationFromQuaternion(rot);
		mTrans.MakeTranslation(trans);
		m_worldMatrix.Mul(mScale, mRot);
		m_worldMatrix.Mul(m_worldMatrix, mTrans);
	}
	void CSkinModel::Draw(
		CRenderContext& renderContext, 
		const CMatrix& viewMatrix, 
		const CMatrix& projMatrix,
		bool isZPrepass
	)
	{
		(void)renderContext;
		if (m_skinModelData != nullptr) {
			DirectX::CommonStates state(Engine().GetD3DDevice());
			//�萔�o�b�t�@���X�V�B
			SVSConstantBuffer vsCb;
			vsCb.mWorld = m_worldMatrix;
			vsCb.mProj = projMatrix;
			vsCb.mView = viewMatrix;
			vsCb.screenParam.x = 0.0f;
			vsCb.screenParam.y = 0.0f;
			vsCb.screenParam.z = Engine().GetFrameBufferWidth();
			vsCb.screenParam.w = Engine().GetFrameBufferHeight();
			vsCb.isZPrepass = isZPrepass ? 1 : 0;
			renderContext.UpdateSubresource(m_cb, vsCb);
			renderContext.VSSetConstantBuffer(0, m_cb);
			renderContext.PSSetConstantBuffer(0, m_cb);
			m_skinModelData->GetBody().Draw(
				Engine().GetD3DDeviceContext(),
				state,
				m_worldMatrix,
				viewMatrix,
				projMatrix
			);
		}
	}
}

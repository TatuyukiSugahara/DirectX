/*!
 * @brief	�J����
 */
#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/graphics/tkCamera.h"
#include "tkEngine2/tkEngine.h"

namespace tkEngine2{
	CCamera::CCamera() :
		m_near(30.1f),
		m_far(500.0f),
		m_viewAngle(CMath::DegToRad(60.0f)),
		m_aspect(0.0f),
		m_isNeedUpdateProjectionMatrix(true),
		m_position( CVector3::Zero),
		m_up( CVector3::Up),
		m_target( CVector3::Zero),
		m_viewMatrix( CMatrix::Identity ),
		m_projectionMatrix( CMatrix::Identity ),
		m_viewProjectionMatrix( CMatrix::Identity ),
		m_viewMatrixInv( CMatrix::Identity )
	{
	}
	CCamera::~CCamera()
	{
	}
	/*!
	 * @brief
	 */
	void CCamera::Update()
	{
		if(m_isNeedUpdateProjectionMatrix){
			float aspect = (float)Engine().GetFrameBufferWidth() / (float)Engine().GetFrameBufferHeight();
			//�v���W�F�N�V�����s����v�Z�B
			m_projectionMatrix.MakeProjectionMatrix(
				m_viewAngle,
				aspect,
				m_near,
				m_far
			);
		}
		//�r���[�s��̎Z�o
		m_viewMatrix.MakeLookAt( m_position, m_target, m_up );
		//�r���[�v���W�F�N�V�����s��̍쐬�B
		m_viewProjectionMatrix.Mul(m_viewMatrix, m_projectionMatrix);
		//�r���[�s��̋t�s����v�Z�B
		m_viewMatrixInv.Inverse( m_viewMatrix );
		//�J�����̉�]�s����擾�B
		m_cameraRotation = m_viewMatrixInv;
		m_cameraRotation.m[3][0] = 0.0f;
		m_cameraRotation.m[3][1] = 0.0f;
		m_cameraRotation.m[3][2] = 0.0f;
		m_cameraRotation.m[3][3] = 1.0f;
	}
	void CCamera::CalcScreenPositionFromWorldPosition(CVector2& screenPos, const CVector3& worldPos) const
	{
		float half_w = (float)Engine().GetFrameBufferWidth() * 0.5f;
		float half_h = (float)Engine().GetFrameBufferHeight() * 0.5f;
		CVector4 _screenPos;
		_screenPos.Set(worldPos.x, worldPos.y, worldPos.z, 1.0f);
		m_viewProjectionMatrix.Mul(_screenPos);
		screenPos.x = (_screenPos.x / _screenPos.w)*half_w;
		screenPos.y = (_screenPos.y / _screenPos.w)*half_h;
	}
}


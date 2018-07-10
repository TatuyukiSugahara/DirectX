/*!
 *@brief	�X�L�����f���B
 */

#pragma once

namespace tkEngine2{
	class CSkinModelData;
	/*!
	 *@brief	�X�L�����f���B
	 */
	class CSkinModel : Noncopyable{
	public:
		/*!
		*@brief QueryEffects�����s�����Ƃ��̃R�[���o�b�N�ɓn���������\���́B
		*/
		struct SQuerryEffectsCbArg {
			DirectX::ModelMeshPart*	meshPart;		//���݂̃G�t�F�N�g�B
		};
		CSkinModel();
		~CSkinModel();
		/*!
		*@brief	������
		*/
		void Init(CSkinModelData& modelData);
		/*!
		*@brief	�X�V�B
		*@details
		*@param[in]		trans	���s�ړ��B
		*@param[in]		rot		��]�B
		*@param[in]		scale	�g��B
		*/
		void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale);
		
		/*!
		*@brief	�`��
		*@param[in]	viewMatrix		�r���[�s��B
		*@param[in]	projMatrix		�v���W�F�N�V�����s��B
		*@param[in]	isZPrepass		ZPrepass?
		*/
		void Draw(
			CRenderContext& renderContext, 
			const CMatrix& viewMatrix, 
			const CMatrix& projMatrix,
			bool isZPrepass = false
		);
	private:
		struct SVSConstantBuffer {
			CMatrix mWorld;
			CMatrix mView;
			CMatrix mProj;
			CVector4 screenParam;
			int isZPrepass;
		};
		CSkinModelData*	m_skinModelData = nullptr;
		CMatrix m_worldMatrix = CMatrix::Identity;
		CConstantBuffer m_cb;			//�萔�o�b�t�@�B
	};
}
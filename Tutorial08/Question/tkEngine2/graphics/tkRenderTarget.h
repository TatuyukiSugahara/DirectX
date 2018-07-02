/*!
 * @brief	�����_�����O�^�[�Q�b�g�B
 */

#pragma once

namespace tkEngine2{
	/*!
	 * @brief	�����_�����O�^�[�Q�b�g�B
	 */
	class CRenderTarget{
	public:
		CRenderTarget();
		~CRenderTarget();
		/*!
		 *@brief	�����_�����O�^�[�Q�b�g�̍쐬�B
		 *@param[in]	w					�����_�����O�^�[�Q�b�g�̕��B
		 *@param[in]	h					�����_�����O�^�[�Q�b�g�̍����B
		 *@param[in]	mipLevel			�~�b�v�}�b�v���x���B0���w�肵���ꍇ�̓~�b�v�}�b�v���T�|�[�g����Ă���GPU�ł�1*1�s�N�Z���܂ł̃~�b�v�}�b�v���쐬�����B
		 *@param[in]	colorFormat			�J���[�o�b�t�@�̃t�H�[�}�b�g�B
		 *@param[in]	depthStencilFormat	�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B
		 *@param[in]	multiSampleDesc		�}���`�T���v�����O�̒�`�B
		 *@param[in]	renderTarget		�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`���Bnullptr���w�肳��Ă�����A�����Ńe�N�X�`���𐶐�����B
		 *@param[in]	depthStencil		�f�v�X�X�e���V���ƂȂ�e�N�X�`���Bnullptr���w�肳��Ă�����A�����Ńe�N�X�`���𐶐�����B
		 *@return	true���Ԃ��Ă�����쐬�����B
		 */
		bool Create(
			int w,
			int h,
			int mipLevel,
			DXGI_FORMAT colorFormat,
			DXGI_FORMAT depthStencilFormat,
			DXGI_SAMPLE_DESC multiSampleDesc,
			ID3D11Texture2D* renderTarget = nullptr,
			ID3D11Texture2D* depthStencil = nullptr
		);
		/*!
		 *@brief	�����_�����O�^�[�Q�b�g�̔j���B
		 */
		void Release();
		/*!
		*@brief	�����_�����O�^�[�Q�b�g���擾�B
		*/
		ID3D11Texture2D* GetRenderTarget()
		{
			return m_renderTarget;
		}
		/*!
		*@brief	�����_�����O�^�[�Q�b�g�r���[�̎擾�B
		*/
		ID3D11RenderTargetView* GetRenderTargetView()
		{
			return m_renderTargetView;
		}
		/*!
		*@brief	�����_�����O�^�[�Q�b�g��SRV���擾�B
		*/
		CShaderResourceView& GetRenderTargetSRV()
		{
			return m_renderTargetSRV;
		}
		/*!
		*@brief	�����_�����O�^�[�Q�b�g��UAV���擾�B
		*/
		CUnorderedAccessView& GetRenderTargetUAV()
		{
			return m_renderTargetUAV;
		}
		/*!
		*@brief	�f�v�X�X�e���V���r���[�̎擾�B
		*/
		ID3D11DepthStencilView* GetDepthStencilView()
		{
			return m_depthStencilView;
		}
		
	private:
		ID3D11Texture2D*			m_renderTarget = nullptr;			//!<�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`���B
		ID3D11RenderTargetView*		m_renderTargetView = nullptr;		//!<�����_�[�^�[�Q�b�g�r���[�B
		ID3D11Texture2D*			m_depthStencil = nullptr;			//!<�f�v�X�X�e���V���ƂȂ�e�N�X�`���B
		ID3D11DepthStencilView*		m_depthStencilView = nullptr;		//!<�f�v�X�X�e���V���r���[�B
		CShaderResourceView			m_renderTargetSRV;					//!<�����_�����O�^�[�Q�b�g��SRV�B
		CUnorderedAccessView		m_renderTargetUAV;					//!<�����_�����O�^�[�Q�b�g��UAV�B
	};
}
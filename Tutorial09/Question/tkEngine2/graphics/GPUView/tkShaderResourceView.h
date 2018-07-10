/*!
 *@brief	ShaderResourceView
 */

#pragma once

namespace tkEngine2{
	class CStructuredBuffer;
	class CTextureData;
	/*!
	 *@brief	ShaderResourceView
	 *@details
	 * �e�N�X�`����X�g���N�`���o�b�t�@�ȂǁA�V�F�[�_�[�Ŏg�p���郊�\�[�X�Ŏg�p����r���[�B
	 */
	class CShaderResourceView : Noncopyable{
	public:
		CShaderResourceView();
		~CShaderResourceView();
		/*!
		 *@brief	StructuredBuffer�p��SRV���쐬�B
		 *@param[in]	structuredBuffer	structuredBuffer
		 */
		bool Create(CStructuredBuffer& structuredBuffer);
		/*!
		*@brief	�e�N�X�`���p��SRV���쐬�B
		*@param[in]	texture		�e�N�X�`���B
		*/
		bool Create(ID3D11Texture2D* texture);
		/*!
		 *@brief	�����I�ȊJ���B
		 *@details
		 * �����I�ɊJ������K�v������Ƃ��Ɏg�p���Ă��������B
		 */
		void Release();
		/*!
		*@brief	ID3D11ShaderResourceView���擾�B
		*/
		ID3D11ShaderResourceView*& GetBody()
		{
			return m_srv;
		}
	private:
		ID3D11ShaderResourceView*		m_srv = nullptr;
	};
}
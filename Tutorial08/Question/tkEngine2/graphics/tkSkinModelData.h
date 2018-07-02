/*!
 *@brief	�X�L�����f���f�[�^�B
 */

#pragma once

namespace tkEngine2{

	class CModelEffect : public DirectX::IEffect {
	public:
		CShader m_vsShader;
		CShader m_psShader;
		ID3D11ShaderResourceView* diffuseTex = nullptr;
		ID3D11ShaderResourceView* tooneTex = nullptr;
		bool isSkining;
	public:
		void __cdecl Apply(ID3D11DeviceContext* deviceContext) override
		{
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShader.GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_psShader.GetBody(), NULL, 0);
			deviceContext->PSSetShaderResources(0, 1, &diffuseTex);
			deviceContext->PSSetShaderResources(1, 1, &tooneTex);

		}

		void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
		{
			*pShaderByteCode = m_vsShader.GetByteCode();
			*pByteCodeLength = m_vsShader.GetByteCodeSize();
		}
		void SetDiffuseTexture(ID3D11ShaderResourceView* tex)
		{
			diffuseTex = tex;
		}
		void SetToonTex(ID3D11ShaderResourceView* tex)
		{
			tooneTex = tex;
		}
	};
	/*!
	*@brief
	*  �X�L���Ȃ����f���G�t�F�N�g�B
	*/
	class CNonSkinModelEffect : public CModelEffect {
	public:
		CNonSkinModelEffect()
		{
			m_vsShader.Load("Assets/shader/model.fx", "VSMain", CShader::EnType::VS);
			m_psShader.Load("Assets/shader/model.fx", "PSMain", CShader::EnType::PS);
			isSkining = false;
		}
	};
	/*!
	*@brief
	*  �X�L�����f���G�t�F�N�g�B
	*/
	class CSkinModelEffect : public CModelEffect {
	public:
		CSkinModelEffect()
		{
			m_vsShader.Load("Assets/shader/model.fx", "VSMainSkin", CShader::EnType::VS);
			m_psShader.Load("Assets/shader/model.fx", "PSMain", CShader::EnType::PS);
			isSkining = true;
		}
	};

	
	/*!
	 *@brief	�X�L�����f���f�[�^�B
	 */
	class CSkinModelData : Noncopyable{
	public:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CSkinModelData();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CSkinModelData();
		/*!
		 *@brief	���[�h�B
		 *@param[in]	filePath	�t�@�C���p�X�B
		 *@return	true���Ԃ��Ă����烍�[�h�����B
		 */
		bool Load( const wchar_t* filePath );
		DirectX::Model& GetBody()
		{
			return *m_modelDx;
		}
	private:
		std::unique_ptr<DirectX::Model> m_modelDx;
	};
}
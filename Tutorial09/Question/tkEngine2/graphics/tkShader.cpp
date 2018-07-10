/*!
 * @brief	�V�F�[�_�[�B
 */
#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/graphics/tkShader.h"
#include "tkEngine2/tkEngine.h"

namespace tkEngine2{
	namespace {
		void ReadFile(const char* filePath, char* readBuff, int& fileSize)
		{
			FILE* fp = fopen(filePath, "rb");
			fseek(fp, 0, SEEK_END);
			fpos_t fPos;
			fgetpos(fp, &fPos);
			fseek(fp, 0, SEEK_SET);
			fileSize = (int)fPos;
			fread(readBuff, fileSize, 1, fp);
			fclose(fp);
		}
		/*!
		 *@brief	���_�V�F�[�_�[���璸�_���C�A�E�g�𐶐��B
		 */
		HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
		{
			// �V�F�[�_�[��񂩂烊�t���N�V�������s���B
			ID3D11ShaderReflection* pVertexShaderReflection = NULL;
			if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
			{
				return S_FALSE;
			}

			// �V�F�[�_�[�����擾�B
			D3D11_SHADER_DESC shaderDesc;
			pVertexShaderReflection->GetDesc(&shaderDesc);

			// ���͏���`��ǂݍ���
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
			for (unsigned int i = 0; i< shaderDesc.InputParameters; i++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
				pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

				// �G�������g��`��ݒ�B
				D3D11_INPUT_ELEMENT_DESC elementDesc;
				elementDesc.SemanticName = paramDesc.SemanticName;
				elementDesc.SemanticIndex = paramDesc.SemanticIndex;
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elementDesc.InstanceDataStepRate = 0;

				// determine DXGI format
				if (paramDesc.Mask == 1)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				}
				else if (paramDesc.Mask <= 3)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				}
				else if (paramDesc.Mask <= 7)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				}
				else if (paramDesc.Mask <= 15)
				{
					if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
					else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				}

				//�G�������g�̒�`��ۑ��B
				inputLayoutDesc.push_back(elementDesc);
			}

			// ���̓��C�A�E�g���쐬�B
			HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

			//���t���N�V�����p�Ɋm�ۂ���������������B
			pVertexShaderReflection->Release();
			return hr;
		}
	}
	CShader::CShader()
	{
	}
	CShader::~CShader()
	{
		Release();
	}
	
	void CShader::Release()
	{
		if(m_shader != nullptr){
			switch (m_shaderType) {
			case EnType::VS:
				((ID3D11VertexShader*)m_shader)->Release();
				break;
			case EnType::PS:
				((ID3D11PixelShader*)m_shader)->Release();
				break;
			case EnType::CS:
				((ID3D11ComputeShader*)m_shader)->Release();
				break;
			}
			m_shader = nullptr;
		}

		if (m_inputLayout != nullptr) {
			m_inputLayout->Release();
			m_inputLayout = nullptr;
		}
	}
	
	bool CShader::Load(const char* filePath, const char* entryFuncName, EnType shaderType)
	{
		Release();
		HRESULT hr = S_OK;

	    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
	    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	    // Setting this flag improves the shader debugging experience, but still allows 
	    // the shaders to be optimized and to run exactly the way they will run in 
	    // the release configuration of this program.
	    dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif
		
		TScopedResource<ID3DBlob> errorBlob;
		//�V�F�[�_�[�����[�h�B
		static char text[5 * 1024 * 1024];
		int fileSize = 0;
		ReadFile(filePath, text, fileSize);
		static const char* shaderModelNames[] = {
			"vs_5_0",
			"ps_5_0",
			"cs_5_0"
		};
		SetCurrentDirectory("Assets/shader");
		hr = D3DCompile(text, fileSize, nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryFuncName,
			shaderModelNames[(int)shaderType], dwShaderFlags, 0, &blobOut.res, &errorBlob.res);
		SetCurrentDirectory("../../");
	    if( FAILED(hr) )
	    {
			if (errorBlob.res != nullptr) {
				static char errorMessage[10 * 1024];
				sprintf(errorMessage, "filePath : %s, %s", filePath, (char*)errorBlob.res->GetBufferPointer());
				MessageBox(NULL, errorMessage, "�V�F�[�_�[�R���p�C���G���[", MB_OK);
			}
	        return false;
	    }
		ID3D11Device* pD3DDevice = Engine().GetD3DDevice();
		switch(shaderType){
		case EnType::VS:{
			//���_�V�F�[�_�[�B
			hr = pD3DDevice->CreateVertexShader(blobOut.res->GetBufferPointer(), blobOut.res->GetBufferSize(), nullptr, (ID3D11VertexShader**)&m_shader );
			if (FAILED(hr)) {
				return false;
			}
			//���̓��C�A�E�g���쐬�B
			hr = CreateInputLayoutDescFromVertexShaderSignature(blobOut.res, pD3DDevice, &m_inputLayout);
			if (FAILED(hr)) {
				//���̓��C�A�E�g�̍쐬�Ɏ��s�����B
				return false;
			}
		}break;
		case EnType::PS:{
			//�s�N�Z���V�F�[�_�[�B
			hr = pD3DDevice->CreatePixelShader(blobOut.res->GetBufferPointer(), blobOut.res->GetBufferSize(), nullptr, (ID3D11PixelShader**)&m_shader);
			if (FAILED(hr)) {
				return false;
			}
		}break;
		case EnType::CS:{
			//�R���s���[�g�V�F�[�_�[�B
			hr = pD3DDevice->CreateComputeShader(blobOut.res->GetBufferPointer(), blobOut.res->GetBufferSize(), nullptr, (ID3D11ComputeShader**)&m_shader);
			if (FAILED(hr)) {
				return false;
			}
		}break;
		}
	    return true;
	}
}
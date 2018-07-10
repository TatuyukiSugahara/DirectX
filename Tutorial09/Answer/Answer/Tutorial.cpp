/*!
 *@brief	�g�D�[���T���v���B
 */
#include "stdafx.h"
#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/tkEngine.h"
#include "tkEngine2/graphics/tkCamera.h"
#include "tkEngine2/timer/tkStopwatch.h"
#include <time.h>
using namespace tkEngine2;


class ToonRender : public IGameObject {
	CSkinModelData skinModelData;
	CSkinModel uniModel;
	CCamera camera;
	CRenderTarget depthTexture;	//�[�x�e�N�X�`���B
	CRenderTarget bokeTexture;	//�{�P�e�N�X�`���B
	CRenderTarget blurBokeTexture;	//�u���[���������{�P�e�N�X�`���B
	CVertexBuffer vertexBuffer;	//���_�o�b�t�@
	CShader vsDofShader;		//��ʊE�[�x�̒��_�V�F�[�_�[�B
	CShader psDofShader;		//�{�P�摜���o�p�̔�ʊE�[�x�̃s�N�Z���V�F�[�_�[�B
	CShader psDofShader_1;
	CShader psDofShader_2;		//�{�P�摜�����B
	//���_�B
	struct SSimpleVertex {
		CVector4 pos;
		CVector2 tex;
	};
public:
	bool Start() override
	{
		skinModelData.Load(L"Assets/modelData/unityChan.cmo");
		uniModel.Init(skinModelData);
		//�J�������������B
		camera.SetPosition({ 0.0f, 1.0f, 2.0f });
		camera.SetTarget({ 0.0f, 1.0f, 0.0f });
		camera.SetUp({ 0.0f, 1.0f, 0.0f });
		camera.SetNear(1.0f);
		camera.Update();
		
		//�[�x�e�N�X�`���o�͗p�̃����_�����O�^�[�Q�b�g���쐬����B
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		depthTexture.Create(
			Engine().GetFrameBufferWidth(),
			Engine().GetFrameBufferHeight(),
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			multiSampleDesc
		);

		//�{�P�e�N�X�`���o�͗p�̃����_�����O�^�[�Q�b�g���쐬����B
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		bokeTexture.Create(
			Engine().GetFrameBufferWidth(),
			Engine().GetFrameBufferHeight(),
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			multiSampleDesc
		);
		//�u���[���������{�P�e�N�X�`���̏o�͐�̃����_�����O�^�[�Q�b�g���쐬����B
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		//�c��1/2�̉𑜓x�ɂ���B
		blurBokeTexture.Create(
			Engine().GetFrameBufferWidth() >> 1,
			Engine().GetFrameBufferHeight() >> 1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			multiSampleDesc
		);
		//���_�o�b�t�@���������B
		InitVertexBuffer();
		//�V�F�[�_�[���������B
		InitShaders();
		return true;
	}
	/*!------------------------------------------------------------------
	* @brief	���_�o�b�t�@���������B
	------------------------------------------------------------------*/
	void InitVertexBuffer()
	{
		//���_�o�b�t�@�̃\�[�X�f�[�^�B
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		//���_�o�b�t�@���쐬�B
		vertexBuffer.Create(4, sizeof(SSimpleVertex), vertices);
	}
	/*!------------------------------------------------------------------
	* @brief	�V�F�[�_�[���������B
	------------------------------------------------------------------*/
	void InitShaders()
	{
		//���_�V�F�[�_�[�����[�h�B
		vsDofShader.Load("Assets/shader/Dof.fx", "VSMain", CShader::EnType::VS);
		//�{�P��摜�𒊏o����s�N�Z���V�F�[�_�[�����[�h����B
		psDofShader.Load("Assets/shader/Dof.fx", "PSDofStep_0", CShader::EnType::PS);
		psDofShader_1.Load("Assets/shader/Dof.fx", "PSDofStep_1", CShader::EnType::PS);
		psDofShader_2.Load("Assets/shader/Dof.fx", "PSDofStep_2", CShader::EnType::PS);
	}
	/*!------------------------------------------------------------------
	 * @brief	�X�V�B
	 ------------------------------------------------------------------*/
	void Update() override
	{
		static float angle = 0.0f;
		CQuaternion rot;
		CVector3 pos = camera.GetPosition();
		if (GetAsyncKeyState(VK_UP)) {
			pos.z -= 0.002f;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			pos.z += 0.002f;
		}
		camera.SetPosition(pos);
		camera.Update();
	}
	/*!------------------------------------------------------------------
	* @brief	ZPrepass�B
	* *@details
	*   �[�x�l���������ނ��߂̃p�X�B
	------------------------------------------------------------------*/
	void ZPrepass(CRenderContext& rc)
	{
		rc.OMSetRenderTargets(1, &depthTexture);
		float ClearColor[4] = { 10000.0f, 10000.0f, 10000.0f, 0.0f }; //red,green,blue,alpha
		rc.ClearRenderTargetView(0, ClearColor);
		uniModel.Draw(rc, camera.GetViewMatrix(), camera.GetProjectionMatrix(), true);
		rc.OMSetRenderTargets(1, NULL);
	}

	/*!------------------------------------------------------------------
	* @brief	�V�[���̕`��B
	------------------------------------------------------------------*/
	void RenderScene(CRenderContext& rc)
	{
		//���C�������_�����O�^�[�Q�b�g�ɖ߂��B
		rc.OMSetRenderTargets(1, &Engine().GetMainRenderTarget());
		uniModel.Draw(rc, camera.GetViewMatrix(), camera.GetProjectionMatrix());
		rc.OMSetRenderTargets(1, NULL);
	}
	/*!------------------------------------------------------------------
	* @brief	DOF�`��B
	------------------------------------------------------------------*/
	void RenderDOF(CRenderContext& rc)
	{
		//�{�P��s�N�Z����I�o�B
		//DOF�X�e�b�v0
		{
			//���_�o�b�t�@��ݒ�B
			rc.IASetVertexBuffer(vertexBuffer);
			//�v���~�e�B�u�̃g�|���W�[��ݒ�B
			rc.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			//���_�V�F�[�_�[��ݒ�B
			rc.VSSetShader(vsDofShader);
			//�s�N�Z���V�F�[�_�[��ݒ�B
			rc.PSSetShader(psDofShader);
			//���̓��C�A�E�g��ݒ�B
			rc.IASetInputLayout(vsDofShader.GetInputLayout());
			//t0�ɃV�[���̃e�N�X�`����ݒ肷��B
			rc.PSSetShaderResource(0, Engine().GetMainRenderTarget().GetRenderTargetSRV());
			//t1�ɐ[�x�e�N�X�`����ݒ肷��B
			rc.PSSetShaderResource(1, depthTexture.GetRenderTargetSRV());
			//�{�P�e�N�X�`�����o�͐�Ƃ��Đݒ肷��B
			rc.OMSetRenderTargets(1, &bokeTexture);
			//�N���A����B
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; //red,green,blue,alpha
			rc.ClearRenderTargetView(0, ClearColor);
			//�`��B
			rc.Draw(4, 0);
			//�����_�����O�^�[�Q�b�g���O���B
			rc.OMSetRenderTargets(1, NULL);
		}
		//�u���[�������ă{�P�摜�𐶐��B
		//DofStep_1
		{
			//�{�P�e�N�X�`�����o�͐�Ƃ��Đݒ肷��B
			rc.OMSetRenderTargets(1, &blurBokeTexture);
			//���o�����e�N�X�`����t2�ɐݒ肷��B
			rc.PSSetShaderResource(2, bokeTexture.GetRenderTargetSRV());
			rc.PSSetShader(psDofShader_1);
			//�N���A����B
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; //red,green,blue,alpha
			rc.ClearRenderTargetView(0, ClearColor);
			rc.RSSetViewport(0.0f, 0.0f, 640.0f, 360.0f);
			//�`��B
			rc.Draw(4, 0);
			//�����_�����O�^�[�Q�b�g���O���B
			rc.OMSetRenderTargets(1, NULL);
		}
		rc.RSSetViewport(0.0f, 0.0f, 1280.0f, 720.0f);
		//�{�P�摜�ƃ��C���V�[���������B
		//DofStep_2
		{
			Engine().ToggleMainRenderTarget();
			rc.OMSetRenderTargets(1, &Engine().GetMainRenderTarget());
			//t3�Ƀu���[�������e�N�X�`����ݒ肷��B�B
			rc.PSSetShaderResource(3, blurBokeTexture.GetRenderTargetSRV());
			rc.PSSetShader(psDofShader_2);
			rc.Draw(4, 0);
			//�����_�����O�^�[�Q�b�g���O���B
			rc.OMSetRenderTargets(1, NULL);
		}
		//SRV����O���K�v������B
		rc.PSUnsetShaderResource(0);
		rc.PSUnsetShaderResource(1);
		
	}
	/*!------------------------------------------------------------------
	* @brief	�`��B
	------------------------------------------------------------------*/
	void Render(CRenderContext& rc) override
	{
		//ZPrepass
		ZPrepass(rc);
		//�V�[���̕`��B
		RenderScene(rc);
		//DOF
		RenderDOF(rc);
	}
};

/*!
 *@brief	���C���֐��B
 */
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
	srand((unsigned)time(NULL));
    //tkEngine2�̏������p�����[�^��ݒ肷��B
	SInitParam initParam;
	initParam.nCmdShow = nCmdShow;
	initParam.hInstance = hInstance;
	initParam.screenWidth = 1280;
	initParam.screenHeight = 720;
	initParam.frameBufferWidth = 1280;
	initParam.frameBufferHeight = 720;
	//�G���W�����������B
	if (Engine().Init(initParam) == true) {
		//�������ɐ����B
		NewGO<ToonRender>(0);
		//�Q�[�����[�v�����s�B
		Engine().RunGameLoop();
	}
	//�G���W���̏I�������B
	Engine().Final();
	
    return 0;
}


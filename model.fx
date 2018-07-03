/*!
 * @brief	���f���V�F�[�_�[�B
 */

#include "modelCB.h"
#include "modelStruct.h"
#include "modelSRV.h"

#define TILE_WIDTH	16		//�^�C���̕��B
#define USE_LIM_LIGHT		//�L���ɂ���ƃ������C�g�p�̃T���v���B

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos;
	pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	psInput.posInView = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = mul(mWorld, In.Normal);
	psInput.Pos = pos;
    return psInput;
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos;
	pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	psInput.posInView = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = mul(mWorld, In.Normal);
	psInput.Pos = pos;
    return psInput;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	if(isZPrepass){
		//ZPrepass
		return In.posInView.z;
	}
	//�X�N���[���̍����(0,0)�A�E����(1,1)�Ƃ�����W�n�ɕϊ�����B
	float2 screenPos = (In.Pos.xy / In.Pos.w) * float2(0.5f, -0.5f) + 0.5f;
	float2 offset = float2( 1.25f/screenParam.z, 1.25f / screenParam.w);
	
	float depth_0 = depthTexture.Sample(Sampler, screenPos).x;


	float2 offsetTbl[] = {
		float2( -offset.x, -offset.y),		//����
		float2( 0.0f, 	   -offset.y),		//��
		float2( offset.x, -offset.y),		//�E��
		float2( offset.x,  0.0f),			//�E
		float2( -offset.x, 0.0f),			//��
		float2( -offset.x, +offset.y),		//����
		float2( 0.0f, 	   +offset.y),		//��
		float2( offset.x,  +offset.y),		//�E��
	};

	for(int i = 0; i < 8; i++){
		float2 screenPos2 = screenPos + offsetTbl[i];

		float depth_1 = depthTexture.Sample(Sampler, screenPos2).x;
		if(depth_1 - depth_0 > 0.025f ){
			//�G�b�W�̐F��Ԃ��B
			return 0.0f;
		}
	}

	//���C�g�̕����͂Ƃ肠�����Œ�ɂ��Ƃ�<-�ق�Ƃ͒萔�o�b�t�@�Ƃ��ő���ׂ��B
#ifdef USE_LIM_LIGHT
	//�������C�g�p�̃T���v���ł̓��C�g�̓��j�e�B�����̐^��납�瓖����B
	float3 lightDir = float3(0.0f, 0.0f, 1.0f);
#else
	float3 lightDir = float3(1.0f, 0.0f, 0.0f);
#endif
	//2�K���̉A�e�����߂�B
	//���C�g�̕����Ɩ@���̓��ς����ƃ��C�g�̋��������܂�B
	//���������l�ȉ�(�����0.2)�Ȃ�Â��e�N�X�`�����g���B
	//����ȏ�Ȃ疾�邢�e�N�X�`�����g���B
	float4 color = 0.0f;
	if(dot(-lightDir, In.Normal) < 0.2f){
		color = float4(darkTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}else{
		color = float4(lightTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}

#ifdef USE_LIM_LIGHT
	//�����𓖂Ă�B
	float3 eyeDir = float3(0.0f, 0.0f, -1.0f);			//�����͌Œ�ɂ��Ă���(�J�������񂵂���A�E�g�B�ق�Ƃ͒萔�o�b�t�@�Ƃ��ő���ׂ��B
	float lim = (1.0f - dot(-In.Normal, eyeDir))* 2.0f;	//�J�����̕����Ɛ����ȏꍇ�t���̋����������Ȃ�B
	lim *= dot(eyeDir, -lightDir);						//���S�ɋt�s���Ă�����baseLim�̒l�����̂܂܂ł�B
	if(lim > 0.7f){
		//�t�����������Ă���B
		color = float4(lightTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}
#endif
	
    return color; 
}


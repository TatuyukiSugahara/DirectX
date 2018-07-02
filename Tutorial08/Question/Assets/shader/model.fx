/*!
 * @brief	���f���V�F�[�_�[�B
 */

#include "modelCB.h"
#include "modelStruct.h"

Texture2D<float4> lightTexture : register(t0);	//�����������Ă���ӏ��̃e�N�X�`��
Texture2D<float4> drakTexture : register(t1);	//�����������Ă���ӏ��̃e�N�X�`��

Texture2D<float4> depthTexture : register(t2);	//�[�x�e�N�X�`���B


sampler Sampler : register(s0);

cbuffer SScrean : register( b0 )
{
	float2 screanParam;
};

#define TILE_WIDTH	16		//�^�C���̕��B


/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos;
	pos = mul(mWorld, In.Position);
	psInput.wPos = pos.xyz;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = In.Normal;
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
	psInput.wPos = pos.xyz;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = In.Normal;
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
		return In.Pos.z / In.Pos.w;
	}
	
	//�X�N���[���̍����(0,0)�A�E����(1,1)�Ƃ�����W�n�ɕϊ�����B
	float2 screenPos = (In.Pos.xy / In.Pos.w) * float2(0.5f, -0.5f) + 0.5f;
	//1�e�N�Z���ƂȂ���B(2.0����2�e�N�Z��)
	float2 offset = float2(2.0 / 1280, 2.0f / 720);
	//���݂̏ꏊ�̐[�x�l�B
	float depth0 = depthTexture.Sample(Sampler, screenPos).x;
	
	float2 screenpos2;
	screenpos2 = screenPos;
	//�E�ƂȂ�
	screenpos2.x = screenPos.x + offset.x;
	//�E�ƂȂ�̐[�x�l���擾�B
	float depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	screenpos2 = screenPos;
	//���ƂȂ�B
	screenpos2.x = screenPos.x - offset.x;
	//���ƂȂ�̐[�x�l���擾�B
	depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	screenpos2 = screenPos;
	//��ƂȂ�B
	screenpos2.y = screenPos.y + offset.y;
	//���ƂȂ�̐[�x�l���擾�B
	depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	screenpos2 = screenPos;
	//��ƂȂ�B
	screenpos2.y = screenPos.y - offset.y;
	//���ƂȂ�̐[�x�l���擾�B
	depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	float4 color;
	float4 lightDir = float4(0.0f,0.0f,1.0f,1.0f);
	if( max(0.0f, -dot(lightDir, In.Normal)) < 0.2f )
	{
		color = float4(drakTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}
	else
	{
		color = float4(lightTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}
	//�����𓖂Ă�B
	float3 eyeDir = float3(0.0f, 0.0f, -1.0f);			//�����͌Œ�ɂ��Ă���(�J�������񂵂���A�E�g�B�ق�Ƃ͒萔�o�b�t�@�Ƃ��ő���ׂ��B
	float lim = (1.0f - dot(-In.Normal, eyeDir))* 2.0f;	//�J�����̕����Ɛ����ȏꍇ�t���̋����������Ȃ�B
	lim *= max(0.0f, dot(eyeDir, -lightDir));						//���S�ɋt�s���Ă�����baseLim�̒l�����̂܂܂ł�B
	if(lim > 0.7f){
		//�t�����������Ă���B
		color = float4(lightTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}
	
    return color; 
}


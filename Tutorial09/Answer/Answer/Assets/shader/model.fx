/*!
 * @brief	���f���V�F�[�_�[�B
 */

#include "modelCB.h"
#include "modelStruct.h"
#include "modelSRV.h"




/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos;
	pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	psInput.wPos = pos;
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
	pos = mul(mView, pos);
	psInput.wPos = pos;
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
		return In.wPos.z;
	}

	float4 color = float4(lightTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
    return color; 
}


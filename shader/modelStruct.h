/*!
 * @brief	���f���֌W�̍\���̃w�b�_�B
 */
 
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float4 Tangent  : TANGENT;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord	: TEXCOORD0;
    uint4  Indices  : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float2 TexCoord : TEXCOORD0;
	float4 Pos			: TEXCOORD1;
	float3 posInView	: TEXCOORD2;		//�J������Ԃł̍��W�B
};
/*!
 * @brief	�s�N�Z���V�F�[�_�[�̏o�́B
 */
struct PSOutput{
	float4 diffuse : SV_Target0;
	float4 normal : SV_Target1;
	float depth : SV_Target2;
};

/*!
 * @brief	ZPrepass�p�̃s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInputZPrepass{
	float4 Position : SV_POSITION;
};

//�|�C���g���C�g�B
struct SPointLight {
	float3	position;		//�ʒu�B
	float3	positionInView;	//�J�������W�n�ł̃|�C���g���C�g�̈ʒu�B
	float4	color;			//�J���[�B
	float4  attn;			//�����萔�B(x������������ƃ��C�g�̖��邳�������Ay��z���傫���Ȃ�ƃ��C�g�������܂œ͂��悤�ɂȂ�B)
};

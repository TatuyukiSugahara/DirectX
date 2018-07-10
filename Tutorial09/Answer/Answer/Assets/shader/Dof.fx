/*!
 * @brief	��ʊE�[�x�B
 */


struct VSIn{
	float4 pos : SV_Position;
	float2 tex : TEXCOORD0;
};
struct PSIn{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

//���C���V�[���̃e�N�X�`��
Texture2D<float4> mainSceneTexture : register(t0);
//�[�x�e�N�X�`���B
Texture2D<float4> depthTexture : register(t1);

sampler Sampler : register(s0);

//--------------------------------------------------------------------------------------
// ���_�V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
PSIn VSMain(VSIn In) 
{
	PSIn psIn;
	psIn.pos = In.pos;
	psIn.tex = In.tex;
	return psIn;
}
//--------------------------------------------------------------------------------------
// �{�P�摜���o�X�e�b�v�B
//--------------------------------------------------------------------------------------
float4 PSDofStep_0( PSIn In ) : SV_Target
{
	//�V�[���J���[���擾�B
	float4 sceneColor = mainSceneTexture.Sample(Sampler, In.tex); 
	//�[�x�l���擾�B
	float depth = depthTexture.Sample(Sampler, In.tex).x;
	if(depth > 3.0f){
		//�[�x�l��3.0�ȏ�Ȃ�{�P������B
		return sceneColor;
	}
    return float4(0.0f, 0.0f, 0.0f, 0.0f); 
}

//�{�P�e�N�X�`���B
Texture2D<float4> bokeTexture : register(t2);
//--------------------------------------------------------------------------------------
// �u���[�X�e�b�v�B�B
//--------------------------------------------------------------------------------------
float4 PSDofStep_1(PSIn In) : SV_Target
{
	//�ߖT�e�N�Z���Ɖ��d�a���s���B
	float2 offset = float2(2.0f / 1280.0f, 2.0f / 720.0f);
	float2 tex = In.tex;
	tex.x += offset.x;
	//�{�P�e�N�X�`�����擾�B
	float4 color = bokeTexture.Sample(Sampler, tex);
	
	tex = In.tex;
	tex.x -= offset.x;
	color += bokeTexture.Sample(Sampler, tex);
	
	tex = In.tex;
	tex.y += offset.y;
	color += bokeTexture.Sample(Sampler, tex);
	
	tex = In.tex;
	tex.y -= offset.y;
	color += bokeTexture.Sample(Sampler, tex);
	
	color /= 4.0f;
	return color;
}
//�u���[�摜
Texture2D<float4> blurTexture : register(t3);
float4 PSDofStep_2(PSIn In) : SV_Target
{
	float4 sceneColor = mainSceneTexture.Sample(Sampler, In.tex);
	float4 blurColor = blurTexture.Sample(Sampler, In.tex);
	float4 color;
	color.xyz = sceneColor.xyz * (1.0f - blurColor.w) + blurColor.xyz * blurColor.w;
	color.w = 1.0f;
	return color;
}
/*!
 * @brief	���f���p�̒萔�o�b�t�@�B
 */
/*!
 * @brief	���_�V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 screenParam;	// �X�N���[���p�����[�^(near, far, screenWidth, screenHeight)
	int isZPrepass;		//Z�v���p�X�H
};

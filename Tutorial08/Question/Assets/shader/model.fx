/*!
 * @brief	モデルシェーダー。
 */

#include "modelCB.h"
#include "modelStruct.h"

Texture2D<float4> lightTexture : register(t0);	//光が当たっている箇所のテクスチャ
Texture2D<float4> drakTexture : register(t1);	//光が当たっている箇所のテクスチャ

Texture2D<float4> depthTexture : register(t2);	//深度テクスチャ。


sampler Sampler : register(s0);

cbuffer SScrean : register( b0 )
{
	float2 screanParam;
};

#define TILE_WIDTH	16		//タイルの幅。


/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
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
 * @brief	スキンありモデル用の頂点シェーダー。
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
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	if(isZPrepass){
		//ZPrepass
		return In.Pos.z / In.Pos.w;
	}
	
	//スクリーンの左上を(0,0)、右下を(1,1)とする座標系に変換する。
	float2 screenPos = (In.Pos.xy / In.Pos.w) * float2(0.5f, -0.5f) + 0.5f;
	//1テクセルとなりを。(2.0だと2テクセル)
	float2 offset = float2(2.0 / 1280, 2.0f / 720);
	//現在の場所の深度値。
	float depth0 = depthTexture.Sample(Sampler, screenPos).x;
	
	float2 screenpos2;
	screenpos2 = screenPos;
	//右となり
	screenpos2.x = screenPos.x + offset.x;
	//右となりの深度値を取得。
	float depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	screenpos2 = screenPos;
	//左となり。
	screenpos2.x = screenPos.x - offset.x;
	//左となりの深度値を取得。
	depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	screenpos2 = screenPos;
	//上となり。
	screenpos2.y = screenPos.y + offset.y;
	//左となりの深度値を取得。
	depth1 = depthTexture.Sample(Sampler, screenpos2).x;
	if(abs(depth0 - depth1) > 0.2f)
	{
		return 0.0f;
	}
	
	screenpos2 = screenPos;
	//上となり。
	screenpos2.y = screenPos.y - offset.y;
	//左となりの深度値を取得。
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
	//リムを当てる。
	float3 eyeDir = float3(0.0f, 0.0f, -1.0f);			//視線は固定にしておく(カメラを回したらアウト。ほんとは定数バッファとかで送るべき。
	float lim = (1.0f - dot(-In.Normal, eyeDir))* 2.0f;	//カメラの方向と垂直な場合逆光の強さが強くなる。
	lim *= max(0.0f, dot(eyeDir, -lightDir));						//完全に逆行していたらbaseLimの値がそのままでる。
	if(lim > 0.7f){
		//逆光が発生している。
		color = float4(lightTexture.Sample(Sampler, In.TexCoord).xyz, 1.0f);
	}
	
    return color; 
}


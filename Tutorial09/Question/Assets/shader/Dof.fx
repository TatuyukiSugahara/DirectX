/*!
 * @brief	被写界深度。
 */


struct VSIn{
	float4 pos : SV_Position;
	float2 tex : TEXCOORD0;
};
struct PSIn{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

//メインシーンのテクスチャ
Texture2D<float4> mainSceneTexture : register(t0);
//深度テクスチャ。
Texture2D<float4> depthTexture : register(t1);
//ボケテクスチャ
Texture2D<float4> bokeTexture : register(t2);
//ブラーテクスチャ
Texture2D<float4> blurTexture : register(t3);

sampler Sampler : register(s0);

//--------------------------------------------------------------------------------------
// 頂点シェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
PSIn VSMain(VSIn In) 
{
	PSIn psIn;
	psIn.pos = In.pos;
	psIn.tex = In.tex;
	return psIn;
}
//--------------------------------------------------------------------------------------
// ボケ画像抽出ステップ。
//--------------------------------------------------------------------------------------
float4 PSDofStep_0( PSIn In ) : SV_Target
{
	//シーンカラーを取得。
	float4 sceneColor = mainSceneTexture.Sample(Sampler, In.tex); 
	//深度値を取得。
	float depth = depthTexture.Sample(Sampler, In.tex).x;
	if(depth > 3.0f){
		//深度値が3.0以上ならボケさせる。
		return sceneColor;
	}
    return float4(0.0f, 0.0f, 0.0f, 0.0f); 
}

//--------------------------------------------------------------------------------------
// ブラー画像抽出ステップ。
//--------------------------------------------------------------------------------------
float4 PSDofStep_1( PSIn In ) : SV_Target
{
	
	//1テクセルとなりを。(2.0だと2テクセル)
	float2 offset = float2(2.0f / 1280.0f, 2.0f / 720.0f);
	
	//ボケテクスチャを取得。
	//右
	float2 tex = In.tex;
	tex.x += offset.x;
	float4 color = bokeTexture.Sample(Sampler, tex);
	//左
	tex = In.tex;
	tex.x -= offset.x;
	color = bokeTexture.Sample(Sampler, tex);
	//上
	tex = In.tex;
	tex.y += offset.y;
	color = bokeTexture.Sample(Sampler, tex);
	//下
	tex = In.tex;
	tex.y -= offset.y;
	color = bokeTexture.Sample(Sampler, tex);
	
	color /= 4;
	
	return color;
	
}


//--------------------------------------------------------------------------------------
// シーン合成
//--------------------------------------------------------------------------------------
float4 PSDofStep_2( PSIn In ) : SV_Target
{
	float4 maincolor = mainSceneTexture.Sample(Sampler, In.tex);
	float4 blurcolor = blurTexture.Sample(Sampler,In.tex);
	
	float4 color;
	color.xyz = maincolor.xyz * (1.0f - blurcolor.w) + blurcolor.xyz * blurcolor.w;
	color.w = 1.0f;
	
	return color;
}

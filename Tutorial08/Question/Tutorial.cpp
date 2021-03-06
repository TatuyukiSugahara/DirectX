/*!
 *@brief	トゥーンサンプル。
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
	CRenderTarget depthBuffer;		//深度を出力するバッファ。
	//頂点。
	struct SSimpleVertex {
		CVector4 pos;
		CVector2 tex;
	};

	//画面の解像度。
	struct Screan
	{
		CVector2 screanSize;
	};
	Screan screan;
	CConstantBuffer screanCB;
public:
	bool Start() override
	{
		//深度値出力用のレンダリングターゲットを作成する。
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		depthBuffer.Create(
			Engine().GetFrameBufferWidth(),
			Engine().GetFrameBufferHeight(),
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			multiSampleDesc
		);

		//スクリーンサイズセット
		screan.screanSize.x = Engine().GetFrameBufferWidth();
		screan.screanSize.y = Engine().GetFrameBufferHeight();

		screanCB.Create(&screan, sizeof(screan));

		skinModelData.Load(L"Assets/modelData/unityChan.cmo");
		uniModel.Init(skinModelData);
		//カメラを初期化。
		camera.SetPosition({ 0.0f, 1.0f, 2.0f });
		camera.SetTarget({ 0.0f, 1.0f, 0.0f });
		camera.SetUp({ 0.0f, 1.0f, 0.0f });
		camera.SetNear(1.0f);
		camera.Update();
		
		return true;
	}
	/*!------------------------------------------------------------------
	 * @brief	更新。
	 ------------------------------------------------------------------*/
	void Update() override
	{
		static float angle = 0.0f;
		CQuaternion rot;
		rot.SetRotation(CVector3::AxisY, angle);
		//angle += 0.01f;
		uniModel.Update({0.0f, 0.0f, 0.0f}, rot, CVector3::One);
	}

	/*!------------------------------------------------------------------
	* @brief	ZPrepass。
	* *@details
	*   深度値を書き込むためのパス。
	------------------------------------------------------------------*/
	void ZPrepass(CRenderContext& rc)
	{
		rc.OMSetRenderTargets(1, &depthBuffer);
		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha
		rc.ClearRenderTargetView(0, ClearColor);
		uniModel.Draw(rc, camera.GetViewMatrix(), camera.GetProjectionMatrix(), true);
		rc.OMSetRenderTargets(1, NULL);
	}

	/*!------------------------------------------------------------------
	* @brief	シーンの描画。
	------------------------------------------------------------------*/
	void RenderScene(CRenderContext& rc)
	{
		/*rc.UpdateSubresource(screanCB, screan);
		rc.PSSetConstantBuffer(0, screanCB);*/

		//メインレンダリングターゲットに戻す。
		rc.OMSetRenderTargets(1, &Engine().GetMainRenderTarget());
		//深度テクスチャを設定。
		rc.PSSetShaderResource(2, depthBuffer.GetRenderTargetSRV());
		//モデル描画。
		uniModel.Draw(rc, camera.GetViewMatrix(), camera.GetProjectionMatrix());
	}
	/*!------------------------------------------------------------------
	* @brief	描画。
	------------------------------------------------------------------*/
	void Render(CRenderContext& rc) override
	{
		//深度値書き込みのためのZPrepass
		ZPrepass(rc);
		
		//シーンの描画。
		RenderScene(rc);
	}
};

/*!
 *@brief	メイン関数。
 */
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
	srand((unsigned)time(NULL));
    //tkEngine2の初期化パラメータを設定する。
	SInitParam initParam;
	initParam.nCmdShow = nCmdShow;
	initParam.hInstance = hInstance;
	initParam.screenWidth = 1280;
	initParam.screenHeight = 720;
	initParam.frameBufferWidth = 1280;
	initParam.frameBufferHeight = 720;
	//エンジンを初期化。
	if (Engine().Init(initParam) == true) {
		//初期化に成功。
		NewGO<ToonRender>(0);
		//ゲームループを実行。
		Engine().RunGameLoop();
	}
	//エンジンの終了処理。
	Engine().Final();
	
    return 0;
}


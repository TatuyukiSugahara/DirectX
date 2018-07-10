/*!
 *@brief	�G���W���B
 */
#pragma once

#include "tkEngine2/graphics/tkRenderTarget.h"
#include "tkEngine2/timer/tkStopwatch.h"
namespace tkEngine2{
	/*!
	 *@brief	�������p�̃p�����[�^�B
	 */
	struct SInitParam{
		SInitParam(){
			memset(this, 0, sizeof(SInitParam));
			gameObjectPrioMax = 32;
		}
		HINSTANCE hInstance;		//!<�A�v���P�[�V�����C���X�^���X�B
		int	screenWidth;			//!<�X�N���[���̕��B
		int	screenHeight;			//!<�X�N���[���̍����B
		int frameBufferHeight;		//!<�t���[���o�b�t�@�̕��B���ꂪ�����𑜓x�B
		int frameBufferWidth;		//!<�t���[���o�b�t�@�̍����B���ꂪ�����𑜓x�B
		int nCmdShow;				//!<
		unsigned char gameObjectPrioMax;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l(32�܂�)
	};
	/*!
	 *@brief	�G���W���B
	 */
	class CEngine : Noncopyable {
	private:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CEngine();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CEngine();
	public:
		/*!
		 *@brief	�������B
		 *@param[in]	initParam		�������p�����[�^�B
		 */
		bool Init( const SInitParam& initParam );
		/*!
		 *@brief	�I�������B
		 */
		void Final();
		/*!
		 *@brief	�Q�[�����[�v�����s�B
		 */
		void RunGameLoop();
		/*!
		 *@brief	�C���X�^���X���擾�B
		 */
		static CEngine& GetInstance()
		{
			static CEngine* instance = nullptr;
			if(instance == nullptr){
				instance = new CEngine;
			}
			return *instance;
		}
		/*!
		*@brief	Direct3DDevice�̎擾�B
		*/
		ID3D11Device* GetD3DDevice() const
		{
			return m_pd3dDevice;
		}
		/*!
		*@brief	ID3D11DeviceContext�̎擾�B
		*/
		ID3D11DeviceContext* GetD3DDeviceContext() const
		{
			return m_pImmediateContext;
		}
		/*!
		*@brief		�t���[���o�b�t�@�̕����擾�B
		*/
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/*!
		*@brief		�t���[���o�b�t�@�̍������擾�B
		*/
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/*!
		* @brief	���C�������_�����O�^�[�Q�b�g��؂�ւ��B
		*/
		void ToggleMainRenderTarget()
		{
			m_currentMainRenderTarget ^= 1;
		}
		CRenderTarget& GetMainRenderTarget()
		{
			return m_mainRenderTarget[m_currentMainRenderTarget];
		}
	private:
		/*!
		 *@brief	�E�B���h�E�������B
		 */
		bool InitWindow( const SInitParam& initParam );
		/*!
		 *@brief	DirectX���������B
		 */
		bool InitDirectX( const SInitParam& initParam );
		/*!
		* @brief	�E�B���h�E�v���V�[�W���B
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		/*!
		* @brief	�X�V�B
		*/
		void Update();
		/*!
		* @brief	���C�������_�����O�^�[�Q�b�g�̓��e���o�b�N�o�b�t�@�ɃR�s�[�B
		*/
		void CopyMainRenderTargetToBackBuffer();
	private:
		HINSTANCE				m_hInst = nullptr;	//!<�A�v���P�[�V�����̃C���X�^���X�B
		HWND					m_hWnd = nullptr;	//!<�E�B���h�E�n���h���B
		D3D_DRIVER_TYPE			m_driverType = D3D_DRIVER_TYPE_NULL; //!<�h���C�o�̎�ނ̃I�v�V�����B
		D3D_FEATURE_LEVEL		m_featureLevel = D3D_FEATURE_LEVEL_11_0;	//!<Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
		ID3D11Device*			m_pd3dDevice = nullptr;						//!<D3D11�f�o�C�X�B
		CRenderContext			m_renderContext;							//!<�����_�����O�R���e�L�X�g�B
		ID3D11DeviceContext*	m_pImmediateContext = nullptr;				//!<D3D11�����f�o�C�X�R���e�L�X�g�B
		IDXGISwapChain*			m_pSwapChain = nullptr;						//!<SwapChain�B
		int						m_currentMainRenderTarget = 0;				//!<���݂̃��C�������_�����O�^�[�Q�b�g�B
		CRenderTarget			m_mainRenderTarget[2];						//!<���C�������_�����O�^�[�Q�b�g�B
		
		int						m_screenWidth = 0;							//!<�X�N���[���̕��B
		int						m_screenHeight = 0;							//!<�X�N���[���̍����B
		int						m_frameBufferWidth = 0;						//!<�t���[���o�b�t�@�̕��B���ꂪ�����𑜓x�B
		int						m_frameBufferHeight = 0;					//!<�t���[���o�b�t�@�̍����B���ꂪ�����𑜓x�B
	public:
		CStopwatch				m_sw;
	};

	/*!
	* @brief	Engine�̃C���X�^���X���擾�B�B
	*/
	static inline CEngine& Engine()
	{
		return CEngine::GetInstance();
	}
}
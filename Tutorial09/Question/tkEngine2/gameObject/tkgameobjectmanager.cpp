/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "tkEngine2/tkEnginePreCompile.h"
#include "tkEngine2/tkEngine.h"
#include "tkEngine2/gameObject/tkgameobjectmanager.h"
#include <future>

namespace tkEngine2{
	void CGameObjectManager::Execute(CRenderContext& renderContext)
	{
		ExecuteDeleteGameObjects();

		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
		//�v�������_�����O�B
		//@todo �������B preRender.Update();
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
		//�V�[���O���t���X�V�B
		UpdateSceneGraph();

		//��ʂ��N���A
		float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
		renderContext.OMSetRenderTargets(1, &Engine().GetMainRenderTarget());
		renderContext.ClearRenderTargetView(0, ClearColor);
		renderContext.RSSetViewport(0.0f, 0.0f, (float)Engine().GetFrameBufferWidth(), (float)Engine().GetFrameBufferHeight());
		
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreRenderWrapper(renderContext);
			}
		}

		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->RenderWrapper(renderContext);
			}
		}

		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostRenderWrapper(renderContext);
			}
		}
		
#if 0
		{
			//�������烌���_�����O���B��������ŁB
			//�����_�����O�X�e�[�g�̏������B
			renderContext[0].Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				D3DCOLOR_RGBA(0, 0, 255, 0), 1.0f, 0
				);
			tkEngine::SViewport vp = {
				0,
				0,
				(unsigned int)tkEngine::CEngine::Instance().GetFrameBufferWidth(),
				(unsigned int)tkEngine::CEngine::Instance().GetFrameBufferHeight(),
				0.0f,
				1.0f
			};
			renderContext[0].SetViewport(vp);
			renderContext[0].SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			renderContext[0].SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			renderContext[0].SetRenderState(D3DRS_SRCBLEND, BLEND_ONE);
			renderContext[0].SetRenderState(D3DRS_DESTBLEND, BLEND_ZERO);
			renderContext[0].SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			renderContext[0].SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			renderContext[0].SetRenderState(D3DRS_ZENABLE, TRUE);
		}
		//�v�������_�����O�B
		preRender.Render(renderContext[0]);

		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreRenderWrapper(renderContext[0]);
			}
		}
		
		if (numRenderContext == 1) {
			CPIXPerfTag tag(renderContext[0], L"Render MainRenderTarget");
			//�V���O���X���b�h�`��B
			//�[�x�������ݗp�̃����_�����O�^�[�Q�b�g��ݒ�B
			renderContext[0].SetRenderTarget(1, Dof().GetDepthRenderTarget());
			//���x�������ݗp�̃����_�����O�^�[�Q�b�g��ݒ�B
			renderContext[0].SetRenderTarget(2, MotionBlur().GetVelocityMapRenderTarget());
			for (GameObjectList objList : m_gameObjectListArray) {
				for (IGameObject* obj : objList) {
					obj->RenderWrapper(renderContext[0]);
				}
			}
		}
		else {
			//�}���`�X���b�h�`��B
			TK_ASSERT(0, "not implement!!");
			
		}
		//�[�x�������ݗp�̃����_�����O�^�[�Q�b�g���O���B
		renderContext[numRenderContext - 1].SetRenderTarget(1, NULL);
		postEffect.Render(renderContext[numRenderContext-1]);
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostRenderWrapper(renderContext[numRenderContext-1]);
			}
		}
#endif
	}
	void CGameObjectManager::UpdateSceneGraph()
	{
		//���[���h�s����X�V�B
		for (auto transform : m_childrenOfRootTransformList) {
			transform->UpdateWorldMatrixAll();
		}
	}
	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//�o�b�t�@��؂�ւ��B
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for(GameObjectList& goList : m_deleteObjectArray[preBufferNo]){
			for(IGameObject* go : goList){
				GameObjectPrio prio = go->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find( goExecList.begin(),goExecList.end(),go );
				if (it != goExecList.end()) {
					//�폜���X�g���珜�O���ꂽ�B
					(*it)->m_isRegistDeadList = false;
					if ((*it)->IsNewFromGameObjectManager()) {
						delete (*it);
					}
				}
				goExecList.erase(it);
			}
			goList.clear();
		}
	}
	void CGameObjectManager::Init( int gameObjectPrioMax )
	{
		TK_ASSERT( gameObjectPrioMax <= GAME_OBJECT_PRIO_MAX, "�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔���傫�����܂��B");
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}
}
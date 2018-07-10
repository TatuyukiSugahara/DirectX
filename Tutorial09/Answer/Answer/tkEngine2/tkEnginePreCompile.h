#pragma once

#pragma warning (disable  : 4201)

#define BUILD_LEVEL_DEBUG		0		//�f�o�b�O�r���h
#define BUILD_LEVEL_PREVIEW		1		//�v���r���[�r���h
#define BUILD_LEVEL_MASTER		2		//�}�X�^�[�r���h�B

/*!
* @brief	�r���h���x���B
* @details
*  BUILD_LEVEL��BUILD_LEVEL_DEBUG�ABUILD_LEVEL_PREVIEW�ABUILD_LEVEL_MASTER</br>
*	�̂R�̒l���Ƃ�܂��B���̂R�̒l�͉��L�̈Ӗ��������܂��B
*		BUILD_LEVEL_DEBUG		���S�ȃf�o�b�O�r���h�B
*		BUILD_LEVEL_PREVIEW		�R�[�h�͍œK������Ă��邪�A�f�o�b�O�@�\�͎c���Ă���B
*		BUILD_LEVEL_MASTER		�}�X�^�[�r���h�B�f�o�b�O�@�\�͊��S�ɍ폜����Ă���B
* �f�o�b�O�R�[�h�𖄂ߍ��ގ��́ABUILD_LEVEL_MASTER�ł͖����ɂȂ�悤�Ɏ������Ă��������B
*/
#ifdef _DEBUG
#define BUILD_LEVEL		BUILD_LEVEL_DEBUG
#elif defined(TK_MASTER)
#define BUILD_LEVEL		BUILD_LEVEL_MASTER
#else
#define BUILD_LEVEL		BUILD_LEVEL_PREVIEW
#endif

#include <windows.h>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <map>
#include <wincodec.h>
#include <wincodecsdk.h>


#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "tkEngine2/policy/tkNoncopyable.h"
#include "tkEngine2/policy/tkScopedResource.h"
#include "tkEngine2/debug/tkLog.h"
#include "tkEngine2/debug/tkAssert.h"
#include "tkEngine2/math/tkMath.h"
#include "tkEngine2/math/tkVector.h"
#include "tkEngine2/math/tkMatrix.h"
#include "graphics/tkRenderContext.h"

#include "gameObject/tkGameObjectManager.h"

#include "tkEngine2\graphics\tkShader.h"
#include "tkEngine2/graphics/GPUBuffer/tkVertexBuffer.h"
#include "tkEngine2/graphics/GPUBuffer/tkStructuredBuffer.h"
#include "tkEngine2/graphics/GPUView/tkShaderResourceView.h"
#include "tkEngine2/graphics/GPUView/tkUnorderedAccessView.h"
#include "tkEngine2/graphics/tkMapper.h"
#include "tkEngine2/graphics/GPUBuffer/tkConstantBuffer.h"
#include "tkEngine2/graphics/tkSamplerState.h"
#include "tkEngine2/graphics/tkSkinModelData.h"
#include "tkEngine2/graphics/tkSkinModel.h"

/*!
 * @brief	�萔�o�b�t�@
 */

#pragma once

#include "tkEngine2/graphics/GPUBuffer/tkGPUBuffer.h"

namespace tkEngine2{
	/*!
	 * @brief	�萔�o�b�t�@�B
	 */
	class CConstantBuffer : public CGPUBuffer{
	public:
		CConstantBuffer();
		~CConstantBuffer();
		/*!
		*@brief ConstantBuffer���쐬�B
		*@param[in]	pInitData		�����f�[�^�B
		*@param[in]	bufferSize		�o�b�t�@�T�C�Y�B
		*/
		bool Create(const void* pInitData, int bufferSize );		
	};
}
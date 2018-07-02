/*!
 * @brief	�X�R�[�v�𔲂����玩���I�ɊJ������郊�\�[�X�B
 */

#pragma once

namespace tkEngine2{
	template<class T>
	class TScopedResource{
	private:
		
	public:
		T* res = nullptr;
		~TScopedResource()
		{
			if (res) {
				res->Release();
			}
		}
		
	};
}
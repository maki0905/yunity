#include <Windows.h>
#include "WinApp.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	MSG msg{};
	// �E�B���h�E��x�{�^�����������܂Ń��[�v
	while (msg.message != WM_QUIT) {
		// Window�Ƀ��b�Z�[�W�����Ă���ŗD��ŏ���������
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// �Q�[������
		}
	}
	OutputDebugStringA("Hello,DirectX!\n");
	return 0;
}
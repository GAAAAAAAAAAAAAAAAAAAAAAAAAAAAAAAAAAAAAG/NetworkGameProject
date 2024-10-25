#include "stdafx.h"

CGameloop::CGameloop()
{
}

CGameloop::~CGameloop()
{

}

void CGameloop::Init(HWND hWnd)
{
	_hwnd = hWnd;
	_timeManager.Init();
	_inputManager.Init(_hwnd);
	_SceneManager.Init();
}

void CGameloop::Update()
{
	_timeManager.Update();
	_inputManager.Update();
	_SceneManager.Update();
}

void CGameloop::Render()
{
    HDC mdc;
    HBITMAP hBitmap;
    RECT rt;

    GetClientRect(_hwnd, &rt);
    _hdc = GetDC(_hwnd);
    mdc = CreateCompatibleDC(_hdc); // �޸� DC ����
    hBitmap = CreateCompatibleBitmap(_hdc, rt.right, rt.bottom); // ��Ʈ�� ����
    SelectObject(mdc, hBitmap); // ��Ʈ���� �޸� DC�� ����

    // ����� ��� �簢������ ä��
    Rectangle(mdc, 0, 0, rt.right, rt.bottom);

    // �� �Ŵ��� ������
    _SceneManager.Render(mdc);

    // �� ������ ������ ȭ�鿡 ����
    BitBlt(_hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);

    // GDI ��ü ���󺹱� �� �ڿ� ����
    DeleteObject(hBitmap); // ��Ʈ�� ����
    DeleteDC(mdc); // �޸� DC ����
    ReleaseDC(_hwnd, _hdc);
}
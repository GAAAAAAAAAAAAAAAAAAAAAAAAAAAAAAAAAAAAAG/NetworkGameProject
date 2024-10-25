#pragma once

enum class KeyType
{
	LeftMouse = VK_LBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D'
};

enum class KeyState
{
	None,
	Press,
	Down,
	Up,

	End
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32_t>(UINT8_MAX) + 1,
	KEY_STATE_COUNT = static_cast<int32_t>(KeyState::End)
};

class InputManager
{
public:
	static InputManager& GetInstance()
	{
		static InputManager instance; // C++11 ������ thread-safe ���� ���� �ʱ�ȭ
		return instance;
	}
	// �̱��� �����̱� ������ ���� �����ڿ� ���� �����ڸ� �����Ͽ� �ν��Ͻ��� ������� �ʵ��� ��
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager() = default;
	~InputManager() = default;

	void Init(HWND hwnd);
	void Update();
	bool IsKeyDown(int key) { return _states[key] == KeyState::Down || _states[key] == KeyState::Press; }
	bool GetButton(KeyType key) { return GetState(key) == KeyState::Press; }
	bool GetButtonDown(KeyType key) { return GetState(key) == KeyState::Down; }
	bool GetButtonUp(KeyType key) { return GetState(key) == KeyState::Up; }
	POINT GetMousePos() { return _mousePos; }
private:
	KeyState GetState(KeyType key) { return _states[static_cast<uint8_t>(key)]; }
private:
	HWND _hwnd;
	vector<KeyState> _states;
	POINT _mousePos;
}; 

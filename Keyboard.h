#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};

		Event() : _type{ Type::Invalid }, _code{ 0u }
		{}

		Event(Type type, unsigned char code) noexcept
			: _type{ type }, _code{ code }
		{}

		bool IsPress() const noexcept
		{
			return _type == Type::Press;
		}

		bool IsValid() const noexcept
		{
			return _type == Type::Invalid;
		}

		bool IsRelease() const noexcept
		{
			return _type == Type::Release;
		}

		unsigned char GetCode() const noexcept
		{
			return _code;
		}

	private:
		Type _type;
		unsigned char _code;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;

	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnable() const noexcept;

private:

	void KeyOnPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;

	template<class _Ty>
	static void TrimBuffer(std::queue<_Ty>& buff) noexcept;

	static const unsigned int _keys = 256u;
	static const unsigned int _bufferSize = 16u;

	bool _autorepeatEnabled = false;
	std::bitset<_keys> _keystates;

	std::queue<Event> _keybuffer;
	std::queue<char> _charbuffer;
};

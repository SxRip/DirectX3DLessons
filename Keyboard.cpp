#include "Keyboard.h"
	
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return _keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (_keybuffer.size() > 0u)
	{
		Keyboard::Event e = _keybuffer.front();
		_keybuffer.pop();
		return e;
	}
	else
		return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return _keybuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	_keybuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (_charbuffer.size() > 0u)
	{
		unsigned char charcode = _charbuffer.front();
		_charbuffer.pop();
		return charcode;
	}

	return 0;
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return _charbuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	_charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	_autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	_autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnable() const noexcept
{
	return _autorepeatEnabled;
}

void Keyboard::KeyOnPressed(unsigned char keycode) noexcept
{
	_keystates[keycode] = true;
	_keybuffer.emplace(Keyboard::Event::Type::Press, keycode);
	TrimBuffer(_keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	_keystates[keycode] = false;
	_keybuffer.emplace(Keyboard::Event::Type::Release, keycode);
	TrimBuffer(_keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	_charbuffer.push(character);
	TrimBuffer(_charbuffer);
}

void Keyboard::ClearState() noexcept
{
	_keystates.reset();
}


template<class _Ty>
inline void Keyboard::TrimBuffer(std::queue<_Ty>& buff) noexcept
{
	while (buff.size() > _bufferSize)
		buff.pop();
}
#include <Windows.h>
#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x, y };
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return _leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return _rightIsPressed;
}

bool Mouse::IsInWindow() const noexcept
{
	return _isInWindow;
}

Mouse::Event Mouse::Read() noexcept
{
	if (_buffer.size() > 0u)
	{
		Mouse::Event e = _buffer.front();
		_buffer.pop();
		return e;
	}

	return Mouse::Event();
}

void Mouse::Flush() noexcept
{
	_buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int nx, int ny) noexcept
{
	x = nx;
	y = ny;

	_buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{		
	_isInWindow = false;
	_buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	_isInWindow = true;
	_buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (_buffer.size() > _bufferSize)
		_buffer.pop();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	_wheelDeltaCarry += delta;

	while (_wheelDeltaCarry >= WHEEL_DELTA)
	{
		_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}

	while (_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

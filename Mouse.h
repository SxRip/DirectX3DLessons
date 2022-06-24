#pragma once
#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:

		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid,

			Enter,
			Leave
		};

	private:
		Type _type;
		bool _leftIsPressed, _rightIsPressed;
		int x, y;

	public:
		Event() noexcept : _type(Type::Invalid), _leftIsPressed{},
			_rightIsPressed{}, x{}, y{}
		{}

		Event(Type type, const Mouse& parent) noexcept : _type(type),
			_leftIsPressed(parent._leftIsPressed), _rightIsPressed(parent._rightIsPressed),
			x(parent.x), y(parent.y)
		{}

		bool IsValid() const noexcept
		{
			return _type == Type::Invalid;
		}

		Type GetType() const noexcept
		{
			return _type;
		}

		std::pair<int, int> GetPos() const noexcept
		{
			return {x, y};
		}

		int GetPosX() const noexcept
		{
			return x;
		}

		int GetPosY() const noexcept
		{
			return y;
		}

		bool LeftIsPressed() const noexcept
		{
			return _leftIsPressed;
		}

		bool RightIsPressed() const noexcept
		{
			return _rightIsPressed;
		}

	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse operator=(const Mouse&) = delete;

	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWindow() const noexcept;


	Mouse::Event Read() noexcept;

	bool IsEmpty() const noexcept { return _buffer.empty(); }

	void Flush() noexcept;

private:

	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	std::queue<Event> _buffer;

	const unsigned int _bufferSize = 16u;
	bool _leftIsPressed, _rightIsPressed, _isInWindow;
	int x, y, _wheelDeltaCarry = 0;
};


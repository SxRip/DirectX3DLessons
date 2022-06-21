#include <sstream>
#include "CustomException.h"

CustomException::CustomException(const int line, const char* file) noexcept
	: _line{line}, _file{file}
{

}

const char* CustomException::what() const noexcept
{
	std::stringstream ss;
	ss << GetType() << std::endl
		<< _GetOriginalStr();
	whatBuff = ss.str();

	return whatBuff.c_str();
}

const char* CustomException::GetType() const noexcept
{
	return "Custom exception";
}

int CustomException::GetLine() const noexcept
{
	return _line;
}

const std::string& CustomException::GetFile() const noexcept
{
	return _file;
}

std::string CustomException::_GetOriginalStr() const noexcept
{
	std::stringstream ss;

	ss << "[LINE] " << _line << std::endl;
	ss << "[FILE] " << _file;

	return ss.str().c_str();
}

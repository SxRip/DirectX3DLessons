#pragma once
#include <exception>
#include <string>

class CustomException : public std::exception
{
public:
	CustomException(const int line, const char* file) noexcept;

	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;

	int GetLine() const noexcept;

	const std::string& GetFile() const noexcept;
	std::string _GetOriginalStr() const noexcept;

private:
	int _line;
	std::string _file;

protected:
	mutable std::string whatBuff;
};


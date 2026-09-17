#pragma once

class ErrorManager {
public:
	static void SetError(const char* errMsg);
	static const char* GetError();
	static void ErrorPopup();

private:
	static const char* error;
};

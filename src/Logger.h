#pragma once
#include <vector>
#include <string>

class Log {
public:
	Log() {}
	~Log() {}

	//Add a message to the log
	void LOG(std::string message);

	//Clean the log
	void Clear();

	//Draw the console window where the log is displayed
	void Log::DrawConsole();

	//Get the messages from the log
	const std::vector<std::string>& GetMessages() const {return msgLog;	}
private:
	//all the messages from the log
	inline static std::vector<std::string> msgLog;
};
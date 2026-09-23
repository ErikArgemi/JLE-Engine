#pragma once
#include <vector>
#include <string>
class Log {
public:
	Log() {}
	~Log() {}

	//Add a message to the log
	void LOG(std::string message) {
		msgLog.push_back(message);
		msgLog.push_back("\n");
	}

	//Clean the log
	void Clear() {
		msgLog.clear();
	}

	//Draw the console window where the log is displayed
	//void DrawConsole();

	//Get the messages from the log
	std::vector<std::string> GetMessages() { return msgLog; }
private:
	static std::vector<std::string> msgLog;
};
#pragma once

#include "App.h"

#define LOG(msg) App::GetInstance().log.LogMessage(msg)

#define LOG_INFO(msg) App::GetInstance().log.LogMessage("[INFO] " + std::string(msg))
#define LOG_ERROR(msg) App::GetInstance().log.LogMessage("[ERROR] " + std::string(msg))
#define LOG_DATA(msg) App::GetInstance().log.LogMessage("[DATA] " + std::string(msg))

//the text to log a message was simply a hassle, this macros is to make it easier to log and identify a log aswell
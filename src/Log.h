#pragma once

#include "Engine.h"

#define LOG(msg) Engine::GetInstance().log.LOG(msg)

#define LOG_INFO(msg) Engine::GetInstance().log.LOG("[INFO] " + std::string(msg))
#define LOG_ERROR(msg) Engine::GetInstance().log.LOG("[ERROR] " + std::string(msg))
#define LOG_DATA(msg) Engine::GetInstance().log.LOG("[DATA] " + std::string(msg))

//the text to log a message was simply a hassle, this macros is to make it easier to log and identify a log aswell
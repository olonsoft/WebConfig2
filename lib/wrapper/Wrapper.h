#pragma once

#include <arduino.h>
#include <WiFiConnection2.h>
#include <WebConfig2.h>
#include <TaskSched.h>

#define MAX_CONNECTION_ATTEMPTS 3

void initialize();
void wrapperLoop();

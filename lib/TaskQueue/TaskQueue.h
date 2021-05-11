#pragma once

#include <Arduino.h>

// ===========================================
// callbacks vector
// ===========================================

using void_task_queue_f = void (*)();

void executeTaskQueue();
void addToTaskQueue(void_task_queue_f f);

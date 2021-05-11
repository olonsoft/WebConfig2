#pragma once

#include <Arduino.h>

#define _TASK_WDT_IDS // To enable task unique IDs

#include <TaskSchedulerDeclarations.h>
#include <vector>

extern std::vector <Task*> toDelete;

extern Scheduler ts;

void GarbageCollector();
void onDisableDeleteTask();
extern Task tGarbageCollection;

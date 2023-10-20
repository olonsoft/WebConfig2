#include <Arduino.h>
#include "project_config.h"
#include "defaults.h"
#include <helper_debug.h>
#include <TaskSched.h>
#include <Wrapper.h>

Scheduler ts; // scheduler should be in main file

Task tWrapper(100 * TASK_MILLISECOND, TASK_FOREVER, &wrapperLoop, &ts, true);

void setup() {
  Serial.begin(115200);
  delay(500);
  LOGLN;
  LOGLN;
  TLOGINFO("\e[5;44mDemo project.\e[m\n");
  Serial.setDebugOutput(true);
  initialize();
}

void loop() {
  ts.execute();
}
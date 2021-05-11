#include <TaskSched.h>
#include <TaskScheduler.h>


std::vector <Task*> toDelete;

//Scheduler ts; // moved to main.cpp

void GarbageCollector() {
  if ( toDelete.empty() ) {
    tGarbageCollection.disable();
    return;
  }
  Task *t = toDelete.front();
  Serial.print(millis()); 
  Serial.print(F("\tTask N")); 
  Serial.print(t->getId()); 
  Serial.println(F(" destroyed"));
  delete t;
  toDelete.erase(toDelete.begin());
}

void onDisableDeleteTask(){
  Task *t = &ts.currentTask();
  unsigned int tid = t->getId();
  toDelete.push_back(t);
  tGarbageCollection.enableIfNot();

  Serial.print(millis()); 
  Serial.print(F("\tTask N"));
  Serial.print(tid); 
  Serial.println(F(" finished"));
}

Task tGarbageCollection(200 * TASK_MILLISECOND, TASK_FOREVER, &GarbageCollector, &ts, false);

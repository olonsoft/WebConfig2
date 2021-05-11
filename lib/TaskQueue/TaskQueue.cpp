#include <Arduino.h>
#include <taskQueue.h>
#include <vector>

std::vector<void_task_queue_f> _taskQueue; // was std::vector<void (*)()> _taskQueue;

/*
 * Executes the function that is on the queue 
 * and deletes it from the queue
 */
void executeTaskQueue() {
    if (!_taskQueue.empty()) {
    Serial.println("Executing from queue.");
    _taskQueue.front()();                  // execute the task
    _taskQueue.erase(_taskQueue.begin());  // delete the task from the queue
  }
}

/*
 * adds a function to be executed, to the queue list.
 * @param  {void(*)()} f : the function to execute
 */
void addToTaskQueue(void_task_queue_f f){ // was void addToTaskQueue(void (*f)()){
  Serial.println("Adding to queue.");
  _taskQueue.push_back(f);
}
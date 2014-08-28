#include <experiment.hpp>

#include <thread>

void Experiment::run() {
  if(_isRunning) {

  }

  _thread = std::thread(&Experiment::runThread, this);
}

bool Experiment::isRunning() const {
  return _isRunning;
}

void Experiment::waitUntilFinished() {
  if(_thread.joinable()) {
    _thread.join();
  }
}

void Experiment::setProgress(const double& progress) {
  if (progress < 0 || progress > 1) {

  }

  if (progress < _progress) {

  }

  _progress = progress;
}

double Experiment::getProgress() const {
  return _progress;
}

void Experiment::runThread() {
  _isRunning = true;
  _progress = 0;
  runImpelementation();
  _isRunning = false;
  _progress = 1;
}

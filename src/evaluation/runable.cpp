#include <hop_bits/evaluation/runable.hpp>

namespace hop {
  void Runable::run() {
    if(_isRunning) {
      //TODO Add exception
    }

    _thread = std::thread(&Runable::runInThread, this);
  }

  bool Runable::isRunning() const {
    return _isRunning;
  }

  void Runable::waitUntilFinished() {
    if(_thread.joinable()) {
      _thread.join();
    }
  }

  void Runable::setProgress(const double& progress) {
    if (progress < 0 || progress > 1) {
      //TODO Add exception
    }

    if (progress < _progress) {
      //TODO Add exception
    }

    _progress = progress;
  }

  double Runable::getProgress() const {
    return _progress;
  }

  void Runable::runInThread() {
    _isRunning = true;
    _progress = 0;
    runImpelementation();
    _isRunning = false;
    _progress = 1;
  }
}

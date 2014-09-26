#include <hop_bits/helper/runable.hpp>

namespace hop {
  void Runable::run() {
    if(isRunning_) {
      //TODO Add exception
    }

    thread_ = std::thread(&Runable::runInThread, this);
  }

  bool Runable::isRunning() const {
    return isRunning_;
  }

  void Runable::waitUntilFinished() {
    if(thread_.joinable()) {
      thread_.join();
    }
  }

  void Runable::setProgress(const double& progress) {
    if (progress < 0 || progress > 1) {
      //TODO Add exception
    }

    if (progress < progress_) {
      //TODO Add exception
    }

    progress_ = progress;
  }

  double Runable::getProgress() const {
    return progress_;
  }

  void Runable::runInThread() {
    isRunning_ = true;
    progress_ = 0;
    runImpelementation();
    isRunning_ = false;
    progress_ = 1;
  }
}

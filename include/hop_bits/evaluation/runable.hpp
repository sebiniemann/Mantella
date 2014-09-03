#pragma once

#include <thread>

namespace hop {
  class Runable {
    public:
      void run();
      bool isRunning() const;
      double getProgress() const;
      void waitUntilFinished();

    protected:
      double _progress;
      bool _isRunning;
      std::thread _thread;

      virtual void runImpelementation() = 0;
      void runInThread();
      void setProgress(const double& progress);
  };
}

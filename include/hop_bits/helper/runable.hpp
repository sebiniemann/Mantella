#pragma once

#include <thread>

namespace hop {
  class Runable {
    public:
      Runable() = default;

      Runable(const Runable&) = delete;
      Runable& operator=(const Runable&) = delete;

      void run();
      bool isRunning() const;
      double getProgress() const;
      void waitUntilFinished();

    protected:
      double progress_;
      bool isRunning_;
      std::thread thread_;

      virtual void runImpelementation() = 0;
      void runInThread();
      void setProgress(const double& progress);
  };
}

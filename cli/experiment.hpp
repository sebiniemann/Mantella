#pragma once

#include <string>
#include <thread>

class Experiment {
  public:
    virtual void run() final;
    virtual bool isRunning() const final;
    virtual double getProgress() const final;
    virtual void waitUntilFinished() final;

  protected:
    double _progress;
    bool _isRunning;
    std::thread _thread;

    virtual void runImpelementation() = 0;
    virtual void runThread() final;
    virtual void setProgress(const double& progress) final;
};

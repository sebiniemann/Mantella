#pragma once

#include <string>
#include <thread>

class Experiment {
  public:
    virtual void run() final;
    virtual bool isRunning() const final;
    virtual double getProgress() const final;
    virtual void waitUntilFinished() final;

    virtual void setExperimentPath(const std::string& experimentPath) final;

  protected:
    double _progress;
    std::string _experimentPath = "./";
    bool _isRunning;
    std::thread _thread;

    virtual void runImpelementation() = 0;
    virtual void runThread() final;
    virtual void setProgress(const double& progress) final;
};

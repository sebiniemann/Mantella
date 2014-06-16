package org.onlineoptimisation;

public enum DefaultMonitorType implements MonitorType {
  CurrentNumberOfFunctionEvaluations,
  CurrentNumberOfIterations,
  CurrentBestParameter,
  CurrentBestObjectiveValue,
  FinalNumberOfFunctionEvaluations,
  FinalNumberOfIterations,
  FinalBestParameter,
  FinalBestObjectiveValue,
  IsFinished,
  IsTerminated
}

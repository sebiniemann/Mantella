package org.onlineoptimisation;

import java.util.ArrayList;
import java.util.List;

public class Statistic {

  protected double       _count;
  protected List<Object> _min;
  protected List<Object> _max;
  protected List<Object> _mean;
  protected List<Object> _variance;

  public Statistic() {
    _max = new ArrayList<>();
    _min = new ArrayList<>();
    _variance = new ArrayList<>();
    _mean = new ArrayList<>();
  }

  public void update(List<Object> values) {
    if (_count > 0) {
      if (_count >= 9007199254740992.0) { // 2^53
        throw new UnsupportedOperationException("No more than 2^53 (approx. 9 * 10^15) samples can be processed without loss of precision.");
      }

      for (int n = 0; n < values.size(); n++) {
        double value = (double) values.get(n);
        
        double min = (double) _min.get(n);
        double max = (double) _max.get(n);
        double mean = (double) _mean.get(n);
        double variance = (double) _variance.get(n);
        
        _min.set(n, Math.min(value, min));
        _max.set(n, Math.max(value, max));
        _variance.set(n, (_count - 1) / _count * variance + (Math.pow(value - mean, 2)) / (_count + 1));
        _mean.set(n, mean + (value - mean) / (_count + 1));
      }
    } else {
      for (Object value : values) {
        _max.add((double) value);
        _min.add((double) value);
        _variance.add(0.0);
        _mean.add((double) value);

      }
    }

    _count++;
  }

  public double getCount() {
    return _count;
  }

  public List<Object> getMin() {
    return _min;
  }

  public List<Object> getMax() {
    return _max;
  }

  public List<Object> getMean() {
    return _mean;
  }

  public List<Object> getStandardDeviation() {
    List<Object> standardDeviation = new ArrayList<>();

    for (int n = 0; n < _variance.size(); n++) {
      standardDeviation.add(Math.sqrt((double) _variance.get(n)));
    }

    return standardDeviation;
  }

}

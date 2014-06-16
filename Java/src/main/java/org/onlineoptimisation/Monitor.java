package org.onlineoptimisation;

import java.util.AbstractMap.SimpleImmutableEntry;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

public class Monitor {

  protected static Set<MonitorType>                                _observedMonitorTypes = new HashSet<>();
  protected static List<SimpleImmutableEntry<MonitorType, Object>> _observedValues       = new ArrayList<>();

  public static void observe(MonitorType... monitorTypes) {
    for (MonitorType monitorType : monitorTypes) {
      if (!_observedMonitorTypes.contains(monitorType)) {
        _observedMonitorTypes.add(monitorType);
      }
    }
  }

  public static void add(MonitorType monitorType, Object value) {
    if (_observedMonitorTypes.contains(monitorType)) {
      _observedValues.add(new SimpleImmutableEntry<>(monitorType, value));
    }
  }

  public static List<Object> get(MonitorType monitorType) {
    List<Object> returnValues = new ArrayList<>();

    for (SimpleImmutableEntry<MonitorType, Object> observedValue : _observedValues) {
      if (observedValue.getKey().equals(monitorType)) {
        returnValues.add(observedValue.getValue());
      }
    }

    return returnValues;
  }

  public static List<List<Object>> get(MonitorType... monitorTypes) {
    List<List<Object>> returnValues = new ArrayList<>();

    List<Object> entry = new ArrayList<>();
    for (MonitorType monitorType : monitorTypes) {
      entry.add(monitorType);
    }

    int lastPosition = -1;
    for (SimpleImmutableEntry<MonitorType, Object> observedValue : _observedValues) {
      for (int n = 0; n < monitorTypes.length; n++) {
        if (observedValue.getKey().equals(monitorTypes[n])) {
          if (n <= lastPosition && lastPosition < monitorTypes.length - 1) {
            throw new RuntimeException();
          }
          
          entry.set(n, observedValue.getValue());
          
          if(n == monitorTypes.length - 1) {
            returnValues.add(new ArrayList<>(entry));
          }
          
          lastPosition = n;
        }
      }
    }

    return returnValues;
  }

  public static void clear() {
    _observedValues.clear();
  }

  public static void clear(MonitorType... monitorTypes) {
    for (Iterator<SimpleImmutableEntry<MonitorType, Object>> iterator = _observedValues.iterator(); iterator.hasNext();) {
      SimpleImmutableEntry<MonitorType, Object> observedValue = iterator.next();

      for (MonitorType monitorType : monitorTypes) {
        if (observedValue.getKey().equals(monitorType)) {
          iterator.remove();
          break;
        }
      }
    }
  }

  public static void remove() {
    clear();
    _observedMonitorTypes.clear();
  }

  public static void remove(MonitorType... monitorTypes) {
    clear(monitorTypes);
    for (MonitorType monitorType : monitorTypes) {
      _observedMonitorTypes.remove(monitorType);
    }
  }

}
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.onlineoptimisation.extension.robotvisualisation.util;

import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Wolff
 */
public class Util {

  public static int[] convertIntegers(List<Integer> integers) {
    int[] ret = new int[integers.size()];
    Iterator<Integer> iterator = integers.iterator();
    for (int i = 0; i < ret.length; i++) {
      ret[i] = iterator.next().intValue();
    }
    return ret;
  }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.onlineoptimisation.extension.robotvisualisation.mathconstructs;

/**
 *
 * @author Wolff
 */
public class Intersect {
  
  public float _x, _y;
  public Circle _first, _second;

  public Intersect(float x, float y, Circle first, Circle second) {
    this._x = x;
    this._y = y;
    this._first = first;
    this._second = second;
  }

  @Override
  public String toString() {
    return "x: " + _x + ", y: " + _y;// + ", first: " + first + ", second: " + second;
  }
}

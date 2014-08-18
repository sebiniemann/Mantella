package org.onlineoptimisation.extension.robotvisualisation.mathconstructs;

/**
 *
 * @author Wolff
 */
public class Circle {

  public float _x, _y, _radius;

  public Circle(float x, float y, float radius) {
    this._x = x;
    this._y = y;
    this._radius = radius;
  }

  @Override
  public String toString() {
    return "x: " + _x + ", y: " + _y + ", radius: " + _radius;
  }
}

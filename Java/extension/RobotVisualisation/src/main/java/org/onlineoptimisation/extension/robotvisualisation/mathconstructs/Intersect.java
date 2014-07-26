/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package visualisation.mathconstructs;

/**
 *
 * @author Wolff
 */
public class Intersect {

  public Intersect(float x, float y, Circle first, Circle second) {
    this.x = x;
    this.y = y;
    this.first = first;
    this.second = second;
  }

  public float x, y;
  public Circle first, second;

  @Override
  public String toString() {
    return "x: " + x + ", y: " + y;// + ", first: " + first + ", second: " + second;
  }
}

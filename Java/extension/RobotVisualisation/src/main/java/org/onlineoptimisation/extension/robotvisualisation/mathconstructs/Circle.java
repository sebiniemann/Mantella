package visualisation.mathconstructs;

/**
 *
 * @author Wolff
 */
public class Circle {

  public float x, y, radius;

  public Circle(float x, float y, float radius) {
    this.x = x;
    this.y = y;
    this.radius = radius;
  }

  @Override
  public String toString() {
    return "x: " + x + ", y: " + y + ", radius: " + radius;
  }
}

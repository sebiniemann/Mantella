package org.onlineoptimisation.extension.robotvisualisation;

import org.onlineoptimisation.extension.robotvisualisation.mathconstructs.Circle;
import org.armadillojava.Mat;
import com.jme3.math.FastMath;
import com.jme3.math.Vector2f;
import com.jme3.math.Vector3f;
import com.jme3.scene.Mesh;
import com.jme3.scene.VertexBuffer.Type;
import com.jme3.util.BufferUtils;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import org.onlineoptimisation.extension.robotvisualisation.mathconstructs.Intersect;

/**
 *
 * @author Wolff
 */
public class WorkSpaceMesh extends Mesh {

  private Mat _armMatrix = null;
  private float _distanceArmEndToEndEffector;

  public WorkSpaceMesh(Mat armMatrix, float distanceArmEndToEndEffector) {
    this._armMatrix = armMatrix;
    this._distanceArmEndToEndEffector = distanceArmEndToEndEffector;

    //We need all the circles first, naturally
    ArrayList<Circle> circles = getCircles(this._armMatrix);
    //Now we calculate the intersections that we need (ie only the "inner" ones); we also directly sort them
    ArrayList<Intersect> sortedIntersects = sortIntersects(getIntersects(circles), getCentrePoint(circles));

    ArrayList<Vector3f> vertices = new ArrayList<Vector3f>();
    ArrayList<Vector2f> texCoords = new ArrayList<Vector2f>();
    ArrayList<Integer> triangleIndexes = new ArrayList<Integer>();

        //Determines the stepping in which to increase the angle
    //TODO: can possibly determined programmatically, but the radii would have to get huge for that to matter
    float stepping = 0.1f;

    Vector3f centrePoint = new Vector3f(getCentrePoint(circles).x, getCentrePoint(circles).y, 0);
    //Adding these as fixpoints, they are also always the "starting point" of textures
    vertices.add(centrePoint);
    texCoords.add(new Vector2f(0, 0));
    //Adding the first one and his lower partner by hand makes the for loop way easier&faster
    vertices.add(new Vector3f(sortedIntersects.get(0)._x, sortedIntersects.get(0)._y, 0));
    texCoords.add(new Vector2f(1, 0));

    for (int i = 0; i < sortedIntersects.size(); i++) {
      //have to get the circle "drawing" the edge first
      Circle currCircle = null;
      float angle = 0f;
      float endingAngle = 0f;

      if (i == sortedIntersects.size() - 1) {
        currCircle = getCircleFor(sortedIntersects.get(i), sortedIntersects.get(0));
        angle = getAngleFor(currCircle, sortedIntersects.get(i));
        endingAngle = getAngleFor(currCircle, sortedIntersects.get(0));
      } else {
        currCircle = getCircleFor(sortedIntersects.get(i), sortedIntersects.get(i + 1));
        angle = getAngleFor(currCircle, sortedIntersects.get(i));
        endingAngle = getAngleFor(currCircle, sortedIntersects.get(i + 1));
      }

      System.out.println("angle: " + angle);
      System.out.println("ending: " + endingAngle);

      while (isSmaller(angle, stepping, endingAngle)) {
        float currAngle = calcNextAngle(angle, stepping, endingAngle);

        float x = currCircle._x + currCircle._radius * FastMath.cos(currAngle);
        float y = currCircle._y + currCircle._radius * FastMath.sin(currAngle);
        Vector3f nextPoint = new Vector3f(x, y, 0);
        vertices.add(nextPoint);
        texCoords.add(new Vector2f(0, 1));
        //ceiling: should add centre -> second-to-last -> nextPoint as a triangle
        triangleIndexes.add(0);
        triangleIndexes.add(vertices.size() - 2);
        triangleIndexes.add(vertices.size() - 1);
        angle = currAngle;
      }
      //finish this section up; if we never got to made a step, we have to connect start and end. otherwise it will make a triangle with the last vertice
      Vector3f endPoint = null;
      if (i == sortedIntersects.size() - 1) {
        endPoint = new Vector3f(sortedIntersects.get(0)._x, sortedIntersects.get(0)._y, 0);
      } else {
        endPoint = new Vector3f(sortedIntersects.get(i + 1)._x, sortedIntersects.get(i + 1)._y, 0);
      }

      vertices.add(endPoint);
      texCoords.add(new Vector2f(1, 0));
      //ceiling:
      triangleIndexes.add(0);
      triangleIndexes.add(vertices.size() - 2);
      triangleIndexes.add(vertices.size() - 1);
    }
    Vector3f[] verticeArray = new Vector3f[vertices.size()];
    vertices.toArray(verticeArray);
    Vector2f[] texCoordsArray = new Vector2f[texCoords.size()];
    texCoords.toArray(texCoordsArray);
    setBuffer(Type.Position, 3, BufferUtils.createFloatBuffer(verticeArray));
    setBuffer(Type.TexCoord, 2, BufferUtils.createFloatBuffer(texCoordsArray));
    setBuffer(Type.Index, 3, BufferUtils.createIntBuffer(visualisation.util.Util.convertIntegers(triangleIndexes)));
    updateBound();
  }

  private float calcArmLength(int index) {
    float length = 0;
    for (int i = 0; i < _armMatrix.n_cols / 2; i += 2) {
      Vector2f curr = new Vector2f((float) _armMatrix.at(index, i), (float) _armMatrix.at(index, i + 1));
      Vector2f next = new Vector2f((float) _armMatrix.at(index, i + 2), (float) _armMatrix.at(index, i + 3));
      length = length + curr.distance(next);
    }
    return length;
  }

  private Intersect[] getIntersectsFor(Circle first, Circle second) {
    float distance = FastMath.sqrt(FastMath.sqr(first._x - second._x) + FastMath.sqr(first._y - second._y));
    float a = (FastMath.sqr(first._radius) - FastMath.sqr(second._radius) + FastMath.sqr(distance)) / (2 * distance);
    float b = distance - a;
    float h = FastMath.sqrt(FastMath.sqr(first._radius) - FastMath.sqr(a));
    Vector2f centerPoint = new Vector2f(first._x + (a * (second._x - first._x) / distance), first._y + (a * (second._y - first._y) / distance));
    //float upperX = middlePoint.x = 
    Intersect upper = new Intersect(centerPoint.x + (h * (second._y - first._y) / distance), centerPoint.y - (h * (second._x - first._x) / distance), first, second);
    Intersect lower = new Intersect(centerPoint.x - (h * (second._y - first._y) / distance), centerPoint.y + (h * (second._x - first._x) / distance), first, second);
    return new Intersect[]{lower, upper};
  }

  private ArrayList<Circle> getCircles(Mat armMatrix) {
    ArrayList<Circle> circles = new ArrayList<Circle>();
    for (int i = 0; i < armMatrix.n_rows; i++) {
      float length = calcArmLength(i) + _distanceArmEndToEndEffector;
      Circle curr = new Circle((float) armMatrix.at(i, 0), (float) armMatrix.at(i, 1), length);
      circles.add(curr);
      System.out.println(curr);
    }
    return circles;
  }

  private ArrayList<Intersect> getIntersects(ArrayList<Circle> circles) {
    ArrayList<Intersect[]> tempIntersects = new ArrayList<Intersect[]>();
    ArrayList<Intersect> intersects = new ArrayList<Intersect>();
    //to avoid double listings, we remove the currently checked circle from the pool for future checks
    ArrayList<Circle> circlePool = new ArrayList<Circle>(circles);
    //iterate over circles and match up with the remaining circles
    for (Circle c : circles) {
      circlePool.remove(c);
      for (Circle c2 : circlePool) {
        Intersect[] temp = getIntersectsFor(c, c2);
        tempIntersects.add(temp);
      }
    }

        //TAKING CARE OF SPECIAL CASES WITH ONLY 1 AND 2 CIRCLES!
    //TODO: do this; put next stuff into else statement
    if (circles.size() == 1) {
    } else if (circles.size() == 2) {
    } else {
      //gotta remove all wrong intersections. for that we calculate the arithmetic mean to get the centre
      Vector2f centre = getCentrePoint(circles);

      for (Intersect[] is : tempIntersects) {
        float dist1 = centre.distance(new Vector2f(is[0]._x, is[0]._y));
        float dist2 = centre.distance(new Vector2f(is[1]._x, is[1]._y));
        if (dist1 < dist2) {
          intersects.add(is[0]);
        } else {
          intersects.add(is[1]);
        }
      }
    }

    return intersects;
  }

  private Vector2f getCentrePoint(ArrayList<Circle> circles) {
    float x = 0;
    float y = 0;
    for (Circle c : circles) {
      x += c._x;
      y += c._y;
    }
    x /= circles.size();
    y /= circles.size();
    Vector2f centre = new Vector2f(x, y);
    return centre;
  }

  private ArrayList<Intersect> sortIntersects(ArrayList<Intersect> intersects, Vector2f centrePoint) {
    ArrayList<Float> angles = new ArrayList<Float>();
    HashMap<Float, Intersect> mapping = new HashMap<Float, Intersect>();
    for (Intersect is : intersects) {
      float angle = centrePoint.angleBetween(new Vector2f(is._x, is._y));
      angles.add(angle);
      mapping.put(angle, is);
    }
    Collections.sort(angles);
    ArrayList<Intersect> orderedList = new ArrayList<Intersect>();
    for (float angle : angles) {
      orderedList.add(mapping.get(angle));
    }
    return orderedList;
  }

  private Circle getCircleFor(Intersect first, Intersect second) {
    if (first._first.equals(second._first) || first._first.equals(second._second)) {
      return first._first;
    } else if (first._second.equals(second._first) || first._second.equals(second._second)) {
      return first._second;
    } else {
      return null;
    }
  }

  private float getAngleFor(Circle circle, Intersect intersect) {
    //return FastMath.acos((intersect.x - circle.x) / circle.radius);
    return FastMath.atan2(intersect._y - circle._y, intersect._x - circle._x);
  }

  private boolean isSmaller(float startAngle, float stepping, float endingAngle) {
    if (areProblematicAngles(startAngle, endingAngle)) {
      return (startAngle + stepping) < ((FastMath.PI - FastMath.abs(endingAngle)) + FastMath.PI);
    } else {
      return (startAngle + stepping) < endingAngle;
    }
  }

  private boolean areProblematicAngles(float startAngle, float endingAngle) {
    if (startAngle > 0 && endingAngle < 0) {
      return true;
    } else {
      return false;
    }
  }

  private float calcNextAngle(float angle, float stepping, float endingAngle) {
    if (areProblematicAngles(angle, endingAngle)) {
      if ((angle + stepping) > FastMath.PI) {
        return -2 * FastMath.PI + angle + stepping;
      } else {
        return angle + stepping;
      }
    } else {
      return angle + stepping;
    }
  }
}

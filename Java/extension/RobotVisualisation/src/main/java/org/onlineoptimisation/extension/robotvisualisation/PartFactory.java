package org.onlineoptimisation.extension.robotvisualisation;

import com.jme3.asset.AssetManager;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Quaternion;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;
import com.jme3.scene.debug.Arrow;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Cylinder;
import com.jme3.scene.shape.Sphere;
import com.jme3.scene.shape.Torus;
import com.jme3.util.TangentBinormalGenerator;

public class PartFactory {

  protected AssetManager _assetManager;

  private final float _endEffectorRadius = 0.025f;
  private final float _armThickness = 0.016f;
  private final float _zOffset = _armThickness * 3;

  public PartFactory(AssetManager assetManager) {
    this._assetManager = assetManager;
  }

  public Spatial getEndEffector(float distanceArmEndToEndEffector) {
    return getEndEffector(distanceArmEndToEndEffector, new Vector3f(0, 0, 0), 0);
  }

  public Spatial getEndEffector(float distanceArmEndToEndEffector, Vector3f endEffectorPos, float endEffectorRotation) {
    Node endEffectorNode = new Node("endEffectorNode");

    Cylinder endEffector = new Cylinder(10, 10, _endEffectorRadius, 0f, _armThickness * 3, true, false);
    Geometry geom = new Geometry("endEffector", endEffector);

    //assign a material and color
    Material mat = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    mat.setColor("Color", ColorRGBA.Black);
    geom.setMaterial(mat);

    //init the arrows
    Arrow xAxis = new Arrow(Vector3f.UNIT_X.mult(distanceArmEndToEndEffector));
    xAxis.setLineWidth(4); // make arrow thicker
    Geometry xArrowGeom = new Geometry("endEffectorXAxis", xAxis);
    Material xMat = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    xMat.getAdditionalRenderState().setWireframe(true);
    xMat.setColor("Color", ColorRGBA.Red);
    xArrowGeom.setMaterial(xMat);

    Arrow yAxis = new Arrow(Vector3f.UNIT_Y.mult(distanceArmEndToEndEffector));
    yAxis.setLineWidth(4); // make arrow thicker
    Geometry yArrowGeom = new Geometry("endEffectorYAxis", yAxis);
    Material yMat = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    yMat.getAdditionalRenderState().setWireframe(true);
    yMat.setColor("Color", ColorRGBA.Green);
    yArrowGeom.setMaterial(yMat);

    //init the "donut" around the endeffector
    Torus torus = new Torus(40, 40, _armThickness / 4, distanceArmEndToEndEffector);
    Geometry torusGeom = new Geometry("endEffectorDonut", torus);
    TangentBinormalGenerator.generate(torus);
    //we can re-use the endeffector material here
    torusGeom.setMaterial(mat);

    //move to position
    endEffectorNode.move(endEffectorPos.add(0, 0, _zOffset));

    endEffectorNode.attachChild(geom);
    endEffectorNode.attachChild(xArrowGeom);
    endEffectorNode.attachChild(yArrowGeom);
    endEffectorNode.attachChild(torusGeom);

    //rotate it
    Quaternion rot = new Quaternion();
    rot.fromAngleAxis(endEffectorRotation, Vector3f.UNIT_Z);
    endEffectorNode.setLocalRotation(rot);

    return endEffectorNode;
  }

  public Spatial getLink(float length, Vector3f position, Vector3f direction) {
    Geometry link = (Geometry) getLink(length);
    link.lookAt(direction, Vector3f.UNIT_Y);
    link.setLocalTranslation(position);
    return link;
  }

  public Spatial getLink(float length) {
    Cylinder linkShape = new Cylinder(20, (int) Math.ceil(length) * 20, _armThickness / 2, length, true, false);
    Geometry linkGeometry = new Geometry("Link", linkShape);

    Material linkMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    linkMaterial.setColor("Color", ColorRGBA.Black);

    linkGeometry.setMaterial(linkMaterial);

    return linkGeometry;
  }

  public Spatial getPrismaticJoint(boolean active, boolean base, float length, Vector3f start, Vector3f end, Vector3f blockPosition) {
    Node node = (Node) getPrismaticJoint(active, base, length, start, end);
    node.getChild("BoxLink").setLocalTranslation(blockPosition);
    return node;
  }

  public Spatial getPrismaticJoint(boolean active, boolean base, float length, Vector3f start, Vector3f end) {
    Node prismaticJoint = new Node();

    if (base) {
      Material baseMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
      baseMaterial.setColor("Color", ColorRGBA.Black);

      Box leftBaseShape = new Box(_armThickness, _armThickness, _armThickness);
      Geometry leftBaseGeometry = new Geometry("leftBaseLink", leftBaseShape);
      //leftBaseGeometry.setLocalTranslation(new Vector3f(0, 0, -length / 2));
      leftBaseGeometry.setLocalTranslation(start);

      leftBaseGeometry.setMaterial(baseMaterial);
      prismaticJoint.attachChild(leftBaseGeometry);

      Box rightbaseShape = new Box(_armThickness, _armThickness, _armThickness);
      Geometry rightbaseGeometry = new Geometry("rightBaseLink", rightbaseShape);
      //rightbaseGeometry.setLocalTranslation(new Vector3f(0, 0, length / 2));
      rightbaseGeometry.setLocalTranslation(end);

      rightbaseGeometry.setMaterial(baseMaterial);
      prismaticJoint.attachChild(rightbaseGeometry);

    }

    Vector3f line = end.subtract(start);
    Cylinder linkShape = new Cylinder(20, 100, _armThickness / 4, length, true, false);
    Geometry linkGeometry = new Geometry("Link", linkShape);

    linkGeometry.lookAt(line, Vector3f.UNIT_Y);

    linkGeometry.setLocalTranslation(start.add(line.mult(0.5f)));

    Material linkMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    linkMaterial.setColor("Color", ColorRGBA.Black);

    linkGeometry.setMaterial(linkMaterial);
    prismaticJoint.attachChild(linkGeometry);

    Box joint = new Box(_armThickness * 2, _armThickness, _armThickness);
    Geometry jointGeometry = new Geometry("BoxLink", joint);
    float angle = line.angleBetween(Vector3f.UNIT_X);
    jointGeometry.rotate(0, 0, angle);

    Material jointMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");

    if (active) {
      jointMaterial.setColor("Color", ColorRGBA.Gray);
    } else {
      jointMaterial.setColor("Color", ColorRGBA.White);
    }

    jointGeometry.setMaterial(jointMaterial);
    prismaticJoint.attachChild(jointGeometry);

    return prismaticJoint;
  }

  public Spatial getSphericalJoint(Vector3f position) {
    Node joint = (Node) getSphericalJoint();
    joint.setLocalTranslation(position);
    return joint;
  }

  public Spatial getSphericalJoint() {
    Node spericalJoint = new Node();

    Sphere jointShape = new Sphere(20, 20, 0.1f, false, false);
    Geometry jointGeometry = new Geometry("Link", jointShape);

    Material jointMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    jointMaterial.setColor("Color", ColorRGBA.White);

    jointGeometry.setMaterial(jointMaterial);
    spericalJoint.attachChild(jointGeometry);

    return spericalJoint;
  }

  public Spatial getUniversalJoint(Vector3f position) {
    Node joint = (Node) getUniversalJoint();
    joint.setLocalTranslation(position);
    return joint;
  }

  public Spatial getUniversalJoint() {
    return getSphericalJoint();
  }

  public Spatial getRotationalJoint(boolean active, boolean base, Vector3f position) {
    System.out.println(_assetManager);
    Node joint = (Node) getRotationalJoint(active, base);
    joint.setLocalTranslation(position);
    return joint;
  }

  public Spatial getRotationalJoint(boolean active, boolean base) {
    Node rotationalJoint = new Node();

    if (base) {
      Cylinder baseShape = new Cylinder(10, 10, _armThickness / 3, _armThickness, _armThickness, true, false);
      Geometry baseGeometry = new Geometry("Link", baseShape);

      Material baseMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
      baseMaterial.setColor("Color", ColorRGBA.White);

      baseGeometry.setMaterial(baseMaterial);
      //baseGeometry.rotateUpTo(Vector3f.UNIT_Z);
      baseGeometry.setLocalTranslation(0, 0, -_armThickness * 1.5f);
      rotationalJoint.attachChild(baseGeometry);
    }

    Sphere jointShape = new Sphere(20, 20, _armThickness, false, false);
    Geometry jointGeometry = new Geometry("Link", jointShape);

    Material jointMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");

    if (active) {
      jointMaterial.setColor("Color", ColorRGBA.Gray);
    } else {
      jointMaterial.setColor("Color", ColorRGBA.White);
    }

    jointGeometry.setMaterial(jointMaterial);
    rotationalJoint.attachChild(jointGeometry);

    return rotationalJoint;
  }
}

package org.onlineoptimisation.extension.robotvisualisation;

import com.jme3.asset.AssetManager;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.FastMath;
import com.jme3.math.Quaternion;
import com.jme3.math.Vector3f;
import com.jme3.scene.Geometry;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Cylinder;
import com.jme3.scene.shape.Sphere;
import com.jme3.scene.shape.Dome;

public class PartFactory {
  
  protected AssetManager _assetManager;
  
  public PartFactory(AssetManager assetManager) {
    _assetManager = assetManager;
  }
  
  public Spatial getLink(float length) {
    Cylinder linkShape = new Cylinder(20, (int) Math.ceil(length) * 20, 0.02f, length, true, false);
    Geometry linkGeometry = new Geometry("Link", linkShape);

    Material linkMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    linkMaterial.setColor("Color", ColorRGBA.Black);

    linkGeometry.setMaterial(linkMaterial);
    
    return linkGeometry;
  }
  
  public Spatial getPrismaticJoint(boolean active, boolean base, float length) {
    Node prismaticJoint = new Node();

    if(base) {
      Material baseMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
      baseMaterial.setColor("Color", ColorRGBA.Black);
      
      Box leftBaseShape = new Box(0.06f, 0.06f, 0.06f);
      Geometry leftBaseGeometry = new Geometry("Link", leftBaseShape);
      leftBaseGeometry.setLocalTranslation(new Vector3f(0, 0, -length / 2));
      
      leftBaseGeometry.setMaterial(baseMaterial);
      prismaticJoint.attachChild(leftBaseGeometry);
      
      Box rightbaseShape = new Box(0.06f, 0.06f, 0.06f);
      Geometry rightbaseGeometry = new Geometry("Link", rightbaseShape);
      rightbaseGeometry.setLocalTranslation(new Vector3f(0, 0, length / 2));
      
      rightbaseGeometry.setMaterial(baseMaterial);
      prismaticJoint.attachChild(rightbaseGeometry);
    }

    Cylinder linkShape = new Cylinder(20, 100, 0.02f, 5f, true, false);
    Geometry linkGeometry = new Geometry("Link", linkShape);
    linkGeometry.setLocalTranslation(new Vector3f(0, 0.18f, 0));

    Material linkMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    linkMaterial.setColor("Color", ColorRGBA.Black);

    linkGeometry.setMaterial(linkMaterial);
    prismaticJoint.attachChild(linkGeometry);
    
    Box joint = new Box(0.12f, 0.12f, 0.24f);
    Geometry jointGeometry = new Geometry("Link", joint);
    jointGeometry.setLocalTranslation(new Vector3f(0, 0.18f, 0));

    Material jointMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");

    if(active) {
      jointMaterial.setColor("Color", ColorRGBA.Gray);
    } else {
      jointMaterial.setColor("Color", ColorRGBA.White);
    }

    jointGeometry.setMaterial(jointMaterial);
    prismaticJoint.attachChild(jointGeometry);
    
    return prismaticJoint;
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
  
  public Spatial getUniversalJoint() {
    return getSphericalJoint();
  }
  
  public Spatial getRotationalJoint(boolean active, boolean base) {
    Node rotationalJoint = new Node();
    
    if(base) {
      Dome baseShape = new Dome(Vector3f.ZERO, 2, 20, 0.2f, false);
      Geometry baseGeometry = new Geometry("Link", baseShape);
      
      Material baseMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
      baseMaterial.setColor("Color", ColorRGBA.White);
      
      baseGeometry.setMaterial(baseMaterial);
      rotationalJoint.attachChild(baseGeometry);
    }

    Sphere jointShape = new Sphere(20, 20, 0.1f, false, false);
    Geometry jointGeometry = new Geometry("Link", jointShape);
    
    Quaternion jointRotation = new Quaternion();
    jointRotation.fromAngles(FastMath.PI/2, 0, 0);
    jointGeometry.setLocalRotation(jointRotation);
    
    jointGeometry.setLocalTranslation(new Vector3f(0, 0.2f, 0));

    Material jointMaterial = new Material(_assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
    
    if(active) {
      jointMaterial.setColor("Color", ColorRGBA.Gray);
    } else {
      jointMaterial.setColor("Color", ColorRGBA.White);
    }

    jointGeometry.setMaterial(jointMaterial);
    rotationalJoint.attachChild(jointGeometry);
    
    return rotationalJoint;
  }
  
}

package org.onlineoptimisation.extension.robotvisualisation;

import com.jme3.app.SimpleApplication;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.FastMath;
import com.jme3.math.Quaternion;
import com.jme3.math.Vector3f;
import com.jme3.post.FilterPostProcessor;
import com.jme3.post.filters.CartoonEdgeFilter;
import com.jme3.scene.Geometry;
import com.jme3.scene.Mesh;
import com.jme3.scene.Spatial;
import com.jme3.scene.debug.Arrow;

/**
 * Sample 2 - How to use nodes as handles to manipulate objects in the scene.
 * You can rotate, translate, and scale objects by manipulating their parent nodes.
 * The Root Node is special: Only what is attached to the Root Node appears in the scene.
 */
public class Main extends SimpleApplication {

  public static void main(String[] args) {
    Main app = new Main();
    app.start();
  }

  @Override
  public void simpleInitApp() {
    viewPort.setBackgroundColor(ColorRGBA.LightGray);
    
    FilterPostProcessor filterPostProcessor = new FilterPostProcessor(assetManager);
    filterPostProcessor.addFilter(new CartoonEdgeFilter());
    viewPort.addProcessor(filterPostProcessor);
    
    flyCam.setEnabled(false);
    inputManager.setCursorVisible(true);
    inputManager.clearMappings();
    new CustomCam(inputManager, cam);

    PartFactory partFactory = new PartFactory(assetManager);
    
    Spatial activePrismaticJoint1 = partFactory.getPrismaticJoint(true, true, 5f);
    rootNode.attachChild(activePrismaticJoint1);

    Spatial activeRotationalJoint1 = partFactory.getRotationalJoint(true, false);
    activeRotationalJoint1.setLocalTranslation(new Vector3f(0, 0.2f, 0));
    rootNode.attachChild(activeRotationalJoint1);
    
    Spatial activePrismaticJoint2 = partFactory.getPrismaticJoint(true, true, 5f);
    activePrismaticJoint2.setLocalTranslation(new Vector3f(5f, 0, 0));
    rootNode.attachChild(activePrismaticJoint2);

    Spatial activeRotationalJoint2 = partFactory.getRotationalJoint(true, false);
    activeRotationalJoint2.setLocalTranslation(new Vector3f(5f, 0.2f, 0));
    rootNode.attachChild(activeRotationalJoint2);
    
    Spatial activePrismaticJoint3 = partFactory.getPrismaticJoint(true, true, 5f);
    Quaternion rotation = new Quaternion();
    rotation.fromAngles(0, FastMath.PI/2, 0);
    activePrismaticJoint3.setLocalRotation(rotation);
    activePrismaticJoint3.setLocalTranslation(new Vector3f(2.5f, 0, 4f));
    rootNode.attachChild(activePrismaticJoint3);

    Spatial activeRotationalJoint3 = partFactory.getRotationalJoint(true, false);
    activeRotationalJoint3.setLocalTranslation(new Vector3f(2.5f, 0.2f, 4f));
    rootNode.attachChild(activeRotationalJoint3);
    
    
    
    Spatial link11 = partFactory.getLink(1.25f);
    link11.setLocalRotation(rotation);
    link11.setLocalTranslation(new Vector3f(0.625f, 0.4f, 0f));
    rootNode.attachChild(link11);
    
    Spatial passiveRotationalJoint1 = partFactory.getRotationalJoint(false, false);
    passiveRotationalJoint1.setLocalTranslation(new Vector3f(1.25f, 0.2f, 0f));
    rootNode.attachChild(passiveRotationalJoint1);
    
    Spatial link12 = partFactory.getLink(1.25f);
    link12.setLocalRotation(rotation);
    link12.setLocalTranslation(new Vector3f(1.875f, 0.4f, 0f));
    rootNode.attachChild(link12);

    
    Spatial link21 = partFactory.getLink(1.25f);
    link21.setLocalRotation(rotation);
    link21.setLocalTranslation(new Vector3f(4.375f, 0.4f, 0f));
    rootNode.attachChild(link21);
    
    Spatial passiveRotationalJoint2 = partFactory.getRotationalJoint(false, false);
    passiveRotationalJoint2.setLocalTranslation(new Vector3f(3.75f, 0.2f, 0f));
    rootNode.attachChild(passiveRotationalJoint2);

    Spatial link22 = partFactory.getLink(1.25f);
    link22.setLocalRotation(rotation);
    link22.setLocalTranslation(new Vector3f(3.125f, 0.4f, 0f));
    rootNode.attachChild(link22);

    
    Spatial link31 = partFactory.getLink(2f);
    link31.setLocalTranslation(new Vector3f(2.5f, 0.4f, 3f));
    rootNode.attachChild(link31);
    
    Spatial passiveRotationalJoint3 = partFactory.getRotationalJoint(false, false);
    passiveRotationalJoint3.setLocalTranslation(new Vector3f(2.5f, 0.2f, 2f));
    rootNode.attachChild(passiveRotationalJoint3);
    
    Spatial link32 = partFactory.getLink(2f);
    link32.setLocalTranslation(new Vector3f(2.5f, 0.4f, 1f));
    rootNode.attachChild(link32);
    
    attachCoordinateAxes(new Vector3f(2.5f, 0, 0));
  }

  //FOR DEBUGGING PURPOSES
  private void attachCoordinateAxes(Vector3f pos) {
      Arrow arrow = new Arrow(Vector3f.UNIT_X);
      arrow.setLineWidth(4); // make arrow thicker
      putShape(arrow, ColorRGBA.Red).setLocalTranslation(pos);

      arrow = new Arrow(Vector3f.UNIT_Y);
      arrow.setLineWidth(4); // make arrow thicker
      putShape(arrow, ColorRGBA.Green).setLocalTranslation(pos);

      arrow = new Arrow(Vector3f.UNIT_Z);
      arrow.setLineWidth(4); // make arrow thicker
      putShape(arrow, ColorRGBA.Blue).setLocalTranslation(pos);
  }

  private Geometry putShape(Mesh shape, ColorRGBA color) {
      Geometry g = new Geometry("coordinate axis", shape);
      Material mat = new Material(assetManager, "Common/MatDefs/Misc/Unshaded.j3md");
      mat.getAdditionalRenderState().setWireframe(true);
      mat.setColor("Color", color);
      g.setMaterial(mat);
      rootNode.attachChild(g);
      return g;
  }
}
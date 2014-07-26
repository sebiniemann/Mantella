package visualisation;

import org.armadillojava.Mat;
import com.jme3.app.SimpleApplication;
import com.jme3.light.AmbientLight;
import com.jme3.light.DirectionalLight;
import com.jme3.material.Material;
import com.jme3.material.RenderState.BlendMode;
import com.jme3.material.RenderState.FaceCullMode;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;
import com.jme3.post.FilterPostProcessor;
import com.jme3.post.filters.CartoonEdgeFilter;
import com.jme3.renderer.RenderManager;
import com.jme3.renderer.queue.RenderQueue.Bucket;
import com.jme3.scene.Geometry;
import com.jme3.scene.Mesh;
import com.jme3.scene.Node;
import com.jme3.scene.debug.Arrow;
import com.jme3.scene.shape.Box;
import com.jme3.scene.shape.Cylinder;
import com.jme3.scene.shape.Sphere;
import com.jme3.system.AppSettings;
import com.jme3.system.JmeCanvasContext;
import java.awt.Canvas;
import java.awt.Dimension;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class JMonkeyApplication extends SimpleApplication {

  private float armThickness = 0.016f;
  private float endEffectorRadius = 0.025f;
  private float zOffset = armThickness * 3;
  private final Node prismaticJointNode = new Node("prismaticJointNode");
  private final Node armsNode = new Node("armsNode");
  private final Node endEffectorNode = new Node("endEffectorNode");
  private CustomCam camera = null;
  private ImageDisplay imageDisplay = null;

  private PartFactory partFactory = null;

  Mat pjStartPos;
  Mat pjEndPos;
  Mat pjLength;
  Mat arms;
  Mat activeJoints;
  Mat linkLength;
  Mat endEffectorPos;
  double endEffectorRot;

  public JMonkeyApplication(int width, int height, Mat pjStartPos, Mat pjEndPos, Mat pjLength, Mat arms, Mat activeJoints, Mat linkLength, Mat endEffectorPos, double endEffectorRot) {

    settings = new AppSettings(true);
    settings.setWidth(width);
    settings.setHeight(height);
    //samples = anti-aliasing
    settings.setSamples(4);
    setPauseOnLostFocus(false);
    setSettings(settings);
    createCanvas();
    JmeCanvasContext ctx = (JmeCanvasContext) getContext();
    ctx.setSystemListener(this);
    Dimension dim = new Dimension(width, height);
    ctx.getCanvas().setPreferredSize(dim);

    imageDisplay = new ImageDisplay(width, height);
    imageDisplay.setPreferredSize(new Dimension(width, height));

    this.pjStartPos = pjStartPos;
    this.pjEndPos = pjEndPos;
    this.pjLength = pjLength;
    this.arms = arms;
    this.activeJoints = activeJoints;
    this.linkLength = linkLength;
    this.endEffectorPos = endEffectorPos;
    this.endEffectorRot = endEffectorRot;
  }

  public void updateVisualisation(Mat pjStartPos, Mat pjEndPos, Mat pjLength, Mat arms, Mat activeJoints, Mat linkLength, Mat endEffectorPos, double endEffectorRot) {
    this.pjStartPos = pjStartPos;
    this.pjEndPos = pjEndPos;
    this.pjLength = pjLength;
    this.arms = arms;
    this.activeJoints = activeJoints;
    this.linkLength = linkLength;
    this.endEffectorPos = endEffectorPos;
    this.endEffectorRot = endEffectorRot;

    prismaticJointNode.detachAllChildren();
    armsNode.detachAllChildren();
    endEffectorNode.detachAllChildren();

    Vector3f VectorEndEffectorPos = new Vector3f((float) endEffectorPos.at(0, 0), (float) endEffectorPos.at(1, 0), (float) endEffectorPos.at(2, 0));

    initArmsAndPrismaticJoints(arms, activeJoints, linkLength, pjStartPos, pjEndPos, pjLength);
    initEndEffector(VectorEndEffectorPos, (float) endEffectorRot, VectorEndEffectorPos.distance(new Vector3f((float) arms.at(0, arms.n_cols - 2), (float) arms.at(0, arms.n_cols - 1), 0)));

  }

  @Override
  public void simpleInitApp() {
    //logger severity needs to be set higher because of mesh debug warnings
    Logger.getLogger("com.jme3.util").setLevel(Level.SEVERE);
    //disable the standard fly cam
    flyCam.setEnabled(false);
    //flyCam.setMoveSpeed(5);
    inputManager.setCursorVisible(true);
    //attach subnodes for our components
    rootNode.attachChild(prismaticJointNode);
    rootNode.attachChild(armsNode);
    rootNode.attachChild(endEffectorNode);

    //need lighting so textured parts are visible
    initLighting();

    //add robot
    partFactory = new PartFactory(assetManager);
    updateVisualisation(pjStartPos, pjEndPos, pjLength, arms, activeJoints, linkLength, endEffectorPos, endEffectorRot);

        //backgroundcolor
    //viewPort.setBackgroundColor(new ColorRGBA(0.925f, 1.0f, 0.81f, 1.0f));
    viewPort.setBackgroundColor(ColorRGBA.LightGray);

        //adding a filter for the shader effect
//        FilterPostProcessor filterPostProcessor = new FilterPostProcessor(assetManager);
//        filterPostProcessor.addFilter(new CartoonEdgeFilter());
//        viewPort.addProcessor(filterPostProcessor);
    //rootnode scaling
    rootNode.setLocalScale(3);

    //camera startposition
    cam.setLocation(new Vector3f(1.5f, 1.5f, 9.0f));
    System.out.println(cam.getDirection());
    System.out.println(cam.getUp());

    //we also want to use our own input settings
    initControls();

    //DEBUG
    attachCoordinateAxes(new Vector3f(-2, 0, -2));
  }

  @Override
  public void simpleUpdate(float tpf) {
    //TODO: add update code
  }

  @Override
  public void simpleRender(RenderManager rm) {
    //TODO: add render code
  }

  public Canvas get3DCanvas() {
    return ((JmeCanvasContext) getContext()).getCanvas();
  }

  public ImageDisplay getPlotCanvas() {
    return imageDisplay;
  }

  public void setPlotMatrix(Mat input) {
    imageDisplay.updateImageContents(input);
  }

  private void initControls() {
    inputManager.clearMappings();
    camera = new CustomCam(inputManager, cam);
  }

  private void initArmsAndPrismaticJoints(Mat arms, Mat activeJoints, Mat linkLength, Mat pjStartPos, Mat pjEndPos, Mat pjLength) {
    ArrayList<Vector3f> prismaticJointArmConnectorBlocks = new ArrayList<Vector3f>();

    for (int i = 0; i < arms.n_rows; i++) {
      prismaticJointArmConnectorBlocks.add(new Vector3f((float) arms.at(i, 0), (float) arms.at(i, 1), 0));
      //go part by part through the arm
      Vector3f previous = null;
      for (int j = 0; j + 1 < arms.n_cols; j += 2) {
        Vector3f curr = new Vector3f((float) arms.at(i, j), (float) arms.at(i, j + 1), zOffset);
        armsNode.attachChild(partFactory.getRotationalJoint(activeJoints.at(i, j / 2) != 0, j == 0, curr));
        //armsNode.attachChild(partFactory.getRotationalJoint(activeJoints.at(i, j / 2) != 0, false, curr));

        if (previous != null) {
          Vector3f line = curr.subtract(previous);
          armsNode.attachChild(partFactory.getLink((float) linkLength.at(i, (j / 2) - 1), previous.add(line.mult(0.5f)), line));
        }

        previous = curr;
      }
    }

    //check for errors in parameters
    if (pjStartPos.n_rows != pjEndPos.n_rows || pjStartPos.n_cols != pjEndPos.n_cols) {
      System.out.println("start- and endmatrices of prismatic joints are not equally long.");
      return;
    }

    //iterate over all possible prismatic joints
    for (int i = 0; i < pjStartPos.n_rows; i++) {
      //convert to vector3f
      Vector3f start = new Vector3f((float) pjStartPos.at(i, 0), (float) pjStartPos.at(i, 1), 0);
      Vector3f end = new Vector3f((float) pjEndPos.at(i, 0), (float) pjEndPos.at(i, 1), 0);
      prismaticJointNode.attachChild(partFactory.getPrismaticJoint(true, true, (float) pjLength.at(i, 0), start, end, prismaticJointArmConnectorBlocks.get(i)));
    }
  }

  private void initEndEffector(Vector3f position, float rotation, float distanceArmEndToEndEffector) {
    endEffectorNode.attachChild(partFactory.getEndEffector(distanceArmEndToEndEffector, position, rotation));
  }

  private void initLighting() {
    //basic lighting so we don't have infinitely dark shadows
    AmbientLight al = new AmbientLight();
    al.setColor(ColorRGBA.White.mult(0.5f));
    rootNode.addLight(al);

    //sun-like lighting
    DirectionalLight dl = new DirectionalLight();
    dl.setColor(ColorRGBA.White);
    dl.setDirection(new Vector3f(1.0f, 1.0f, 0).normalizeLocal());
    rootNode.addLight(dl);
  }

  public void setBackgroundColor(ColorRGBA color) {
    viewPort.setBackgroundColor(color);
  }

  public void setCameraLocation(Vector3f location) {
    cam.setLocation(location);
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

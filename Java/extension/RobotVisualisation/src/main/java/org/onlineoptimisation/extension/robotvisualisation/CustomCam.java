package org.onlineoptimisation.extension.robotvisualisation;

import com.jme3.input.InputManager;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.AnalogListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseAxisTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.math.Matrix3f;
import com.jme3.math.Quaternion;
import com.jme3.math.Vector3f;
import com.jme3.renderer.Camera;

public class CustomCam implements AnalogListener, ActionListener {

  protected InputManager _inputManager;
  protected Camera       _cam;

  protected float        _translationSpeed;
  protected float        _rotationSpeed;

  protected boolean      _rotatePressed;

  public CustomCam(InputManager inputManager, Camera cam) {
    _inputManager = inputManager;
    _cam = cam;

    _translationSpeed = 10f;
    _rotationSpeed = 1f;

    _rotatePressed = false;
    
    inputManager.addMapping("Up",
      new KeyTrigger(KeyInput.KEY_W),
      new KeyTrigger(KeyInput.KEY_UP)
      );
    inputManager.addMapping("Down",
      new KeyTrigger(KeyInput.KEY_S),
      new KeyTrigger(KeyInput.KEY_DOWN)
      );
    inputManager.addMapping("Left",
      new KeyTrigger(KeyInput.KEY_A),
      new KeyTrigger(KeyInput.KEY_LEFT)
      );
    inputManager.addMapping("Right",
      new KeyTrigger(KeyInput.KEY_D),
      new KeyTrigger(KeyInput.KEY_RIGHT)
      );

    inputManager.addMapping("Zoom in", new MouseAxisTrigger(MouseInput.AXIS_WHEEL, false));
    inputManager.addMapping("Zoom out", new MouseAxisTrigger(MouseInput.AXIS_WHEEL, true));

    inputManager.addMapping("Rotate", new MouseButtonTrigger(MouseInput.BUTTON_LEFT));

    inputManager.addMapping("Rotate up", new MouseAxisTrigger(MouseInput.AXIS_Y, true));
    inputManager.addMapping("Rotate down", new MouseAxisTrigger(MouseInput.AXIS_Y, false));
    inputManager.addMapping("Rotate left", new MouseAxisTrigger(MouseInput.AXIS_X, true));
    inputManager.addMapping("Rotate right", new MouseAxisTrigger(MouseInput.AXIS_X, false));

    inputManager.addMapping("Reset", new KeyTrigger(KeyInput.KEY_SPACE));

    inputManager.addListener(this, new String[]{"Up", "Left", "Down", "Right", "Zoom in", "Zoom out", "Rotate", "Rotate up", "Rotate down", "Rotate left", "Rotate right", "Reset"});

    resetCamera();
  }

  public void onAnalog(String name, float keyPressed, float timePerFrame) {
    switch (name) {
      case "Up":
        moveCamAlongZ(keyPressed * _translationSpeed);
        break;
      case "Down":
        moveCamAlongZ(-keyPressed * _translationSpeed);
        break;
      case "Left":
        moveCamAlongX(keyPressed * _translationSpeed);
        break;
      case "Right":
        moveCamAlongX(-keyPressed * _translationSpeed);
        break;
      case "Zoom in":
        moveCamAlongY(-keyPressed);
        break;
      case "Zoom out":
        moveCamAlongY(keyPressed);
        break;
      case "Rotate up":
        rotateCamera(keyPressed * _rotationSpeed, _cam.getLeft());
        break;
      case "Rotate down":
        rotateCamera(-keyPressed * _rotationSpeed, _cam.getLeft());
        break;
      case "Rotate left":
        rotateCamera(keyPressed * _rotationSpeed, _cam.getUp());
        break;
      case "Rotate right":
        rotateCamera(-keyPressed * _rotationSpeed, _cam.getUp());
        break;
    }
  }

  @Override
  public void onAction(String name, boolean keyPressed, float timePerFrame) {
    switch (name) {
      case "Rotate":
        _rotatePressed = keyPressed;
        break;
      case "Reset":
        resetCamera();
        break;
    }

    if (_rotatePressed) {
      _inputManager.setCursorVisible(false);
    } else {
      _inputManager.setCursorVisible(true);
    }
  }

  private void moveCamAlongY(float value) {
    Vector3f pos = _cam.getLocation().clone();
    pos.addLocal(new Vector3f(0, value, 0));
    _cam.setLocation(pos);
  }

  private void moveCamAlongX(float value) {
    Vector3f pos = _cam.getLocation().clone();
    pos.addLocal(new Vector3f(value, 0, 0));
    _cam.setLocation(pos);
  }

  private void moveCamAlongZ(float value) {
    Vector3f pos = _cam.getLocation().clone();
    pos.addLocal(new Vector3f(0, 0, value));
    _cam.setLocation(pos);
  }

  protected void rotateCamera(float value, Vector3f axis) {
    if (!_rotatePressed) {
      return;
    }

    Matrix3f matrix = new Matrix3f();
    matrix.fromAngleNormalAxis(value, axis);

    Vector3f up = _cam.getUp();
    Vector3f left = _cam.getLeft();
    Vector3f dir = _cam.getDirection();

    matrix.mult(up, up);
    matrix.mult(left, left);
    matrix.mult(dir, dir);

    Quaternion quaternion = new Quaternion();
    quaternion.fromAxes(left, up, dir);
    quaternion.normalizeLocal();

    _cam.setAxes(quaternion);
  }

  protected void resetCamera() {
    Vector3f location = new Vector3f(0, 10, 0);
    Vector3f x = new Vector3f(1, 0, 0);
    Vector3f y = new Vector3f(0, 0, 1);
    Vector3f direction = new Vector3f(0, -1, 0);
    _cam.setFrame(location, x, y, direction);
  }
}
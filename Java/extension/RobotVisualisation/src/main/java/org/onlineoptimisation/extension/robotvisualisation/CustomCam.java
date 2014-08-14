/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
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

/**
 *
 * @author Wolff
 */
public class CustomCam implements AnalogListener, ActionListener {

  private static String[] _mappings = new String[]{"Up", "Left", "Down", "Right", "ZoomIn", "ZoomOut", "RotateDrag", "Control", "CamUp", "CamDown", "CamLeft", "CamRight"};
  InputManager _inputManager;
  boolean _canRotate = false;
  boolean _controlPressed = false;
  float _movespeed = 10f;
  float _rotationSpeed = 1f;
  Camera _cam;
  Vector3f _initialUpVector;

  public CustomCam(InputManager inputManager, Camera cam) {
    this._inputManager = inputManager;
    this._cam = cam;
    _initialUpVector = cam.getUp().clone();

    //this always moves along the axis, no matter camera rotation (see "onAnalog" below)
    inputManager.addMapping("Up", new KeyTrigger(KeyInput.KEY_W));
    inputManager.addMapping("Left", new KeyTrigger(KeyInput.KEY_A));
    inputManager.addMapping("Down", new KeyTrigger(KeyInput.KEY_S));
    inputManager.addMapping("Right", new KeyTrigger(KeyInput.KEY_D));

    inputManager.addMapping("ZoomIn", new MouseAxisTrigger(MouseInput.AXIS_WHEEL, false));
    inputManager.addMapping("ZoomOut", new MouseAxisTrigger(MouseInput.AXIS_WHEEL, true));

    inputManager.addMapping("RotateDrag", new MouseButtonTrigger(MouseInput.BUTTON_LEFT));
    inputManager.addMapping("Control", new KeyTrigger(KeyInput.KEY_LCONTROL));

    inputManager.addMapping("CamLeft", new MouseAxisTrigger(MouseInput.AXIS_X, true),
            new KeyTrigger(KeyInput.KEY_LEFT));

    inputManager.addMapping("CamRight", new MouseAxisTrigger(MouseInput.AXIS_X, false),
            new KeyTrigger(KeyInput.KEY_RIGHT));

    inputManager.addMapping("CamUp", new MouseAxisTrigger(MouseInput.AXIS_Y, false),
            new KeyTrigger(KeyInput.KEY_UP));

    inputManager.addMapping("CamDown", new MouseAxisTrigger(MouseInput.AXIS_Y, true),
            new KeyTrigger(KeyInput.KEY_DOWN));

    inputManager.addListener(this, _mappings);
  }

  public void onAnalog(String name, float value, float tpf) {
    if (name.equals("Up")) {
      moveCamAlongY(value);
    } else if (name.equals("Left")) {
      moveCamAlongX(-value);
    } else if (name.equals("Down")) {
      moveCamAlongY(-value);
    } else if (name.equals("Right")) {
      moveCamAlongX(value);
    } else if (name.equals("ZoomIn")) {
      moveCamAlongZ(-value);
    } else if (name.equals("ZoomOut")) {
      moveCamAlongZ(value);
    } else if (name.equals("CamUp")) {
      rotateCamera(-value * 1, _cam.getLeft());
    } else if (name.equals("CamDown")) {
      rotateCamera(value * 1, _cam.getLeft());
    } else if (name.equals("CamLeft")) {
      rotateCamera(value, _initialUpVector);
    } else if (name.equals("CamRight")) {
      rotateCamera(-value, _initialUpVector);
    }
  }

  public void onAction(String name, boolean isPressed, float tpf) {
    if (name.equals("RotateDrag")) {
      _canRotate = isPressed;
    } else if (name.equals("Control")) {
      _controlPressed = isPressed;
    }
    if (_canRotate && _controlPressed) {
      _inputManager.setCursorVisible(false);
    } else {
      _inputManager.setCursorVisible(true);
    }
  }

  private void moveCamAlongY(float value) {
    Vector3f vel = new Vector3f(0, value * _movespeed, 0);
    Vector3f pos = _cam.getLocation().clone();

    pos.addLocal(vel);
    _cam.setLocation(pos);
  }

  private void moveCamAlongX(float value) {
    Vector3f vel = new Vector3f(value * _movespeed, 0, 0);
    Vector3f pos = _cam.getLocation().clone();

    pos.addLocal(vel);
    _cam.setLocation(pos);
  }

  private void moveCamAlongZ(float value) {
    Vector3f vel = new Vector3f(0, 0, value);
    Vector3f pos = _cam.getLocation().clone();

    pos.addLocal(vel);
    _cam.setLocation(pos);
  }

  public void setMoveSpeed(float value) {
    _movespeed = value;
  }

  protected void rotateCamera(float value, Vector3f axis) {
    //camera shall only rotate when ctrl and the left mouse button is pressed
    if (!_controlPressed || !_canRotate) {
      return;
    }
    
    //some math stuff to rotate
    Matrix3f mat = new Matrix3f();
    mat.fromAngleNormalAxis(_rotationSpeed * value, axis);

    Vector3f up = _cam.getUp();
    Vector3f left = _cam.getLeft();
    Vector3f dir = _cam.getDirection();

    mat.mult(up, up);
    mat.mult(left, left);
    mat.mult(dir, dir);

    Quaternion q = new Quaternion();
    q.fromAxes(left, up, dir);
    q.normalizeLocal();

    _cam.setAxes(q);
  }
}

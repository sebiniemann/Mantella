
![Schematic representation of a 4PRUS parallel kinematic machine]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/parallel_kinematic_machine_4PRUS.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="ParallelKinematicMachine4PRUS" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setRedundantJointStartPositions" %}, {% include reference prefix=include.anchor_prefix name="setRedundantJointEndPositions" %}, {% include reference prefix=include.anchor_prefix name="setEndEffectorJointPositions" %}, {% include reference prefix=include.anchor_prefix name="setLinkLengths" %}
- Model<br>
  {% include reference prefix=include.anchor_prefix name="getModel" %}, {% include reference prefix=include.anchor_prefix name="isWithinWorkspace" %}, {% include reference prefix=include.anchor_prefix name="getActuation" %}, {% include reference prefix=include.anchor_prefix name="getEndEffectorPose" %}
- Measurements<br>
  {% include reference prefix=include.anchor_prefix name="getEndEffectorPoseError" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

---
{% include label prefix=include.anchor_prefix name="ParallelKinematicMachine4PRUS" %}
**ParallelKinematicMachine4PRUS()** {% include noexcept %}

- Creates a default instance of this class.

---
{% include label prefix=include.anchor_prefix name="setRedundantJointStartPositions" %}
**<small>void</small> .setRedundantJointStartPositions( <small>arma::Mat&lt;double&gt;::fixed&lt;3, 4&gt;</small> X )** {% include noexcept %}

- Parameterises the start position of the redundant joints \\(\left\\{J_0^\text{redundant,start}, \ldots, J_3^\text{redundant,start}\right\\}\\), relative to the robot's base.
- The *i*-te row stands for the *x,y,z*-coordinate of \\(J_i^\text{redundant,start}\\).

---
{% include label prefix=include.anchor_prefix name="setRedundantJointEndPositions" %}
**<small>void</small> .setRedundantJointEndPositions( <small>arma::Mat&lt;double&gt;::fixed&lt;3, 4&gt;</small> X )** {% include noexcept %}

- Parameterises the end position of the redundant joints \\(\left\\{J_0^\text{redundant,end}, \ldots, J_3^\text{redundant,end}\right\\}\\), relative to the robot's base.
- The *i*-te row stands for the *x,y,z*-coordinate of \\(J_i^\text{redundant,end}\\).

---
{% include label prefix=include.anchor_prefix name="setEndEffectorJointPositions" %}
**<small>void</small> .setEndEffectorJointPositions( <small>arma::Mat&lt;double&gt;::fixed&lt;3, 4&gt;</small> X )** {% include noexcept %}

- Parameterises the position of the end-effector joints \\(\left\\{J_0^\text{end-effector}, \ldots, J_3^\text{end-effector}\right\\}\\), relative to the endeffector's position.
- The *i*-te row stands for the *x,y,z*-coordinate of \\(J_i^\text{end-effector}\\).

---
{% include label prefix=include.anchor_prefix name="setLinkLengths" %}
**<small>void</small> .setLinkLengths( <small>arma::Mat&lt;double&gt;::fixed&lt;2, 4&gt;</small> X )** {% include noexcept %}

- Parameterises the link lenghts \\(\left(\left(L_0^1, L_0^2\right), \ldots, \left(L_3^1, L_3^2\right)\right)\\), starting from the active joints to the end-effector joints.
- The *i*-te row stands for the link lengts \\(\left(L_i^1, L_i^2\right)\\).

---
{% include label prefix=include.anchor_prefix name="getModel" %}
**<small>arma::Cube&lt;double&gt;::fixed&lt;3, 4, 2&gt;</small> .getModel( <small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> E, <small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> R )**

- Returns the position of all actively or passively moveable joints \\(\left(\left(J_0^\text{base}, \ldots, J_3^\text{base}\right), \left(J_0^\text{intermediate}, \ldots, J_3^\text{intermediate}\right), \left(J_0^\text{end-effector}, \ldots, J_3^\text{end-effector}\right)\right)\\) to fully describe the given robot.
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].

---
{% include label prefix=include.anchor_prefix name="isWithinWorkspace" %}
**<small>bool</small> .isWithinWorkspace( <small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> E, <small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> R )**

- Returns true, if the end-effector position *E* is reachable within the workspace, using the redundancy resolution *R*, and false otherwise.
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].

---
{% include label prefix=include.anchor_prefix name="getActuation" %}
**<small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> .getActuation( <small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> E, <small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> R )**

- Calculates the actuation of the active joints to reach the end-effector pose *E*, using the redundancy resolution *R*.
- The *i*-te row stands for the rotation of the *i*-te joint.
- The result lies within \\(\left[-\pi, \pi\right]\\).
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].
- **Requirement:** The end-effector pose *E* must be reachable, using the redundancy resolution *R.

---
{% include label prefix=include.anchor_prefix name="getEndEffectorPose" %}
**<small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> .getEndEffectorPose( <small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> A, <small>arma::Row&lt;double&gt;::fixed&lt;3&gt;</small> Q, <small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> R )**

- Calculates the end-effector pose, given the active joints actuation *A* and end-effector roll,pitch,yaw-rotation *Q* in radians, using the redundancy resolution *R*.
- Returns the *x,y,z*-coordinate of the end-effector, concatenated with its roll,pitch,yaw-rotation.
- **Note:** The end-effector pose is not required to be reachable.
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].

---
{% include label prefix=include.anchor_prefix name="getEndEffectorPoseError" %}
**<small>double</small> .getEndEffectorPoseError( <small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> E, <small>arma::Row&lt;double&gt;::fixed&lt;4&gt;</small> R )**

- Returns an estimated deviation from the actuated end-effector pose and the actual one.
- The estimation is based on the spectral-norm of the Jacobians of the direct and inverse kinematics.
- The result lies within \\(\left[1, \infty\right]\\), whereas 1 indicates no deviation at all and \\(\infty\\) an unlimited deviation.
- The result will also be \\(\infty\\) for all unreachable end-effector poses.
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *robotic_parallel_kinematic_machine_4prus*.

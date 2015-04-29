![Schematic representation of a multi-level Stewart platform]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/multi_level_stewart_platform.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="MultiLevelStewartPlatform" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setPlatformLevels" %}
- Model<br>
  {% include reference prefix=include.anchor_prefix name="getModel" %}, {% include reference prefix=include.anchor_prefix name="isWithinWorkspace" %}, {% include reference prefix=include.anchor_prefix name="getActuation" %}, {% include reference prefix=include.anchor_prefix name="getEndEffectorPose" %}
- Measurements<br>
  {% include reference prefix=include.anchor_prefix name="getEndEffectorPoseError" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

---
{% include label prefix=include.anchor_prefix name="MultiLevelStewartPlatform" %}
**MultiLevelStewartPlatform()** {% include noexcept %}

- Creates a default instance of this class.

---
{% include label prefix=include.anchor_prefix name="setPlatformLevels" %}
**<small>void</small> .setPlatformLevels( <small>std::vector&lt;mant::ParallelKinematicMachine6PUPS&gt;</small> X )**

- Parameterises the platform levels \\(\left\{\text{6UPS}_0, \text{6UPS}_1, \ldots\right\}\\).
- The *i*-te 6UPS stands for the *i*-te platform level.
- **Requirement:** All platform levels must be redundancy free, .i.e \\(J_{\text{6UPS}_i, j}^\text{redundant,start} = J_{\text{6UPS}_i, j}^\text{redundant,end}\\).

---
{% include label prefix=include.anchor_prefix name="getModel" %}
**<small>arma::Cube&lt;double&gt;::fixed&lt;3, 6, 2&gt;</small> .getModel( <small>arma::Mat&lt;double&gt;</small> E )**

- Returns for each platform level the position of all actively or passively moveable joints \\(\left(\left(\left(J_{\text{6UPS}_0, 0}^\text{base}, \ldots, J_{\text{6UPS}_0, 5}^\text{base}\right), \left(J_{\text{6UPS}_0, 0}^\text{end-effector}, \ldots, J_{\text{6UPS}_0, 5}^\text{end-effector}\right)\right), \left(\left(J_{\text{6UPS}_1, 0}^\text{base}, \ldots, J_{\text{6UPS}_1, 5}^\text{base}\right), \left(J_{\text{6UPS}_1, 0}^\text{end-effector}, \ldots, J_{\text{6UPS}_1, 5}^\text{end-effector}\right)\right), \ldots\right)\\), to fully describe the given robot.
- **Requirement:** The number of columns in *E* must be equal to the number of platform levels.
- **Requirement:** The number of rows in *E* must be equal to 6.
- **Requirement:** The pose given by the *i*-te row in *E* must be reachable by the *i*-te platform level.

---
{% include label prefix=include.anchor_prefix name="isWithinWorkspace" %}
**<small>bool</small> .isWithinWorkspace( <small>std::vector&lt;arma::Col&lt;double&gt;::fixed&lt;6&gt;&gt;</small> E)**

- Returns true, if each end-effector position in *E* is reachable for the corresponding platform level, and false otherwise.
- An end-effector position is reachable, iff \\(P_{\text{6UPS}_i}^\text{min} \leq\\) `.getActuation(E, {})` \\(\leq P_{\text{6UPS}_i}^\text{max}\\).
- **Requirement:** The number of columns in *E* must be equal to the number of platform levels.
- **Requirement:** The number of rows in *E* must be equal to 6.

---
{% include label prefix=include.anchor_prefix name="getActuation" %}
**<small>arma::Mat&lt;double&gt;</small> .getActuation( <small>std::vector&lt;arma::Col&lt;double&gt;::fixed&lt;6&gt;&gt;</small> E)**

- Calculates for each platform level the actuation of the active joints to reach the corresponding end-effector pose in *E*.
- The *i*-te row stands for the *i*-te platform level.
- The result lies within [0, 1], whereas 0 stands for no actuation and 1 for \\(P_i^\text{max} - P_i^\text{min}\\).
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].
- **Requirement:** The end-effector pose *E* must be reachable, using the redundancy resolution *R.

---
{% include label prefix=include.anchor_prefix name="getEndEffectorPose" %}
**<small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> .getEndEffectorPose( <small>arma::Row&lt;double&gt;::fixed&lt;6&gt;</small> A, <small>arma::Row&lt;double&gt;::fixed&lt;3&gt;</small> Q, <small>arma::Row&lt;double&gt;::fixed&lt;6&gt;</small> R )**

- Calculates the end-effector pose, given the active joints actuation *A* and end-effector roll,pitch,yaw-rotation *Q* in radians, using the redundancy resolution *R*.
- Returns the *x,y,z*-coordinate of the end-effector, concatenated with its roll,pitch,yaw-rotation.
- **Note:** The end-effector pose is not required to be reachable.
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].

---
{% include label prefix=include.anchor_prefix name="getEndEffectorPoseError" %}
**<small>double</small> .getEndEffectorPoseError( <small>arma::Col&lt;double&gt;::fixed&lt;6&gt;</small> E, <small>arma::Row&lt;double&gt;::fixed&lt;6&gt;</small> R )**

- Returns an estimated deviation from the actuated end-effector pose and the actual one.
- The estimation is based on the spectral-norm of the Jacobians of the direct and inverse kinematics.
- The result lies within [\\(\left[1, \infty\right]\\)], whereas 1 indicates no deviation at all and \\(\infty\\) an unlimited deviation.
- The result will also be \\(\infty\\) for all unreachable end-effector poses.
- **Requirement:** The number of elements in *R* must match the number of redundant joints.
- **Requirement:** All elements in *R* must be within [0, 1].

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *robotic_multi_level_stewart_platform*.
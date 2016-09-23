
.. cpp:function:: template <T1, T2, T3, T4, T5> circle_sphere_intersection(circle_centre, circle_radius, circle_normal, sphere_centre, sphere_radius)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T1** - The circle centre value type.
    * **T2** - The circle radius type.
    * **T3** - The circle normal value type.
    * **T4** - The sphere centre value type.
    * **T5** - The sphere radius type.
  
  **Function parameters**
   
    * **circle_centre** (``std::array<T1, 3>``) - The circle centre.
    * **circle_radius** (``T2``) - The circle radius **(must be positive (including 0))**.
    * **circle_normal** (``std::array<T3, 3>``) - The circle normal.
    * **sphere_centre** (``std::array<T4, 3>``) - The sphere centre.
    * **sphere_radius** (``T5``) - The sphere radius **(must be positive (including 0))**.

  **Returns**
   
    ``std::array<typename std::common_type<T1, T2, T3, T4, T5>::type, 6>, bool>`` - The second value will be true if the circle and sphere intersect and false otherwise. The first value will contain both intersections (if they exist).
    
  .. note::
  
    The whole computation will be done in ``std::common_type<T1, T2, T3, T4, T5>::type`` and cast the provided parameters if necessary. It is therefore advisable to ensure that ``T1 = T2 = T3 = T4 = T5`` to avoid casts at runtime.
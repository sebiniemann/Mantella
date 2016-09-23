
.. cpp:function:: template <T1, T2, T3, T4> circle_circle_intersection(first_centre, circle_radius, sphere_centre, sphere_radius)

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T1** - The first circle centre's value type.
    * **T2** - The circle radius type.
    * **T4** - The sphere centre value type.
    * **T5** - The sphere radius type.
  
  **Function parameters**
   
    * **first_centre** (``std::array<T1, 2>``) - The first circle centre.
    * **first_radius** (``T2``) - The first circle radius **(must be positive (including 0))**.
    * **sphere_centre** (``std::array<T3, 2>``) - The second circle centre.
    * **sphere_radius** (``T4``) - The second circle radius **(must be positive (including 0))**.

  **Returns**
   
    ``std::array<typename std::common_type<T1, T2, T3, T4, T5>::type, 6>, bool>`` - The second value will be true if both circles intersect and false otherwise. The first value will contain both intersections (if they exist).
    
  .. note::
  
    The whole computation will be done in ``std::common_type<T1, T2, T3, T4, T5>::type`` and cast the provided parameters if necessary. It is therefore advisable to ensure that ``T1 = T2 = T3 = T4 = T5`` to avoid casts at runtime.
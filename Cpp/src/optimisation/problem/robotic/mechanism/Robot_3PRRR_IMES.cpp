/*******************************************************************************
 * Copyright (c) 2013 Sebastian Niemann <niemann@sra.uni-hannover.de> and contributors.
 *
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/MIT
 *******************************************************************************/
#pragma once

#include <armadillo>
using arma::Mat;
using arma::endr;

#include <optimisation/problem/robotic/mechanism/Robot_NPRRR.cpp>
using optimisation::problem::robotic::mechanism::Robot_NPRRR;

/**
 * A 3(P̲)R̲RR robot, as build by the Institute of Mechatronic Systems, Leibniz Universität Hannover.
 * 
 * @author Sebastian Niemann <niemann@sra.uni-hannover.de>
 */
namespace optimisation {
  namespace problem {
    namespace robotic {
      namespace mechanism {
        class Robot_3PRRR_IMES: public Robot_NPRRR {
          public:
            /**
             * Specifies the parameter for an actual 3(P̲)R̲RR robot.
             */
            Robot_3PRRR_IMES() :
                Robot_NPRRR(
                    // {First kinematic chain, Second kinematic chain, Third kinematic chain}
                    {
                        -0.000066580445834, -0.092751709777083, 0.092818290222917, // X-axis
                        0.106954081945581, -0.053477040972790, -0.053477040972790 // Y-Axis
                    },
                    // {First kinematic chain, Second kinematic chain, Third kinematic chain}
                    {
                        0.58215, 0.58152, 0.58121, // Near the base
                        0.61975, 0.62002, 0.61964  // ear the end effector
                    },
                    // {First kinematic chain, Second kinematic chain, Third kinematic chain}
                    {
                        0.090782289181018, 0.0, 1.1996, // X-axis
                        1.001794428720682, -0.2, -0.2  // Y-Axis
                    },
                    // {First kinematic chain, Second kinematic chain, Third kinematic chain}
                    {
                        1.090757710818982, 0.0, 1.1996, // X-axis
                        1.008805571279318, 0.8, 0.8  // Y-Axis
                    }) {
            }
            ;
        };
      }
    }
  }
}

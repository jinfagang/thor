/*
 * Copyright (c) 2020 Fagang Jin.
 *
 * This file is part of thor
 * (see manaai.cn).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
//
// Created by jintian on 19-4-30.
//

#ifndef THOR_INCLUDE_GEOMETRY_H_
#define THOR_INCLUDE_GEOMETRY_H_


#ifdef USE_EIGEN
#include "eigen3/Eigen/Core"
#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"

namespace thor {

namespace geometry {

Eigen::Matrix3d Quaternion2RotationMatrix(const double x, const double y,
                                          const double z, const double w);

Eigen::Quaterniond EulerAngle2Quaternion(const double yaw, const double pitch,
                                         const double roll);

}
}
#endif


#endif //THOR_INCLUDE_GEOMETRY_H_

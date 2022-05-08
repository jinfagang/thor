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
// Created by fagangjin on 11/12/2019.
//

#include "thor/image.h"

namespace thor {
namespace image {

static const float kMean[3] = {0.485f, 0.456f, 0.406f};
static const float kStdDev[3] = {0.229f, 0.224f, 0.225f};
static const int map_[7][3] = {{0, 0, 0},   {128, 0, 0},   {0, 128, 0},
                               {0, 0, 128}, {128, 128, 0}, {128, 0, 128},
                               {0, 128, 0}};

float *Normalize(cv::Mat img) {
  // cv::Mat image(img.rows, img.cols, CV_32FC3);
  float *data;
  data = (float *)calloc(img.rows * img.cols * 3, sizeof(float));
  for (int c = 0; c < 3; ++c) {
    for (int i = 0; i < img.rows; ++i) {  //获取第i行首像素指针
      cv::Vec3b *p1 = img.ptr<cv::Vec3b>(i);
      // cv::Vec3b *p1 = image.ptr<cv::Vec3b>(i);
      for (int j = 0; j < img.cols; ++j) {
        data[c * img.cols * img.rows + i * img.cols + j] =
            (p1[j][c] / 255. - kMean[c]) / kStdDev[c];
      }
    }
  }
  return data;
}

float *HWC2CHW(cv::Mat img, const float kMeans[3], const float kStds[3]) {
  // convert HWC input normal image into CHW format
  // we have to make sure img data type is float
  float *data;
  data = (float *)calloc(img.rows * img.cols * 3, sizeof(float));
  for (int c = 0; c < 3; ++c) {
    for (int i = 0; i < img.rows; ++i) {
      cv::Vec3b *p1 = img.ptr<cv::Vec3b>(i);
      for (int j = 0; j < img.cols; ++j) {
        data[c * img.cols * img.rows + i * img.cols + j] =
            (p1[j][c] - kMeans[c]) / kStds[c];
      }
    }
  }
  return data;
}

// float *HWC2CHW_fast(cv::Mat img, const float kMeans[3], const float kStds[3])
// {
//   // haven't implemented yet
// }

cv::Mat read2mat(float *prob, cv::Mat out) {
  for (int i = 0; i < 128; ++i) {
    cv::Vec<float, 7> *p1 = out.ptr<cv::Vec<float, 7>>(i);
    for (int j = 0; j < 128; ++j) {
      for (int c = 0; c < 7; ++c) {
        p1[j][c] = prob[c * 128 * 128 + i * 128 + j];
      }
    }
  }
  return out;
}

cv::Mat map2threeunchar(cv::Mat real_out, cv::Mat real_out_) {
  for (int i = 0; i < 512; ++i) {
    cv::Vec<float, 7> *p1 = real_out.ptr<cv::Vec<float, 7>>(i);
    cv::Vec3b *p2 = real_out_.ptr<cv::Vec3b>(i);
    for (int j = 0; j < 512; ++j) {
      int index = 0;
      float swap;
      for (int c = 0; c < 7; ++c) {
        if (p1[j][0] < p1[j][c]) {
          swap = p1[j][0];
          p1[j][0] = p1[j][c];
          p1[j][c] = swap;
          index = c;
        }
      }
      p2[j][0] = map_[index][2];
      p2[j][1] = map_[index][1];
      p2[j][2] = map_[index][0];
    }
  }
  return real_out_;
}

cv::Mat resizeAlongShortest(cv::Mat img, int target_w, int target_h) {
  cv::Mat intermediateImg, outputImg;
  int delta_w, delta_h, top, left, bottom, right;
  int new_w = img.size().width;
  int new_h = img.size().height;

  if (static_cast<float>(target_w / img.size().width) <
      static_cast<float>(target_h / img.size().height)) {
    new_w = target_w;
    new_h = (img.size().height * target_w) / img.size().width;
  } else {
    new_h = target_h;
    new_w = (img.size().width * target_h) / img.size().height;
  }
  cv::resize(img, intermediateImg, cv::Size(new_w, new_h));
  float w_scale = target_w / static_cast<float>(new_w);
  float h_scale = target_h / static_cast<float>(new_h);
  delta_w = target_w - new_w;
  delta_h = target_h - new_h;
  top = floor(delta_h / 2);
  bottom = delta_h - floor(delta_h / 2);
  left = floor(delta_w / 2);
  right = delta_w - floor(delta_w / 2);
  cv::copyMakeBorder(intermediateImg, outputImg, top, bottom, left, right,
                     cv::BORDER_CONSTANT, (0, 0, 0));
  return outputImg;
}

}  // namespace image


}  // namespace thor

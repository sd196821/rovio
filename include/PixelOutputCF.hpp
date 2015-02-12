/*
* Copyright (c) 2014, Autonomous Systems Lab
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
* names of its contributors may be used to endorse or promote products
* derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef PIXELOUTPUTCF_HPP_
#define PIXELOUTPUTCF_HPP_

#include "FilterState.hpp"
#include "CoordinateTransform.hpp"
#include "Camera.hpp"

namespace LWF {

class PixelOutput: public State<VectorElement<2>>{
 public:
  static constexpr unsigned int _pix = 0;
  PixelOutput(){
  }
  ~PixelOutput(){};
};
template<typename STATE>
class PixelOutputCF:public CoordinateTransform<STATE,PixelOutput>{
 public:
  typedef CoordinateTransform<STATE,PixelOutput> Base;
  using Base::eval;
  typedef typename Base::mtInput mtInput;
  typedef typename Base::mtOutput mtOutput;
  typedef typename Base::mtMeas mtMeas;
  typedef typename Base::mtJacInput mtJacInput;
  int ind_;
  rovio::Camera* mpCamera_;
  PixelOutputCF(rovio::Camera* mpCamera){
    ind_ = 0;
    mpCamera_ = mpCamera;
  };
  void setIndex(int ind){
    ind_ = ind;
  }
  ~PixelOutputCF(){};
  mtOutput eval(const mtInput& input, const mtMeas& meas, double dt = 0.0) const{
    mtOutput output;
    // MrMV = MrMV
    // qVM = qVM
    cv::Point2f c;
    mpCamera_->bearingToPixel(input.template get<mtInput::_nor>(ind_),c);
    output.template get<mtOutput::_pix>() = Eigen::Vector2d(c.x,c.y);
    return output;
  }
  mtJacInput jacInput(const mtInput& input, const mtMeas& meas, double dt = 0.0) const{
    mtJacInput J;
    J.setZero();
    cv::Point2f c;
    Eigen::Matrix<double,2,2> J_temp;
    mpCamera_->bearingToPixel(input.template get<mtInput::_nor>(ind_),c,J_temp);
    J.template block<2,2>(mtOutput::template getId<mtOutput::_pix>(),mtInput::template getId<mtInput::_nor>(ind_)) = J_temp;
    return J;
  }
};

}


#endif /* PIXELOUTPUTCF_HPP_ */

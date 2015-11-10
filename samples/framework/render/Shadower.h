/*
Copyright (c) 2010, The Cinder Project: http://libcinder.org
All rights reserved.

This code is intended for use with the Cinder C++ library: http://libcinder.org

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and
the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

// this is a modified version from Cinder ShaowMap sample

#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Vector.h"

using namespace ci;
typedef std::shared_ptr<class ShadowMap> ShadowMapRef;

class ShadowMap
{
   public:
      static ShadowMapRef create (int size)
      {
         return ShadowMapRef (new ShadowMap{size});
      }

      ShadowMap (int size)
      {
         reset (size);
      }

	  ~ShadowMap() = default;

      void reset (int size);

      const gl::FboRef & getFbo() const
      {
         return shadowMap;
      }

      const gl::Texture2dRef & getTexture() const
      {
         return textureShadowMap;
      }

      float	getAspectRatio() const
      {
         return shadowMap->getAspectRatio();
      }

      ivec2	getSize() const
      {
         return shadowMap->getSize();
      }

   private:
      gl::FboRef shadowMap;
      gl::Texture2dRef textureShadowMap;
};


struct LightData
{
   bool	toggleViewpoint;
   float distanceRadius;
   float fov;
   CameraPersp camera;
   vec3	viewpoint;
   vec3	target;
};


struct Shadower
{
	Shadower() = default;
	~Shadower() = default;
 
   bool setup();

   gl::GlslProgRef shader;
   ShadowMapRef shadowMap;
   int shadowMapSize;
   bool	onlyShadowMap;
   LightData light;
   int shadowTechnique;
   float depthBias;
   bool	enableNormSlopeOffset;
   float randomOffset;
   int numRandomSamples;
   float polygonOffsetFactor, polygonOffsetUnits;

}; // end class Shadower


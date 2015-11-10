

#include "Shadower.h"
#include "ShaderSource.h"

void ShadowMap::reset (int size)
{
   gl::Texture2d::Format depthFormat;
   depthFormat.setInternalFormat (GL_DEPTH_COMPONENT32F);
   depthFormat.setMagFilter (GL_LINEAR);
   depthFormat.setMinFilter (GL_LINEAR);
   depthFormat.setWrap (GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
   depthFormat.setCompareMode (GL_COMPARE_REF_TO_TEXTURE);
   depthFormat.setCompareFunc (GL_LEQUAL);
   textureShadowMap = gl::Texture2d::create (size, size, depthFormat);
   gl::Fbo::Format fboFormat;
   fboFormat.attachment (GL_DEPTH_ATTACHMENT, textureShadowMap);
   shadowMap = gl::Fbo::create (size, size, fboFormat);
}

bool Shadower::setup()
{
   shadowMapSize = 2048;
   light.distanceRadius = 100.0f;
   light.viewpoint = vec3 (light.distanceRadius);
   light.fov = 10.0f;
   light.target = vec3 (0);
   light.toggleViewpoint = false;
   shadowTechnique = 3;
   depthBias = -0.0005f;
   randomOffset = 1.2f;
   numRandomSamples = 32;
   enableNormSlopeOffset = false;
   onlyShadowMap = false;
   polygonOffsetFactor = polygonOffsetUnits = 3.0f;

   try
   {
      shader = gl::GlslProg::create (shadowVert, shadowFrag);
   }
   catch (const gl::GlslProgCompileExc & exc)
   {
      return false;
   }

   shadowMap = ShadowMap::create (shadowMapSize);
   light.camera.setPerspective (light.fov, shadowMap->getAspectRatio(), 0.5, 500.0);
   return true;
}

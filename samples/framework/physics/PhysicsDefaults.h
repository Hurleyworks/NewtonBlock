 
#pragma once

#include <string>
#include <cinder/CinderAssert.h>
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include "cinder/AxisAlignedBox.h"
#include "cinder/Color.h"
#include "PhysicsData.h"

const ci::Color DEFAULT_COLOR = ci::Color(1.0f, 0.0f, 0.0f);
const ci::vec3 DEFAULT_GRAVITY = ci::vec3(0.0f, -9.8f, 0.0f);
const ci::vec3 DEFAULT_SCALE = ci::vec3(1.0f, 1.0f, 1.0f);
const EngineState DEFAULT_ENGINE_STATE = EngineState::Paused;
const int DEFAULT_SUBSTEPS = 8;
const float DEFAULT_FRAMERATE = 1.0f / 60.0f;
const BodyType DEFAULT_BODYTYPE = BodyType::None;
const CollisionShape DEFAULT_SHAPE = CollisionShape::Box;
const FlexType DEFAULT_FLEX = FlexType::Rigid;
const float DEFAULT_DYNAMIC_MASS = 2.0f;
const float STATIC_MASS = 0.0f;
const ci::vec3 DEFAULT_FORCE = DEFAULT_GRAVITY;
const std::string DEFAULT_BODY_NAME = "unnamed physics body";
const std::string DEFAULT_PROJECTILE_NAME = "projectile";







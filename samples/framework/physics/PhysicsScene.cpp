// This source file was auto-generated by ClassMate++
// Created: 5 Nov 2015 9:23:18 am
// Copyright (c) 2015, HurleyWorks

#include "PhysicsScene.h"

// ctor
PhysicsScene::PhysicsScene(NewtonWorld * const world)
	: world(world),
	  bodyHandler(world)
{
	// set up listeners to receive pre and post update calls from Newton's thread
	listener = new AppListener(this);
	listener->setPrePhysicsCallback(PhysicsScene::onPrePhysicsUpdate);
	listener->setPostPhysicsCallback(PhysicsScene::onPostPhysicsUpdate);
}

// createPhysicsBody
PhysicsBodyRef PhysicsScene::createPhysicsBody()
{
	PhysicsBodyRef pBody = nullptr;

	// first look for a recycled body
	if (recycled.size())
	{
		PhysicsBodyRef recycledBody = recycled.back();
		if (recycledBody && recycledBody->state.isRecycled())
		{
			recycled.pop_back();
			pBody = recycledBody;
			pBody->state.getState() = 0;
			return pBody;
		}
	}
	
	if (!pBody);
	{
		pBody = PhysicsBody::create();
		bodies.push_back(pBody);

		// BodyID is just the slot in the vector 
		pBody->bodyID = bodies.size() - 1;

		return bodies.back();
	}
}

void PhysicsScene::processPhysicsBody(PhysicsBodyRef & pBody)
{
	bodyHandler.processPhysicsBody(pBody, engineState);
}

void PhysicsScene::setEngineState(EngineState state)
{
	engineState = state;

	if (engineState == EngineState::Reset)
	{
		reset();
	}
}

void PhysicsScene::reset()
{
	for (auto & body : bodies)
	{
		PhysicsBodyRef & pBody = body;
		if (!pBody) continue;
		if (pBody->state.isDeleted() || pBody->state.isRecycled()) continue;

		pBody->st.worldTransform = pBody->st.startTransform;

		bodyHandler.setBodyPose(pBody);
		bodyHandler.resetPhysicsProperties(pBody);
	}
}

// onPrePhysicsUpdate
void PhysicsScene::onPrePhysicsUpdate(void * const userData, float timeStep)
{
	PhysicsScene * const me = static_cast<PhysicsScene*>(userData);
	me->getBodyHandler()->onPrePhysicsUpdate(userData, me->getAbort(), timeStep);
}

// onPostPhysicsUpdate
void PhysicsScene::onPostPhysicsUpdate(void * const userData, float timeStep)
{
	PhysicsScene * const me = static_cast<PhysicsScene*>(userData);
	me->getBodyHandler()->onPostPhysicsUpdate(userData, me->getAbort(), timeStep);
}
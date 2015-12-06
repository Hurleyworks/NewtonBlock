// This source file was auto-generated by ClassMate++
// Created: 12 Jan 2013 8:17:38 am
// Copyright (c) 2013, HurleyWorks

#include "Pick.h"
#include "PhysicsBody.h"

using namespace ci;

// ctor
Pick::Pick ()
	: param_ (1.0f),
	  body_(nullptr),
	  normal_(0.0f)
{	
}

// RayCastFilter
dFloat Pick::RayCastFilter (const NewtonBody* const body, 
									   const NewtonCollision* const collisionHit, 
									   const dFloat* const contact,
								       const dFloat* const normal,
								       dLong collisionID, 
								       void* const userData,
								       dFloat intersetParamm)
{	
	if (!body) return intersetParamm;

	PhysicsBody * const pBody = static_cast<PhysicsBody*>( NewtonBodyGetUserData(body));
	if (!pBody)
	{
		
		CI_ASSERT(false);
		return intersetParamm;
	}

	Pick* const pickData = static_cast<Pick*>(userData);
	if (!pickData) return intersetParamm;
	
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	
	pickData->body_ = body;
	
	if (intersetParamm < pickData->param_) 
	{
		pickData->param_ = intersetParamm;
		pickData->normal_ = vec3 (normal[0], normal[1], normal[2]);
	}

	return intersetParamm;
}

// RayCastPrefilter
unsigned Pick::RayCastPrefilter (const NewtonBody * body,
                                 const NewtonCollision * const collision, 
                                 void * const userData)
{	
	if (!body) return 0;

	PhysicsBody* const pBody = static_cast<PhysicsBody*>(NewtonBodyGetUserData(body));
	if (!pBody) return 0;
	
	return pBody->state.isPickable() ? 1 : 0;
}

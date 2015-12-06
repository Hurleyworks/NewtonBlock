// This header file was auto-generated by ClassMate++
// Created: 5 Nov 2015 10:20:56 am
// Copyright (c) 2015, HurleyWorks

#pragma once

#include "../../../NewtonAPI.h"
#include <cinder/Vector.h>
#include <cinder/Matrix.h>
#include <cinder/Ray.h>
#include "PhysicsBody.h"

class BodyOps
{

 public:
	BodyOps (NewtonWorld * const world);
	~BodyOps ();
	
	static bool glmToNewton(const ci::mat4 & transform, dMatrix & pose);

	PhysicsBodyRef pickBody(const ci::Ray & mouseRay);
	NewtonCollision * createCollisionShape(PhysicsBodyRef & pBody);
	NewtonBody * createRigidBody(PhysicsBodyRef & pBody, const dMatrix & startPose, const NewtonCollision * const shape);
	NewtonMesh * triMeshToNewtonMesh(const ci::TriMeshRef & triMesh);

 private:
	NewtonWorld * const world;

	NewtonCollision * createBoxShape(PhysicsBodyRef & pBody);
	NewtonCollision * createBallShape(PhysicsBodyRef & pBody);
	NewtonCollision * createConvexHullShape(PhysicsBodyRef & pBody);
	NewtonCollision * createStaticMeshShape(PhysicsBodyRef & pBody);
	NewtonCollision * createCompositeShape(PhysicsBodyRef & pBody);

}; // end class BodyOps

inline bool BodyOps::glmToNewton(const ci::mat4 & transform, dMatrix & pose)
{
	std::memcpy(&pose[0][0], &transform[0], 16 * sizeof(float));

	return pose.SanityCheck() ? true : false;
}
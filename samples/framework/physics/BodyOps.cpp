// This source file was auto-generated by ClassMate++
// Created: 5 Nov 2015 10:20:56 am
// Copyright (c) 2015, HurleyWorks

#include "BodyOps.h"
#include <Cinder/Matrix.h>
#include <Cinder/AxisAlignedBox.h>
#include <Cinder/TriMesh.h>
#include "Entity.h"

using namespace cinder;
using namespace ci;

template <typename Type>
Type theMax(const Type a, const Type b, const Type c) { return (a < b) ? ((b < c) ? c : b) : ((a < c) ? c : a); }

// ctor
BodyOps::BodyOps (NewtonWorld * const world)
	: world(world)
{	
}

// dtor
BodyOps::~BodyOps ()
{	
}

NewtonCollision * BodyOps::createCollisionShape(PhysicsBodyRef & pBody)
{
	if (!pBody) return nullptr;
	if (!pBody->triMesh) return nullptr;
	if (!pBody->triMesh->getNumTriangles()) return nullptr;
	if (!pBody->triMesh->getNumVertices()) return nullptr;

	// reuse the source's collision shape if we're an instance
	if (pBody->state.isInstance() && pBody->instancedFrom)
	{
		NewtonBody * const newtonBody = (NewtonBody*)pBody->instancedFrom->userData;
		return newtonBody ? NewtonBodyGetCollision(newtonBody) : nullptr;
	}
	
	switch (pBody->desc.shape)
	{
		case CollisionShape::Box:				return createBoxShape(pBody);
		case CollisionShape::Ball:				return createBallShape(pBody);
		case CollisionShape::ConvexHull:		return createConvexHullShape(pBody);
		case CollisionShape::Mesh:				return createStaticMeshShape(pBody);
		case CollisionShape::Composite:			return createCompositeShape(pBody);

		default:								return nullptr;
	}

	return nullptr;
}

NewtonBody * BodyOps::createRigidBody(PhysicsBodyRef & pBody, const dMatrix & startPose, const NewtonCollision * const shape)
{
	NewtonBody * newtonBody = NewtonCreateDynamicBody(world, shape, &startPose[0][0]);
	if (!newtonBody) return nullptr;

	// save the body raw ptr as the user data for this body
	NewtonBodySetUserData(newtonBody, pBody.get());

	// save the body ptr as user data for the component
	pBody->userData = (NewtonBody*)newtonBody;

	dFloat mass = pBody->desc.mass;
	NewtonBodySetMassProperties(newtonBody, mass, shape);

	NewtonBodySetCollisionScale(newtonBody, pBody->st.scale[0], pBody->st.scale[1], pBody->st.scale[2]);

	// bodies can have a destructor. 
	// this is a function callback that can be used to destroy any local data stored 
	// and that need to be destroyed before the body is destroyed. 
	NewtonBodySetDestructorCallback(newtonBody, Entity::destroyBodyCallback);

	// set the function callback to apply the external forces and torque to the body
	// the most common force is Gravity
	NewtonBodySetForceAndTorqueCallback(newtonBody, Entity::applyForceAndTorqueCallback);

	// set the function callback to set the transformation state of the graphic entity associated with this body 
	// each time the body change position and orientation in the physics world
	NewtonBodySetTransformCallback(newtonBody, Entity::transformCallback);

	// keep CC on because Julio told me to
	NewtonBodySetContinuousCollisionMode(newtonBody, 1);

	return newtonBody;
}

NewtonCollision * BodyOps::createBoxShape(PhysicsBodyRef & pBody)
{
	AxisAlignedBox modelBounds = pBody->st.modelBound;

	dVector minBox(&modelBounds.getMin()[0]);
	dVector maxBox(&modelBounds.getMax()[0]);

	dVector size(maxBox - minBox);
	dVector origin((maxBox + minBox).Scale(0.5f));
	size.m_w = 1.0f;
	origin.m_w = 1.0f;

	dMatrix offset(dGetIdentityMatrix());
	offset.m_posit = origin;

	return NewtonCreateBox(world, size.m_x, size.m_y, size.m_z, (int)pBody->bodyID, &offset[0][0]);
}

NewtonCollision * BodyOps::createBallShape(PhysicsBodyRef & pBody)
{
	AxisAlignedBox modelBounds = pBody->st.modelBound;

	dVector minBox(&modelBounds.getMin()[0]);
	dVector maxBox(&modelBounds.getMax()[0]);

	dVector radius(maxBox - minBox);
	radius[0] *= .5f;
	radius[1] *= .5f;
	radius[2] *= .5f;

	// in case it's not a perfect sphere, use the largest radius
	dFloat r = theMax(radius[0], radius[1], radius[2]);

	dVector origin((maxBox + minBox).Scale(0.5f));
	radius.m_w = 1.0f;
	origin.m_w = 1.0f;

	dMatrix offset(dGetIdentityMatrix());
	offset.m_posit = origin;

	return NewtonCreateSphere(world, r, (int)pBody->bodyID, &offset[0][0]);
}

NewtonCollision * BodyOps::createConvexHullShape(PhysicsBodyRef & pBody)
{
	AxisAlignedBox modelBounds = pBody->st.modelBound;

	dVector minBox(&modelBounds.getMin()[0]);
	dVector maxBox(&modelBounds.getMax()[0]);

	dVector size(maxBox - minBox);
	dVector origin((maxBox + minBox).Scale(0.5f));
	size.m_w = 1.0f;
	origin.m_w = 1.0f;

	dMatrix offset(dGetIdentityMatrix());
	offset.m_posit = origin;

	dFloat tolerance = 0.0f;
	int pointCount = (int)pBody->triMesh->getNumVertices();

	return NewtonCreateConvexHull(world, pointCount, &pBody->triMesh->getBufferPositions()[0], 3 * sizeof(dFloat), tolerance, (int)pBody->bodyID, &offset[0][0]);
}

NewtonCollision * BodyOps::createStaticMeshShape(PhysicsBodyRef & pBody)
{
	size_t triCount = pBody->triMesh->getNumTriangles();

	// create an empty collision tree
	NewtonCollision * const collision = NewtonCreateTreeCollision(world, 0);

	// start adding faces to the collision tree 
	NewtonTreeCollisionBeginBuild(collision);

	for (int i = 0; i < triCount; i++)
	{
		vec3 p0, p1, p2;
		pBody->triMesh->getTriangleVertices(i, &p0, &p1, &p2);

		dVector face[3];
		face[0] = dVector(p0[0], p0[1], p0[2]);
		face[1] = dVector(p1[0], p1[1], p1[2]);
		face[2] = dVector(p2[0], p2[1], p2[2]);

		NewtonTreeCollisionAddFace(collision, 3, &face[0].m_x, sizeof(dVector), i);
	}

	// end adding faces to the collision tree, also optimize the mesh for best performance
	#ifdef NDEBUG
		NewtonTreeCollisionEndBuild(collision, 1);
	#else
		NewtonTreeCollisionEndBuild(collision, 0);
	#endif

	return collision;
}

NewtonCollision * BodyOps::createCompositeShape(PhysicsBodyRef & pBody)
{
	NewtonMesh * const mesh = triMeshToNewtonMesh(pBody->triMesh);
	if (!mesh) return nullptr;

	// create a convex approximation from the original mesh, 256 convex max and no more than 100 vertex convex hulls
	NewtonMesh* const convexApproximation = NewtonMeshApproximateConvexDecomposition(mesh, 0.01f, 0.2f, 256, 100, nullptr, nullptr);
	if (!convexApproximation) return nullptr;

	NewtonMeshDestroy(mesh);

	// create a compound collision by creation a convex hull of each segment of the source mesh 
	NewtonCollision* const compound = NewtonCreateCompoundCollisionFromMesh(world, convexApproximation, 0.001f, 0, 0);

	NewtonMeshDestroy(convexApproximation);

	return compound;
}

NewtonMesh * BodyOps::triMeshToNewtonMesh(const ci::TriMeshRef & triMesh)
{
	size_t triCount = triMesh->getNumTriangles();
	size_t pointCount= triMesh->getNumVertices();

	NewtonMesh * const newtonMesh = NewtonMeshCreate(world);
	if (!newtonMesh) return nullptr;

	const vec3 * const positions = triMesh->getPositions<3>();

	// newton dVector needs 4 components with last set to 0
	float * const vertices = new float[pointCount * 4];
	float * verts = vertices;
	for (int i = 0; i < pointCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			*verts++ = positions[i][j];
		}
			
		*verts++ = 0;
	}
	

	// the number of vetices of each face 
	int * faceIndexCount = new int[triCount];
	for (int i = 0; i < triCount; i++)
		faceIndexCount[i] = 3;

	// a user id that can be use to assign a material to the face 
	int * faceMaterialIndex = new int[triCount];
	for (int i = 0; i < triCount; i++)
		faceMaterialIndex[i] = 0;

	// we can just use a single dummy value here if
	// we don't have the real normal or uv values
	const float dummyNormal[] = { 0, 1, 0 };
	const float dummyV0[] = { 0, 0 };
	const float dummyV1[] = { 0, 0 };

	size_t indexCount = triMesh->getNumIndices();
	std::vector<uint32_t> & indexes = triMesh->getIndices();
	int * const indices = new int[indexCount];
	std::memcpy(indices, &indexes[0], indexCount * sizeof(int));

	int * normalIndex = new int[indexCount];
	for (int i = 0; i < indexCount; i++)
		normalIndex[i] = 0;

	int * uv0Index = new int[indexCount];
	for (int i = 0; i < indexCount; i++)
		uv0Index[i] = 0;

	int * uv1Index = new int[indexCount];
	for (int i = 0; i < indexCount; i++)
		uv1Index[i] = 0;

	NewtonMeshBuildFromVertexListIndexList(newtonMesh, triCount, faceIndexCount, faceMaterialIndex,
											vertices, 4 * sizeof(float), indices,
											dummyNormal, 3 * sizeof(float), normalIndex,
											dummyV0, 2 * sizeof(float), uv0Index,
											dummyV1, 2 * sizeof(float), uv1Index);

	delete[] vertices;
	delete[] faceIndexCount;
	delete[] faceMaterialIndex;
	delete[] normalIndex;
	delete[] uv0Index;
	delete[] uv1Index;

	return newtonMesh;;
}
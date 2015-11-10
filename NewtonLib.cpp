
#pragma once

#ifdef _MSC_VER 
#ifdef _M_X64
#ifndef _WIN_64_VER
#define _WIN_64_VER
#endif
#else 		
#ifndef _WIN_32_VER
#define _WIN_32_VER
#endif
#endif

#if _MSC_VER >= 1400
#define HAVE_STRUCT_TIMESPEC
#endif
#endif


#ifdef _MSC_VER 
#if !(defined (_WIN_32_VER) || defined (_WIN_64_VER))
#error "most define _WIN_32_VER or _WIN_64_VER for a correct CPU target"
#endif
#endif

#if (defined (_WIN_32_VER) || defined (_WIN_64_VER))
#pragma warning (disable: 4100) //unreferenced formal parameter
#pragma warning (disable: 4201) //nonstandard extension used : nameless struct/union
#pragma warning (disable: 4324) //structure was padded due to __declspec(align())
#pragma warning (disable: 4530) // C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#if _MSC_VER >= 1400
#pragma warning (disable: 4577) // 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. Specify /EHsc
#pragma warning (disable: 4456) // declaration of 'key' hides previous local declaration
#pragma warning (disable: 4457) // declaration of 'body' hides function parameter
#endif

#include <io.h> 
#include <direct.h> 
#include <malloc.h>
#include <float.h>
#include <stdarg.h>
#include <process.h>

#ifdef _DEBUG
#pragma warning (disable: 4127)	//conditional expression is constant
#endif

#pragma warning (push, 3) 
#include <windows.h>
#include <crtdbg.h>
#ifndef _DURANGO
#include <tlhelp32.h>
#endif
#pragma warning (pop) 
#endif

#if (defined (_WIN_32_VER) || defined (_WIN_64_VER))
#define DG_SSE4_INSTRUCTIONS_SET
#include <intrin.h>
#endif

#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <memory>
#include <thread>
#include <atomic>   
#include <mutex>
#include <condition_variable>

// core
#include "NewtonModule/core/dgTypes.h"
#include "NewtonModule/core/dgMemory.h"
#include "NewtonModule/core/dgDebug.h"
#include "NewtonModule/core/dgFastQueue.h"
#include "NewtonModule/core/dgList.h"
#include "NewtonModule/core/dgStack.h"
#include "NewtonModule/core/dgArray.h"
#include "NewtonModule/core/dgHeap.h"
#include "NewtonModule/core/dgGraph.h"
#include "NewtonModule/core/dgVector.h"
#include "NewtonModule/core/dgPlane.h"
#include "NewtonModule/core/dgCRC.h"
#include "NewtonModule/core/dgRtti.h"
#include "NewtonModule/core/dgRef.h"
#include "NewtonModule/core/dgRefCounter.h"
#include "NewtonModule/core/dgTree.h"
#include "NewtonModule/core/dgRandom.h"
#include "NewtonModule/core/dgQuaternion.h"
#include "NewtonModule/core/dgMatrix.h"
#include "NewtonModule/core/dgGeneralVector.h"
#include "NewtonModule/core/dgGeneralMatrix.h"
#include "NewtonModule/core/dgSmallDeterminant.h"
#include "NewtonModule/core/dgGoogol.h"
#include "NewtonModule/core/dgNode.h"
#include "NewtonModule/core/dgThread.h"
#include "NewtonModule/core/dgMutexThread.h"
#include "NewtonModule/core/dgThreadHive.h"
#include "NewtonModule/core/dgAsyncThread.h"
#include "NewtonModule/core/dgSPDMatrix.h"
#include "NewtonModule/core/dgObb.h"
#include "NewtonModule/core/dgConvexHull3d.h"
#include "NewtonModule/core/dgConvexHull4d.h"
#include "NewtonModule/core/dgIntersections.h"
#include "NewtonModule/core/dgPolygonSoupDatabase.h"
#include "NewtonModule/core/dgPolyhedra.h"
#include "NewtonModule/core/dgPolygonSoupBuilder.h"
#include "NewtonModule/core/dgPolyhedraMassProperties.h"
#include "NewtonModule/core/dgDelaunayTetrahedralization.h"
#include "NewtonModule/core/dgAABBPolygonSoup.h"
#include "NewtonModule/core/dgPathFinder.h"

// dMath
#include "NewtonModule/dMath/dStdAfxMath.h"
#include "NewtonModule/dMath/dMathDefines.h"
#include "NewtonModule/dMath/dVector.h"
#include "NewtonModule/dMath/dMatrix.h"
#include "NewtonModule/dMath/dQuaternion.h"
#include "NewtonModule/dMath/dLinearAlgebra.h"

// mesh
#include "NewtonModule/meshUtil/dgMeshEffect.h"

// core
#include "NewtonModule/core/dgtypes.cpp"
#include "NewtonModule/core/dgMemory.cpp"
#include "NewtonModule/core/dgTree.cpp"
#include "NewtonModule/core/dgRef.cpp"
#include "NewtonModule/core/dgRefCounter.cpp"
#include "NewtonModule/core/dgRandom.cpp"
#include "NewtonModule/core/dgDebug.cpp"
#include "NewtonModule/core/dgCRC.cpp"
#include "NewtonModule/core/dgMatrix.cpp"
#include "NewtonModule/core/dgQuaternion.cpp"
#include "NewtonModule/core/dgGeneralMatrix.cpp"
#include "NewtonModule/core/dgGeneralVector.cpp"
#include "NewtonModule/core/dgSmallDeterminant.cpp"
#include "NewtonModule/core/dgGoogol.cpp"
#include "NewtonModule/core/dgNode.cpp"
#include "NewtonModule/core/dgThread.cpp"
#include "NewtonModule/core/dgAsyncThread.cpp"
#include "NewtonModule/core/dgMutexThread.cpp"
#include "NewtonModule/core/dgThreadHive.cpp"
#include "NewtonModule/core/dgSPDMatrix.cpp"
#include "NewtonModule/core/dgObb.cpp"
#include "NewtonModule/core/dgConvexHull3d.cpp"
#include "NewtonModule/core/dgConvexHull4d.cpp"
#include "NewtonModule/core/dgIntersections.cpp"
#include "NewtonModule/core/dgPolygonSoupBuilder.cpp"
#include "NewtonModule/core/dgPolyhedra.cpp"
#include "NewtonModule/core/dgPolyhedraMassProperties.cpp"
#include "NewtonModule/core/dgDelaunayTetrahedralization.cpp"
#include "NewtonModule/core/dgAABBPolygonSoup.cpp"

// dMath
#include "NewtonModule/dMath/dLinearAlgebra.cpp"
#include "NewtonModule/dMath/dMathDefines.cpp"
#include "NewtonModule/dMath/dMatrix.cpp"
#include "NewtonModule/dMath/dQuaternion.cpp"
#include "NewtonModule/dMath/dStdAfxMath.cpp"
#include "NewtonModule/dMath/dVector.cpp"

// physics
#include "NewtonModule/physics/dgBallConstraint.cpp"
#include "NewtonModule/physics/dgBilateralConstraint.cpp"
#include "NewtonModule/physics/dgBodyMasterList.cpp"
#include "NewtonModule/physics/dgCollision.cpp"
#include "NewtonModule/physics/dgCollisionCompound.cpp"
#include "NewtonModule/physics/dgCollisionBox.cpp"
#include "NewtonModule/physics/dgCollisionBVH.cpp"
#include "NewtonModule/physics/dgCollisionCapsule.cpp"
#include "NewtonModule/physics/dgCollisionChamferCylinder.cpp"
#include "NewtonModule/physics/dgCollisionCone.cpp"
#include "NewtonModule/physics/dgCollisionConvex.cpp"
#include "NewtonModule/physics/dgCollisionConvexHull.cpp"
#include "NewtonModule/physics/dgCollisionConvexPolygon.cpp"
#include "NewtonModule/physics/dgCollisionCylinder.cpp"
#include "NewtonModule/physics/dgCollisionDeformableMesh.cpp"
#include "NewtonModule/physics/dgCollisionHeightField.cpp"
#include "NewtonModule/physics/dgCollisionMesh.cpp"
#include "NewtonModule/physics/dgCollisionNull.cpp"
#include "NewtonModule/physics/dgCollisionScene.cpp"
#include "NewtonModule/physics/dgCollisionSphere.cpp"
#include "NewtonModule/physics/dgCollisionUserMesh.cpp"
#include "NewtonModule/physics/dgConstraint.cpp"
#include "NewtonModule/physics/dgContact.cpp"
#include "NewtonModule/physics/dgCorkscrewConstraint.cpp"
#include "NewtonModule/physics/dgDeformableContact.cpp"
#include "NewtonModule/physics/dgHingeConstraint.cpp"
#include "NewtonModule/physics/dgNarrowPhaseCollision.cpp"
#include "NewtonModule/physics/dgSlidingConstraint.cpp"
#include "NewtonModule/physics/dgUniversalConstraint.cpp"
#include "NewtonModule/physics/dgUpVectorConstraint.cpp"
#include "NewtonModule/physics/dgUserConstraint.cpp"
#include "NewtonModule/physics/dgWorldDynamicsParallelSolver.cpp"
#include "NewtonModule/physics/dgWorldDynamicsSimpleSolver.cpp"
#include "NewtonModule/physics/dgWorldDynamicUpdate.cpp"
#include "NewtonModule/physics/dgWorld.cpp"
#include "NewtonModule/physics/dgCollisionInstance.cpp"
#include "NewtonModule/physics/dgCollisionTaperedCylinder.cpp"
#include "NewtonModule/physics/dgCollisionTaperedCapsule.cpp"
#include "NewtonModule/physics/dgBody.cpp"
#include "NewtonModule/physics/dgDeformableBody.cpp"
#include "NewtonModule/physics/dgDynamicBody.cpp"
#include "NewtonModule/physics/dgKinematicBody.cpp"
#include "NewtonModule/physics/dgBroadPhase.cpp"
#include "NewtonModule/physics/dgCollisionDeformableClothPatch.cpp"
#include "NewtonModule/physics/dgCollisionDeformableSolidMesh.cpp"
#include "NewtonModule/physics/dgDeformableBodiesUpdate.cpp"
#include "NewtonModule/physics/dgCollisionCompoundFractured.cpp"
#include "NewtonModule/physics/dgBroadPhaseDefault.cpp"
#include "NewtonModule/physics/dgBroadPhasePersistent.cpp"
#include "NewtonModule/physics/dgSkeletonContainer.cpp"
#include "NewtonModule/physics/dgBroadPhaseAggregate.cpp"

// mesh
#include "NewtonModule/meshUtil/dgMeshEffect1.cpp"
#include "NewtonModule/meshUtil/dgMeshEffect2.cpp"
#include "NewtonModule/meshUtil/dgMeshEffect3.cpp"
#include "NewtonModule/meshUtil/dgMeshEffect4.cpp"
#include "NewtonModule/meshUtil/dgMeshEffect5.cpp"
#include "NewtonModule/meshUtil/dgMeshEffect6.cpp"




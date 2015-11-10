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

// mesh
#include "NewtonModule/meshUtil/dgMeshEffect.h"

// physics
#include "NewtonModule/physics/dgBody.h"
#include "NewtonModule/physics/dgWorld.h"
#include "NewtonModule/physics/dgContact.h"
#include "NewtonModule/physics/dgCollision.h"
#include "NewtonModule/physics/dgConstraint.h"
#include "NewtonModule/physics/dgDynamicBody.h"
#include "NewtonModule/physics/dgCollisionBVH.h"
#include "NewtonModule/physics/dgKinematicBody.h"
#include "NewtonModule/physics/dgCollisionMesh.h"
#include "NewtonModule/physics/dgCollisionScene.h"
#include "NewtonModule/physics/dgBodyMasterList.h"
#include "NewtonModule/physics/dgUserConstraint.h"
#include "NewtonModule/physics/dgBallConstraint.h"
#include "NewtonModule/physics/dgHingeConstraint.h"
#include "NewtonModule/physics/dgSkeletonContainer.h"
#include "NewtonModule/physics/dgCollisionInstance.h"
#include "NewtonModule/physics/dgCollisionUserMesh.h"
#include "NewtonModule/physics/dgSlidingConstraint.h"
#include "NewtonModule/physics/dgUpVectorConstraint.h"
#include "NewtonModule/physics/dgCollisionConvexHull.h"
#include "NewtonModule/physics/dgUniversalConstraint.h"
#include "NewtonModule/physics/dgCorkscrewConstraint.h"
#include "NewtonModule/physics/dgBroadPhaseAggregate.h"
#include "NewtonModule/physics/dgCollisionHeightField.h"
#include "NewtonModule/physics/dgCollisionConvexPolygon.h"
#include "NewtonModule/physics/dgCollisionDeformableMesh.h"
#include "NewtonModule/physics/dgCollisionCompoundFractured.h"

// dMath
#include "NewtonModule/dMath/dStdAfxMath.h"
#include "NewtonModule/dMath/dMathDefines.h"
#include "NewtonModule/dMath/dVector.h"
#include "NewtonModule/dMath/dMatrix.h"
#include "NewtonModule/dMath/dQuaternion.h"
#include "NewtonModule/dMath/dLinearAlgebra.h"

// newton
#include "NewtonModule/newton/Newton.h"
#include "NewtonModule/newton/NewtonClass.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../../../NewtonAPI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

int64_t totalMemoryUsed = 0;

void * physicsAlloc(int sizeInBytes)
{
	totalMemoryUsed += sizeInBytes;
	return new char[sizeInBytes];
}

void physicsFree(void * ptr, int sizeInBytes)
{
	totalMemoryUsed -= sizeInBytes;
	delete[](char*)ptr;
}

class HelloNewtonApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void HelloNewtonApp::setup()
{
	NewtonSetMemorySystem(physicsAlloc, physicsFree);

	NewtonWorld * const world = NewtonCreate();
	CI_ASSERT(world);

	NewtonWorldSetUserData(world, this);
	NewtonSetSolverModel(world, 1);
	NewtonSetFrictionModel(world, 0);
	NewtonInvalidateCache(world);
	NewtonSetThreadsCount(world, std::thread::hardware_concurrency());

	NewtonDestroy(world);

	CI_ASSERT(totalMemoryUsed == 0);
}

void HelloNewtonApp::mouseDown( MouseEvent event )
{
}

void HelloNewtonApp::update()
{
}

void HelloNewtonApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( HelloNewtonApp, RendererGl )

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Ray.h"

#include "Standard.h"
#include "../../framework/physics/PhysicsHandler.h"
#include "../../framework/render/Shadower.h"
#include "View.h"

class TeapotsApp : public App
{

public:
	TeapotsApp();
	~TeapotsApp();

	void setup() override;
	void resize() override;
	void update() override;
	void draw() override;

	void mouseMove(ci::app::MouseEvent event) override;
	void mouseDown(ci::app::MouseEvent event) override;
	void mouseDrag(ci::app::MouseEvent event) override;
	void mouseUp(ci::app::MouseEvent event) override;

	void keyDown(KeyEvent event) override;
	void keyUp(KeyEvent event) override;

 private:
	PhysicsHandler physics;
	ViewRef gui;
	double prevt = 0;
	double cpuTime = 0;
	bool usingGui = false;
	Shadower shadower;
	Rand rand;
	bool ok = true;

	CameraPersp	camera;
	CameraUi cameraUI;
	
	ivec2 mousePos;
	Color bgColor = Color(0.1f, 0.11f, 0.12f);
	Color potColors[3];

	gl::BatchRef teapot, teapotShadowed;
	gl::BatchRef ground, groundShadowed;
	ci::TriMeshRef teapotMesh;

	void render(const gl::GlslProgRef & glsl = nullptr);
	void cleanup();
	void drawGrid(float size = 100.0f, float step = 10.0f);
	void createBodies();
	void emitBody(const Ray & mouseRay);

};

CINDER_APP(TeapotsApp, RendererGl(),
	[&](App::Settings *settings)
{
	settings->setWindowSize((int)appWidth, (int)appHeight);
	settings->setFrameRate(60.0f);
	settings->setTitle(getTitle());
	settings->setHighDensityDisplayEnabled();
})

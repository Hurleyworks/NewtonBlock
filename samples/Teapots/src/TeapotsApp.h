#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"

#include "Standard.h"

using namespace ci;
using namespace ci::app;
using namespace std;

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
	CameraPersp	camera;
	CameraUi cameraUI;
	
	ivec2 mousePos;
	Color bgColor = Color(0.1f, 0.11f, 0.12f);

	void render();
	void cleanup();

};

CINDER_APP(TeapotsApp, RendererGl(),
	[&](App::Settings *settings)
{
	settings->setWindowSize((int)appWidth, (int)appHeight);
	settings->setFrameRate(60.0f);
	settings->setTitle(getTitle());
	settings->setHighDensityDisplayEnabled();
})

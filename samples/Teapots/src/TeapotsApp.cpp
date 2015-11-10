
#include "TeapotsApp.h"

TeapotsApp::TeapotsApp()
{

}

TeapotsApp::~TeapotsApp()
{
	
}

void TeapotsApp::setup()
{

}

void TeapotsApp::update()
{

}

void TeapotsApp::cleanup()
{
	
}

void TeapotsApp::draw()
{

	render();
}

void TeapotsApp::render()
{
	gl::clear(bgColor);

	// specify the camera matrices
	gl::pushMatrices();
	gl::setMatrices(camera);

	// specify render states
	gl::ScopedDepth depth(true, true);
	
}

void TeapotsApp::mouseMove(ci::app::MouseEvent event)
{
	mousePos = event.getPos();
}

void TeapotsApp::mouseDown(ci::app::MouseEvent event)
{
	cameraUI.mouseDown(event.getPos());
}

void TeapotsApp::mouseDrag(ci::app::MouseEvent event)
{
	mousePos = event.getPos();
	cameraUI.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());
}

void TeapotsApp::mouseUp(ci::app::MouseEvent event)
{
}

void TeapotsApp::keyDown(KeyEvent event)
{
	switch (event.getCode())
	{
		case KeyEvent::KEY_ESCAPE:
			quit();
			break;

		case KeyEvent::KEY_f:
			setFullScreen(!isFullScreen());
			break;
	}
}

void TeapotsApp::keyUp(KeyEvent event)
{
}

void TeapotsApp::resize()
{
	camera.setPerspective(60.0f, getWindowAspectRatio(), 0.1f, 1000.0f);
	cameraUI.setCamera(&camera);
}


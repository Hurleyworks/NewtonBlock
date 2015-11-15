
#include "TeapotsApp.h"
#include "../../framework/physics/util/MeshOps.h"

using namespace ci;
using namespace ci::app;
using namespace std;

TeapotsApp::TeapotsApp()
{
	float b = .8f;
	potColors[0] = ci::Color(0.4f, 0.2f, b);
	potColors[1] = ci::Color(0.1f, 0.4f, b);
	potColors[2] = ci::Color(0.1f, 0.2f, b);
}

TeapotsApp::~TeapotsApp()
{
	
}

void TeapotsApp::setup()
{
	gl::enableVerticalSync(false);

	Rand::randomize();

	try
	{
		physics.init();
		shadower.setup();
		gui = std::make_shared<View>();
		gui->create(getWindow(), &physics);

	}
	catch (std::exception & e)
	{
		gui->postWarningMessage("Fatal Error", "Setup failed");
		ok = false;
		return;
	}

	createBodies();
	camera.lookAt(vec3(3, 10, 18), vec3(0, 0, 0));

	gl::enableDepthRead();
	gl::enableDepthWrite();

	gui->postInfoMessage("Instructions", "Use RMB to fire teapot projectiles while simulation is running");
}

void TeapotsApp::update()
{
	if (!ok) return;

	if (physics.engine()->isEngineRunning())
		physics.engine()->advancePhysics();

	float e = (float)getElapsedSeconds();

	float x = shadower.light.distanceRadius * sin(0.25f * e);
	float z = shadower.light.distanceRadius * cos(0.25f * e);

	shadower.light.viewpoint.x = x;
	shadower.light.viewpoint.z = z;
	shadower.light.camera.lookAt(shadower.light.viewpoint, shadower.light.target);

	camera.lookAt(vec3(x / 6.0f, 10, z / 6.0f), vec3(0, 0, 0));
}

void TeapotsApp::cleanup()
{
	
}

void TeapotsApp::draw()
{

	if (!ok) return;

	// performance data
	double t, dt;
	t = getElapsedSeconds();
	dt = t - prevt;
	prevt = t;

	gl::clear(bgColor);

	// Offset to help combat surface acne (self-shadowing)
	gl::enable(GL_POLYGON_OFFSET_FILL);

	glPolygonOffset(shadower.polygonOffsetFactor, shadower.polygonOffsetUnits);

	// Render scene into shadow map
	gl::setMatrices(shadower.light.camera);
	gl::viewport(shadower.shadowMap->getSize());
	{
		gl::ScopedFramebuffer bindFbo(shadower.shadowMap->getFbo());
		gl::clear();
		render();
	}

	// Render shadowed scene
	gl::setMatrices(shadower.light.toggleViewpoint ? shadower.light.camera : camera);
	gl::viewport(toPixels(getWindowSize()));
	{
		gl::ScopedGlslProg bind(shadower.shader);
		gl::ScopedTextureBind texture(shadower.shadowMap->getTexture());
		shadower.shader->uniform("uShadowMap", 0);
		shadower.shader->uniform("uShadowMatrix", shadower.light.camera.getProjectionMatrix() * shadower.light.camera.getViewMatrix());
		shadower.shader->uniform("uShadowTechnique", shadower.shadowTechnique);
		shadower.shader->uniform("uDepthBias", shadower.depthBias);
		shadower.shader->uniform("uOnlyShadowmap", shadower.onlyShadowMap);
		shadower.shader->uniform("uRandomOffset", shadower.randomOffset);
		shadower.shader->uniform("uNumRandomSamples", shadower.numRandomSamples);
		shadower.shader->uniform("uEnableNormSlopeOffset", shadower.enableNormSlopeOffset);
		shadower.shader->uniform("uLightPos", vec3(gl::getModelView() * vec4(shadower.light.viewpoint, 1.0)));
		render(shadower.shader);
	}
	gl::disable(GL_POLYGON_OFFSET_FILL);

	drawGrid();

	gui->draw(getElapsedSeconds());

	// Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
	cpuTime = getElapsedSeconds() - t;
	gui->updatePerfGraph((float)dt, (float)cpuTime);
}

void TeapotsApp::render(const gl::GlslProgRef & glsl)
{
	for (auto & pBody : physics.scene()->getBodies())
	{
		if (!pBody) continue;
		if (!pBody->state.isReady()) continue;

		gl::ScopedModelMatrix scpModelMatrix;
		gl::setModelMatrix(pBody->st.worldTransform);
		gl::scale(pBody->st.scale);

		if (pBody->desc.name == "Ground")
		{
			gl::ScopedColor bluish(ci::Color(0.1f, 0.2f, 0.3f));
			if (glsl)
			{
				groundShadowed->draw();
			}
			else
				ground->draw();
		}
		else
		{
			gl::ScopedColor blue(pBody->desc.color);
			if (glsl)
			{
				teapotShadowed->draw();
			}
			else
				teapot->draw();
		}
	}
}

void TeapotsApp::drawGrid(float size, float step)
{
	gl::color(Colorf(0.2f, 0.2f, 0.2f));
	for (float i = -size; i <= size; i += step)
	{
		gl::drawLine(vec3(i, 0.0f, -size), vec3(i, 0.0f, size));
		gl::drawLine(vec3(-size, 0.0f, i), vec3(size, 0.0f, i));
	}
}

void TeapotsApp::createBodies()
{
	auto positionGlsl = gl::getStockShader(gl::ShaderDef());

	// make an oversized static pot
	auto staticPot = ci::TriMesh::create(geom::Teapot().subdivisions(8));
	ground = gl::Batch::create(*staticPot, positionGlsl);
	groundShadowed = gl::Batch::create(*staticPot, shadower.shader);

	BodyDesc d;
	d.bodyType = BodyType::Static;
	d.shape = CollisionShape::Mesh;
	d.mass = STATIC_MASS;
	d.name = "Ground";

	SpaceTime st;
	st.modelBound = staticPot->calcBoundingBox();
	st.scale = vec3(14);
	st.worldTransform = glm::translate(vec3(0.0f, -10.0f, 0.0f));
	st.startTransform = st.worldTransform;
	physics.addBody(staticPot, d, st);

	// create a dynamic teapot
	teapotMesh = ci::TriMesh::create(geom::Teapot().subdivisions(6));

	// cinder doesn't create centered teapots so
	// center it's vertices around the origin *before*
	// creating the batch
	MeshOps ops;
	ops.centerVertices(teapotMesh);

	teapot = gl::Batch::create(*teapotMesh, positionGlsl);
	teapotShadowed = gl::Batch::create(*teapotMesh, shadower.shader);

	int bodyCount = 30;
#ifdef NDEBUG
	bodyCount *= 20;
#endif

	float offset = 6.0f;
	for (int i = 0; i < bodyCount; i++)
	{
		BodyDesc d;
		d.bodyType = BodyType::Dynamic;
		d.shape = CollisionShape::ConvexHull;
		d.name = "Teapot";
		d.mass = 10.0f;
		d.force = vec3(0.0f, -80.0f, 0.0f);
		d.color = potColors[rand.nextInt(3)];

		SpaceTime st;
		st.scale = vec3(rand.nextFloat(.5, 1.5));
		st.modelBound = teapotMesh->calcBoundingBox();
		st.worldTransform = glm::translate(vec3(0.0f, offset, 0.0f));
		st.startTransform = st.worldTransform;
		st.worldBound = teapotMesh->calcBoundingBox(st.worldTransform);

		physics.addBody(teapotMesh, d, st);

		offset += 3.0f;
	}
}

void TeapotsApp::emitBody(const Ray & mouseRay)
{
	if (!teapotMesh || !ok) return;

	BodyDesc d;
	d.bodyType = BodyType::Dynamic;
	d.shape = CollisionShape::ConvexHull;
	d.name = DEFAULT_PROJECTILE_NAME;
	d.mass = 40.0f;
	d.force = vec3(rand.nextFloat(5.0f, 15.0f));  // anti gravity teapot bullets
	d.color = ci::Color(1.0f, 0.3f, 0.0f);

	SpaceTime st;
	st.scale = vec3(rand.nextFloat(.25f, 1.0f));
	st.modelBound = teapotMesh->calcBoundingBox();
	st.worldTransform = glm::translate(mouseRay.getOrigin());
	st.startTransform = st.worldTransform;
	st.worldBound = teapotMesh->calcBoundingBox(st.worldTransform);

	// impulse settings
	st.speed = 100.0f;
	st.direction = glm::normalize(mouseRay.getDirection());

	PhysicsBodyRef pBody = physics.addBody(teapotMesh, d, st);

	// remember to set the impulse flag
	pBody->state.getState() |= PBodyState::HasImpulseApplied;
}

void TeapotsApp::mouseMove(ci::app::MouseEvent event)
{
	mousePos = event.getPos();
	gui->mouseMove(event);
}

void TeapotsApp::mouseDown(ci::app::MouseEvent event)
{
	mousePos = event.getPos();
	usingGui = gui->mouseDown(event);

	// don't update cameraUI if we're using the gui
	if (!usingGui)
		cameraUI.mouseDown(event.getPos());

	// fire a new torus into the scene on RMB down
	if (event.isRightDown() && physics.engine()->isEngineRunning())
	{
		Ray ray = camera.generateRay(mousePos, getWindowSize());
		emitBody(ray);
	}
}

void TeapotsApp::mouseDrag(ci::app::MouseEvent event)
{
	// don't update cameraUI if we're using the gui
	if (!usingGui)
		cameraUI.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());

	gui->mouseDrag(event);;
}

void TeapotsApp::mouseUp(ci::app::MouseEvent event)
{
	gui->mouseUp(event);
	usingGui = false;
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
	gui->resize(getWindowSize());
}


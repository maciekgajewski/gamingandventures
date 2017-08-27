#include <ecs/ecs.hh>

#include <irrlicht/irrlicht.h>

#include <boost/optional.hpp>

#include <iostream>
#include <random>


namespace Components
{

// Something that is rendered
struct Rendered
{
	irr::scene::ISceneNode* node_;
};

// Animated bouncing ball
struct Ball
{
	float radius_;
	irr::core::vector3df velocity_;
};


}

void createBalls(int count, irr::scene::ISceneManager* smgr, Ecs::Ecs& ecs, irr::video::ITexture* texture)
{
	std::minstd_rand rd;
	std::uniform_real_distribution<float> heightDist(8.0f, 50.0f);
	std::uniform_real_distribution<float> posDist(-1000.0f, 1000.0f);
	std::uniform_real_distribution<float> sizeDist(2.0f, 10.0f);
	for (int i = 0; i < count; i++)
	{
		Ecs::EntityId id = ecs.CreateEntity("Ball");

		Components::Ball& ball = ecs.AddComponentToEntity<Components::Ball>(id);
		ball.radius_ = sizeDist(rd);

		Components::Rendered& r = ecs.AddComponentToEntity<Components::Rendered>(id);
		r.node_ = smgr->addSphereSceneNode(ball.radius_);
		r.node_->setMaterialTexture(0, texture);
		r.node_->setPosition({posDist(rd), heightDist(rd) + ball.radius_, posDist(rd)});
		r.node_->getMaterial(0).DiffuseColor.set(255, 255, 0, 0);
		//r.node_->getMaterial(0).AmbientColor.set(255, 20, 20, 20);
	}
}

void animateBalls(Ecs::Ecs& ecs, double dt, irr::scene::ITriangleSelector* tsel)
{
	ecs.ForEach<Components::Ball, Components::Rendered>([&](Ecs::EntityId id, Components::Ball& ball, Components::Rendered& ren)
	{
		ball.velocity_ += irr::core::vector3df{0.0f, -9.8f, 0.0f} * dt;

		irr::core::vector3df pos = ren.node_->getPosition();
		irr::core::vector3df newPos = pos + ball.velocity_ * dt;

		// bounce
		/* naive implementation with ground always at 0
		if (newPos.Y < 0)
		{
			newPos.Y = -newPos.Y;
			ball.velocity_.Y *= -1;
		}
		*/

		// see if tghe ball may intersect the ground
		irr::core::vector3df boxDiag(ball.radius_, ball.radius_, ball.radius_);
		irr::core::aabbox3df collisionBox(pos - boxDiag, pos + boxDiag);

		static const int TRIANGLES = 32;
		irr::core::triangle3df triangles[TRIANGLES];
		int collided = 0;
		tsel->getTriangles(triangles, TRIANGLES, collided, collisionBox);

		// temporary:
		if (collided > 0)
		{
			newPos.Y = -newPos.Y;
			ball.velocity_.Y *= -1;
			std::cout << "ball " << id << " collided with " << collided << " triangles" << std::endl;
		}

		ren.node_->setPosition(newPos);
	});
}

template<typename T>
std::ostream& operator<<(std::ostream& s, const irr::core::vector3d<T>& v)
{
	return s << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
}

struct EventReceiver : public irr::IEventReceiver
{
	bool OnEvent(const irr::SEvent& event) override
	{
		// mouse
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED)
			{
				if (prevX_ && *prevX_ != event.MouseInput.X && camera_)
				{
					static const double SENSITIVITY = 1.0;

					irr::core::vector3df rotation = camera_->getRotation();
					rotation.Y += SENSITIVITY * event.MouseInput.X - *prevX_;
					camera_->setRotation(rotation);
				}
				prevX_ = event.MouseInput.X;
				return true;
			}
		}
		else if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			if (event.KeyInput.Key == irr::KEY_KEY_W)
			{
				if (event.KeyInput.PressedDown)
					camVeclocity_ = 1.0;
				else
					camVeclocity_ = 0.0;
				return true;
			}
			if (event.KeyInput.Key == irr::KEY_KEY_S)
			{
				if (event.KeyInput.PressedDown)
					camVeclocity_ = -1.0;
				else
					camVeclocity_ = 0.0;
				return true;
			}
			if (event.KeyInput.Key == irr::KEY_ESCAPE)
			{
				quit_ = true;
				return true;
			}

		}
		// keyboard

		return false;
	}

	boost::optional<std::int32_t> prevX_;
	irr::scene::ICameraSceneNode* camera_ = nullptr;
	double camVeclocity_ = 0;
	bool quit_ = false;
};

void moveCam(irr::scene::ICameraSceneNode* cam, double velV, double dt)
{
	static const double SPEED = 10.0;

	irr::core::vector3df rotation = cam->getRotation();
	irr::core::vector3df velocity(0, 0, velV);
	velocity.rotateXZBy(-rotation.Y);
	irr::core::vector3df velocityMpl = velocity * dt * SPEED;

	irr::core::vector3df position = cam->getPosition();
	cam->setPosition(position + velocityMpl);
	cam->setTarget(cam->getTarget() + velocityMpl);
}

int main(int , char**)
{
	EventReceiver eventReceiver;

	// Init renderer
	irr::IrrlichtDevice *device = irr::createDevice(
		irr::video::EDT_OPENGL,
		irr::core::dimension2d<irr::u32>(1024, 768),
		16,
		false, // fullscreen
		false, // stencil buffer
		false, // vsync
		&eventReceiver); // even receiver

	device->setWindowCaption(L"Balls! (in space)");

	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::ITimer* timer = device->getTimer();

	// build world

	Ecs::Ecs ecs;

	ecs.RegisterComponentType<Components::Rendered>("Rendered");
	ecs.RegisterComponentType<Components::Ball>("Ball");

	// floor
	irr::scene::ITriangleSelector* groundTriangeSelector = nullptr;
	{
		Ecs::EntityId floorId = ecs.CreateEntity("Floor");
		Components::Rendered& r = ecs.AddComponentToEntity<Components::Rendered>(floorId);

		auto mesh = smgr->addHillPlaneMesh(
			"hills",
			{10, 10},	// mesh size
			{100, 100},	// tile count
			nullptr,	// material
			7.0,		// hill height
			{5, 5},		// count hills
			{100, 100}	// texture repeat
			);
		assert(mesh);
		r.node_ = smgr->addMeshSceneNode(mesh);

		//r.node_ = smgr->addSphereSceneNode();
		r.node_->setMaterialType(irr::video::EMT_SOLID);
		r.node_->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		r.node_->setMaterialTexture(0, driver->getTexture("ground.jpg"));
		//r.node_->setDebugDataVisible(irr::scene::EDS_NORMALS);


		groundTriangeSelector = smgr->createOctreeTriangleSelector(mesh, r.node_);
	}

	// balls
	static const int BALLS = 200;
	irr::video::ITexture* ballTexture = driver->getTexture("ball.jpg");
	createBalls(BALLS, smgr, ecs, ballTexture);

	// Lights, camera, action!
	irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0, irr::core::vector3df(0,5,-40), irr::core::vector3df(0,5,50));
	camera->bindTargetAndRotation(true);
	auto light = smgr->addLightSceneNode(0, irr::core::vector3df{100, 100, 100}, irr::video::SColor(1.0, 1.0, 1.0, 1.0), 1000.0);
	assert(light);
	light->setLightType(irr::video::ELT_POINT);
	auto& lightData = light->getLightData();
	lightData.Attenuation = {1.0f, 0.0, 0.0};
	lightData.DiffuseColor.set(1.0, 1.0, 1.0, 1.0);
	lightData.AmbientColor.set(1.0, 0.2, 0.2, 0.4);

	eventReceiver.camera_ = camera;

	std::uint32_t time = timer->getTime();
	while (device->run() && !eventReceiver.quit_)
	{
		driver->beginScene(
			true, // back buffer
			true // zBuffer
			,irr::video::SColor(255,100,101,140)
			);
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		std::uint32_t prevTime = time;
		time = timer->getTime();

		double dt = (time - prevTime)/1000.0;
		animateBalls(ecs, dt, groundTriangeSelector);
		moveCam(camera, eventReceiver.camVeclocity_, dt);
	}

	return 0;
}

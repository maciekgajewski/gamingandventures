#include <ecs/ecs.hh>

#include <irrlicht/irrlicht.h>

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

int main(int , char**)
{
	// Init renderer
	irr::IrrlichtDevice *device = irr::createDevice(
		irr::video::EDT_OPENGL,
		irr::core::dimension2d<irr::u32>(1024, 768),
		16,
		false, // fullscreen
		false, // stencil buffer
		false, // vsync
		0); // even receiver

	device->setWindowCaption(L"Balls! (in space)");

	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	irr::scene::ISceneManager* smgr = device->getSceneManager();

	// build world

	Ecs::Ecs ecs;

	ecs.RegisterComponentType<Components::Rendered>("Rendered");
	ecs.RegisterComponentType<Components::Ball>("Ball");

	// floor
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

	}

	// balls
	static const int BALLS = 100;
	irr::video::ITexture* ballTexture = driver->getTexture("ball.jpg");
	createBalls(BALLS, smgr, ecs, ballTexture);

	// Lights, camera, action!
	smgr->addCameraSceneNode(0, irr::core::vector3df(0,5,-40), irr::core::vector3df(0,5,50));
	auto light = smgr->addLightSceneNode(0, irr::core::vector3df{100, 100, 100}, irr::video::SColor(1.0, 1.0, 1.0, 1.0), 1000.0);
	assert(light);
	light->setLightType(irr::video::ELT_POINT);
	auto& lightData = light->getLightData();
	lightData.Attenuation = {1.0f, 0.0, 0.0};
	lightData.DiffuseColor.set(1.0, 1.0, 1.0, 1.0);
	lightData.AmbientColor.set(1.0, 0.2, 0.2, 0.4);

	while (device->run())
	{
		driver->beginScene(
			true, // back buffer
			true // zBuffer
			,irr::video::SColor(255,100,101,140)
			);
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}

	return 0;
}

#include "ecs.hh"

#include <irrlicht/irrlicht.h>

#include <iostream>


namespace Components
{

// Something that is rendered
struct Rendered
{
	irr::scene::ISceneNode* node_;
};


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

	irr::video::IVideoDriver* driver = device->getVideoDriver();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	irr::scene::ISceneManager* smgr = device->getSceneManager();

	// build world

	Ecs::Ecs ecs;

	ecs.RegisterComponentType<Components::Rendered>("Rendered");

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



		auto& mat = r.node_->getMaterial(0);
		mat.DiffuseColor.set(255, 20, 200, 20);
		mat.EmissiveColor.set(0, 0, 0, 0);
		mat.AmbientColor.set(0, 0, 0, 0);
	}

	smgr->addCameraSceneNode(0, irr::core::vector3df(0,5,-40), irr::core::vector3df(0,0,50));
	auto light = smgr->addLightSceneNode(0, irr::core::vector3df{100, 100, 100}, irr::video::SColor(1.0, 1.0, 1.0, 1.0), 1000.0);
	assert(light);
	light->setLightType(irr::video::ELT_POINT);
	auto& lightData = light->getLightData();
	lightData.Attenuation = {1.0f, 0.0, 0.0};
	lightData.DiffuseColor.set(1.0, 1.0, 1.0, 1.0);

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

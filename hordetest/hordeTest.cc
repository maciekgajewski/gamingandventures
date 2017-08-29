#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>



void setupWindow(int width, int height, bool fullscreen)
{
	// Create OpenGL window
	GLFWwindow* window = glfwCreateWindow(width, height, "Hello", nullptr, nullptr);
	if (!window)
		throw std::runtime_error("Failed to create window");

	glfwMakeContextCurrent(window);


}

int main(int, char**)
{
	std::cout << "hello" << std::endl;

	if (!glfwInit())
	{
		std::cout << "failed to initialize glfw" << std::endl;
		return -1;
	}


	try
	{
		// make window
		GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", nullptr, nullptr);
		if (!window)
		{
			throw std::runtime_error("Failed to create window");
		}

		glfwMakeContextCurrent(window);


		// init Horde
		if(!h3dInit())
		{
			throw std::runtime_error("Failed to initialize Horde");
		}

		// load resources
		H3DRes pipeline = h3dAddResource( H3DResTypes::Pipeline, "forward.pipeline.xml", 0 );
		H3DRes sphere = h3dAddResource( H3DResTypes::SceneGraph, "sphere.scene.xml", 0 );


		if (!h3dutLoadResourcesFromDisk("resources"))
		{
			throw std::runtime_error("failed to load resources");
		}

		// Camera
		H3DNode camNode = h3dAddCameraNode( H3DRootNode, "Camera", pipeline);
		h3dSetNodeTransform(camNode,
			0, 0, 0,
			0 ,0, 0,
			1, 1, 1 );

		// Sphere
		H3DNode sphereNode = h3dAddNodes( H3DRootNode, sphere);
//		h3dSetNodeTransform(sphereNode,
//			0, -20, 0, // t xyz
//			0, 0, 0, // r xyz
//			20, 20, 20 // s xyz
//			);

		// Add light source
		H3DNode light = h3dAddLightNode( H3DRootNode, "Light1", 0, "LIGHTING", "SHADOWMAP" );
		h3dSetNodeTransform( light, 0, 15, 10, -60, 0, 0, 1, 1, 1 );
		h3dSetNodeParamF( light, H3DLight::RadiusF, 0, 30 );
		h3dSetNodeParamF( light, H3DLight::FovF, 0, 90 );
		h3dSetNodeParamI( light, H3DLight::ShadowMapCountI, 1 );
		h3dSetNodeParamF( light, H3DLight::ShadowMapBiasF, 0, 0.01f );
		h3dSetNodeParamF( light, H3DLight::ColorF3, 0, 1.0f );
		h3dSetNodeParamF( light, H3DLight::ColorF3, 1, 0.8f );
		h3dSetNodeParamF( light, H3DLight::ColorF3, 2, 0.7f );
		h3dSetNodeParamF( light, H3DLight::ColorMultiplierF, 0, 1.0f );

		// Customize post processing effects
		/*
		H3DNode matRes = h3dFindResource( H3DResTypes::Material, "postHDR.material.xml" );
		h3dSetMaterialUniform( matRes, "hdrExposure", 2.5f, 0, 0, 0 );
		h3dSetMaterialUniform( matRes, "hdrBrightThres", 0.5f, 0, 0, 0 );
		h3dSetMaterialUniform( matRes, "hdrBrightOffset", 0.08f, 0, 0, 0 );
		*/

		//h3dSetOption( H3DOptions::DebugViewMode, 1.0f );
		//h3dSetOption( H3DOptions::WireframeMode, 1.0f );
		h3dSetNodeParamI( camNode, H3DCamera::ViewportXI, 0 );
		h3dSetNodeParamI( camNode, H3DCamera::ViewportYI, 0 );
		h3dSetNodeParamI( camNode, H3DCamera::ViewportWidthI, 800 );
		h3dSetNodeParamI( camNode, H3DCamera::ViewportHeightI, 600);

		// Set virtual camera parameters
		h3dSetupCameraView( camNode, 45.0f, (float)800.0 / 600.0, 0.1f, 1000.0f );
		h3dResizePipelineBuffers( pipeline, 800, 600 );

		/* Loop until the user closes the window */
		int i = 0;
		while (!glfwWindowShouldClose(window))
		{
			h3dSetNodeTransform(sphereNode,
				0, -20 + 0.01*i, -40, // t xyz
				0, 0, 0, // r xyz
				20, 20, 20 // s xyz
				);
				// render

			h3dRender(camNode);

			// Finish rendering of frame
			h3dFinalizeFrame();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
			i++;
		}

	}
	catch(...)
	{
		h3dutDumpMessages();
		glfwTerminate();
		throw;
	}

	h3dutDumpMessages();
	glfwTerminate();
}

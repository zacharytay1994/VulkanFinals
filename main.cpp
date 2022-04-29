#include "Internal/Window/wnd_helper.h"
#include "Internal/VulkanRenderer.h"

#include <stdexcept>
#include <iostream>

struct TempPushConstant
{
	glm::vec4 pos;
	glm::mat4 matrix;
};

int main ()
{
	bool validation { true };
	bool renderdoc { true };

	WND_HELPER::Window window;
	if ( !window.Initialize ( { 500 , 300 , false } ) )
	{
		throw std::runtime_error ( "Failed to initialize window!" );
	}
	std::cout << "### Window created successfully." << std::endl;

	// create the vulkan renderer with config
	VK_OBJECT::ConfigRef config = std::make_shared<VK_OBJECT::Config> ( validation , renderdoc );
	RENDERER::Vulkan vulkan_renderer ( window.GetHandle () , "Vulkan Finals" , config );

	// create a graphics pipeline with this push constant going to the vertex shader
	vulkan_renderer.CreateGraphicsPipeline<VK_OBJECT::Model::PushConstant> ( "default" , "Assets/Shaders/vert.spv" , "Assets/Shaders/frag.spv" );
	vulkan_renderer.BindGraphicsPipeline ( "default" );

	// bind model
	vulkan_renderer.AddModel ( "Cube" , VK_OBJECT::CubeVertices , VK_OBJECT::CubeIndices );
	vulkan_renderer.AddModel ( "DemonSkull" , "Assets/Models/demon-skull-textured/source/Skull_textured.fbx" );

	vulkan_renderer.AddImage ( "Diffuse" , "Assets/Textures/TD_Checker_Base_Color.png");
	vulkan_renderer.AddImage ( "Normal" , "Assets/Textures/TD_Checker_Normal_OpenGL.png" );

	float time { 0.0f };
	while ( !window.WindowShouldClose () )
	{
		window.PollEvents ();
		if ( window.WindowShouldClose () )
		{
			break;
		}
		vulkan_renderer.StartFrame ();

		// draw commands here
		// ...
		vulkan_renderer.Draw ( "DemonSkull" , 0.0f , 0.0f , 0.0f , 0.0f , time , 0.0f );
		//vulkan_renderer.Draw ( "Cube" , 2.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 2.0f );

		vulkan_renderer.EndFrame ();
		time += 0.1f;
	}
	vulkan_renderer.Wait ();
}
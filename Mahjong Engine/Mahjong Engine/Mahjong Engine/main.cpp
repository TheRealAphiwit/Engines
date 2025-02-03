#include "EditorGUI.h"
#include "DragonEngine.h"
#include "ResourceHandler.h"
#include "VirtualObject.h"
#include "DotGraphics.h"
#include "EntityHandler.h"
#include "MessageHandler.h"

#include <vector>
#include <iostream>
#include <glfw3.h>
#include <stdio.h>

int main()
{
	ResourceHandler& resources = ResourceHandler::GetInstance();
	MessageSystem::MessageHandler& messageHandler = MessageSystem::MessageHandler::GetInstance();
    DotsRendering::DotsInitData renderData = DotsRendering::Initialize(1200, 720);
	DotsRendering::EntityHandler& entityHandler = DotsRendering::EntityHandler::GetInstance();
	entityHandler.Initialize();
    messageHandler.ProcessQueue("Entity");
    Engine::DragonEngine* engine = new Engine::DragonEngine(renderData.window, renderData.camera);
    Characters::EditorGUI* Gui = new Characters::EditorGUI(renderData.window, &resources, renderData.camera);

    glfwSetInputMode(renderData.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // GLFW_CURSOR diff

    float lastTime = 0;
    float currentTime = 0;
    float delta = 0;

    std::vector<VirtualObject*> objects;

    while (!DotsRendering::ShouldClose())
    {
		objects = entityHandler.GetObjects();
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        DotsRendering::BeginRender(engine->myCamera);
        Gui->Render(objects);
        DotsRendering::End();

        engine->Update(delta);

        // 0 check first- currently it freezes
		// messageHandler.ProcessQueue("Entity");
    }

	glfwTerminate();
    return 0;
}
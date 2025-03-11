#include "EditorGUI.h"
#include "DragonEngine.h"
#include "DotGraphics.h"
#include "Winds_Physics.h"

#include "ResourceHandler.h"
#include "EntityHandler.h"
#include "MessageHandler.h"
#include "GameObjectHandler.h"
// #include "OpenGLFrameBuffer.h"

#include <glfw3.h>
#include <iostream>
#include <stdio.h>
#include "VirtualObject.h"
#include <vector>

int main()
{
	ResourceHandler& resources = ResourceHandler::GetInstance();
	MessageSystem::MessageHandler& messageHandler = MessageSystem::MessageHandler::GetInstance();
    DotsRendering::DotsInitData renderData = DotsRendering::Initialize(1200, 720);
	DotsRendering::EntityHandler& entityHandler = DotsRendering::EntityHandler::GetInstance();
    Engine::GameObjectHandler& gameObjectHandler = Engine::GameObjectHandler::GetInstance();

    entityHandler.Initialize();
    messageHandler.ProcessQueue("Entity");
    Engine::DragonEngine* engine = new Engine::DragonEngine(renderData.window, renderData.camera);
    Characters::EditorGUI* Gui = new Characters::EditorGUI(renderData.window, &resources, renderData.camera, engine);
    Winds::Winds_Physics* Physics = new Winds::Winds_Physics(engine);

    glfwSetInputMode(renderData.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // GLFW_CURSOR diff

    float lastTime = 0;
    float currentTime = 0;
    float delta = 0;

    std::vector<VirtualObject*> objects;
    std::vector<GameObject*> gameObjects;

    while (!DotsRendering::ShouldClose())
    {
		objects = entityHandler.GetObjects();
        gameObjects = gameObjectHandler.GetObjects();
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        if (engine->IsSimulating())
        {
            Physics->Simulate(delta);
        }

        DotsRendering::BeginRender(engine->myCamera);
        Gui->Render(objects, gameObjects);
        DotsRendering::End();

        engine->Update(delta); 

        // 0 check first- currently it freezes
		// messageHandler.ProcessQueue("Entity");
    }

	glfwTerminate();
    return 0;
}
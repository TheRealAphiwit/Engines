#include "GameObjectHandler.h"
#include "VirtualObject.h"
#include "DotGraphics.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Cube.h"
#include "Collisions.h"

using namespace DotsRendering;

void Engine::GameObjectHandler::ProcessMessages(MessageSystem::Message* aMessage)
{
}

std::future<GameObject*> Engine::GameObjectHandler::CreateGameObject(std::shared_ptr<std::string> name, Mesh* mesh, Texture* texture, Shader* shader, Winds::Collider* collider)
{
    return std::async(std::launch::async, [this, name, mesh, texture, shader, collider]()
        {
            // Create VirtualObject first
            std::shared_ptr<VirtualObject> virtualObject = std::make_shared<VirtualObject>(name, mesh, texture, shader);

            // Create GameObject
            GameObject* newGameObject = new GameObject(virtualObject.get(), collider);

            // Store in the handler
            {
                std::lock_guard<std::mutex> lock(myObjectsMutex);
                myObjects.push_back(newGameObject);
                myObjects_sPtr.push_back(virtualObject);
            }

            return newGameObject;
        });
}

std::future<void> Engine::GameObjectHandler::DeleteGameObject(GameObject* object)
{
    return std::async(std::launch::async, [this, object]()
        {
            if (!object) return;

            std::lock_guard<std::mutex> lock(myObjectsMutex);

            // Remove VirtualObject first
            myObjects_sPtr.erase(
                std::remove_if(myObjects_sPtr.begin(), myObjects_sPtr.end(),
                    [object](const std::shared_ptr<VirtualObject>& vo)
                    {
                        return vo.get() == object->GetVirtualObject();
                    }),
                myObjects_sPtr.end()
            );

            // Find and delete the GameObject
            auto it = std::find(myObjects.begin(), myObjects.end(), object);
            if (it != myObjects.end())
            {
                delete* it;
                myObjects.erase(it);
            }
        });
}

std::future<GameObject*> Engine::GameObjectHandler::CreateDefaultCube()
{
    return std::async
    (
        std::launch::async, [this]()
        {
            // Lock to ensure thread safety
            std::lock_guard<std::mutex> lock(myObjectsMutex);

            // Create the VirtualObject for the cube
            VirtualObject* vo = new VirtualObject(std::make_shared<std::string>("Cube"), myCube, myTexture, myShader);

            // Create the GameObject
            GameObject* newGameObject = new GameObject(vo, new Winds::BoxCollider(vo->Position, vo->GetExtents())); // This won't work - getExtents() not implemented

            // Store the new objects
            myObjects.push_back(newGameObject);

            return newGameObject;
        }
    );
}

std::vector<GameObject*> Engine::GameObjectHandler::GetObjects()
{
    std::lock_guard<std::mutex> lock(myObjectsMutex);
    return myObjects;
}

Engine::GameObjectHandler::GameObjectHandler()
{
}

Engine::GameObjectHandler::~GameObjectHandler()
{
    std::lock_guard<std::mutex> lock(myObjectsMutex);
    for (GameObject* obj : myObjects)
    {
        delete obj;
    }
    myObjects.clear();
    myObjects_sPtr.clear();
}

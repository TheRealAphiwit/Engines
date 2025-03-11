#pragma once

#include "GameObject.h"

namespace Characters
{
	class GameObjectEntry
	{
	public:
		GameObjectEntry();
		GameObjectEntry(GameObject* aGameObject);
		~GameObjectEntry();

		void Update();
		GameObject* GetGameObject() { return myGameObject; }

		bool Opened = false;

	private:
		GameObject* myGameObject;
	};
}
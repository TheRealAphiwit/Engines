#pragma once

class VirtualObject;
namespace Characters
{
	class ObjectEntry
	{
	public:

		ObjectEntry();
		ObjectEntry(VirtualObject* anObject);
		~ObjectEntry();

		void Update();
		VirtualObject* GetObject() { return myObject; } // Can this be here?

		bool Opened = false;

	private:
		VirtualObject* myObject;
	};
}
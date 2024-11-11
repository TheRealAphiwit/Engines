#pragma once

namespace HopefullyWorkGraphics
{
	/*class Graphics
	{
	public:
		Graphics();
		~Graphics();

		void Initialize();
		void Shutdown();
		void Update();
		void Render();
	};*/

	void Initialize(int Width, int Height);
	void Render();
	bool ShouldClose();
}
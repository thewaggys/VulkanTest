#pragma once

#include "Rendering/device/window/GameEngine_window.hpp"
#include "Rendering/device/GameEngine_device.hpp"
#include "Rendering/gameObject/ge_gameObject.hpp"
#include "Rendering/ge_renderer.hpp"

#include <memory>
#include <vector>

namespace GameEngine {
	class FirstApp {

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp &operator=(const FirstApp&) = delete;

		void run();
	private:
		void loadGameObjects();

		LveWindow geWindow{WIDTH, HEIGHT, "this my window yo"};
		GeDevice geDevice{geWindow};
		GeRenderer geRenderer{geWindow, geDevice};
		std::vector<GeGameObject> gameObjects;
	};
}

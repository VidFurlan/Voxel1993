#pragma once

#include <atomic>
#include <condition_variable>
#include <stack>

#include "renderer.hpp"

class RendererManager {
   public:
	RendererManager();

	void render();
	void freeRenderer(Renderer *renderer);

   private:
    std::stack<Renderer *> freeRenderers;

    std::condition_variable cv;
    std::mutex mtx;

    std::atomic<int> activeRenderers = 0;
};

extern RendererManager rendererManager;

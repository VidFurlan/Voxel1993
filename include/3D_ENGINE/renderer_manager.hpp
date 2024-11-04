#pragma once

#include <atomic>
#include <condition_variable>

#include "chunk.hpp"
#include "libs/ThreadsafeQueue.hpp"
#include "renderer.hpp"

class RendererManager {
   public:
	RendererManager();

	void render();
	void freeRenderer(Renderer *renderer);

	static const int MAX_RENDERERS = 20;

   private:
	void renderChunk(Chunk *chunk);

	ThreadsafeQueue<Renderer *> freeRenderers;

	std::condition_variable cv;
	std::mutex mtx;

	std::atomic<int> activeRenderers = 0;
};

extern RendererManager rendererManager;

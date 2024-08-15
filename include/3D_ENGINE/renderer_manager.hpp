#pragma once

#include <condition_variable>
#include <queue>
#include <unordered_set>

#include "chunk.hpp"
#include "renderer.hpp"

class RendererManager {
   public:
	RendererManager();

	void render();
	void addChunk(Chunk *chunk);
	void freeRenderer(Renderer *renderer);

   private:
	std::queue<Chunk *> chunksToRender;
	std::unordered_set<Renderer *> freeRenderers;
	std::unordered_set<Renderer *> busyRenderers;

    std::condition_variable cv;
    std::mutex mtx;
};

extern RendererManager rendererManager;

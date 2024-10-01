#include "renderer_manager.hpp"

#include <condition_variable>
#include <mutex>
#include <thread>

#include "chunk.hpp"
#include "renderer.hpp"
#include "world.hpp"

RendererManager rendererManager;

RendererManager::RendererManager() {}

void RendererManager::render() {
	for (int i = 0; i < World::LOADED_WORLD_SIZE; i++) {
		for (int j = 0; j < World::LOADED_WORLD_SIZE; j++) {
			for (int k = 0; k < World::LOADED_WORLD_SIZE; k++) {
				Chunk *chunk = world.getChunk(i, j, k);
                
                if (chunk == nullptr) {
                    continue;
                }

				Renderer *renderer;
				if (freeRenderers.empty()) {
				 	renderer = new Renderer();
				} else {
                    renderer = freeRenderers.top();
                    freeRenderers.pop();
				}
                activeRenderers++;

				std::thread renderThread(&Chunk::render, chunk, renderer);
				renderThread.detach();
			}
		}
	}

	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, [this] { return activeRenderers == 0; });
}

void RendererManager::freeRenderer(Renderer *renderer) {
    freeRenderers.push(renderer);
    activeRenderers--;
	if (activeRenderers == 0) {
		cv.notify_all();
	}
}

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ratio>
#include <thread>

#include "renderer_manager.hpp"
#include "chunk.hpp"
#include "renderer.hpp"
#include "world.hpp"

RendererManager rendererManager;

RendererManager::RendererManager() {}

void RendererManager::render() {
	for (int i = 0; i < World::LOADED_WORLD_SIZE; i++) {
		for (int j = 0; j < World::LOADED_WORLD_SIZE; j++) {
			for (int k = 0; k < World::LOADED_WORLD_SIZE; k++) {
				renderChunk(world.getChunk(i, j, k));
			}
		}
	}

	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, [this] { return activeRenderers == 0; });
}

void RendererManager::renderChunk(Chunk *chunk) {
	if (chunk == nullptr) return;

	while (activeRenderers >= MAX_RENDERERS && freeRenderers.size() <= MAX_RENDERERS) {

	}

	Renderer *renderer;

    auto it = freeRenderers.pop();
    if (it.has_value()) {
        renderer = it.value();
    } else {
        renderer = new Renderer();
    }
	activeRenderers++;

	std::thread renderThread(&Chunk::render, chunk, renderer);
	renderThread.detach();
}

void RendererManager::freeRenderer(Renderer *renderer) {
	freeRenderers.push(renderer);
	activeRenderers--;
	if (activeRenderers == 0) {
		cv.notify_all();
	}
}

#include "renderer_manager.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

#include "chunk.hpp"
#include "renderer.hpp"

RendererManager rendererManager;

RendererManager::RendererManager() {
    chunksToRender.push(new Chunk(0, 0, 0));
}

void RendererManager::render() {
    Chunk *chunk = chunksToRender.front();
	while (!chunksToRender.empty()) {
		Chunk *chunk = chunksToRender.front();
		chunksToRender.pop();

        Renderer *renderer;
        if (freeRenderers.empty()) {
            renderer = new Renderer();
        }
        else {
            renderer = *freeRenderers.begin();
        }

        busyRenderers.insert(renderer);
        freeRenderers.erase(renderer);

		std::thread renderThread(&Chunk::render, chunk, renderer);
        renderThread.detach();
    }

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return busyRenderers.empty(); });

    chunksToRender.push(chunk);
}

void RendererManager::freeRenderer(Renderer *renderer) {
    std::lock_guard<std::mutex> lock(mtx);
    freeRenderers.insert(renderer);
    busyRenderers.erase(renderer);
    if (busyRenderers.empty()) {
        cv.notify_all();
    }
}

void RendererManager::addChunk(Chunk *chunk) { chunksToRender.push(chunk); }

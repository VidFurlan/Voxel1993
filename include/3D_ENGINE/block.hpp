#pragma once

#include <array>
enum BlockType {
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt,
};

enum BlockFace {
    XPos = 0,
    XNeg = 1,
    YPos = 2,
    YNeg = 3,
    ZPos = 4,
    ZNeg = 5
};

class Block {
   public:
    static const int BLOCK_SIZE = 10;

    bool visibleFaces[6] = {1};

	bool isActive() { return active; }
	void setActive(bool active);
    void setBlockType(BlockType blockType) { this->blockType = blockType; }

   private:
	bool active = true;
	BlockType blockType;
};

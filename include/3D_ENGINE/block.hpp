#pragma once

enum BlockType {
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt,
};

class Block {
   public:
	bool isActive() { return active; }
	void setActive(bool active);
    void setBlockType(BlockType blockType) { this->blockType = blockType; }

    static const int BLOCK_SIZE = 10;

   private:
	bool active;
	BlockType blockType;
};

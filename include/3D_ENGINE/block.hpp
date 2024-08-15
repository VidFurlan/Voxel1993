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

   private:
	bool active;
	BlockType blockType;
};

#pragma once

#include "../Module.h"
#include "../../../DrawUtils.h"

class StorageESP : public IModule {
private:
	int tickTimeout = 0;
	std::vector<AABB> bufferedChestList;
	std::mutex listLock;

	bool normalChest = true;
	bool trappedChest = true;
	bool enderChest = true;
	bool barrel = true;
	bool undyedShulkerBox = true;
	bool shulkerBox = true;

public:
	StorageESP();
	~StorageESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(C_GameMode* gm) override;
};

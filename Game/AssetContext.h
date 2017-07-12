#pragma once
#include "ResourceManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "FontManager.h"

struct AssetContext
{
	std::unique_ptr<StateManager> state_manager;
	std::unique_ptr<TextureManager> texture_manager;
	std::unique_ptr<FontManager> font_manager;
};

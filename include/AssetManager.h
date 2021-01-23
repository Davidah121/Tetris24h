#pragma once
#include <vector>
#include "Image.h"

struct Asset
{
    std::string name;
    Image* image;
};

class AssetManager
{
public:
    ~AssetManager();
    static AssetManager* getAssetManager();
    Image* getAsset(std::string name);
private:
    AssetManager();
    static AssetManager* instance;
    std::vector<Asset> imageAssets;
};
#include "AssetManager.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager::AssetManager()
{
    if(AssetManager::instance == nullptr)
    {
        imageAssets.push_back( {"background", Image::loadBMP("Assets/Background.bmp")} );
        
        imageAssets.push_back( {"titleText", Image::loadBMP("Assets/Title.bmp")} );
        imageAssets.push_back( {"player1Text", Image::loadBMP("Assets/Player1Text.bmp")} );
        imageAssets.push_back( {"player2Text", Image::loadBMP("Assets/Player2Text.bmp")} );
        imageAssets.push_back( {"optionsText", Image::loadBMP("Assets/OptionsText.bmp")} );
        imageAssets.push_back( {"startText", Image::loadBMP("Assets/StartText.bmp")} );
        imageAssets.push_back( {"quitText", Image::loadBMP("Assets/QuitText.bmp")} );
        imageAssets.push_back( {"volumeText", Image::loadBMP("Assets/VolumeText.bmp")} );
        imageAssets.push_back( {"1PlayerText", Image::loadBMP("Assets/1PlayerText.bmp")} );
        imageAssets.push_back( {"2PlayerText", Image::loadBMP("Assets/2PlayerText.bmp")} );
        imageAssets.push_back( {"loseText", Image::loadBMP("Assets/LOSEText.bmp")} );
        imageAssets.push_back( {"winText", Image::loadBMP("Assets/WINText.bmp")} );
        imageAssets.push_back( {"0Text", Image::loadBMP("Assets/0.bmp")} );
        imageAssets.push_back( {"1Text", Image::loadBMP("Assets/1.bmp")} );
        imageAssets.push_back( {"2Text", Image::loadBMP("Assets/2.bmp")} );
        imageAssets.push_back( {"3Text", Image::loadBMP("Assets/3.bmp")} );
        imageAssets.push_back( {"4Text", Image::loadBMP("Assets/4.bmp")} );
        imageAssets.push_back( {"5Text", Image::loadBMP("Assets/5.bmp")} );
        imageAssets.push_back( {"6Text", Image::loadBMP("Assets/6.bmp")} );
        imageAssets.push_back( {"7Text", Image::loadBMP("Assets/7.bmp")} );
        imageAssets.push_back( {"8Text", Image::loadBMP("Assets/8.bmp")} );
        imageAssets.push_back( {"9Text", Image::loadBMP("Assets/9.bmp")} );
        imageAssets.push_back( {"10Text", Image::loadBMP("Assets/10.bmp")} );
        
        imageAssets.push_back( {"downArrow", Image::loadBMP("Assets/DownArrow.bmp")} );
        imageAssets.push_back( {"upArrow", Image::loadBMP("Assets/UpArrow.bmp")} );
        
        imageAssets.push_back( {"blueBlock", Image::loadBMP("Assets/Blue_Block.bmp")} );
        imageAssets.push_back( {"darkPurpleBlock", Image::loadBMP("Assets/DarkPurple_Block.bmp")} );
        imageAssets.push_back( {"grayBlock", Image::loadBMP("Assets/Gray_Block.bmp")} );
        imageAssets.push_back( {"greenBlock", Image::loadBMP("Assets/Green_Block.bmp")} );
        imageAssets.push_back( {"magentaBlock", Image::loadBMP("Assets/Magenta_Block.bmp")} );
        imageAssets.push_back( {"orangeBlock", Image::loadBMP("Assets/Orange_Block.bmp")} );
        imageAssets.push_back( {"redBlock", Image::loadBMP("Assets/Red_Block.bmp")} );
        imageAssets.push_back( {"yellowBlock", Image::loadBMP("Assets/Yellow_Block.bmp")} );

        imageAssets.push_back( {"iBlock", Image::loadBMP("Assets/I_Block.bmp")} );
        imageAssets.push_back( {"jBlock", Image::loadBMP("Assets/J_Block.bmp")} );
        imageAssets.push_back( {"lBlock", Image::loadBMP("Assets/L_Block.bmp")} );
        imageAssets.push_back( {"oBlock", Image::loadBMP("Assets/O_Block.bmp")} );
        imageAssets.push_back( {"sBlock", Image::loadBMP("Assets/S_Block.bmp")} );
        imageAssets.push_back( {"tBlock", Image::loadBMP("Assets/T_Block.bmp")} );
        imageAssets.push_back( {"zBlock", Image::loadBMP("Assets/Z_Block.bmp")} );

        for(Asset a : imageAssets)
        {
            a.image->setColorToNoAlpha({0,0,0,255});
        }
    }
}

AssetManager::~AssetManager()
{
    for(int i=0; i<imageAssets.size(); i++)
    {
        delete imageAssets[i].image;
    }
    imageAssets.clear();

    instance = nullptr;
}

AssetManager* AssetManager::getAssetManager()
{
    if(instance==nullptr)
        instance = new AssetManager();
    return instance;
}

Image* AssetManager::getAsset(std::string name)
{
    Image* img = nullptr;

    for(Asset a : imageAssets)
    {
        if(a.name == name)
        {
            img = a.image;
            break;
        }
    }
    return img;
}
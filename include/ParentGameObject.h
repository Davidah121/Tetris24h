#pragma once

class ParentGameObject
{
public:
    virtual void update(){}
    virtual void render(){}

    int depth = 0;
    bool visible = true;
};
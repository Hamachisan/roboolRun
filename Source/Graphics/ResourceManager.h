#pragma once

#include "Graphics/ModelResource.h"
#include <string>
#include <memory>
#include <map>



class ResourceManager
{
private:
    ResourceManager(){}
    ~ResourceManager(){}

public:
    static ResourceManager* Instance()
    {
        static ResourceManager instance;
        return &instance;
    }

    void init();

    std::shared_ptr<ModelResource> LoadModelResource(const char* filename);

private:
    //using ModelMap = std::map<std::string, std::weak_ptr<ModelResource>>;
    using ModelMap = std::map<std::string, std::shared_ptr<ModelResource>>;

    ModelMap models;

};


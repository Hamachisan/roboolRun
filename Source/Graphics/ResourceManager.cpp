

#include "Graphics/Graphics.h"
#include "Graphics/ResourceManager.h"



void ResourceManager::init()
{
    std::shared_ptr<ModelResource> model;
    char* filename{};
    
    filename = "Data/Model/sun.mdl";
    model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
    models.emplace(filename, model);

    model = nullptr;

    filename = "Data/Model/moon.mdl";
    model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
    models.emplace(filename, model);

    model = nullptr;

    filename = "Data/Model/moon_black.mdl";
    model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
    models.emplace(filename, model);

    model = nullptr;

    filename = "Data/Model/shadow.mdl";
    model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
    models.emplace(filename, model);
}



std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{

    std::shared_ptr<ModelResource> model;

    auto it = models.find(filename);
    if (it != models.end())
    {
        //model = it->second.lock();          // weak_ptr
        model = it->second;                 // shared_ptr

        if (model) return model;
    }

    model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);

    models.emplace(filename, model);


    return model;
}
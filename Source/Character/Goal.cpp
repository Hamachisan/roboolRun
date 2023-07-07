#include "Goal.h"

Goal::Goal()
{
	model = new Model("Data/Model/Goal/goal.mdl");
	position.z = 1200;
	scale.x = scale.y = scale.z = 0.05f;
	angle.y = 1.5708;
}
Goal::~Goal()
{

}


void Goal::Update(float elapsedTime)
{

}

void Goal::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}
#include"BackObj.h"
#include"Graphics/Graphics.h"
#include"BackObjManager.h"

void BackObj::Destroy()
{
	BackObjManager::Instance().Remove(this);
}
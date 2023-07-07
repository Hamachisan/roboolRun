#include	"Graphics/Graphics.h"
#include	"Light.h"

Light::Light(LightType lightType)
	: lightType(lightType)
{
}

void Light::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//if (ImGui::TreeNode(lightTypeName[static_cast<int>(lightType)]))
	if (ImGui::Begin("Light", nullptr, ImGuiTreeNodeFlags_None))
	{
		switch (lightType)
		{
		case	LightType::Directional:
		
			if (ImGui::CollapsingHeader("lightDirection", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::SliderFloat("direction.x", &direction.x, -1.0f, 1.0f);
				ImGui::SliderFloat("direction.y", &direction.y, -1.0f, 1.0f);
				ImGui::SliderFloat("direction.z", &direction.z, -1.0f, 1.0f);
			}
			ImGui::ColorEdit3("color", &color.x);
			break;
		
		}
		//ImGui::TreePop();
	}
	ImGui::End();
}

void Light::DrawDebugPrimitive()
{
	//	åªèÛÇÕì¡Ç…âΩÇ‡ÇµÇ»Ç¢
}

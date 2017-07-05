#include "Hooks.h"
#include "Globals.h"

DrawModelExecuteFn oDrawModelExecute;

void ForceMaterial(IMaterial* material, Color color)
{
	if (material != nullptr)
	{
		RenderView->SetColorModulation(color.Base());
		ModelRender->ForcedMaterialOverride(material);
	}
}

Color espColors;
void __stdcall Game::DrawModelExecute(void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
{
	Hooks::pModel->UnHook();
	if (Engine->IsInGame() && Global::LocalPlayer && Options.Visuals.ESP.Enabled)
	{
		if (info.pModel)
		{
			float red[3];
			red[0] = 1.0f;
			red[1] = 0.0f;
			red[2] = 0.0f;

			float yellow[3];
			yellow[0] = 1.0f;
			yellow[1] = 1.0f;
			yellow[2] = 0.0f;

			std::string modelName = ModelInfo->GetModelName(info.pModel);
			if (modelName.find(XorStr("models/player")) != std::string::npos && Options.Visuals.ESP.Chams)
			{
				CBaseEntity* pEntity = ClientEntList->GetClientEntity(info.entity_index);

				IMaterial *mats[MAXSTUDIOSKINS];
				studiohdr_t* hdr = ModelInfo->GetStudioModel(info.pModel);


				if (pEntity && pEntity->GetAlive())
				{
					if (Options.Visuals.ESP.EnemyOnly)
					{
						if (pEntity->GetTeam() == Global::LocalPlayer->GetTeam())
							return;
					}

					ModelInfo->GetModelMaterials(info.pModel, hdr->numtextures, mats);

					for (auto i = 0; i < hdr->numtextures; i++)
					{
						auto mat = mats[i];
						if (!mat)
							continue;

						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, true);
						mat->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, true);
						mat->AlphaModulate(1.0f);

						ModelRender->ForcedMaterialOverride(hidden_tex);
						RenderView->SetColorModulation(red);

						ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);

						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, false);
						mat->AlphaModulate(1.0f);

						ModelRender->ForcedMaterialOverride(visible_tex);
						RenderView->SetColorModulation(yellow);
					}
				}
				else
				{
					ModelInfo->GetModelMaterials(info.pModel, hdr->numtextures, mats);
					for (int i = 0; i < hdr->numtextures; i++)
					{
						IMaterial* mat = mats[i];
						if (!mat)
							continue;

						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, false);
						mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
						mat->AlphaModulate(1.0f);

						ModelRender->ForcedMaterialOverride(visible_tex);
						RenderView->SetColorModulation(Color::White().Base());
					}
				}
			}
			else if (modelName.find(XorStr("flash")) != std::string::npos && Options.Visuals.Removals.Flash)
			{
				IMaterial* flash = MaterialSystem->FindMaterial(XorStr("effects\\flashbang"), XorStr(TEXTURE_GROUP_CLIENT_EFFECTS));
				IMaterial* flashWhite = MaterialSystem->FindMaterial(XorStr("effects\\flashbang_white"), XorStr(TEXTURE_GROUP_CLIENT_EFFECTS));
				flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				ModelRender->ForcedMaterialOverride(flash);
				ModelRender->ForcedMaterialOverride(flashWhite);
			}

			else if (modelName.find(XorStr("arms")) != std::string::npos && Options.Visuals.Removals.Hands > 0)
			{
				IMaterial* Hands = MaterialSystem->FindMaterial(modelName.c_str(), XorStr(TEXTURE_GROUP_MODEL));

				if (Options.Visuals.Removals.Hands == 1)
				{
					Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
					Hands->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false);
					Hands->AlphaModulate(1.0f);
				}
				else if (Options.Visuals.Removals.Hands == 2)
				{
					Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
					Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
					Hands->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, false);
					Hands->AlphaModulate(1.0f);
				}
				else if (Options.Visuals.Removals.Hands == 3)
				{
					Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
					Hands->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
					Hands->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, true);
					Hands->AlphaModulate(0.5);
				}

				ModelRender->ForcedMaterialOverride(Hands);
			}

			else if (modelName.find(XorStr("weapon")) != std::string::npos && Options.Visuals.Removals.Weapon)
			{
				if (!(modelName.find(XorStr("arms")) != std::string::npos))
				{
					IMaterial* Weapon = MaterialSystem->FindMaterial(modelName.c_str(), XorStr(TEXTURE_GROUP_MODEL));
					Weapon->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					ModelRender->ForcedMaterialOverride(Weapon);
				}
			}
		}
	}

	ModelRender->DrawModelExecute(context, state, info, pCustomBoneToWorld);
	ModelRender->ForcedMaterialOverride(nullptr);
	Hooks::pModel->ReHook();
}
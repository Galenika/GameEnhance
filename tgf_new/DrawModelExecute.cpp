#include "Cheat.h"
DrawModelExecuteFn oDrawModelExecute;

void ForceMaterial( IMaterial* material, Color color )
{
	if( material != NULL )
	{
		I::RenderView->SetColorModulation( color.Base() );
		I::ModelRender->ForcedMaterialOverride( material );
	}
}

void __stdcall Hooks::DrawModelExecute( void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	H::ModelRender->UnHook();

	if( I::Engine->IsInGame() && G::LocalPlayer )
	{
		if( info.pModel )
		{
			std::string modelName = I::ModelInfo->GetModelName( info.pModel );

			if( modelName.find( strenc( "models/player" ) ) != std::string::npos && Vars.Visuals.Chams.Enabled )
			{
				CBaseEntity* pModelEntity = ( CBaseEntity* )I::ClientEntList->GetClientEntity( info.entity_index );
				if( pModelEntity && pModelEntity->GetAlive() && !pModelEntity->GetDormant() )
				{
					float hidden1 = Vars.Visuals.Chams.EnemyColorWall[1];
					float hidden2 = Vars.Visuals.Chams.EnemyColorWall[2];
					float hidden3 = Vars.Visuals.Chams.EnemyColorWall[3];

					float hidden1t = Vars.Visuals.Chams.TeamColorWall[1];
					float hidden2t = Vars.Visuals.Chams.TeamColorWall[2];
					float hidden3t = Vars.Visuals.Chams.TeamColorWall[3];


					float vis1 = Vars.Visuals.Chams.EnemyColor[1];
					float vis2 = Vars.Visuals.Chams.EnemyColor[2];
					float vis3 = Vars.Visuals.Chams.EnemyColor[3];

					float vis1t = Vars.Visuals.Chams.TeamColor[1];
					float vis2t = Vars.Visuals.Chams.TeamColor[2];
					float vis3t = Vars.Visuals.Chams.TeamColor[3];

					Color render_color_hidden = pModelEntity->GetTeam() == G::LocalPlayer->GetTeam() ? Color(hidden1t, hidden2t, hidden3t) : Color(hidden1, hidden2, hidden3);
					Color render_color_visible = pModelEntity->GetTeam() == G::LocalPlayer->GetTeam() ? Color(vis1t, vis2t, vis3t) : Color(vis1, vis2, vis3);

					if( Vars.Visuals.Chams.XQZ )
					{
						ForceMaterial( Vars.Visuals.Chams.Mode == 0 ? hidden_flat : hidden_tex, render_color_hidden );
						I::ModelRender->DrawModelExecute( context, state, info, pCustomBoneToWorld );

						if( Vars.Visuals.Chams.Mode == 0 )
							hidden_flat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
						else
							hidden_tex->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
					}

					if( Vars.Visuals.Chams.Mode == 0 )
						visible_flat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
					else
						visible_tex->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );

					ForceMaterial( Vars.Visuals.Chams.Mode == 0 ? visible_flat : visible_tex, render_color_visible );
					I::ModelRender->DrawModelExecute( context, state, info, pCustomBoneToWorld );
				}
			}

			else if( modelName.find( strenc( "flash" ) ) != std::string::npos && Vars.Visuals.Removals.Flash )
			{
				IMaterial* flash = I::MaterialSystem->FindMaterial( charenc( "effects\\flashbang" ), charenc( TEXTURE_GROUP_CLIENT_EFFECTS ) );
				IMaterial* flashWhite = I::MaterialSystem->FindMaterial( charenc( "effects\\flashbang_white" ), charenc( TEXTURE_GROUP_CLIENT_EFFECTS ) );
				flash->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
				flashWhite->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
				I::ModelRender->ForcedMaterialOverride( flash );
				I::ModelRender->ForcedMaterialOverride( flashWhite );
			}

			else if (modelName.find("arms") != std::string::npos && Vars.Visuals.Removals.HandsMode == 1)
			{
				IMaterial* Hands = I::MaterialSystem->FindMaterial(modelName.c_str(), charenc(TEXTURE_GROUP_MODEL));
				Hands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				I::ModelRender->ForcedMaterialOverride(Hands);
			}

			if (modelName.find(strenc("arms")) != std::string::npos && Vars.Visuals.Removals.HandsMode == 2)
			{
				static IMaterial* Hands = I::MaterialSystem->CreateMaterial(false, true, false);

				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				I::RenderView->SetColorModulation(colors);
				I::RenderView->SetBlend(1.0f);
				I::ModelRender->ForcedMaterialOverride(Hands);
			}

			else if (modelName.find("arms") != std::string::npos && Vars.Visuals.Removals.HandsMode == 3)
			{
				static IMaterial* Hands = I::MaterialSystem->CreateMaterial(false, true, false);

				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.004f;
					colors[counter] += 0.004f;
				}

				I::RenderView->SetColorModulation(colors);
				I::RenderView->SetBlend(0.7f);
				I::ModelRender->ForcedMaterialOverride(Hands);
			}

			else if( modelName.find( "weapon" ) != std::string::npos && Vars.Visuals.Removals.Weapon )
			{
				if( !( modelName.find( "arms" ) != std::string::npos ) )
				{
					IMaterial* Weapon = I::MaterialSystem->FindMaterial( modelName.c_str(), charenc( TEXTURE_GROUP_MODEL ) );
					Weapon->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
					I::ModelRender->ForcedMaterialOverride( Weapon );
				}
			}
		}
	}
	I::ModelRender->DrawModelExecute( context, state, info, pCustomBoneToWorld );
	I::ModelRender->ForcedMaterialOverride( NULL );
	H::ModelRender->ReHook();
}

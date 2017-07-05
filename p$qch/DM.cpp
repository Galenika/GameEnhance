#include "DM.h"

namespace DM {
	DrawModel_t g_fnOriginalDrawModel;
	void __fastcall Hooked_DrawModel(void* thisptr, void* edx, SSDK::DrawModelResults_t *pResults, const SSDK::DrawModelInfo_t& info, SSDK::matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const SSDK::Vector &modelOrigin, int flags){
		const char *modelName = info.m_pStudioHdr->name;

		if (strstr(modelName, XorStr("shadow"))) {
			g_fnOriginalDrawModel(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
			return;
		}

		if (strstr(modelName, XorStr("player"))) {
			SSDK::IMaterial *material;

			float col[3];

			CBaseEntity *pEntity = (CBaseEntity*)info.m_pClientEntity;

			switch (*pEntity->GetTeamNum()) {
			case 2:
				col[0] = 1.f;
				col[1] = 1.f;
				col[2] = 0.f;
				break;
			case 3:
				col[0] = 0.f;
				col[1] = 1.f;
				col[2] = 0.f;
				break;
			default:
				col[0] = 1.f;
				col[1] = 1.f;
				col[2] = 1.f;
				break;
			}

			material = SSDK::I::MatSystem()->FindMaterial(XorStr("chams_ignorez"), TEXTURE_GROUP_MODEL);
			SSDK::I::StudioRender()->SetColorModulation(col);
			SSDK::I::StudioRender()->ForcedMaterialOverride(material);
			g_fnOriginalDrawModel(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

			switch (*pEntity->GetTeamNum()) {
			case 2:
				col[0] = 1.f;
				col[1] = 0.f;
				col[2] = 0.f;
				break;
			case 3:
				col[0] = 0.f;
				col[1] = 0.f;
				col[2] = 1.f;
				break;
			default:
				col[0] = 1.f;
				col[1] = 1.f;
				col[2] = 1.f;
				break;
			}

			material = SSDK::I::MatSystem()->FindMaterial(XorStr("chams"), TEXTURE_GROUP_MODEL);
			SSDK::I::StudioRender()->SetColorModulation(col);
			SSDK::I::StudioRender()->ForcedMaterialOverride(material);
		}

		if (strstr(modelName, XorStr("weapon"))){
			if (!strstr(modelName, XorStr("v_")) && !strstr(modelName, XorStr("arms"))) {
				//SSDK::I::CVar()->ConsolePrintf("%s\n", info.m_pStudioHdr->name);
				SSDK::IMaterial *material;
				material = SSDK::I::MatSystem()->FindMaterial(XorStr("chams_ignorez"), TEXTURE_GROUP_MODEL);
				SSDK::I::StudioRender()->ForcedMaterialOverride(material);
				g_fnOriginalDrawModel(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
				material = SSDK::I::MatSystem()->FindMaterial(XorStr("chams"), TEXTURE_GROUP_MODEL);
				SSDK::I::StudioRender()->ForcedMaterialOverride(material);
			}
		}

		g_fnOriginalDrawModel(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		SSDK::I::StudioRender()->ForcedMaterialOverride(NULL);
	}


	void CreateVisibleVMT() {
		std::ifstream file(XorStr("csgo\\materials\\chams.vmt"));

		if (!file.good()) {
			std::string str =
				XorStr(R"###("VertexLitGeneric"
            {
                "$basetexture" "VGUI/white_additive"
                "$ignorez"     "0"
                "$nofog"       "1"
                "$model"       "1"
                "$nocull"      "1"
                "$halflambert" "1"
            })###");

			std::ofstream(XorStr("csgo\\materials\\chams.vmt")) << str;
		}
	}

	void CreateIgnoreZVMT() {
		std::ifstream file(XorStr("csgo\\materials\\chams_ignorez.vmt"));

		if (!file.good()) {
			std::string str =
				XorStr(R"###("VertexLitGeneric"
            {
                "$basetexture" "VGUI/white_additive"
                "$ignorez"     "1"
                "$nofog"       "1"
                "$model"       "1"
                "$nocull"      "1"
                "$halflambert" "1"
            })###");

			std::ofstream(XorStr("csgo\\materials\\chams_ignorez.vmt")) << str;
		}
	}

	void CreateVisibleWireVMT() {
		std::ifstream file(XorStr("csgo\\materials\\chams_wire.vmt"));

		if (!file.good()) {
			std::string str =
				XorStr(R"###("VertexLitGeneric"
            {
                "$basetexture" "VGUI/white_additive"
                "$ignorez"     "0"
                "$nofog"       "1"
                "$model"       "1"
                "$nocull"      "1"
                "$halflambert" "1"
				"$wireframe"   "1"
            })###");

			std::ofstream(XorStr("csgo\\materials\\chams_wire.vmt")) << str;
		}
	}

	void InitChams() {
		CreateVisibleVMT();
		CreateVisibleWireVMT();
		CreateIgnoreZVMT();
	}
}
#pragma once

#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin)

void skinchanger()
{
	//Utilities::Log("APPLY SKIN APPLY SKIN");
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

	int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) // CHANGE
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (pEntity)
		{
			ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

			IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

			if (pOwner)
			{
				if (pOwner == pLocal)
				{
					std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

					if (!(sWeapon.find("models/weapons", 0) != std::string::npos))
						continue;

					if (sWeapon.find("c4_planted", 0) != std::string::npos)
						continue;

					if (sWeapon.find("thrown", 0) != std::string::npos)
						continue;

					if (sWeapon.find("smokegrenade", 0) != std::string::npos)
						continue;

					if (sWeapon.find("flashbang", 0) != std::string::npos)
						continue;

					if (sWeapon.find("fraggrenade", 0) != std::string::npos)
						continue;

					if (sWeapon.find("molotov", 0) != std::string::npos)
						continue;

					if (sWeapon.find("decoy", 0) != std::string::npos)
						continue;

					if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
						continue;

					if (sWeapon.find("ied", 0) != std::string::npos)
						continue;

					if (sWeapon.find("w_eq_", 0) != std::string::npos)
						continue;

					CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

					ClientClass *pClass = Interfaces::Client->GetAllClasses();

					if (Menu::Window.SkinchangerTab.SkinEnable.GetState())
					{
						int Model = Menu::Window.SkinchangerTab.KnifeModel.GetIndex();

						int M41S = Menu::Window.SkinchangerTab.M41SSkin.GetIndex();
						int M4A4 = Menu::Window.SkinchangerTab.M4A4Skin.GetIndex();
						int AK47 = Menu::Window.SkinchangerTab.AK47Skin.GetIndex();
						int AWP = Menu::Window.SkinchangerTab.AWPSkin.GetIndex();
						int GLOCK = Menu::Window.SkinchangerTab.GLOCKSkin.GetIndex();
						int USPS = Menu::Window.SkinchangerTab.USPSSkin.GetIndex();
						int DEAGLE = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
						int FIVE7 = Menu::Window.SkinchangerTab.FIVESEVENSkin.GetIndex();
						int AUG = Menu::Window.SkinchangerTab.AUGSkin.GetIndex();
						int FAMAS = Menu::Window.SkinchangerTab.FAMASSkin.GetIndex();
						int G3SG1 = Menu::Window.SkinchangerTab.G3SG1Skin.GetIndex();
						int Galil = Menu::Window.SkinchangerTab.GALILSkin.GetIndex();
						int M249 = Menu::Window.SkinchangerTab.M249Skin.GetIndex();
						int MAC10 = Menu::Window.SkinchangerTab.MAC10Skin.GetIndex();
						int P90 = Menu::Window.SkinchangerTab.P90Skin.GetIndex();
						int UMP45 = Menu::Window.SkinchangerTab.UMP45Skin.GetIndex();
						int XM1014 = Menu::Window.SkinchangerTab.XM1014Skin.GetIndex();
						int BIZON = Menu::Window.SkinchangerTab.BIZONSkin.GetIndex();
						int MAG7 = Menu::Window.SkinchangerTab.MAG7Skin.GetIndex();
						int NEGEV = Menu::Window.SkinchangerTab.NEGEVSkin.GetIndex();
						int SAWEDOFF = Menu::Window.SkinchangerTab.SAWEDOFFSkin.GetIndex();
						int TEC9 = Menu::Window.SkinchangerTab.TECNINESkin.GetIndex();
						int P2000 = Menu::Window.SkinchangerTab.P2000Skin.GetIndex();
						int MP7 = Menu::Window.SkinchangerTab.MP7Skin.GetIndex();
						int MP9 = Menu::Window.SkinchangerTab.MP9Skin.GetIndex();
						int NOVA = Menu::Window.SkinchangerTab.NOVASkin.GetIndex();
						int P250 = Menu::Window.SkinchangerTab.P250Skin.GetIndex();
						int SCAR20 = Menu::Window.SkinchangerTab.SCAR20Skin.GetIndex();
						int SG553 = Menu::Window.SkinchangerTab.SG553Skin.GetIndex();
						int SSG08 = Menu::Window.SkinchangerTab.SSG08Skin.GetIndex();
						int Magnum = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();

						if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CKnife)
						{
							if (Menu::Window.SkinchangerTab.SkinName.getText().length() > 1)
							{
								auto pCustomName = MakePtr(char*, pWeapon, 0x301C);
								strcpy_s(pCustomName, 32, Menu::Window.SkinchangerTab.SkinName.getText().c_str());
							}
						}

						if (Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != NULL && Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != "")
						{
							int st = atoi(Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str());

							if (Menu::Window.SkinchangerTab.StatTrakEnable.GetState())
								*pWeapon->FallbackStatTrak() = st;
						}

						int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();

						switch (weapon)
						{
						case 7: // AK47 
						{
							switch (AK47)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 341;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 14;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 44;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 172;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 180;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 394;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 300;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 226;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 282;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 302;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 316;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 340;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 380;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 422;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 456;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 474;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 490;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 506;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 524;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 600;
								break;
							default:
								break;
							}
						}
						break;
						case 16: // M4A4
						{
							switch (M4A4)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 155;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 187;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 255;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 309;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 215;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 336;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 384;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 400;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 449;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 471;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 480;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 512;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 533;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 588;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 632;
								break;
							default:
								break;
							}
						}
						break;
						case 60: // M4A1
						{
							switch (M41S)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 60;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 430;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 77;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 254;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 189;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 301;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 217;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 257;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 321;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 326;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 360;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 383;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 440;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 445;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 497;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 548;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 587;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 631;
								break;
							default:
								break;
							}
						}
						break;
						case 9: // AWP
						{
							switch (AWP)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 174;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 344;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 84;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 30;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 51;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 72;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 181;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 259;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 395;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 212;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 227;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 251;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 279;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 424;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 446;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 451;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 475;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 525;
								break;
							default:
								break;
							}
						}
						break;
						case 61: // USP
						{
							switch (USPS)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 25;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 60;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 183;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 339;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 217;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 221;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 236;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 277;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 290;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 313;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 318;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 332;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 364;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 454;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 489;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 504;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 540;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 637;
								break;
							default:
								break;
							}
						}
						break;
						case 4: // Glock
						{
							switch (GLOCK)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 2;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 3;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 38;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 40;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 48;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 437;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 99;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 159;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 399;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 208;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 230;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 278;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 293;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 353;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 367;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 381;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 479;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 495;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 532;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 607;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 586;
								break;
							default:
								break;
							}
						}
						break;
						case 1: // Deagle
						{
							switch (DEAGLE)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 37;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 347;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 468;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 469;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 232;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 17;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 40;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 61;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 90;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 185;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 231;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 237;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 397;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 328;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 273;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 296;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 351;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 425;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 470;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 509;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 527;
								break;
							default:
								break;
							}
						}
						break;
						case 3: // Five Seven
						{
							switch (FIVE7)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 3;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 44;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 46;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 78;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 141;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 151;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 254;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 248;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 210;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 223;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 252;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 265;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 274;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 464;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 352;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 377;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 387;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 427;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 510;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 530;
								break;
							default:
								break;
							}
						}
						break;
						case 8: // AUG
						{
							switch (AUG)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 9;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 33;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 280;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 305;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 375;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 442;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 444;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 455;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 507;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 541;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 601;
								break;
							default:
								break;
							}
						}
						break;
						case 10: // Famas
						{
							switch (FAMAS)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 22;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 47;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 92;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 429;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 154;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 178;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 194;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 244;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 218;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 260;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 288;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 371;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 477;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 492;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 529;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 604;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 626;
								break;
							default:
								break;
							}
						}
						break;
						case 11: // G3SG1
						{
							switch (G3SG1)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 8;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 6;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 46;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 72;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 74;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 147;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 170;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 195;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 229;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 294;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 465;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 464;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 382;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 438;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 493;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 511;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 545;
								break;
							default:
								break;
							}
						}
						break;
						case 13: // Galil
						{
							switch (Galil)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 22;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 83;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 428;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 76;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 119;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 398;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 192;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 308;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 216;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 237;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 241;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 264;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 297;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 379;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 460;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 478;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 494;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 546;
								break;
							default:
								break;
							}
						}
						break;
						case 14: // M249
						{
							switch (M249)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 22;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 75;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 202;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 243;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 266;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 401;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 452;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 472;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 496;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 547;
								break;
							default:
								break;
							}
						}
						break;
						case 17: // Mac 10
						{
							switch (MAC10)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 101;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 3;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 32;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 17;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 38;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 433;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 98;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 157;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 188;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 337;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 246;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 284;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 310;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 333;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 343;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 372;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 402;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 498;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 534;
								break;
							default:
								break;
							}
						}
						break;
						case 19: // P90
						{
							switch (P90)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 342;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 20;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 22;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 100;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 67;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 111;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 124;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 156;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 234;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 169;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 175;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 182;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 244;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 228;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 283;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 311;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 335;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 359;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 486;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 516;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 636;
								break;
							default:
								break;
							}
						}
						break;
						case 24: // UMP-45
						{
							switch (UMP45)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 37;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 15;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 17;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 436;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 70;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 93;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 169;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 175;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 193;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 392;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 281;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 333;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 362;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 441;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 488;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 556;
								break;
							default:
								break;
							}
						}
						break;
						case 25: // XM1014
						{
							switch (XM1014)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 166;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 238;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 42;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 96;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 95;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 135;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 151;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 169;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 205;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 240;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 251;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 393;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 320;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 314;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 348;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 370;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 407;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 505;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 521;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 557;
								break;
							default:
								break;
							}
						}
						break;
						case 26: // Bizon
						{
							switch (BIZON)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 13;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 164;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 25;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 70;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 148;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 149;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 159;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 171;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 203;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 224;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 236;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 267;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 306;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 323;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 349;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 376;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 457;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 459;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 508;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 526;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 542;
								break;
							default:
								break;
							}
						}
						break;
						case 27: // Mag 7
						{
							switch (MAG7)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 462;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 34;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 32;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 100;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 39;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 431;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 99;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 171;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 177;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 198;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 291;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 385;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 473;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 499;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 535;
								break;
							default:
								break;
							}
						}
						break;
						case 28: // Negev
						{
							switch (NEGEV)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 28;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 432;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 157;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 201;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 240;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 285;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 298;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 317;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 355;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 369;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 483;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 514;
								break;
							default:
								break;
							}
						}
						break;
						case 29: // Sawed Off
						{
							switch (SAWEDOFF)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 345;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 22;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 30;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 83;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 38;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 41;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 434;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 119;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 171;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 204;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 405;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 246;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 250;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 390;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 256;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 323;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 458;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 459;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 517;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 552;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 638;
								break;
							default:
								break;
							}
						}
						break;
						case 30: // Tec 9
						{
							switch (TEC9)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 101;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 2;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 463;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 17;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 36;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 439;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 159;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 179;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 248;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 206;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 216;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 242;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 272;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 289;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 303;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 374;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 459;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 520;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 539;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 555;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 614;
								break;
							default:
								break;
							}
						}
						break;
						case 32: // P2000
						{
							switch (P2000)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 104;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 32;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 21;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 25;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 36;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 485;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 38;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 71;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 95;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 184;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 211;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 338;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 246;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 275;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 327;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 346;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 357;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 389;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 442;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 443;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 515;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 550;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 591;
								break;
							default:
								break;
							}
						}
						break;
						case 33: // MP7
						{
							switch (MP7)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 2;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 102;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 5;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 28;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 11;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 15;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 22;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 36;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 141;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 235;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 245;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 209;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 213;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 250;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 354;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 365;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 423;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 442;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 481;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 500;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 536;
								break;
							default:
								break;
							}
						}
						break;
						case 34: // MP9
						{
							switch (MP9)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 482;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 33;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 100;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 39;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 61;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 148;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 141;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 199;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 329;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 262;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 366;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 368;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 386;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 403;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 448;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 549;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 609;
								break;
							default:
								break;
							}
						}
						break;
						case 35: // Nova
						{
							switch (NOVA)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 3;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 166;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 164;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 25;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 62;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 99;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 107;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 158;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 170;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 191;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 214;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 225;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 263;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 286;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 294;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 299;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 356;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 450;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 484;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 537;
								break;
							default:
								break;
							}
						}
						break;
						case 36: // P250
						{
							switch (P250)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 102;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 34;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 162;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 15;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 164;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 77;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 99;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 168;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 258;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 207;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 219;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 404;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 230;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 271;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 295;
								break;
							case 16:
								*pWeapon->FallbackPaintKit() = 464;
								break;
							case 17:
								*pWeapon->FallbackPaintKit() = 358;
								break;
							case 18:
								*pWeapon->FallbackPaintKit() = 373;
								break;
							case 19:
								*pWeapon->FallbackPaintKit() = 388;
								break;
							case 20:
								*pWeapon->FallbackPaintKit() = 426;
								break;
							case 21:
								*pWeapon->FallbackPaintKit() = 466;
								break;
							case 22:
								*pWeapon->FallbackPaintKit() = 467;
								break;
							case 23:
								*pWeapon->FallbackPaintKit() = 501;
								break;
							case 24:
								*pWeapon->FallbackPaintKit() = 551;
								break;
							default:
								break;
							}
						}
						break;
						case 38: // Scar 20
						{
							switch (SCAR20)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 165;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 100;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 46;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 70;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 116;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 157;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 196;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 232;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 391;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 298;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 312;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 406;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 453;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 502;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 518;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 597;
								break;
							default:
								break;
							}
						}
						break;
						case 39: // SG553
						{
							switch (SG553)
							{
							case 1:
								*pWeapon->FallbackPaintKit() = 39;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 98;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 410;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 347;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 287;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 298;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 363;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 378;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 487;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 519;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 553;
								break;
							default:
								break;
							}
						}
						break;
						case 40: // SSG08
						{
							switch (SSG08)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 26;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 60;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 96;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 99;
								break;
							case 4:
								*pWeapon->FallbackPaintKit() = 157;
								break;
							case 5:
								*pWeapon->FallbackPaintKit() = 200;
								break;
							case 6:
								*pWeapon->FallbackPaintKit() = 222;
								break;
							case 7:
								*pWeapon->FallbackPaintKit() = 233;
								break;
							case 8:
								*pWeapon->FallbackPaintKit() = 253;
								break;
							case 9:
								*pWeapon->FallbackPaintKit() = 304;
								break;
							case 10:
								*pWeapon->FallbackPaintKit() = 319;
								break;
							case 11:
								*pWeapon->FallbackPaintKit() = 361;
								break;
							case 12:
								*pWeapon->FallbackPaintKit() = 503;
								break;
							case 13:
								*pWeapon->FallbackPaintKit() = 538;
								break;
							case 14:
								*pWeapon->FallbackPaintKit() = 554;
								break;
							case 15:
								*pWeapon->FallbackPaintKit() = 624;
								break;
							default:
								break;
							}
						}
						break;
						case 64: // Revolver
						{
							switch (Magnum)
							{
							case 0:
								*pWeapon->FallbackPaintKit() = 27;
								break;
							case 1:
								*pWeapon->FallbackPaintKit() = 12;
								break;
							case 2:
								*pWeapon->FallbackPaintKit() = 522;
								break;
							case 3:
								*pWeapon->FallbackPaintKit() = 523;
								break;
							default:
								break;
							}
						}
						break;
						default:
							break;
						}



						if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
						{
							auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
							if (Menu::Window.SkinchangerTab.KnifeName.getText().length() > 1)
							{
								strcpy_s(pCustomName1, 32, Menu::Window.SkinchangerTab.KnifeName.getText().c_str());
							}

							if (Model == 0) // Bayonet
							{
								*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iBayonet;
								*pWeapon->WorldModelIndex() = iBayonet + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}
							}
							else if (Model == 1) // Bowie Knife
							{
								*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iBowie;
								*pWeapon->WorldModelIndex() = iBowie + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}

							}
							else if (Model == 2) // Butterfly Knife
							{
								*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iButterfly;
								*pWeapon->WorldModelIndex() = iButterfly + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}

							}
							else if (Model == 3) // Falchion Knife
							{
								*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iFalchion;
								*pWeapon->WorldModelIndex() = iFalchion + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 558; // Lore
								}
							}
							else if (Model == 4) // Flip Knife
							{
								*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iFlip;
								*pWeapon->WorldModelIndex() = iFlip + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 559; // Lore
								}

							}
							else if (Model == 5) // Gut Knife
							{
								*pWeapon->ModelIndex() = iGut; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iGut;
								*pWeapon->WorldModelIndex() = iGut + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 560; // Lore
								}


							}
							else if (Model == 6) // Huntsman Knife
							{
								*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iHuntsman;
								*pWeapon->WorldModelIndex() = iHuntsman + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 559; // Lore
								}


							}
							else if (Model == 7) // Karambit
							{
								*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iKarambit;
								*pWeapon->WorldModelIndex() = iKarambit + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}

							}
							else if (Model == 8) // M9 Bayonet
							{
								*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iM9Bayonet;
								*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 562; // Lore
								}

							}


							else if (Model == 10) // Shadow Daggers
							{
								*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
								*pWeapon->ViewModelIndex() = iDagger;
								*pWeapon->WorldModelIndex() = iDagger + 1;
								*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;

								int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

								if (Skin == 0)
								{
									*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
								}
								else if (Skin == 1)
								{
									*pWeapon->FallbackPaintKit() = 12; // Crimson Web
								}
								else if (Skin == 2)
								{
									*pWeapon->FallbackPaintKit() = 27; // Bone Mask
								}
								else if (Skin == 3)
								{
									*pWeapon->FallbackPaintKit() = 38; // Fade
								}
								else if (Skin == 4)
								{
									*pWeapon->FallbackPaintKit() = 40; // Night
								}
								else if (Skin == 5)
								{
									*pWeapon->FallbackPaintKit() = 42; // Blue Steel
								}
								else if (Skin == 6)
								{
									*pWeapon->FallbackPaintKit() = 43; // Stained
								}
								else if (Skin == 7)
								{
									*pWeapon->FallbackPaintKit() = 44; // Case Hardened
								}
								else if (Skin == 8)
								{
									*pWeapon->FallbackPaintKit() = 59; // Slaughter
								}
								else if (Skin == 9)
								{
									*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
								}
								else if (Skin == 10)
								{
									*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
								}
								else if (Skin == 11)
								{
									*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
								}
								else if (Skin == 12)
								{
									*pWeapon->FallbackPaintKit() = 143; // Urban Masked
								}
								else if (Skin == 13)
								{
									*pWeapon->FallbackPaintKit() = 175; // Scorched
								}
								else if (Skin == 14)
								{
									*pWeapon->FallbackPaintKit() = 323; // Rust Coat
								}
								else if (Skin == 15)
								{
									*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
								}
								else if (Skin == 16)
								{
									*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
								}
								else if (Skin == 17)
								{
									*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
								}
								else if (Skin == 18)
								{
									*pWeapon->FallbackPaintKit() = 413; // Marble Fade
								}
								else if (Skin == 19)
								{
									*pWeapon->FallbackPaintKit() = 414; // Rust Coat
								}
								else if (Skin == 20)
								{
									*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
								}
								else if (Skin == 21)
								{
									*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									*pWeapon->FallbackPaintKit() = 561; // Lore
								}
							}
						}

						*pWeapon->OwnerXuidLow() = 0;
						*pWeapon->OwnerXuidHigh() = 0;
						*pWeapon->FallbackWear() = 0.001f;
						*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

					}
				}
			}

		}
	}
}
#include "Cheat.h"

void ChangeKnifeColor(CBaseCombatWeapon* pWeapon, int nBayonet, int nFlip, int nGut, int nKarambit, int nM9Bayonet, int nHuntsman, int nButterfly, int nFalchion, int nDagger, int nBowie)// gay ass code dont judge
{
	if (Vars.Misc.KnifeModel == 1)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nBayonet;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nBayonet + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 500;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDb;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 2)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nFlip;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nFlip + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 505;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDf;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 3)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nGut;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nGut + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 506;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDg;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 4)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nKarambit;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nKarambit + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 507;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDk;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 5)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nM9Bayonet;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nM9Bayonet + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 508;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDm;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 6)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nHuntsman;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nHuntsman + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 509;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDh;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 7)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nButterfly;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nButterfly + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 515;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDbutter;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 8)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nFalchion;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nFalchion + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 512;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDfal;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 9)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nDagger;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nDagger + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 516;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDdag;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
	else if (Vars.Misc.KnifeModel == 10)
	{
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.ViewModelIndex) = nBowie;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.WorldModelIndex) = nBowie + 1;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iItemDefinitionIndex) = 514;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_nFallbackPaintKit) = Vars.Misc.KnifeIDbow;
		*reinterpret_cast<int*>(DWORD(pWeapon) + offsets.m_iEntityQuality) = 3;
	}
}


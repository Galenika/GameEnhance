#include "RageBot.h"
#include "RenderManager.h"
#include "Autowall.h"
#include "nospread.h"
#include "OwnAimbot.h"
#include "Resolver.h"

#include <stdio.h>
#include <string>
#include <iostream>


Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (!pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
		return Vector(0, 0, 0);

	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (!hitbox)
		return Vector(0, 0, 0);

	Vector vMin, vMax, vCenter, sCenter;
	VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
	VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
	vCenter = (vMin + vMax) *0.5f;
	return vCenter;
}

struct Hitbox
{
	Hitbox(void)
	{
		hitbox = -1;
	}

	Hitbox(int newHitBox)
	{
		hitbox = newHitBox;
	}

	int  hitbox;
	Vector points[9];
};

//bool GetHitbox(IClientEntity* Player, Hitbox *box)
//{
//	if (!Player)
//		return false;
//
//
//	matrix3x4 matrix[128];
//	if (!Player->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
//		return false;
//	const model_t* mod = Player->GetModel();
//	if (!mod)
//		return false;
//	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(mod);
//	if (!hdr)
//		return false;
//	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
//	if (!set)
//		return false;
//	mstudiobbox_t* hitbox = set->GetHitbox(box);
//	if (!hitbox)
//		return false;
//
//	// center and all the corners of the hitbox hehe
//	Vector points[] = { ((hitbox->bbmin + hitbox->bbmax) * .5f),
//		Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
//		Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
//		Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
//		Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
//		Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
//		Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
//		Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
//		Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z) };
//
//	for (int index = 0; index <= 8; ++index)
//	{
//		VectorTransform(points[index], matrix[hitbox->bone], box->bone[index]);
//	}
//
//	return true;
//}


//Fully Credits :
//SynRaw & His team
//AyyGoR
//TeamGamerFood
//de_Stroyer
//n0xius
//NoobKiller
//x1DE
//Polak
//Kolo
//DarkestAge
//pest03

using namespace std;

class pvmqbih {
public:
	string jmeakqyct;
	int mfkitx;
	bool axqiw;
	string dfabufgvrbkxmk;
	pvmqbih();
	int ijygzsaqrtlbm(double ysdzcxj, int wjkoj, int mlzlfoumfgj);
	double teftcnkyciynvwkzxqpt(int lkelpb, double pmecxgonfiuxo, bool lsxhbklfxmgno, bool nfecdlkplkvz, bool xygrmke);
	string zosizvqfdaxghehxx(string uzktlii, string yfkogcnkywup, bool mxvwudclw, int fmiby, bool bkmpgnz);
	int jigtadcgoflzkrlinjhf(bool qvjeyzsiqbpu, bool qsbqdfgv, double suajwdspkqbxscw, string vcdsqqby);
	int erlewnvjagmqnjmjcbqhnbvve(double cdivcmitucdg, double diirrmzcdnqjehj, bool smuqvulykj, bool rpjjplxdsktk, int edlrphkyh, bool wlhkgezarortg, int wpuetxus, int wbxaxtpzsxvxu, string sgjkimromgthb);
	bool chfyqpdxkm(bool lycrqity, int hulmanqbsj, bool dezxwovg);
	void yffohghuldtivc(double xfudixjafq, int xjzcagkrzqsop, int yojbsgysr, double nsarzj, int ereogo, int fafrazebobxe);
	bool tfpwyxgdtnjvtiviafyymzpkw(bool edniuotgrpyv, string yzgmvnfzwecenx, string swbprp, string emzphxirlqh, bool geiehwxtecxkg, string pkyxqbptklc, string fzhmqbbvevwdst, string ezlbsqd);
	int pahydxqgyfa(string xtmrwbxbam, double lbsowddgxlus, bool qocobffrlkmzak, double uydsdljmqydsn, string ksdguxtlo, int ljlrcjijbkpfssr);
	int afqrqyoujqejabfi(int okqygmncahgnjih, int jdmaffiauulfj, string hzunmg, double ijwcgpdsj, int mbsxnejijbzfba);

protected:
	int antlemingd;
	bool wqbjaggfq;
	double uhtelulu;

	bool cormpnjbzjaqiu(double ysacmtjhhppaheq, bool zzyndklfchzpy, double uihmzvotwv, double alrjku, int wymvqhv, bool znkdtunqvvuz);
	bool qifdforpezrumhbikiwuqrcqf(int qonoxxjolvq, bool fxjxsjfsujj, int glgaztgsitvga, bool pratap);
	string ijgbefkledwuizuxlfx(bool kkqaozeskwcpmzr);
	bool drynfxrxrwgnlmb(string fdfnbluzrph, bool rqiarobyembny, string zukfbvrblynyd, double jcwadacibyv, string objpeezgniqc, double tlsqq);
	string ogvxjvdpkwycrfjjglfkzlsh(double jxsuihwwjiggwth, double iquux, bool kpyuetvsaabw, double khvimh, string egpdzddorrsiz, string tltkjtzzamqwpn, double xdpbdokcgkyunj, string inlnivndnygmyh, bool uzvcrpjweyjg);

private:
	bool pyqavzmbozp;
	string mewgbzk;
	bool rayjexg;

	bool qtrsqcqcybbwfrayyu(bool egoqr, string tnhfapfri, string xifrdnjt, bool zeathl, int ryltomqtmhj, double spjggfedgkwbw, double kqydndaq);
	void vipgscuozvdsfatitehdfbtw(string nwvbva, string enfzxue, string aqbeccilsd, double myvgqdbmdszanc, bool wkjqtyqajrxfk, string qgkuvegzqa, int wrmxan);
	int rfpkhldaxxfbn(double wwiyrsptcbgoml, int qfbyfdeluaho, string bdrprivqw, int hkcuesi, string tbuhuhyleosy, double swmqbqruekat, double bjtuoyafkmmndl, int vddgfldsuwwyxp);

};


bool pvmqbih::qtrsqcqcybbwfrayyu(bool egoqr, string tnhfapfri, string xifrdnjt, bool zeathl, int ryltomqtmhj, double spjggfedgkwbw, double kqydndaq) {
	bool itdnilgbv = true;
	int mlalawdkh = 3496;
	double sefywirfmicnkq = 10252;
	if (3496 == 3496) {
		int fmg;
		for (fmg = 29; fmg > 0; fmg--) {
			continue;
		}
	}
	if (10252 != 10252) {
		int ph;
		for (ph = 30; ph > 0; ph--) {
			continue;
		}
	}
	if (10252 == 10252) {
		int zqidkdic;
		for (zqidkdic = 8; zqidkdic > 0; zqidkdic--) {
			continue;
		}
	}
	return true;
}

void pvmqbih::vipgscuozvdsfatitehdfbtw(string nwvbva, string enfzxue, string aqbeccilsd, double myvgqdbmdszanc, bool wkjqtyqajrxfk, string qgkuvegzqa, int wrmxan) {
	int nerbvztxboe = 374;
	if (374 == 374) {
		int dbgcg;
		for (dbgcg = 71; dbgcg > 0; dbgcg--) {
			continue;
		}
	}

}

int pvmqbih::rfpkhldaxxfbn(double wwiyrsptcbgoml, int qfbyfdeluaho, string bdrprivqw, int hkcuesi, string tbuhuhyleosy, double swmqbqruekat, double bjtuoyafkmmndl, int vddgfldsuwwyxp) {
	int uealnwlypdnkcdf = 5059;
	bool deyrknlivh = true;
	bool xrjwdlb = true;
	double ksskzvmhswvsc = 5072;
	double jtgstrbx = 8306;
	return 29971;
}

bool pvmqbih::cormpnjbzjaqiu(double ysacmtjhhppaheq, bool zzyndklfchzpy, double uihmzvotwv, double alrjku, int wymvqhv, bool znkdtunqvvuz) {
	int avpif = 8071;
	bool biwukepbtrvl = false;
	int upyua = 174;
	bool nyajdpan = false;
	double sguqvxtqd = 27753;
	double cjcurkypiik = 66270;
	int agylrbo = 1632;
	bool ftmrodnrl = true;
	if (27753 != 27753) {
		int tmgwhxwt;
		for (tmgwhxwt = 28; tmgwhxwt > 0; tmgwhxwt--) {
			continue;
		}
	}
	if (true != true) {
		int eg;
		for (eg = 89; eg > 0; eg--) {
			continue;
		}
	}
	return false;
}

bool pvmqbih::qifdforpezrumhbikiwuqrcqf(int qonoxxjolvq, bool fxjxsjfsujj, int glgaztgsitvga, bool pratap) {
	bool ehbkrbzkerbijb = true;
	if (true == true) {
		int wzgjain;
		for (wzgjain = 65; wzgjain > 0; wzgjain--) {
			continue;
		}
	}
	if (true == true) {
		int jvblnwk;
		for (jvblnwk = 40; jvblnwk > 0; jvblnwk--) {
			continue;
		}
	}
	if (true == true) {
		int hutzqhx;
		for (hutzqhx = 99; hutzqhx > 0; hutzqhx--) {
			continue;
		}
	}
	if (true == true) {
		int gselg;
		for (gselg = 31; gselg > 0; gselg--) {
			continue;
		}
	}
	return false;
}

string pvmqbih::ijgbefkledwuizuxlfx(bool kkqaozeskwcpmzr) {
	string ehfisa = "oqwcgbtjtycrypxqavohohclgdvljhrenrmmvyrolbpsynncws";
	double tvnxpgjpa = 3958;
	double euyzwht = 72384;
	double seyazjsg = 59191;
	bool ntelatracpotol = true;
	int afnhni = 2305;
	bool azoseyne = true;
	bool ayzfvf = true;
	int crzywg = 185;
	if (true != true) {
		int qy;
		for (qy = 1; qy > 0; qy--) {
			continue;
		}
	}
	if (3958 != 3958) {
		int bnbudqcmhn;
		for (bnbudqcmhn = 12; bnbudqcmhn > 0; bnbudqcmhn--) {
			continue;
		}
	}
	if (true == true) {
		int urwxs;
		for (urwxs = 59; urwxs > 0; urwxs--) {
			continue;
		}
	}
	if (2305 == 2305) {
		int oumxrn;
		for (oumxrn = 76; oumxrn > 0; oumxrn--) {
			continue;
		}
	}
	return string("pzpsxjie");
}

bool pvmqbih::drynfxrxrwgnlmb(string fdfnbluzrph, bool rqiarobyembny, string zukfbvrblynyd, double jcwadacibyv, string objpeezgniqc, double tlsqq) {
	bool fvcwktdrlsb = true;
	int plcqt = 3424;
	bool wnhrfcbktln = true;
	int hciomcxto = 7498;
	int ntwhsvs = 936;
	double bapxrycqftwkgxj = 50528;
	if (true == true) {
		int monvfpyc;
		for (monvfpyc = 78; monvfpyc > 0; monvfpyc--) {
			continue;
		}
	}
	if (7498 != 7498) {
		int uvwiwjtxw;
		for (uvwiwjtxw = 84; uvwiwjtxw > 0; uvwiwjtxw--) {
			continue;
		}
	}
	if (7498 == 7498) {
		int qyjrvose;
		for (qyjrvose = 33; qyjrvose > 0; qyjrvose--) {
			continue;
		}
	}
	return false;
}

string pvmqbih::ogvxjvdpkwycrfjjglfkzlsh(double jxsuihwwjiggwth, double iquux, bool kpyuetvsaabw, double khvimh, string egpdzddorrsiz, string tltkjtzzamqwpn, double xdpbdokcgkyunj, string inlnivndnygmyh, bool uzvcrpjweyjg) {
	bool ifadvlytlyzl = false;
	string tbjyztuleh = "nohqlfyfdwnudyhvpsiyedlvverdnbuvml";
	double gochslixgur = 47692;
	int lhfeuaisee = 850;
	int iqiaemui = 1434;
	bool yshod = true;
	bool efkfgxe = true;
	double qozlenkqybp = 53273;
	double dygccolyjztdhc = 58435;
	bool wjxxl = false;
	if (850 == 850) {
		int wzxox;
		for (wzxox = 69; wzxox > 0; wzxox--) {
			continue;
		}
	}
	if (1434 == 1434) {
		int qwax;
		for (qwax = 95; qwax > 0; qwax--) {
			continue;
		}
	}
	return string("");
}

int pvmqbih::ijygzsaqrtlbm(double ysdzcxj, int wjkoj, int mlzlfoumfgj) {
	string adamtvnj = "osibjvoyqcjrkkpqhbryklbieivgzatmrsddpajpnddafblyrtaibnczfaepsncnnpgrfntcg";
	bool gskvp = false;
	int qhyoujvfdl = 531;
	double rgmuu = 47329;
	string gbztxxpaarsgfyl = "rvgwmrbfxdpyaltnjzztagzlnxfnagcjeigfkxaudoiewteenabzhhqgzurmvolapszkrgmljzlqyvydggkdqapfiuokef";
	bool txublq = false;
	int nxzwrirq = 3792;
	double eyhejppmi = 268;
	if (268 == 268) {
		int zak;
		for (zak = 54; zak > 0; zak--) {
			continue;
		}
	}
	if (268 != 268) {
		int ryuclfu;
		for (ryuclfu = 56; ryuclfu > 0; ryuclfu--) {
			continue;
		}
	}
	return 7255;
}

double pvmqbih::teftcnkyciynvwkzxqpt(int lkelpb, double pmecxgonfiuxo, bool lsxhbklfxmgno, bool nfecdlkplkvz, bool xygrmke) {
	bool xhljfmpi = false;
	string pwbbkwfqysch = "ftqzhuafgqnggkflnuepmcimqbtcygubqnsjvlqvawqpzhaadsoomqhjfnurjywbfdxzugmjknkisjcguvqnd";
	string uliafet = "oywwddvthhyrtoioyctgblromttnpfewuyblwqaxyfqpeaqifqzpfjogsrbzyfvuvwhgssemuioorsjggzynitgfd";
	bool cbrktcsrshxoop = false;
	double tkvyofwgyul = 23677;
	string jbkbtgji = "xkujbffcokcoxzeuzm";
	if (string("ftqzhuafgqnggkflnuepmcimqbtcygubqnsjvlqvawqpzhaadsoomqhjfnurjywbfdxzugmjknkisjcguvqnd") != string("ftqzhuafgqnggkflnuepmcimqbtcygubqnsjvlqvawqpzhaadsoomqhjfnurjywbfdxzugmjknkisjcguvqnd")) {
		int ooxnc;
		for (ooxnc = 67; ooxnc > 0; ooxnc--) {
			continue;
		}
	}
	return 98046;
}

string pvmqbih::zosizvqfdaxghehxx(string uzktlii, string yfkogcnkywup, bool mxvwudclw, int fmiby, bool bkmpgnz) {
	int whlupxlul = 621;
	int wjsrrqvqmgl = 809;
	string vwizslxrbralho = "yoxuzzspircxbmewcrorpakswynzwnkwfoghabdyttbpqjjjyfitsokrortdc";
	bool gdmtlnumdl = false;
	int mizypfxbmd = 2234;
	if (string("yoxuzzspircxbmewcrorpakswynzwnkwfoghabdyttbpqjjjyfitsokrortdc") == string("yoxuzzspircxbmewcrorpakswynzwnkwfoghabdyttbpqjjjyfitsokrortdc")) {
		int zexqk;
		for (zexqk = 85; zexqk > 0; zexqk--) {
			continue;
		}
	}
	if (809 == 809) {
		int iogtlo;
		for (iogtlo = 38; iogtlo > 0; iogtlo--) {
			continue;
		}
	}
	if (621 != 621) {
		int uyxkuzbj;
		for (uyxkuzbj = 86; uyxkuzbj > 0; uyxkuzbj--) {
			continue;
		}
	}
	return string("gzeypxonbkezv");
}

int pvmqbih::jigtadcgoflzkrlinjhf(bool qvjeyzsiqbpu, bool qsbqdfgv, double suajwdspkqbxscw, string vcdsqqby) {
	bool hioaitqngirrrym = true;
	int ifzzjcjbelyiqvo = 1912;
	double ghdzwlwtbeyx = 35622;
	bool qlwfdosvoilz = false;
	int cpojar = 236;
	string yajdwdhzjqwb = "iiepggcgkjdamzzkfsodlxwdkarqogxskahhfdnxfwrzaqavmqjhpwmctansjtushpeenq";
	bool dmdqg = true;
	int qhurcfhnvjm = 1855;
	int qvcai = 781;
	if (string("iiepggcgkjdamzzkfsodlxwdkarqogxskahhfdnxfwrzaqavmqjhpwmctansjtushpeenq") == string("iiepggcgkjdamzzkfsodlxwdkarqogxskahhfdnxfwrzaqavmqjhpwmctansjtushpeenq")) {
		int dyomnu;
		for (dyomnu = 36; dyomnu > 0; dyomnu--) {
			continue;
		}
	}
	return 95439;
}

int pvmqbih::erlewnvjagmqnjmjcbqhnbvve(double cdivcmitucdg, double diirrmzcdnqjehj, bool smuqvulykj, bool rpjjplxdsktk, int edlrphkyh, bool wlhkgezarortg, int wpuetxus, int wbxaxtpzsxvxu, string sgjkimromgthb) {
	string vrapgkbon = "yaq";
	double acxifsxccmt = 12703;
	string qgstd = "zelqfnfwfkjrxnixcfjspucwewcxbly";
	double aryikwyaoloi = 47900;
	string rfermapdxfz = "wlfgtfwzplucarfda";
	bool ltysxxfgzfol = false;
	int hiqilkks = 2491;
	bool cmlijwgzmwwy = false;
	double yfdeggfsc = 20591;
	return 73473;
}

bool pvmqbih::chfyqpdxkm(bool lycrqity, int hulmanqbsj, bool dezxwovg) {
	double ikpdalqscba = 12989;
	return true;
}

void pvmqbih::yffohghuldtivc(double xfudixjafq, int xjzcagkrzqsop, int yojbsgysr, double nsarzj, int ereogo, int fafrazebobxe) {

}

bool pvmqbih::tfpwyxgdtnjvtiviafyymzpkw(bool edniuotgrpyv, string yzgmvnfzwecenx, string swbprp, string emzphxirlqh, bool geiehwxtecxkg, string pkyxqbptklc, string fzhmqbbvevwdst, string ezlbsqd) {
	string rttsefttingz = "cosxubfqavatzgjaidbtcasifpemuhiudizmshoxqptzsubuuzrpgqqbv";
	double sckngcmhzwk = 23138;
	return false;
}

int pvmqbih::pahydxqgyfa(string xtmrwbxbam, double lbsowddgxlus, bool qocobffrlkmzak, double uydsdljmqydsn, string ksdguxtlo, int ljlrcjijbkpfssr) {
	bool cjvowwboqvm = false;
	double ncezvlmzutr = 45581;
	int wfmwlhjwh = 2072;
	string gbjnxoij = "qkktmwnbavtrdotrmqdwzbjoabyjbmagbaqcmfppsjhl";
	bool kzmqxqmfvyze = false;
	string yxqwybjhdgoi = "sxntelltnrmrxnywmme";
	string ljwqmn = "rnxtzhnvurxgjtvzksbgncynghgstbyjxdeksmjazokfrwvjyzmqljlynkehn";
	return 79946;
}

int pvmqbih::afqrqyoujqejabfi(int okqygmncahgnjih, int jdmaffiauulfj, string hzunmg, double ijwcgpdsj, int mbsxnejijbzfba) {
	int jtqdqg = 1851;
	int uphovphzhfswag = 6784;
	if (6784 == 6784) {
		int neegu;
		for (neegu = 17; neegu > 0; neegu--) {
			continue;
		}
	}
	if (1851 == 1851) {
		int kafpicnpcp;
		for (kafpicnpcp = 96; kafpicnpcp > 0; kafpicnpcp--) {
			continue;
		}
	}
	if (1851 == 1851) {
		int gpjl;
		for (gpjl = 2; gpjl > 0; gpjl--) {
			continue;
		}
	}
	return 10095;
}
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
float LagFix()
{
	float updaterate = Interfaces::CVar->FindVar("cl_updaterate")->fValue;
	ConVar* minupdate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar* maxupdate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->fValue;

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->fValue;

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->fValue;
	ConVar* cmin = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->fValue != 1)
		ratio = clamp(ratio, cmin->fValue, cmax->fValue);


	return max(lerp, ratio / updaterate);
}
#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / Interfaces::Globals->interval_per_tick ) )
pvmqbih::pvmqbih() {
	this->ijygzsaqrtlbm(21280, 5559, 1940);
	this->teftcnkyciynvwkzxqpt(331, 17777, false, false, true);
	this->zosizvqfdaxghehxx(string("qdxwpfhsxlycqnypceqnhmpkfldaewyaexloresshxyzngxwocecccrmfynrrsylvqpnvnnqjzsrymkguyixd"), string("ovnpjepclgzojeoxrpulhndmymwylyagqdomzvtnabudujxyfyxlglvjmfupbjrlyjvatdohexgbbtbntpmnnbiztiwvfcxfbj"), true, 6393, false);
	this->jigtadcgoflzkrlinjhf(true, true, 8852, string("kfwsmguvjhrhuxlplxyqwrjrhldtqz"));
	this->erlewnvjagmqnjmjcbqhnbvve(3044, 23115, false, true, 7658, false, 5370, 151, string("xxqgmtvoxwdtgbxbypixvyjfmcqmmefibbpmuqwybakdhioiloymxyozpppyyjttfkvertzthourgruyj"));
	this->chfyqpdxkm(true, 1466, true);
	this->yffohghuldtivc(62072, 362, 225, 58847, 926, 1649);
	this->tfpwyxgdtnjvtiviafyymzpkw(true, string("nawanmnt"), string("vubsducpvototczprfwjhmwcxtzxmtneptpkcbnhlvwhgrlwlfdyolpibfnrsvrmbbjbl"), string("sur"), true, string("qmoqybeptzsfesaaxalmnmpzgvhanbedkevgxizgcgytxhzguyscmhbjjbpjroxypgtjxlecebvkmpqqexyqmcmfovgiglpheo"), string("byv"), string("ewimmzzkeuzpdwtbblnldcediiiwncl"));
	this->pahydxqgyfa(string("uqvybzasnjdjfzecclyimzhurbterlcpbvuzgtsgdpvttztjecmvyorbwcteivufurmgrihskxvjnbxqpymddkhsmim"), 21824, true, 64330, string("gstgqejqegoeuftabtpwjaxdppgaoppnzjqixgqgepwvinaucyfovqooiopxovupfobaubjn"), 5414);
	this->afqrqyoujqejabfi(3054, 1109, string("nhcvddtcprbcfjjewkqiqjlehfuqlpdwmkcmxdyygaaytezbzkwcplgswemexhuinuzgyylfbuinbnerhwiviqdgonczchqaged"), 13905, 1932);
	this->cormpnjbzjaqiu(11408, false, 1641, 23720, 1601, false);
	this->qifdforpezrumhbikiwuqrcqf(2643, false, 2186, false);
	this->ijgbefkledwuizuxlfx(true);
	this->drynfxrxrwgnlmb(string("vwxfnjwnbvgjlqrkijvywyjinuekhmbvidedqwaoluuydkkzcnrdjyxmggbwhtvigqwmdbqodplyxyqyfbiwohwuzzexfsg"), true, string("lbxrrbmbhadactleovlasirnaialoejbahlukkbyuyvaansdgxvlpwcvnjaksbjmtqroqnpqqdxsnwkyczbex"), 12559, string("oydnatjtazjdbzqqhqikcocs"), 8301);
	this->ogvxjvdpkwycrfjjglfkzlsh(9180, 36053, true, 1607, string("cncvcxyercmcwsswtukpdzyybtgxyfyokzwwcebzbffjgtgvzajsrqqlhenjmnolvwgxbzksigkxseuhux"), string("eojchejgp"), 26205, string("qejgzmojjyfibhitloyfdosenkltbibmcmjyaabd"), false);
	this->qtrsqcqcybbwfrayyu(true, string("hmhjmhcovkgjczbcpdlddtqycoqxpigwomzvjvenpvcbxbtdmbfoctrrtfqwdugtloycahddpxlgfepvbaqazihfauvwhq"), string("tgggjwsykfkmmamvqzdm"), false, 2949, 8375, 2259);
	this->vipgscuozvdsfatitehdfbtw(string("hdiwmouxoxtuqepiacaxwiurmtapucegptiqsdijdabachhojzxnsseekewrcagzmikmoeixwl"), string("jfkdzmhkcijxinnsevuezgavcjrtpexxdlbntxwziememvkvzpnlxlljqsbxvibteywmxocpjguperfzmmwkxgbkukwbw"), string("rlsljebtgqlintojtqqogotcueixdnqujqyvxrnooadhrmxushvmx"), 42264, true, string("wsugocpptdjzqajykyveelwopkv"), 2696);
	this->rfpkhldaxxfbn(15012, 991, string("iibjbqdpdlffqmjfyzagufszyeabivaaarsxaszokjzyqjdrzoadykmnzvcyttgbuzapyndypbkimqjrgaezq"), 1370, string("bfjsepeabbenedbwrblmgqzztvcvislnvjobiyxbygqdbbnnueanmvlxulcbubntweuqudfdrrubdiaqttnenolqquqpocz"), 58694, 10652, 1830);
}





std::string FindWeaponName(std::string name)
{
	std::string Name = name;
	//Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	//-- Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

std::string C4 = "C4";
std::string Nade = "HEGrenade";
std::string Flash = "Flashbang";
std::string Smoke = "SmokeGrenade";
std::string Fire = "IncendiaryGrenade";
std::string Molo = "MolotovGrenade";
std::string Decoy = "DecoyGrenade";
std::string Knife = "Knife";
void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}
Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}
void CRageBot::Draw()
{

}
int AimHeight;
void CRageBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (pCmd->buttons & IN_USE)
		return;

	if (Menu::Window.RageBotTab.AimType.GetIndex() == 0)
	{
		// Master switch
		//if (!Menu::Window.RageBotTab.Active.GetState())
		//return;

		// Anti-Aim
		if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
			DoAntiAim(pCmd, bSendPacket);




		// Aimbot
		if (Menu::Window.RageBotTab.AimbotEnable.GetState())
			DoAimbot(pCmd, bSendPacket);

		// No Spread
		//if (Menu::Window.RageBotTab.Postion.GetState())
			//DoNoSpread(pCmd);

		AimHeight = Menu::Window.RageBotTab.PointScale.GetValue();

		// Recoil
		if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
			DoNoRecoil(pCmd);
		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}

		// Aimstep
		if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
		{
			Vector AddAngs = pCmd->viewangles - LastAngle;
			if (AddAngs.Length2D() > 25.f)
			{
				Normalize(AddAngs, AddAngs);
				AddAngs *= 25;
				pCmd->viewangles = LastAngle + AddAngs;
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
			}
		}

		LastAngle = pCmd->viewangles;
	}
	else
	{
		Rage2Bot* AimbotPowerKappa;
		AimbotPowerKappa->doAim(pCmd, Menu::Window.RageBotTab.AimbotEnable.GetState(), Menu::Window.RageBotTab.AimbotAutoPistol.GetState(), Menu::Window.RageBotTab.AimbotAutoFire.GetState(), false);
		if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
			DoAntiAim(pCmd, bSendPacket);

		AimHeight = Menu::Window.RageBotTab.PointScale.GetValue();

		// Recoil
		if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
			DoNoRecoil(pCmd);
		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}
bool Ready2Silent(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	float serverTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
	if (pWeapon->GetNextPrimaryAttack() > serverTime)
		return false;

	return true;
}
bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

//void CRageBot::NormalizeVector(Vector& vec)
//{
//	for (int i = 0; i < 3; ++i)
//	{
//		while (vec[i] > 180.f)
//			vec[i] -= 360.f;
//
//		while (vec[i] < -180.f)
//			vec[i] += 360.f;
//	}
//
//	vec[2] = 0.f;
//}
//
//void CRageBot::ClampAngles(Vector& vecAngles)
//{
//	if (vecAngles[0] > 89.f)
//		vecAngles[0] = 89.f;
//	if (vecAngles[0] < -89.f)
//		vecAngles[0] = -89.f;
//	if (vecAngles[1] > 180.f)
//		vecAngles[1] = 180.f;
//	if (vecAngles[1] < -180.f)
//		vecAngles[1] = -180.f;
//
//	vecAngles[2] = 0.f;
//}

bool isAbleToShot(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	int iTickBase = pLocal->GetTickBase();
	float flNextPrimaryAttack = pWeapon->GetNextPrimaryAttack();
	float flCurTime = iTickBase * Interfaces::Globals->interval_per_tick;
	return(!(flNextPrimaryAttack > flCurTime));
}

int TickDelay = 0;
bool Switch = false;

void CRageBot::DoNoSpread(CUserCmd *pCmd)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (GameUtils::IsBallisticWeapon(pWeapon))
	{
		NoSpread.RollSpread(pWeapon, pCmd->random_seed, pCmd, pCmd->viewangles);
	}
}

Vector TickPrediction(Vector AimPoint, IClientEntity* pTarget)
{
	return AimPoint + (pTarget->GetVelocity() * Interfaces::Globals->interval_per_tick);
}

// Functionality
void CRageBot::DoAimbot(CUserCmd *pCmd, bool& bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;
	//IsLocked = false;

	std::vector<std::string> InfoToAimbot;
	IClientEntity* pWeapon2 = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pLocal->GetActiveWeaponHandle());
	if (pWeapon2)
	{
		ClientClass* cClass2 = (ClientClass*)pWeapon2->GetClientClass();
		if (cClass2)
		{
			// Draw it
			InfoToAimbot.push_back(FindWeaponName(cClass2->m_pNetworkName));
			for (auto MyWeapon2 : InfoToAimbot)
			{
				if (MyWeapon2 == Knife || MyWeapon2 == C4 || MyWeapon2 == Fire || MyWeapon2 == Molo || MyWeapon2 == Smoke || MyWeapon2 == Nade || MyWeapon2 == Flash || MyWeapon2 == Decoy)
					return;
			}
		}
	}

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	bool bReady2Silent = Ready2Silent(pLocal, pWeapon);
	if (pWeapon)
	{
		if (!bReady2Silent && bSendPacket == false)
			bSendPacket = true;
		if (!GameUtils::IsSniper(pWeapon)) {
			Switch = false;
			TickDelay = 0;
		}

		if (TickDelay)
			TickDelay--;

		if (Switch) {
			if (!TickDelay)
				Switch = false;

			if (!isAbleToShot)
				Switch = false;
		}

		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon) || !isAbleToShot(hackManager.pLocal(), pWeapon))
		{
			-IN_ATTACK;
			return;
		}
	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV <= Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		// Target selection type
		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		}

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	Globals::Target = pTarget;
	Globals::TargetID = TargetID;

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		// Key
		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector Point;

			Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, AimHeight);


		if (Menu::Window.RageBotTab.MultiPoint.GetState())
		{
			Point = BestPoint(pTarget, AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		if (Menu::Window.RageBotTab.PerfectAccuracy.GetState())
			Point = TickPrediction(Point, pTarget);

		float Spread = pWeapon->GetAccuracyPenalty();
		//float HitChance = 75.f + (Menu::Window.RageBotTab.HitChanceAmount.GetValue() / 4);


		if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
		{
			FindNewTarget = false;
			IsLocked = true;
			if (Menu::Window.RageBotTab.AccuracyAutoStop.GetIndex() == 1)
			{
				if (pCmd->buttons & IN_FORWARD)
				{
					pCmd->forwardmove = 73;
				}

				if (pCmd->buttons & IN_BACK)
				{
					pCmd->forwardmove = -73;
				}

				if (pCmd->buttons & IN_MOVERIGHT)
				{
					pCmd->sidemove = 73;
				}

				if (pCmd->buttons & IN_MOVELEFT)
				{
					pCmd->sidemove = -73;
				}
			}
			if (Menu::Window.RageBotTab.AccuracyAutoStop.GetIndex() == 2)
			{
				pCmd->forwardmove = 0.f;
				pCmd->sidemove = 0.f;
			}
			if (Menu::Window.RageBotTab.AccuracyAutoCrouch.GetState() && !pLocal->IsDormant())
			{
				pCmd->buttons |= IN_DUCK;
			}
			if (Menu::Window.RageBotTab.AccuracyAutoScope.GetState() && !pLocal->IsScoped() && GameUtils::IsSniper(pWeapon))
			{
				pCmd->buttons |= IN_ATTACK2;

				if (!Switch) {
					if (isAbleToShot(pLocal, pWeapon)) {
						Switch = true;
						TickDelay = 16;
					}
				}
			}
			if (!Menu::Window.RageBotTab.AccuracySpreadLimit.GetState() || Spread <= (Menu::Window.RageBotTab.AccuracyMinimumSpread.GetValue() / 100.f))
				//if (!Menu::Window.RageBotTab.HitChance.GetState() || (1.0f - pWeapon->GetAccuracyPenalty()) * 100.f >= HitChance)
			{
				if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					GameUtils::NormaliseViewAngle(pCmd->viewangles);
					IsLocked = true;
					if (Menu::Window.RageBotTab.AccuracyAutoScope.GetState() && GameUtils::IsSniper(pWeapon)) {
						if (TickDelay == 0) {

							pCmd->buttons |= IN_ATTACK;
							if (pCmd->buttons & IN_ATTACK)
								Globals::Shots += 1;
						}
					}
					else {
						pCmd->buttons |= IN_ATTACK;
						if (pCmd->buttons & IN_ATTACK)
							Globals::Shots += 1;
					}

				}
			}
				if (Menu::Window.RageBotTab.EnginePred.GetState())
	{
		pCmd->tick_count = TIME_TO_TICKS(LagFix());
	}

		}

		// Auto Pistol
		static bool WasFiring = false;
		CSWeaponInfo WeaponInfo = pWeapon->GetCSWpnData(pWeapon, 456);
		//if (!WeaponInfo.m_IsFullAuto && Menu::Window.RageBotTab.AimbotAutoPistol.GetState())
		//{
		//	if (pCmd->buttons & IN_ATTACK)
		//	{
		//		if (WasFiring)
		//		{
		//			pCmd->buttons &= ~IN_ATTACK;
		//		}
		//	}

		//	WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
		//}
		if (!bReady2Silent && !(pCmd->buttons & IN_ATTACK))
			bSendPacket = true;
	}

}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				// Spawn Check
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 360.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	// Target selection
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{
	// Target selection
	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov <= Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

//class QAngleByValue;
//class QAngle
//{
//public:
//	float x, y, z;
//
//	QAngle(void);
//	QAngle(float X, float Y, float Z);
//
//	operator QAngleByValue &() { return *((QAngleByValue *)(this)); }
//	operator const QAngleByValue &() const { return *((const QAngleByValue *)(this)); }
//
//	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
//	void Random(float minVal, float maxVal);
//
//	bool IsValid() const;
//	void Invalidate();
//
//	bool IsZero()
//	{
//		CHECK_VALID(*this);
//		if (this->x == 0.f && this->y == 0.f && this->z == 0.f)
//			return true;
//
//		return false;
//	}
//
//	float operator[](int i) const;
//	float& operator[](int i);
//
//	float* Base();
//	float const* Base() const;
//
//	bool operator==(const QAngle& v) const;
//	bool operator!=(const QAngle& v) const;
//
//	QAngle& operator+=(const QAngle &v);
//	QAngle& operator-=(const QAngle &v);
//	QAngle& operator*=(float s);
//	QAngle& operator/=(float s);
//
//	float   Length() const;
//	float   LengthSqr() const;
//
//	QAngle& operator=(const QAngle& src);
//
//	QAngle  operator-(void) const;
//
//	QAngle  operator+(const QAngle& v) const;
//	QAngle  operator-(const QAngle& v) const;
//	QAngle  operator*(float fl) const;
//	QAngle  operator/(float fl) const;
//
//	QAngle Clamp();
//	QAngle Mod(float N);
//
//	inline QAngle QAngle::Normalize()
//	{
//		QAngle vector;
//		float length = this->Length();
//
//		if (length != 0) {
//			vector.x = x / length;
//			vector.y = y / length;
//			vector.z = z / length;
//		}
//		else
//			vector.x = vector.y = 0.0f; vector.z = 1.0f;
//
//		return vector;
//	}
//
//	QAngle Normalized()
//	{
//		if (this->x != this->x)
//			this->x = 0;
//		if (this->y != this->y)
//			this->y = 0;
//		if (this->z != this->z)
//			this->z = 0;
//
//		if (this->x > 89.f)
//			this->x = 89.f;
//		if (this->x < -89.f)
//			this->x = -89.f;
//
//		while (this->y > 180)
//			this->y -= 360;
//		while (this->y <= -180)
//			this->y += 360;
//
//		if (this->y > 180.f)
//			this->y = 180.f;
//		if (this->y < -180.f)
//			this->y = -180.f;
//
//		this->z = 0;
//
//		return *this;
//	}
//};

int CRageBot::GetTargetHealth()
{
	// Target selection
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov <= Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
					
				}
			}
		}
	}

	return target;
}




void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	// Ghetto rcs shit, implement properly later
	std::vector<std::string> InfoToRcs;
	IClientEntity* rcsweapon  = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pLocal->GetActiveWeaponHandle());
	if (rcsweapon)
	{
		ClientClass* RCSClass = (ClientClass*)rcsweapon->GetClientClass();
		if (RCSClass)
		{
			// Draw it
			InfoToRcs.push_back(FindWeaponName(RCSClass->m_pNetworkName));
			for (auto rcsweapon2 : InfoToRcs)
			{
				if (rcsweapon2 == Knife || rcsweapon2 == C4 || rcsweapon2 == Fire || rcsweapon2 == Molo || rcsweapon2 == Smoke || rcsweapon2 == Nade || rcsweapon2 == Flash || rcsweapon2 == Decoy || rcsweapon2 == "AWP")
					return;
			}
		}
	}

	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		{
			pCmd->viewangles -= AimPunch * 2;
			if (Menu::Window.MiscTab.OtherSafeMode.GetState())
			{
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
			}
		}
	}
}













// Credits to Valve and Shad0w
#pragma once



bool fixmove;






int CRageBot::HitScan(IClientEntity* pEntity)
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar > 1.00)
	{
		std::vector<int> HitBoxesToScan;
		bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();

		// Get the hitboxes to scan
#pragma region GetHitboxesToScan
		int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
		int HitBox = Menu::Window.RageBotTab.TargetHitbox.GetIndex();
		if (HitScanMode == 0)
		{
			// No Hitscan, just a single hitbox	
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				break;
			}
		}
		else if (HitScanMode == 1)
		{
			if (HitBox == 0)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			}
			else if (HitBox == 1)
			{

				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			}
			else if (HitBox == 2)
			{

				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			}
			else if (HitBox == 3)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			}
		}
		else if (HitScanMode == 2)
		{
			if (HitBox == 0)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);

				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			}
			else if (HitBox == 1)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);

				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			}
			else if (HitBox == 2)
			{

				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			}
			else if (HitBox == 3)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			}
		}
		else if (HitScanMode == 3)
		{
			if (HitBox == 0)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			}
			else if (HitBox == 1)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			}
			else if (HitBox == 2)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			}
			else if (HitBox == 3)
			{
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			}
		}


#pragma endregion Get the list of shit to scan

		// check hits
		for (auto HitBoxID : HitBoxesToScan)
		{
			if (AWall)
			{
				if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				{
					return HitBoxID;
				}
				else
				{
				IClientEntity *pLocal = hackManager.pLocal();
				//CSWeaponInfo weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData(pWeapon, 456);
				Vector Point = GetHitboxPosition(pEntity, HitBoxID);
				Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
				float Damage = (Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue() / 01);
				Color c = Color(255, 255, 255, 255);
				if (!GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				{
					if (CanHit(Point, &Damage))
					{
						c = Color(0, 255, 0, 255);
						if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue() / 01)
						{
							return HitBoxID;
						}
					}
				}
				else
				{
					return HitBoxID;
				}
			}
		}

			else
			{
				if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
					return HitBoxID;
			}
		}
	}
	return -1;
}


bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool& bSendPacket)
{
	bool ReturnValue = false;
	// Get the full angles
	if (point.Length() == 0) return ReturnValue;
	Vector PointScaleangle;
	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();
	PointScaleangle = point;

	CalcAngle(src, PointScaleangle, angles);
	GameUtils::NormaliseViewAngle(angles);

	/*
	WTF IS THIS STUPID CHECK? OMG

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
	return ReturnValue;
	}
	*/

	IsLocked = true;
	//-----------------------------------------------

	// Aim Step Calcs
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; // Don't just use the viewangs because you need to consider aa

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AccuracySpreadLimit.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}


	GameUtils::NormaliseViewAngle(angles);

	if (Menu::Window.RageBotTab.AimbotPSilent.GetIndex() == 1)
	{
		pCmd->viewangles = angles;
	}

	if (!Menu::Window.RageBotTab.AimbotPSilent.GetIndex() == 1 || !Menu::Window.RageBotTab.AimbotPSilent.GetIndex() == 2)
		Interfaces::Engine->SetViewAngles(angles);
	Vector Oldview = pCmd->viewangles;

	if (Menu::Window.RageBotTab.AimbotPSilent.GetIndex() == 2)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;

		if (ChokedPackets < 6)
		{
			bSendPacket = false;
			pCmd->viewangles = angles;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles = Oldview;
			ChokedPackets = -1;
			ReturnValue = false;
		}

		//pCmd->viewangles.z = 0;
	}
	LastAngle = pCmd->viewangles;
	return ReturnValue;
}
namespace AntiAims
{
	// Pitches
	void StaticPitch(CUserCmd *pCmd)
	{
		// Down//
		pCmd->viewangles.x = 89.f;
	}

	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up) pCmd->viewangles.x = -89.f;
		else pCmd->viewangles.x = 89.f;
		up = !up;
	}
	void Emotion2(CUserCmd *pCmd, bool& bSendPacket) {
		pCmd->viewangles.x = -89.f;
		if (bSendPacket)
			pCmd->viewangles.x = 89.f;
	}
	void clickbait(CUserCmd *pCmd)
	{
		{
			int random = rand() % 100;
			int random2 = rand() % 1000;

			static bool dir;
			static float current_x = pCmd->viewangles.x;

			if (random == 1) dir = !dir;

			if (dir)
				current_x += 700;
			else
				current_x -= 34;

			pCmd->viewangles.x = current_x;

			if (random == random2)
				pCmd->viewangles.x += random;

		}

	}
	void FakeBackward(CUserCmd *pCmd, bool& bSendPacket)
	{
		bool y1 = false;
		if (y1)
		{

			pCmd->viewangles.y += 180;
			bSendPacket = false;
			y1 = false;
		}
		else if (!y1)
		{

			pCmd->viewangles.y -= 90;
			bSendPacket = true;
			y1 = true;
		}

	}
	// Yaws
	void FastSpint(CUserCmd *pCmd)
	{
		static bool switchaa = true;
		if (switchaa)
		{
			//bSendPacket = true;
			Vector FastSpin;
			FastSpin.y = (Interfaces::Globals->curtime * 5000.0);
			GameUtils::ClampViewAngle(FastSpin);
			pCmd->viewangles.y = FastSpin.y;
		}
	}
	void FastSpintFake(CUserCmd *pCmd, bool& bSendPacket)
	{
		static bool switchaa = true;
		if (switchaa)
		{
			bSendPacket = true;
			Vector FastSpin;
			FastSpin.y = (Interfaces::Globals->curtime * 5000.0);
			GameUtils::ClampViewAngle(FastSpin);
			pCmd->viewangles.y = FastSpin.y;
			switchaa = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y += 180;
			switchaa = true;
		}

	}

	void SlowSpin(CUserCmd *pCmd)
	{
		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 10;
		else
			current_y -= 10;

		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;

	}

	void BackJitter(CUserCmd *pCmd)
	{
		int random = 179 - rand() % 100;
		static float current_y = pCmd->viewangles.y;
		current_y += random;
		pCmd->viewangles.y = current_y;

	}
	void MoveFix(CUserCmd *cmd, Vector &realvec)
	{
		Vector vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector vMove2;
		VectorAngles(vMove, vMove2);

		flYaw = DEG2RAD(cmd->viewangles.y - realvec.y + vMove2.y);
		cmd->forwardmove = cos(flYaw) * flSpeed;
		cmd->sidemove = sin(flYaw) * flSpeed;

		if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
			cmd->forwardmove = -cmd->forwardmove;
	}

	void Flip(CUserCmd *pCmd)
	{
		static bool back = false;
		back = !back;
		if (back)
			pCmd->viewangles.y -= rand() % 100;
		else
			pCmd->viewangles.y += rand() % 100;

	}

	void Backwards(CUserCmd *pCmd)
	{
		static bool back = false;
		back = !back;
		if (back)
			pCmd->viewangles.y -= 180.0f;
		else
			pCmd->viewangles.y += 180.0f;
	}
	void JitterLispPitch(CUserCmd *pCmd)
	{
		int aa = 1;
		if (aa == 1)
		{
			pCmd->viewangles.x += 18000000089;
			aa = 2;
		}
		else if (aa == 2)
		{
			pCmd->viewangles.x -= 18000000089;
			aa = 3;
		}
		else if (aa == 3)
		{
			pCmd->viewangles.x += 3600000089.000000;
			aa = 4;
		}
		else if (aa == 4)
		{
			pCmd->viewangles.x = -3600000089.000000;
			aa = 1;
		}

	}
	void SideLisp(CUserCmd* cmd)
	{
		static int switchy = 1;
		if (switchy == 1)
		{
			cmd->viewangles.y -= 9000000090.000000090;
			switchy = 2;
		}
		else if (switchy == 2)
		{
			cmd->viewangles.y += 9000000090.000000090;
			switchy = 3;
		}
		else if (switchy == 3)
		{
			cmd->viewangles.y += 27000000270.000000270;
			switchy = 4;
		}
		else if (switchy == 4)
		{

			cmd->viewangles.y -= 27000000270.000000270;
			switchy = 1;
		}
	}

	void pFakeRandom(CUserCmd *cmd, bool& bSendPacket)
	{
		static bool faked;
		if (faked)
		{
			bSendPacket = true;
			cmd->viewangles.y = rand() % 360;
			faked = false;
		}
		else
		{
			bSendPacket = false;
			if (Menu::Window.RageBotTab.RealRandom.GetIndex() == 0)//Forward
			{

			}
			if (Menu::Window.RageBotTab.RealRandom.GetIndex() == 1)//Backward
			{
				cmd->viewangles += 180;
			}
			if (Menu::Window.RageBotTab.RealRandom.GetIndex() == 2)//Left
			{
				cmd->viewangles += 90;
			}
			if (Menu::Window.RageBotTab.RealRandom.GetIndex() == 3)//Right
			{
				cmd->viewangles -= 90;
			}
			faked = true;
		}
	}


	void FakeHead(CUserCmd *pCmd)
	{
		static bool ySwitch = false;
		int veloc = hackManager.pLocal()->GetVelocity().Length();

		if (veloc == 1)
		{
			veloc = 255;
		}

		if (ySwitch)
			pCmd->viewangles.y -= 1003004000 * veloc;
		else
			pCmd->viewangles.y += 1801188099 * veloc;

		ySwitch = !ySwitch;
	}
	void AntiLisp(CUserCmd *pCmd)
	{
		int veloc1 = hackManager.pLocal()->GetVelocity().Length();
		if (veloc1 == 1)
		{
			veloc1 = 255;
		}
		pCmd->viewangles.y += 600030020120 * veloc1;
	}
	void FakeJitter(CUserCmd *pCmd, bool& bSendPacket)
	{
		static bool ySwitch = false;
		if (ySwitch)
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 150;
			ySwitch = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y += 150;
			ySwitch = true;
		}

	}
	void FakeJitter2(CUserCmd *pCmd)
	{
		static bool ySwitch = false;
		if (ySwitch)
		{
			//bSendPacket = true;
			pCmd->viewangles.y -= 150;
			ySwitch = false;
		}
		else
		{
			//bSendPacket = false;
			pCmd->viewangles.y += 150;
			ySwitch = true;
		}

	}
	void fakeback(CUserCmd *pCmd, bool bSendPacket)
	{
		static int ticks;
		bool faked;
		bSendPacket = true;
		if (!bSendPacket)
		{ 
	ticks++;
			pCmd->viewangles.y += 90;
			faked = false;
		}
		else
		{
ticks++;
			pCmd->viewangles.y += 180;
			faked = true; 
		}
		if (ticks > 100)
		{
			bSendPacket != bSendPacket;
			ticks = 0;
		}
		
	}

	
	
	void fakeyawlisp(CUserCmd *pCmd)
	{
		static bool y = false;
		if (y)
		{
			float urmom1 = pCmd->viewangles.y = 241802833559552.f;
			pCmd->viewangles.y = urmom1;
		}
		else
		{
			float urmom1 = pCmd->viewangles.y = 2255955338208142.f;
			pCmd->viewangles.y = urmom1;
		}
		y = !y;
	}
	void CantTouch(CUserCmd *cmd)
	{
		static bool ySwitch = false;

		if (ySwitch)
			cmd->viewangles.y += 323210000;
		else
			cmd->viewangles.y -= 323210000;

		ySwitch = !ySwitch;
	}
	void FakeZeroEmotion(CUserCmd *cmd, bool& bSendPacket)
	{
		static bool ySwitch = false;
		if (ySwitch)
		{
			bSendPacket = true;
			cmd->viewangles.x = 89.f;
			ySwitch = false;
		}
		else
		{
			bSendPacket = false;
			cmd->viewangles.x = 0;
			ySwitch = true;
		}
	}

	void FakeUPMM(CUserCmd *cmd, bool& bSendPacket)
	{
		static bool ySwitch = false;
		if (ySwitch)
		{
			bSendPacket = true;
			cmd->viewangles.x = 89.f;
			ySwitch = false;
		}
		else
		{
			bSendPacket = false;
			cmd->viewangles.x = -89.f;
			ySwitch = true;
		}
	}
	void FakeZerooo(CUserCmd *cmd, bool& bSendPacket)
	{
		static bool ySwitch = false;
		if (ySwitch)
		{
			bSendPacket = true;
			cmd->viewangles.y = 180.f;
			ySwitch = false;
		}
		else
		{
			bSendPacket = false;
			cmd->viewangles.y = 0.f;
			ySwitch = true;
		}
	}
	void LispDown(CUserCmd *cmd, bool down)
	{
		if (down)
		{
			cmd->viewangles.x = 18000000089;
		}
		else {
			cmd->viewangles.x = -18000000089;
		}
	}


	//=============pFAKE SPINS KIPPO CREDITS: AYYGOR
	void FakeSpinLeft(CUserCmd* pCmd, bool& bSendPacket)
	{
		static bool faked;
		if (faked)
		{
			bSendPacket = true;
			Vector FastSpin;
			FastSpin.y = (Interfaces::Globals->curtime * 500.0);
			GameUtils::ClampViewAngle(FastSpin);
			pCmd->viewangles.y = FastSpin.y;
			faked = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y -= 90;
			faked = true;
		}
	}

	void FakeSpinRight(CUserCmd* pCmd, bool& bSendPacket)
	{
		static bool faked;
		if (faked)
		{
			bSendPacket = true;
			Vector FastSpin;
			FastSpin.y = (Interfaces::Globals->curtime * 800.0);
			GameUtils::ClampViewAngle(FastSpin);
			pCmd->viewangles.y = FastSpin.y;
			faked = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y += 90;
			faked = true;
		}
	}

	//=====================================================END OF pSLIENTSPINS KAPPA====================================
	void CorrectFakeDown(CUserCmd* pCmd) {
		if (GetAsyncKeyState('W'))
			pCmd->forwardmove = -450.f;
		if (GetAsyncKeyState('S'))
			pCmd->forwardmove = 450.f;
	}
	void CorrectMovementAngle(CUserCmd* pCmd)
	{
#define YawAA Menu::Window.RageBotTab.AntiAimYaw.GetIndex()
		//if (RageBotAntiAim.AntiAimYaw.GetIndex() == 3) //SideWays Correct
		//{
		//	if (GetAsyncKeyState('W'))
		//		pCmd->forwardmove = -450.f;
		//	if (GetAsyncKeyState('S'))
		//		pCmd->forwardmove = 450.f;
		//}
		//if (RageBotAntiAim.AntiAimYaw.GetIndex() == 1 || RageBotAntiAim.AntiAimYaw.GetIndex() == 2) //BackWard & FakeBackWard Correct
		//{
		//	if (GetAsyncKeyState('W'))
		//		pCmd->forwardmove = -450.f;
		//	if (GetAsyncKeyState('S'))
		//		pCmd->forwardmove = 450.f;
		//}


		//Here Fix For Down and other aa's. Credits : SynRaw, AyyGoR
		Vector FixAngle = pCmd->viewangles;

		if (YawAA > 0 && YawAA != 7)
		{
			if (GetAsyncKeyState('W'))
				pCmd->forwardmove = -450.f;
			if (GetAsyncKeyState('S'))
				pCmd->forwardmove = 450.f;
		}
		else if (YawAA == 7)
		{
			if (GetAsyncKeyState('W'))
				pCmd->forwardmove = -450.f;
			if (GetAsyncKeyState('S'))
				pCmd->forwardmove = 450.f;
			if (GetAsyncKeyState('D'))
				pCmd->sidemove = -450.f;
			if (GetAsyncKeyState('A'))
				pCmd->sidemove = 450.f;
		}

	}
	void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
	{
		//side/forward move correction
		float deltaView = pCmd->viewangles.y - vOldAngles.y;
		float f1;
		float f2;

		if (vOldAngles.y < 0.f)
			f1 = 360.0f + vOldAngles.y;
		else
			f1 = vOldAngles.y;

		if (pCmd->viewangles.y < 0.0f)
			f2 = 360.0f + pCmd->viewangles.y;
		else
			f2 = pCmd->viewangles.y;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);
		deltaView = 360.0f - deltaView;

		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	}

}
Vector localPlyViewPos;
void aimAA(CUserCmd *pCmd) {

	float len, closest = 8192;
	Vector pos;
	for (int i = 1; i <= Interfaces::Globals->maxClients; i++) {
		IClientEntity *ent = Interfaces::EntList->GetClientEntity(i);


		pos = ent->GetHeadPos();
		if (len > closest)
			continue;

		closest = len;
	}

	pCmd->viewangles.y = pos.y;
}

//bool EdgeAntiAim(IClientEntity *localPlayer, CUserCmd *cmd, float flWall, float flCornor)
//{
//	static CTraceFilter filter;
//
//	bool ret = false;
//
//	Vector localPosition = localPlayer->GetEyePosition();
//
//	for (int y = 0; y < 360; y++)
//	{
//		Vector tmp(10.0f, cmd->viewangles.y, 0.0f);
//		tmp.y += y;
//		Normalize(tmp, tmp);
//
//		Vector forward;
//		AngleVectors(tmp, &forward);
//
//		float length = ((16.0f + 3.0f) + ((16.0f + 3.0f) * sin(10.0f * PI / 180))) + 7.0f;
//		forward *= length;
//
//		Ray_t ray;
//		trace_t traceData;
//
//		ray.Init(localPosition, (localPosition + forward));
//		Interfaces::Trace->TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &traceData);
//
//		if (traceData.fraction != 1.0f)
//		{
//			Vector angles;
//			Vector Negate = traceData.plane.normal;
//
//			Negate *= -1;
//			VectorAngles(Negate, angles);
//
//			tmp.y = angles.y;
//			Normalize(tmp, tmp);
//			trace_t leftTrace, rightTrace;
//
//			Vector left, right;
//			AngleVectors(tmp + Vector(0.0f, 30.0f, 0.0f), &left);
//			AngleVectors(tmp - Vector(0.0f, 30.0f, 0.0f), &right);
//
//			left *= (length + (length * sin(30.0f * PI / 180)));
//			right *= (length + (length * sin(30.0f * PI / 180)));
//
//			ray.Init(localPosition, (localPosition + left));
//			Interfaces::Trace->TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &leftTrace);
//
//			ray.Init(localPosition, (localPosition + right));
//			Interfaces::Trace->TraceRay(ray, 0x200400B, (CTraceFilter*)&filter, &rightTrace);
//
//			if ((leftTrace.fraction == 1.0f)
//				&& (rightTrace.fraction != 1.0f))
//			{
//				tmp.y -= flCornor;
//				// LEFT
//			}
//			else if ((leftTrace.fraction != 1.0f)
//				&& (rightTrace.fraction == 1.0f))
//			{
//				tmp.y += flCornor;
//				// RIGHT
//			}
//
//			cmd->viewangles.y = tmp.y;
//			//cmd->viewangles.y -= flWall;
//			ret = true;
//		}
//	}
//
//	return ret;
//}

void VectorAngles3D(const Vector&vecForward, Vector&vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}

void AtTarget(IClientEntity *Target, CUserCmd *pCmd) {
	if (!Target)
		return;

	if ((Target->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) || Target->IsDormant() || !Target->IsAlive() || Target->GetHealth() <= 0)
		return;

	Vector TargetPosition = Target->GetEyePosition();
	CalcAngle(hackManager.pLocal()->GetEyePosition(), TargetPosition, pCmd->viewangles);
}

//CTraceFilterNoPlayer tfNoPlayers;
//
//void ClampAngles(Vector& vecAngles)
//{
//	if (vecAngles[0] > 89.f)
//		vecAngles[0] = 89.f;
//	if (vecAngles[0] < -89.f)
//		vecAngles[0] = -89.f;
//	if (vecAngles[1] > 180.f)
//		vecAngles[1] = 180.f;
//	if (vecAngles[1] < -180.f)
//		vecAngles[1] = -180.f;
//
//	vecAngles[2] = 0.f;
//}
//
//void vectorAnglesVec(Vector forward, Vector &angles)
//{
//	if (forward[1] == 0.0f && forward[0] == 0.0f)
//	{
//		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
//		angles[1] = 0.0f;
//	}
//	else
//	{
//		float len2d = sseSqrt((forward[0] * forward[0]) + (forward[1] * forward[1]));
//
//		angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
//		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));
//
//		if (angles[0] < 0.0f) angles[0] += 360.0f;
//		if (angles[1] < 0.0f) angles[1] += 360.0f;
//	}
//
//	angles[2] = 0.0f;
//}
//void EdgeAntiAim2(IClientEntity *pLocal, float &fYawOut) {
//	Vector vEyePos = pLocal->GetEyePosition();
//
//	ITraceFilter* tracefilter;
//
//	for (int y = 0; y < 360; y++)
//	{
//		Vector qTmp(10.0f, fYawOut, 0.0f);
//		qTmp.y += y;
//
//		if (qTmp.y > 180.0)
//			qTmp.y -= 360.0;
//		else if (qTmp.y < -180.0)
//			qTmp.y += 360.0;
//
//		GameUtils::ClampViewAngle(qTmp);
//
//		Vector vForward;
//
//		AngleVectors2(qTmp, vForward);
//
//		float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
//		vForward *= fLength;
//
//		trace_t trData;
//
//		Vector vTraceEnd = vEyePos + vForward;
//
//		Ray_t ray;
//
//		ray.Init(vEyePos, vTraceEnd);
//		Interfaces::Trace->TraceRay(ray, 0x4600400B, tracefilter, &trData);
//
//
//		if (trData.fraction != 1.0f)
//		{
//			Vector angles;
//
//			Vector vNegative = Vector(trData.plane.normal.x * -1.0f, trData.plane.normal.y * -1.0f, trData.plane.normal.z * -1.0f);
//
//			AngleVectors2(vNegative, angles);
//
//			NormalizeVector(angles);
//
//			qTmp.y = angles.y;
//
//			GameUtils::ClampViewAngle(qTmp);
//
//			trace_t trLeft, trRight;
//
//			Vector vLeft, vRight;
//			AngleVectors2(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
//			AngleVectors2(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);
//
//			vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
//			vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
//
//			vTraceEnd = vEyePos + vLeft;
//
//			ray.Init(vEyePos, vTraceEnd);
//			Interfaces::Trace->TraceRay(ray, 0x4600400B, tracefilter, &trLeft);
//
//			vTraceEnd = vEyePos + vRight;
//
//			ray.Init(vEyePos, vTraceEnd);
//			Interfaces::Trace->TraceRay(ray, 0x4600400B, tracefilter, &trRight);
//
//			if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
//				qTmp.y -= 90.f;
//			else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
//				qTmp.y += 90.f;
//
//			if (qTmp.y > 180.0)
//				qTmp.y -= 360.0;
//			else if (qTmp.y < -180.0)
//				qTmp.y += 360.0;
//
//			fYawOut = qTmp.y;
//
//			if (fYawOut > 180.0)
//				fYawOut -= 360.0;
//			else if (fYawOut < -180.0)
//				fYawOut += 360.0;
//
//			//if (!bHeadOnWall)
//			//	bHeadOnWall = true;
//
//			return;
//		}
//	}
//
//	fYawOut -= 180.0f;
//
//	if (fYawOut > 180.0)
//		fYawOut -= 360.0;
//	else if (fYawOut < -180.0)
//		fYawOut += 360.0;
//
//	//if (bHeadOnWall)
//	//	bHeadOnWall = false;
//}





void VectorAngles2(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / M_PI;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / M_PI;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}
void AngleVectors2(const Vector& qAngles, Vector& vecForward)
{
	float sp, sy, cp, cy;
	SinCos((float)(qAngles[1] * (M_PI / 180.f)), &sy, &cy);
	SinCos((float)(qAngles[0] * (M_PI / 180.f)), &sp, &cp);

	vecForward[0] = cp*cy;
	vecForward[1] = cp*sy;
	vecForward[2] = -sp;
}

bool EdgeAntiAim(IClientEntity* pLocalBaseEntity, CUserCmd* cmd, float flWall, float flCornor)
{
	Ray_t ray;
	trace_t tr;

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocalBaseEntity;

	auto bRetVal = false;
	auto vecCurPos = pLocalBaseEntity->GetEyePosition();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, cmd->viewangles.y, 0.f);
		vecDummy.y += i;

		NormalizeVector(vecDummy);

		Vector vecForward;
		AngleVectors2(vecDummy, vecForward);

		auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		vecForward *= flLength;

		ray.Init(vecCurPos, (vecCurPos + vecForward));
		Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector qAngles;
			auto vecNegate = tr.plane.normal;

			vecNegate *= -1.f;
			VectorAngles2(vecNegate, qAngles);

			vecDummy.y = qAngles.y;

			NormalizeVector(vecDummy);
			trace_t leftTrace, rightTrace;

			Vector vecLeft;
			AngleVectors2(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

			Vector vecRight;
			AngleVectors2(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

			vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
			vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

			ray.Init(vecCurPos, (vecCurPos + vecLeft));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

			ray.Init(vecCurPos, (vecCurPos + vecRight));
			Interfaces::Trace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				vecDummy.y -= flCornor; // left
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				vecDummy.y += flCornor; // right			

			cmd->viewangles.y = vecDummy.y;
			cmd->viewangles.y -= flWall;
			cmd->viewangles.x = 89.f;
			bRetVal = true;
		}
	}
	return bRetVal;
}

// AntiAim
void CRageBot::DoAntiAim(CUserCmd *pCmd, bool& bSendPacket)
{
	if (pCmd->buttons & IN_ATTACK)
		return;
	// If the aimbot is doing something don't do anything
	if (IsAimStepping || pCmd->buttons & IN_USE)
		return;

	// Weapon shit
	std::vector<std::string> InfoToAA;
	IClientEntity* pWeapon3 = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon3)
	{
		ClientClass* CClass = (ClientClass*)pWeapon3->GetClientClass();
		if (CClass)
		{
			// Draw it
			InfoToAA.push_back(FindWeaponName(CClass->m_pNetworkName));
			for (auto MyWeapon4 : InfoToAA)
			{
				//MyWeapon4 == Knife
				if (MyWeapon4 == C4 || MyWeapon4 == Fire || MyWeapon4 == Molo || MyWeapon4 == Smoke || MyWeapon4 == Nade || MyWeapon4 == Flash || MyWeapon4 == Decoy)
					return;
			}
		}
	}


	if (Menu::Window.RageBotTab.AtTargets.GetState()) {
		IClientEntity *Target = Interfaces::EntList->GetClientEntity(this->TargetID);
		AtTarget(Target, pCmd);
	}

	if (Menu::Window.RageBotTab.EdgeAntiAim.GetState()) {
		auto bEdge = EdgeAntiAim(hackManager.pLocal(), pCmd, 360.f, pCmd->viewangles.x);
		if (bEdge)
			return;
	}

	//AntiAims::CorrectMovement(OldAngle, pCmd, OldForward, OldSide);
	// Anti-Aim Pitch
	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex())
	{
	case 0:
		break;
	case 1:
		// Emotion
		AntiAims::StaticPitch(pCmd);
		break;
	case 2:
		// UP
		pCmd->viewangles.x = -89;
		break;
	case 3:
		// Jitter
		AntiAims::JitterPitch(pCmd);
		break;
	case 4:
		
		pCmd->viewangles.x = 271.f;
		break;
	case 5:
		pCmd->viewangles.x = -271.f;
		break;
	case 6:
		//Fake UP
		pCmd->viewangles.x = 1080;
		break;
	case 7:
		// Fake UP Inversed
		pCmd->viewangles.x = -1080;
		break;
	case 8:
		// Fake UP
		pCmd->viewangles.x = 0;
		break;
	case 9:
		AntiAims::FakeZeroEmotion(pCmd, bSendPacket);
		break;
	case 10:
		AntiAims::FakeUPMM(pCmd, bSendPacket);
		break;
	case 11:
		pCmd->viewangles.x = -180;
		break;
	case 12:
		pCmd->viewangles.x = 180;
		break;
	}

	//Anti-Aim Yaw
	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
	{
	case 0:
		// No Yaw AA
		break;
	case 1: {
		Vector OriginalY = pCmd->viewangles;
		pCmd->viewangles.y = OriginalY.y + 180.0;
		break;
	}

	case 2: {
		AntiAims::fakeback(pCmd, bSendPacket);
		break;
	}

	case 3: {
		Vector v372 = pCmd->viewangles;
		if (pCmd->command_number % 2) {
			pCmd->viewangles.y = v372.y + 180.0;
		}
		break;
	}
	case 4:
	{
		Vector v372 = pCmd->viewangles;
		if (pCmd->command_number % 2) {
			pCmd->viewangles.y = v372.y + 90.0;
		}
		else
		{
			pCmd->viewangles.y = v372.y - 90.0;
		}

		break;
	}
	case 5:
		AntiAims::FakeJitter(pCmd, bSendPacket);
		break;
	case 6:
		AntiAims::FakeJitter2(pCmd);
		break;
	case 7: {
		Vector FastSpin;
		FastSpin.y = (Interfaces::Globals->curtime * 800.0);
		GameUtils::ClampViewAngle(FastSpin);
		pCmd->viewangles.y = FastSpin.y;
		break;
	}

	case 8: {
		AntiAims::FastSpint(pCmd);
		break;
	}


	case 9: {
		AntiAims::FastSpintFake(pCmd, bSendPacket);
		break;
	}

	case 10: {
		pCmd->viewangles.y = 0;
		break;
	}

	case 11: {
		AntiAims::FakeZerooo(pCmd, bSendPacket);
		break;
	}
		
	case 12:
	{
		AntiAims::FakeSpinRight(pCmd, bSendPacket);
		break;
	}
	case 13:
	{
		AntiAims::FakeSpinLeft(pCmd, bSendPacket);
		break;
	}
	case 14:
	{
		AntiAims::pFakeRandom(pCmd, bSendPacket);
		break;
	}
	



	}

}


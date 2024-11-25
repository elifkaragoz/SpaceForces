 /*
 * gCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gFont.h"


class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();
	void windowResized(int w, int h);
	void showNotify();
	void hideNotify();

private:
	gApp* root;
	static const int PRESSED = 0, RELEASED = 1, DRAGGED = 2;
	static const int OYUN_LOAD = -1, OYUN_NORMAL = 0, OYUN_MENU = 1, OYUN_BITTI = 2, OYUN_SES = 3;
	int oyunmodu;
	gImage logo;
	//map
	gImage map;
	int mapx, mapy;
	void mapSetup();
	void mapDraw();
	//platform
	gImage oyuncuplatform;
	static const int yolsayisi = 2;
	static const int USTYOL = 0, ALTYOL = 1;
	int oyuncuplatformx[yolsayisi], oyuncuplatformy[yolsayisi];
	int oyuncuplatformw[yolsayisi], oyuncuplatformh[yolsayisi];
	int oyuncusecilenplatform;
	void oyuncuPlatformSetup();
	void oyuncuPlatformDraw();
	//
	gImage dusmanplatform;
	int dusmanplatformx[yolsayisi], dusmanplatformy[yolsayisi];
	int dusmanplatformw[yolsayisi], dusmanplatformh[yolsayisi];
	int dusmansecilenplatform;
	void dusmanPlatformSetup();
	void dusmanPlatformDraw();
	//platform animasyon
	gImage platformanimasyon[25];
	int platformanimasyonx[2], platformanimasyony[2];
	int platformanimframe;
	//ayarbuton
	static const int NORMAL = 0, TIKLA = 1;
	gImage ayarbuton[2];// [0]=>NORMAL [1]=>TIKLA
	int ayarbutonx, ayarbutony, ayarbutonw, ayarbutonh;
	int ayarbutondurum;
	void ayarbutonSetup();
	void ayarbutonDraw();
	void ayarbutonTikla(int farex, int farey, int hangimetod);
	//menu
	gImage menu;
	int menux, menuy, menuw, menuh;
	void menuSetup();
	void menuDraw();
	//devambuton
	gImage devambuton[2];// [0]=>NORMAL [1]=>TIKLA
	int devambutonx, devambutony, devambutonw, devambutonh;
	int devambutondurum;
	void devambutonSetup();
	void devambutonDraw();
	void devambutonTikla(int farex, int farey, int hangimetod);
	// return buton
	gImage returnButon[2];
	int returnbutonx, returnbutony, returnbutonw, returnbutonh;
	int returnbutondurum;
	void returnButonSetup();
	void returnButonDraw();
	void returnButonTikla(int farex, int farey, int method);
	// ses buton
	gImage sesButon[2];
	int sesbutonx, sesbutony, sesbutonw, sesbutonh;
	int sesbutondurum;
	void sesButonSetup();
	void sesButonDraw();
	void sesButonTikla(int farex, int farey, int method);
	//gemimarketi
	static const int gemicesidi = 4;
	gImage gemimarketi[gemicesidi];
	int gemimarketiw[gemicesidi];
	int gemimarketix[gemicesidi];
	int gemimarketiy[gemicesidi];
	int gemimarketih[gemicesidi];
	void gemimarketiSetup();
	void gemimarketiDraw();
	void gemimarketiTikla(int farex, int farey);
	//oyuncu gemi
	static const int SARIGEMI = 0, SIYAHGEMI = 1, YILDIZGEMI = 2, DIKENGEMI = 3;
	int gemimaxframe[gemicesidi];
	gImage gemiresmi[gemicesidi][25];
	int gemicanlari[gemicesidi];
	void gemiSetup();
	void gemiDraw();
	//oyuncu gemisi
	static const int OGEMI_TUR = 0, OGEMI_FRAME = 1, OGEMI_X = 2, OGEMI_Y = 3, OGEMI_W = 4,
			OGEMI_H = 5, OGEMI_HIZX = 6, OGEMI_FGSAYAC = 7, OGEMI_FGSMAX = 8, OGEMI_SM = 9,
			OGEMI_YOL = 10, OGEMI_MERMI = 11, OGEMI_RELOAD = 12, OGEMI_CAN = 13;
	std::deque<int> oyuncuyenigemi;
	std::deque<std::deque<int>> ogemilistesi;
	void ogemiUret(int tur);
	void ogemiDraw();
	void ogemiHareketi();
	//dusman gemi --> ayrý olmasýnýn nedeni size'larýn ayný olmayacak olmasý(farklý gemiler seçilebilir)
	static const int DGEMI_TUR = 0, DGEMI_FRAME = 1, DGEMI_X = 2, DGEMI_Y = 3, DGEMI_W = 4,
			DGEMI_H = 5, DGEMI_HIZX = 6, DGEMI_FGSAYAC = 7, DGEMI_FGSMAX = 8, DGEMI_SM = 9, DGEMI_YOL = 10,
			DGEMI_RELOAD = 11, DGEMI_MERMI = 12, DGEMI_CAN = 13;
	std::deque<int> dusmanyenigemi;
	std::deque<std::deque<int>> dgemilistesi;
	void dgemiUret();
	void dgemiDraw();
	void dgemiHareketi();
	int dusmanUretici;
	// saldiri kontrolleri
	gRect carpisan1, carpisan2; //dikdörtgen oluþturmamýzý saðlar (rectangle). -> CenterX metodu var merkez noktasýný alabiliriz
	void oyuncuGemiSaldiriKonrtolu();
	void dusmanGemiSaldiriKonrtolu();
	// mermiler
	static const int mermicesidi = 8;
	static const int OMERMI_TUR = 0, OMERMI_FRAME = 1, OMERMI_X = 2, OMERMI_Y = 3,
					 OMERMI_W = 4, OMERMI_H = 5, OMERMI_HIZX = 6, OMERMI_HIZY = 7, OMERMI_ROTATE = 8;
	enum
	{
		OMERMI = 0, DMERMI = 1 // deger vermezsen soldan saga deger verir
	};
	gImage mermiresmi[mermicesidi];
	void mermiYukle();
	std::deque<int> oyuncuyenimermi;
	std::deque<std::deque<int>> omermilistesi;
	int ogemimermisi[gemicesidi];
	void omermiUret(int ogemi);
	void oyuncugemiAtesEtsinMi();
	void omermiDraw();
	void omermiHareketi();
	// dmermi
	static const int DMERMI_TUR = 0, DMERMI_FRAME = 1, DMERMI_X = 2, DMERMI_Y = 3,
					 DMERMI_W = 4, DMERMI_H = 5, DMERMI_HIZX = 6, DMERMI_HIZY = 7, DMERMI_ROTATE = 8;
	std::deque<int> dusmanyenimermi;
	std::deque<std::deque<int>> dmermilistesi;
	int dgemimermisi[gemicesidi];
	void dmermiUret(int dgemi);
	void dmermiDraw();
	void dmermiHareketi();
	void dusmanGemiAtesEtsinMi();
	// carpisma metotlari
	void omermiVsDusmangemi();
	void dmermiVsOyuncuGemi();
	// patlama efekti
	static const int patlamamaxframe = 15;
	gImage patlamaresmi[patlamamaxframe];
	int patlamaframe, patlamagosterilsin;
	void patlamaSetup();
	void patlamaDraw();
	void patlamaUret(int gemiid);
	void oPatlamaUret(int gemiid);
	std::deque<int> yenipatlama;
	std::deque<std::deque<int>> patlamalistesi;
	int patlax, patlay, patlaw, patlah;
	// gemi fiyatlari
	gImage gemifiyatresmi[gemicesidi];
	int gemifiyatx[gemicesidi], gemifiyaty[gemicesidi];
	int para;
	gFont parafont;
	void gemiFiyatSetup();
	void gemiFiyatDraw();
	int gemifiyati[gemicesidi];
	gImage pararesmi;
	int pararesmix, pararesmiy;
	bool parayeterli[gemicesidi];
	// oyun sonu
	gImage oyunsonu;
	int oyunsonux, oyunsonuy;
	void oyunSonuSetup();
	void oyunSonuDraw();
	// oyun sonucu
	gImage oyunsonusonucu[2];
	int oyunsonusonucux, oyunsonusonucuy;
	static const int KAYIP = 0, KAZANC = 1;
	int oyunsonucu;
	// restart butonu
	gImage restartbuton[2];
	int restartbutondurum;
	int restartbutonx, restartbutony, restartbutonw, restartbutonh;
	void restartbutonSetup();
	void restartbutonTikla(int farex, int farey, int hangimetod);
	//  exit butonu
	gImage exitbuton[2];
	int exitbutondurum;
	int exitbutonx, exitbutony, exitbutonw, exitbutonh;
	void exitbutonSetup();
	void exitbutonTikla(int farex, int farey, int hangimetod);
	// ses barý
	gImage sesbari;
	int sesbarix, sesbariy, sesbariw, sesbarih;
	float sescubukx, sescubuky, sescubukw, sescubukh;
	void sesbariSetup();
	void sesbariDraw();
	void sesbariTikla(int x, int y, int eventtype);
};

#endif /* GCANVAS_H_ */











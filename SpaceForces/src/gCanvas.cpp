/*
* gCanvas.cpp
*
*  Created on: 8 Þub 2024
*      Author: elif
*/

#include "gCanvas.h"
#include "MenuCanvas.h"

gCanvas::gCanvas(gApp* root) : gBaseCanvas(root)
{
	this->root = root;
}

gCanvas::~gCanvas()
{
}

void gCanvas::setup() // setup loading ekranıdır fps gibi seyler cagrılmaz (getFramerate()). 0'a esitliyoruz.
{
	oyunmodu = OYUN_LOAD;
	parafont.loadFont("FreeSansBold.ttf", 18);
	logo.loadImage("glistengine_logo.png");
	mapSetup();
	oyuncuPlatformSetup();
	mermiYukle();
	dusmanPlatformSetup();
	ayarbutonSetup();
	menuSetup();
	gemimarketiSetup();
	gemiSetup();
	patlamaSetup();
	gemiFiyatSetup();
	oyunSonuSetup();
	sesbariSetup();
	restartbutonSetup();
	exitbutonSetup();
	dusmanUretici = 0; // reload || space time
	oyunmodu = OYUN_NORMAL;
}

void gCanvas::update() {
	// gLogi("gs") << "oyunmodu" << oyunmodu;
	if (oyunmodu != OYUN_NORMAL)
		return;
	ogemiHareketi();
	// gLogi("kontrol") << "geldi0";
	dgemiHareketi();
	oyuncuGemiSaldiriKonrtolu();
	oyuncugemiAtesEtsinMi();
	dusmanGemiAtesEtsinMi();
	dusmanGemiSaldiriKonrtolu();
	omermiHareketi();
	dmermiHareketi();
	omermiVsDusmangemi();
	dmermiVsOyuncuGemi();
	if (dusmanUretici < root->getFramerate() * 2) // fps degerini verir
		dusmanUretici += 1;
	else {
		dusmanUretici = 0;
		dgemiUret();
	}
}

void gCanvas::draw()
{
	mapDraw();
	oyuncuPlatformDraw();
	dusmanPlatformDraw();
	gemimarketiDraw();
	gemiFiyatDraw();
	omermiDraw();
	dmermiDraw();
	gemiDraw();
	patlamaDraw();
	ayarbutonDraw();
	menuDraw();
	oyunSonuDraw();
}

void gCanvas::keyPressed(int key)
{
//	gLogi("gCanvas") << "keyPressed:" << key;
	if(key == 32) {
		exit(0);
	}
}

void gCanvas::keyReleased(int key)
{
//	gLogi("gCanvas") << "keyReleased:" << key;
}

void gCanvas::charPressed(unsigned int codepoint)
{
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y)
{
	// gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseDragged(int x, int y, int button)
{
	//gLogi("gCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
	if (oyunmodu == OYUN_MENU)
	{
		devambutonTikla(x, y, DRAGGED);
		returnButonTikla(x, y, DRAGGED);
		sesButonTikla(x, y, DRAGGED);
		restartbutonTikla(x, y, DRAGGED);
	}
	if (oyunmodu == OYUN_SES)
	{
		sesButonTikla(x, y, DRAGGED);
	}
	if (oyunmodu == OYUN_NORMAL)
			ayarbutonTikla(x, y, DRAGGED);
	if (oyunmodu == OYUN_BITTI) {
		restartbutonTikla(x, y, DRAGGED);
		exitbutonTikla(x, y, DRAGGED);
	}
}

void gCanvas::mousePressed(int x, int y, int button)
{
	//gLogi("gCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
	//platformu secen bolum
	gemimarketiTikla(x, y);
	if (oyunmodu == OYUN_MENU || oyunmodu == OYUN_SES)
	{
		devambutonTikla(x, y, PRESSED);
		returnButonTikla(x, y, PRESSED);
		sesButonTikla(x, y, PRESSED);
		sesbariTikla(x, y, PRESSED);
	}
	if (oyunmodu == OYUN_BITTI) {
		restartbutonTikla(x, y, PRESSED);
		exitbutonTikla(x, y, PRESSED);
	}
	if (oyunmodu == OYUN_NORMAL)
		ayarbutonTikla(x, y, PRESSED);

	for (int sira = 0; sira < 2; ++sira)
	{
		if(oyuncuplatformx[sira] < x && x < oyuncuplatformx[sira] + oyuncuplatformw[sira] &&
				y > oyuncuplatformy[sira] && y < oyuncuplatformy[sira] + oyuncuplatformh[sira])
		{
			oyuncusecilenplatform = sira;
			platformanimframe = 1;
		}
	}
}

void gCanvas::mouseReleased(int x, int y, int button)
{
	//gLogi("gCanvas") << "mouseReleased" << ", button:" << button;
	//if(ayarbutondurum == TIKLA)ayarbutondurum = NORMAL;
	if (oyunmodu == OYUN_MENU || oyunmodu == OYUN_SES)
	{
		devambutonTikla(x, y, RELEASED);
		returnButonTikla(x, y, RELEASED);
		sesButonTikla(x, y, RELEASED);
	}
	if (oyunmodu == OYUN_NORMAL)
		ayarbutonTikla(x, y, RELEASED);
	if (oyunmodu == OYUN_BITTI) {
		restartbutonTikla(x, y, RELEASED);
		exitbutonTikla(x, y, RELEASED);
	}
}

void gCanvas::mouseScrolled(int x, int y)
{
//	gLogi("gCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseEntered() {

}

void gCanvas::mouseExited() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

void gCanvas::ayarbutonTikla(int farex, int farey, int hangimetod)
{
	if(farex > ayarbutonx && farex < ayarbutonx + ayarbutonw
	&& farey > ayarbutony && farey < ayarbutony + ayarbutonh)
	{
		if(hangimetod == PRESSED)
			ayarbutondurum = TIKLA;
		if(hangimetod == RELEASED)
		{
			ayarbutondurum = NORMAL;
			if(oyunmodu == OYUN_NORMAL)
				oyunmodu = OYUN_MENU;
		}
		if(hangimetod == DRAGGED)
			ayarbutondurum = TIKLA;
	}
	else ayarbutondurum = NORMAL;
}

void gCanvas::ayarbutonSetup()
{
	ayarbutondurum = NORMAL;//butonun baslangicta hangi resmini gosterecegi
	//butonun durumlarinin resimleri
	ayarbuton[NORMAL].loadImage("ayarlar_buton.png");//NORMAL
	ayarbuton[TIKLA].loadImage("ayarlar_butonu_tiklandi.png");//TIKLA
	ayarbutonw = ayarbuton[ayarbutondurum].getWidth();
	ayarbutonh = ayarbuton[ayarbutondurum].getHeight();
	ayarbutonx = getWidth() - ayarbutonw;
	ayarbutony = 0;
}

void gCanvas::ayarbutonDraw()
{
	ayarbuton[ayarbutondurum].draw(ayarbutonx, ayarbutony);
}

void gCanvas::oyuncuPlatformSetup()
{
	//resim yuklendi
	oyuncuplatform.loadImage("silahli_gemi1.png");
	                    // ** //
	oyuncuplatformw[USTYOL] = oyuncuplatform.getWidth();
	oyuncuplatformh[USTYOL] = oyuncuplatform.getHeight();
	oyuncuplatformx[USTYOL] = 260;
	oyuncuplatformy[USTYOL] = 150;
	//konumu ayarlandi altyol
	oyuncuplatformw[ALTYOL] = oyuncuplatform.getWidth();
	oyuncuplatformh[ALTYOL] = oyuncuplatform.getHeight();
	oyuncuplatformx[ALTYOL] = 260;
	oyuncuplatformy[ALTYOL] = 150 + 514;
	//baslangicta secilen platform
	oyuncusecilenplatform = USTYOL;
	//platform animasyonlari yukleniyor
	for (int frame = 0; frame < 25; ++frame)
	{
		platformanimasyon[frame].loadImage("animasyon/platform_"+ gToStr(frame) +".png");
	}
	//platform animasyonun yatay konumu ayarlaniyor
	platformanimasyonx[USTYOL] = oyuncuplatformx[USTYOL] + 10;
	platformanimasyonx[ALTYOL] = oyuncuplatformx[ALTYOL] + 10;
	//platform animasyonun dikey konumu ayarlaniyor
	platformanimasyony[USTYOL] = oyuncuplatformy[USTYOL];
	platformanimasyony[ALTYOL] = oyuncuplatformy[ALTYOL];
	//platform animasyon baslangic
	platformanimframe = 1;
}

void gCanvas::oyuncuPlatformDraw()
{
	for (int sira = 0; sira < yolsayisi; ++sira)
		oyuncuplatform.draw(oyuncuplatformx[sira], oyuncuplatformy[sira]);
	//platform animasyon
	platformanimasyon[platformanimframe].draw(platformanimasyonx[oyuncusecilenplatform],
			platformanimasyony[oyuncusecilenplatform]);
	if(platformanimframe < 24)platformanimframe += 1;
}

void gCanvas::dusmanPlatformSetup()
{
	dusmanplatform.loadImage("silahli_dusmangemi1.png");
	//konumu ayarlandi ustyol
	dusmanplatformw[USTYOL] = dusmanplatform.getWidth();
	dusmanplatformh[USTYOL] = dusmanplatform.getHeight();
	dusmanplatformx[USTYOL] = 1610;
	dusmanplatformy[USTYOL] = 150;
	//konumu ayarlandi altyol
	dusmanplatformw[ALTYOL] = dusmanplatform.getWidth();
	dusmanplatformh[ALTYOL] = dusmanplatform.getHeight();
	dusmanplatformx[ALTYOL] = 1610;
	dusmanplatformy[ALTYOL] = 150 + 514;
	//baslangicta secilen platform
	dusmansecilenplatform = USTYOL;
}

void gCanvas::dusmanPlatformDraw()
{
	for (int sira = 0; sira < yolsayisi; ++sira)
		dusmanplatform.draw(dusmanplatformx[sira], dusmanplatformy[sira]);
}



void gCanvas::mapSetup()
{
	map.loadImage("map.png");
	mapx = 0;
	mapy = 0;
}

void gCanvas::mapDraw()
{
	map.draw(mapx, mapy);
}

void gCanvas::menuSetup()
{
	menu.loadImage("pause/pause_ekran.png");
	menuw = menu.getWidth();
	menuh = menu.getHeight();
	menux = getWidth()/2 - menuw/2;
	menuy = 0;
	devambutonSetup();
	returnButonSetup();
	sesButonSetup();
}

void gCanvas::menuDraw()
{
	if(oyunmodu == OYUN_MENU || oyunmodu == OYUN_SES) {
		menu.draw(menux, menuy);
		devambutonDraw();
		returnButonDraw();
		sesButonDraw();
		sesbariDraw();
	}
}

void gCanvas::devambutonTikla(int farex, int farey, int hangimetod)
{
	if(farex > devambutonx && farex < devambutonx + devambutonw
	&& farey > devambutony && farey < devambutony + devambutonh)
	{
		if(hangimetod == PRESSED)
			devambutondurum = TIKLA;
		if(hangimetod == RELEASED)
		{
			devambutondurum = NORMAL;
			if(oyunmodu == OYUN_MENU)
				oyunmodu = OYUN_NORMAL;
		}
		if(hangimetod == DRAGGED)
			devambutondurum = TIKLA;
	}
	else devambutondurum = NORMAL;
}

void gCanvas::devambutonSetup()
{
	devambutondurum = NORMAL;
	devambuton[NORMAL].loadImage("pause/oyna.png");//NORMAL
	devambuton[TIKLA].loadImage("pause/oyna_tiklandi.png");//TIKLA
	devambutonw = devambuton[devambutondurum].getWidth();
	devambutonh = devambuton[devambutondurum].getHeight();
	devambutonx = menux + menuw - devambutonw * 1.8f;
	devambutony = menuy + menuh - devambutonh * 1.5f;
}

void gCanvas::devambutonDraw()
{
	devambuton[devambutondurum].draw(devambutonx, devambutony);
}

void gCanvas::returnButonTikla(int farex, int farey, int hangimetod)
{
	if(farex > returnbutonx && farex < returnbutonx + returnbutonw
	&& farey > returnbutony && farey < returnbutony + returnbutonh)
	{
		if(hangimetod == PRESSED)
			returnbutondurum = TIKLA;
		if(hangimetod == RELEASED)
		{
			returnbutondurum = NORMAL;
			MenuCanvas* cnv = new MenuCanvas(root);
			appmanager->setCurrentCanvas(cnv);
		}
		if(hangimetod == DRAGGED)
			returnbutondurum = TIKLA;
	}
	else returnbutondurum = NORMAL;
}

void gCanvas::returnButonSetup()
{
	returnbutondurum = NORMAL;
	returnButon[NORMAL].loadImage("pause/menu.png");
	returnButon[TIKLA].loadImage("pause/menu_tiklandi.png");
	returnbutonw = returnButon[returnbutondurum].getWidth();
	returnbutonh = returnButon[returnbutondurum].getHeight();
	returnbutonx = menux + menuw / 2 - returnbutonw / 2;
	returnbutony = menuy + menuh - returnbutonh - returnbutonh / 2;
}

void gCanvas::returnButonDraw()
{
	returnButon[returnbutondurum].draw(returnbutonx,returnbutony);
}

void gCanvas::sesButonTikla(int farex, int farey, int hangimetod)
{
	if(farex > sesbutonx && farex < sesbutonx + sesbutonw
	&& farey > sesbutony && farey < sesbutony + sesbutonh)
	{
		if(hangimetod == PRESSED)
			sesbutondurum = TIKLA;
		if(hangimetod == RELEASED)
		{
			sesbutondurum = NORMAL;
			if(oyunmodu == OYUN_MENU) oyunmodu = OYUN_SES;
			else if (oyunmodu == OYUN_SES) oyunmodu = OYUN_MENU;
		}
		if(hangimetod == DRAGGED)
			sesbutondurum = TIKLA;
	}else sesbutondurum = NORMAL;
}

void gCanvas::sesButonSetup()
{
	sesbutondurum = NORMAL; // baslangic degeri
	// indexin seslarý
	sesButon[NORMAL].loadImage("pause/ses_buton.png");
	sesButon[TIKLA].loadImage("pause/sessiz_buton.png");
	sesbutonw = sesButon[sesbutondurum].getWidth();
	sesbutonh = sesButon[sesbutondurum].getHeight();
	sesbutonx = menux + menuw - (sesbutonw * 4.5f);
	sesbutony = devambutony;
}

void gCanvas::sesButonDraw()
{
	sesButon[sesbutondurum].draw(sesbutonx,sesbutony);
}

void gCanvas::gemimarketiTikla(int farex, int farey)
{
	for (int sira = 0; sira < gemicesidi; ++sira)
	{
		if(farex > gemimarketix[sira] && farex < gemimarketix[sira] + gemimarketiw[sira] &&
				farey > gemimarketiy[sira] && farey < gemimarketiy[sira] + gemimarketih[sira])
		{
			if (para >= gemifiyati[sira])
			{
				para -= gemifiyati[sira];
				ogemiUret(sira);
			}
		}
	}
}

void gCanvas::gemiSetup()
{
	// gemi canlari
	gemicanlari[SARIGEMI] = 5;
	gemicanlari[SIYAHGEMI] = 3;
	gemicanlari[YILDIZGEMI] = 8;
	gemicanlari[DIKENGEMI] = 15;
	// gemi frame
	gemimaxframe[SARIGEMI] = 25;
	gemimaxframe[SIYAHGEMI] = 16;
	gemimaxframe[YILDIZGEMI] = 25;
	gemimaxframe[DIKENGEMI] = 25;
	//gemi resimlerini yukle
	for (int gemi = 0; gemi < gemicesidi; gemi += 1)
		//secilen gemi icin resim yukleme
		for (int frame = 0; frame < gemimaxframe[gemi]; ++frame)
			gemiresmi[gemi][frame].loadImage("animasyon/ship_" + gToStr(gemi + 1)+ "_" + gToStr(frame + 1)+ ".png");
}

void gCanvas::gemimarketiSetup()
{
    for (int var = 0; var < gemicesidi; ++var)
    {
        gemimarketi[var].loadImage("animasyon/ship_"+ gToStr(var+1)+"_1.png");
        gemimarketiw[var] = gemimarketi[var].getWidth();
        gemimarketih[var] = gemimarketi[var].getHeight();
        gemimarketix[var] = 172 - gemimarketiw[var]/2;
        gemimarketiy[var] = (var + 0.5f)*270 - gemimarketih[var]/2;
    }
}
void gCanvas::gemimarketiDraw()
{
    for (int var = 0; var <gemicesidi ; ++var)
        gemimarketi[var].draw(gemimarketix[var],gemimarketiy[var]);
}

void gCanvas::ogemiUret(int tur)
{
	//gemiyi uret
	oyuncuyenigemi.push_back(tur);//tur index:0
	oyuncuyenigemi.push_back(0);//frame index:1
	oyuncuyenigemi.push_back(oyuncuplatformx[oyuncusecilenplatform] +
			((oyuncuplatformw[oyuncusecilenplatform] - gemiresmi[tur][0].getWidth()) / 2));//x(yatay)
	oyuncuyenigemi.push_back(oyuncuplatformy[oyuncusecilenplatform] +
			((oyuncuplatformh[oyuncusecilenplatform] - gemiresmi[tur][0].getHeight()) / 2));//y(dikey)
	oyuncuyenigemi.push_back(gemiresmi[tur][0].getWidth());//w
	oyuncuyenigemi.push_back(gemiresmi[tur][0].getHeight());//h
	oyuncuyenigemi.push_back(+10);//hiz x
	oyuncuyenigemi.push_back(0); //gecikme sayaci
	oyuncuyenigemi.push_back((int)60 / gemimaxframe[tur]);
	oyuncuyenigemi.push_back(0); // saldiri modu
	oyuncuyenigemi.push_back(oyuncusecilenplatform); // yol
	oyuncuyenigemi.push_back(ogemimermisi[tur]); // gemilere gore mermi turu
	oyuncuyenigemi.push_back(60); // reload
	oyuncuyenigemi.push_back(gemicanlari[tur]); // can

	//uretilen gemiyi listeye gonder
	ogemilistesi.push_back(oyuncuyenigemi);
	//yeni gemiyi hazirla
	oyuncuyenigemi.clear();
}

void gCanvas::gemiDraw()
{
	ogemiDraw();
	dgemiDraw();
}

void gCanvas::ogemiDraw()
{
	for (int sira = 0; sira < ogemilistesi.size(); sira +=1)
	{
		gemiresmi[ogemilistesi[sira][OGEMI_TUR]][ogemilistesi[sira][OGEMI_FRAME]].draw(ogemilistesi[sira][OGEMI_X],
				ogemilistesi[sira][OGEMI_Y], ogemilistesi[sira][OGEMI_W], ogemilistesi[sira][OGEMI_H], 180.0f);
		if (ogemilistesi[sira][OGEMI_FGSAYAC] < ogemilistesi[sira][OGEMI_FGSMAX]) // frame geciktirme.. 25 -> 2.4, 16 -> 3.75
				ogemilistesi[sira][OGEMI_FGSAYAC]++;
		else
		{
			ogemilistesi[sira][OGEMI_FGSAYAC] = 0;
			if (oyunmodu == OYUN_NORMAL)
				ogemilistesi[sira][OGEMI_FRAME] += 1;
		}
		if (ogemilistesi[sira][OGEMI_FRAME] >= gemimaxframe[ogemilistesi[sira][OGEMI_TUR]])
			ogemilistesi[sira][OGEMI_FRAME] = 0;
	}
}

void gCanvas::ogemiHareketi()
{
	// panele vardiginda durmali
	// saldiri modunda gectigi zaman durmali
	for (int sira = 0; sira < ogemilistesi.size(); ++sira)
	{
		if (ogemilistesi[sira][OGEMI_SM] == 0 && (ogemilistesi[sira][OGEMI_X] <
								dusmanplatformx[ogemilistesi[sira][OGEMI_YOL]]))
			ogemilistesi[sira][OGEMI_X] += ogemilistesi[sira][OGEMI_HIZX];
		else
		{
			oyunmodu = OYUN_BITTI;
			oyunsonucu = KAZANC;
		}
	}
}

void gCanvas::dgemiUret() // sayaç kavramı ile otomatik üretim
{
	// default -> hiçbir seçim yokken iki tane random üretip gönder
	int dyol = (int)gRandom(yolsayisi);
	int dtur = (int)gRandom(gemicesidi);
	if (ogemilistesi.size() > dgemilistesi.size()) // daha buyuk savunma gemisi
		dtur = 3;
	//gLogi("dtur") << "dtur: " << dtur << "dtur: " << ogemilistesi.size() << "dgemi: " << dgemilistesi.size();
	dusmanyenigemi.push_back(dtur);
	dusmanyenigemi.push_back(0);
	dusmanyenigemi.push_back(dusmanplatformx[dyol] + dusmanplatformw[dyol]/2 - gemiresmi[dtur][0].getWidth()/2);
	dusmanyenigemi.push_back(dusmanplatformy[dyol] + dusmanplatformh[dyol]/2 - gemiresmi[dtur][0].getHeight()/2);
	dusmanyenigemi.push_back(gemiresmi[dtur][0].getWidth());
	dusmanyenigemi.push_back(gemiresmi[dtur][0].getHeight());
	dusmanyenigemi.push_back(-10);
	dusmanyenigemi.push_back(0); //gecikme sayaci
	dusmanyenigemi.push_back((int)60 / gemimaxframe[dtur]);
	dusmanyenigemi.push_back(0); // saldiri modu
	dusmanyenigemi.push_back(dyol); // yol
	dusmanyenigemi.push_back(60); // reload
	dusmanyenigemi.push_back(dgemimermisi[dtur]);
	dusmanyenigemi.push_back(gemicanlari[dtur]); // can
	dgemilistesi.push_back(dusmanyenigemi);
	dusmanyenigemi.clear();
}

void gCanvas::dgemiDraw()
{
	for (int sira = 0; sira < dgemilistesi.size(); ++sira)
	{
		gemiresmi[dgemilistesi[sira][DGEMI_TUR]][dgemilistesi[sira][DGEMI_FRAME]].draw(dgemilistesi[sira][DGEMI_X], dgemilistesi[sira][DGEMI_Y]);
		if (dgemilistesi[sira][DGEMI_FGSAYAC] < dgemilistesi[sira][DGEMI_FGSMAX])
			dgemilistesi[sira][DGEMI_FGSAYAC]++;
		else
		{
			dgemilistesi[sira][DGEMI_FGSAYAC] = 0;
			if (oyunmodu == OYUN_NORMAL)
				dgemilistesi[sira][DGEMI_FRAME] += 1;
		}
		if (dgemilistesi[sira][DGEMI_FRAME] >= gemimaxframe[dgemilistesi[sira][DGEMI_TUR]])
			dgemilistesi[sira][DGEMI_FRAME] = 0;
	}
}

void gCanvas::dgemiHareketi()
{
	for (int sira = 0; sira < dgemilistesi.size(); ++sira)
	{
		if (dgemilistesi[sira][DGEMI_SM] == 0 && dgemilistesi[sira][DGEMI_X] > oyuncuplatformx[dgemilistesi[sira][DGEMI_YOL]])
			dgemilistesi[sira][DGEMI_X] += dgemilistesi[sira][DGEMI_HIZX];
		else
		{
			oyunmodu = OYUN_BITTI;
			oyunsonucu = KAYIP;
		}
	}
}

void gCanvas::oyuncuGemiSaldiriKonrtolu()
{
	// oyuncu gemilerini sec
	for (int ogemi = 0; ogemi < ogemilistesi.size(); ++ogemi)
	{
		ogemilistesi[ogemi][OGEMI_SM] = 0; // saldırı modunu resetleme
		// secilen oyuncu gemisinin hitbox'ını olustur
		carpisan1.set(ogemilistesi[ogemi][OGEMI_X], ogemilistesi[ogemi][OGEMI_Y],
				(ogemilistesi[ogemi][OGEMI_X] + ogemilistesi[ogemi][OGEMI_W]), (ogemilistesi[ogemi][OGEMI_Y] + ogemilistesi[ogemi][OGEMI_H]));
		// butun dusman gemilerini sec
		for (int dgemi = 0; dgemi < dgemilistesi.size(); ++dgemi)
		{
			// dusman gemisinin hitbox'ini olustur
			carpisan2.set(dgemilistesi[dgemi][DGEMI_X], dgemilistesi[dgemi][DGEMI_Y],
				(dgemilistesi[dgemi][DGEMI_X] + dgemilistesi[dgemi][DGEMI_W]), (dgemilistesi[dgemi][DGEMI_Y] + dgemilistesi[dgemi][DGEMI_H]));
			// 2 hotbox'i karsilastir
			if (ogemilistesi[ogemi][OGEMI_YOL] == dgemilistesi[dgemi][DGEMI_YOL] && abs(carpisan1.centerX() - carpisan2.centerX()) < 400)
			{	// oyuncu gemisi saldiri modu
				ogemilistesi[ogemi][OGEMI_SM] = 1; // for dongusunu sonlandır
				dgemi = dgemilistesi.size() - 1;
			}
		}
	}
}

void gCanvas::dusmanGemiSaldiriKonrtolu()
{
	for (int dgemi = 0; dgemi < dgemilistesi.size(); ++dgemi)
	{
		dgemilistesi[dgemi][DGEMI_SM] = 0;
		carpisan2.set(dgemilistesi[dgemi][DGEMI_X], dgemilistesi[dgemi][DGEMI_Y],
				(dgemilistesi[dgemi][DGEMI_X] + dgemilistesi[dgemi][DGEMI_W]), (dgemilistesi[dgemi][DGEMI_Y] + dgemilistesi[dgemi][DGEMI_H]));
		for (int ogemi = 0; ogemi < ogemilistesi.size(); ++ogemi)
		{
			carpisan1.set(ogemilistesi[ogemi][OGEMI_X], ogemilistesi[ogemi][OGEMI_Y],
				(ogemilistesi[ogemi][OGEMI_X] + ogemilistesi[ogemi][OGEMI_W]), (ogemilistesi[ogemi][OGEMI_Y] + ogemilistesi[ogemi][OGEMI_H]));
			if (dgemilistesi[dgemi][DGEMI_YOL] == ogemilistesi[ogemi][OGEMI_YOL] && abs(carpisan2.centerX() - carpisan1.centerX()) < 400)
			{
				dgemilistesi[dgemi][DGEMI_SM] = 1;
				ogemi = ogemilistesi.size() - 1;
			}
		}
	}
}

void gCanvas::mermiYukle()
{
	mermiresmi[0].loadImage("mavi_ates.png");
	mermiresmi[1].loadImage("maviates2.png");
	mermiresmi[2].loadImage("maviates3.png");
	mermiresmi[3].loadImage("maviates4.png");
	mermiresmi[4].loadImage("kirmizi_ates.png");
	mermiresmi[5].loadImage("kirmiziates2.png");
	mermiresmi[6].loadImage("kirmiziates3.png");
	mermiresmi[7].loadImage("kirmiziates4.png");
	ogemimermisi[SARIGEMI] = 0;
	ogemimermisi[SIYAHGEMI] = 1;
	ogemimermisi[YILDIZGEMI] = 2;
	ogemimermisi[DIKENGEMI] = 3;
	dgemimermisi[SARIGEMI] = 4;
	dgemimermisi[SIYAHGEMI] = 5;
	dgemimermisi[YILDIZGEMI] = 6;
	dgemimermisi[DIKENGEMI] = 7;
}

void gCanvas::omermiUret(int ogemi)
{
	// x, y, w, h, hizx, hizy, frame, mermi turu
	oyuncuyenimermi.push_back(ogemilistesi[ogemi][OGEMI_MERMI]); // tur
	oyuncuyenimermi.push_back(0); // frame
	oyuncuyenimermi.push_back(ogemilistesi[ogemi][OGEMI_X] + ogemilistesi[ogemi][OGEMI_W] / 2 -
							  mermiresmi[ogemilistesi[ogemi][OGEMI_MERMI]].getWidth() / 2); // x
	oyuncuyenimermi.push_back(ogemilistesi[ogemi][OGEMI_Y] + ogemilistesi[ogemi][OGEMI_H] /2 -
							  mermiresmi[ogemilistesi[ogemi][OGEMI_MERMI]].getHeight() / 2); // y
	oyuncuyenimermi.push_back(mermiresmi[ogemilistesi[ogemi][OGEMI_MERMI]].getWidth()); // w
	oyuncuyenimermi.push_back(mermiresmi[ogemilistesi[ogemi][OGEMI_MERMI]].getHeight()); // h
	oyuncuyenimermi.push_back(+20); // hizx
	oyuncuyenimermi.push_back(0); // hizy
	oyuncuyenimermi.push_back(0); // rotate
	omermilistesi.push_back(oyuncuyenimermi);
	oyuncuyenimermi.clear();
}

void gCanvas::oyuncugemiAtesEtsinMi()
{
	// gemilerin listesi
	for (int ogemi = 0; ogemi < ogemilistesi.size(); ++ogemi)
	{
		// geminin reload suresi dolmus mu
		if (ogemilistesi[ogemi][OGEMI_RELOAD] < 60)
			ogemilistesi[ogemi][OGEMI_RELOAD]++;
		else // dolmus ise gemi adina mermi uret
		{
			ogemilistesi[ogemi][OGEMI_RELOAD] = 0;
			omermiUret(ogemi);
			root->sesPlay(root->SES_ATES);
		}
	}
}

void gCanvas::omermiDraw()
{
	for (int omermi = 0; omermi < omermilistesi.size(); ++omermi)
	{
		mermiresmi[omermilistesi[omermi][OMERMI_TUR]].draw(omermilistesi[omermi][OMERMI_X],
														   omermilistesi[omermi][OMERMI_Y],
														   omermilistesi[omermi][OMERMI_W],
														   omermilistesi[omermi][OMERMI_H],
														   omermilistesi[omermi][OMERMI_ROTATE]);
		// mermiyi dondur
		// omermilistesi[omermi][OMERMI_ROTATE] += 3;
	}
}

void gCanvas::omermiHareketi()
{
	for (int omermi = 0; omermi < omermilistesi.size(); ++omermi)
	{
		omermilistesi[omermi][OMERMI_X] += omermilistesi[omermi][OMERMI_HIZX];
		if (omermilistesi[omermi][OMERMI_X] > getWidth()) {
			omermilistesi.erase(omermilistesi.begin() + omermi); // gemilistesi ile işlem yapınca boyutu geçmeye çalıştığında hata veriyor
			break;
		}
	}
}

void gCanvas::dmermiUret(int dgemi)
{
	dusmanyenimermi.push_back(dgemilistesi[dgemi][DGEMI_MERMI]); // tur
	dusmanyenimermi.push_back(0); // frame
	dusmanyenimermi.push_back(dgemilistesi[dgemi][DGEMI_X] + dgemilistesi[dgemi][DGEMI_W] / 2 -
							  mermiresmi[dgemilistesi[dgemi][DGEMI_MERMI]].getWidth() / 2); // x
	dusmanyenimermi.push_back(dgemilistesi[dgemi][DGEMI_Y] + dgemilistesi[dgemi][DGEMI_H] /2 -
							  mermiresmi[dgemilistesi[dgemi][DGEMI_MERMI]].getHeight() / 2); // y
	dusmanyenimermi.push_back(mermiresmi[dgemilistesi[dgemi][DGEMI_MERMI]].getWidth()); // w
	dusmanyenimermi.push_back(mermiresmi[dgemilistesi[dgemi][DGEMI_MERMI]].getHeight()); // h
	dusmanyenimermi.push_back(-20); // hizx
	dusmanyenimermi.push_back(0); // hizy
	dusmanyenimermi.push_back(0); // rotate
	dmermilistesi.push_back(dusmanyenimermi);
	dusmanyenimermi.clear();
}

void gCanvas::dmermiDraw()
{
	for (int dmermi = 0; dmermi < dmermilistesi.size(); dmermi++)
	{
		// mermiyi cizdir
		mermiresmi[dmermilistesi[dmermi][DMERMI_TUR]].draw(dmermilistesi[dmermi][DMERMI_X],
								 	 	 	 	 	 	   dmermilistesi[dmermi][DMERMI_Y],
														   dmermilistesi[dmermi][DMERMI_W],
														   dmermilistesi[dmermi][DMERMI_H],
														   dmermilistesi[dmermi][DMERMI_ROTATE]);
		// mermiyi dondurme (rotate)
		// dmermilistesi[dmermi][DMERMI_ROTATE] = (dmermilistesi[dmermi][DMERMI_ROTATE] + 3) % 360;
		// dmermilistesi[dmermi][DMERMI_ROTATE] += 3;
	}
}

void gCanvas::dmermiHareketi()
{
	for (int dmermi = 0; dmermi < dmermilistesi.size(); dmermi++)
	{
		dmermilistesi[dmermi][DMERMI_X] += dmermilistesi[dmermi][DMERMI_HIZX];
		if (dmermilistesi[dmermi][DMERMI_X] <= 0)
		{
			dmermilistesi.erase(dmermilistesi.begin() + dmermi);
			break;
		}
	}
}

void gCanvas::dusmanGemiAtesEtsinMi()
{
	// gemilerin listesi
	for (int dgemi = 0; dgemi < dgemilistesi.size(); ++dgemi)
	{
		// geminin reload suresi dolmus mu
		if (dgemilistesi[dgemi][DGEMI_RELOAD] < 60)
			dgemilistesi[dgemi][DGEMI_RELOAD]++;
		else // dolmus ise gemi adina mermi uret
		{
			dgemilistesi[dgemi][DGEMI_RELOAD] = 0;
			dmermiUret(dgemi);
			root->sesPlay(root->SES_ATES);
		}
	}
}

void gCanvas::omermiVsDusmangemi()
{
	// oyuncu mermilerini secme
	for (int omermi = 0; omermi < omermilistesi.size(); ++omermi)
	{
		// secilen mermi icin hitbox olusturma
		carpisan1.set(omermilistesi[omermi][OMERMI_X], omermilistesi[omermi][OMERMI_Y],
					  omermilistesi[omermi][OMERMI_X] + omermilistesi[omermi][OMERMI_W],
					  omermilistesi[omermi][OMERMI_Y] + omermilistesi[omermi][OMERMI_H]);
		// dusman gemilerini secme
		for (int dgemi = 0; dgemi < dgemilistesi.size(); ++dgemi)
		{
			// secilen dusman gemisi icin hitbox olusturma
			carpisan2.set(dgemilistesi[dgemi][DGEMI_X], dgemilistesi[dgemi][DGEMI_Y],
						  dgemilistesi[dgemi][DGEMI_X] + dgemilistesi[dgemi][DGEMI_W],
						  dgemilistesi[dgemi][DGEMI_Y] + dgemilistesi[dgemi][DGEMI_H]);
			// hitboxlarin karsilastirilmasi
			if (carpisan1.intersects(carpisan2))
			{
				// carpisma var ise yok et!
				patlamaUret(dgemi);
				para += gemifiyati[dgemilistesi[dgemi][DGEMI_TUR]];
				omermilistesi.erase(omermilistesi.begin() + omermi);
				dgemilistesi.erase(dgemilistesi.begin() + dgemi);
				break;
				// patlama efekti goster --> gemiyi silmeden koordinatlarını almalıyız
				// patlama sesi goster
			}
		}
	}
}

void gCanvas::patlamaSetup()
{
	patlamaframe = 0;
	patlamagosterilsin = 0;
	for (int frame = 0; frame < patlamamaxframe; ++frame)
		patlamaresmi[frame].loadImage("arapatlamaanimasyon/patlama_" + gToStr(frame + 1) +".png");
}

void gCanvas::patlamaDraw()
{
	for (int patlaid = 0; patlaid < patlamalistesi.size(); ++patlaid)
	{
		patlamaresmi[patlamalistesi[patlaid][0]].draw(patlamalistesi[patlaid][1], patlamalistesi[patlaid][2],
													  patlamalistesi[patlaid][3], patlamalistesi[patlaid][4]);
		if (patlamalistesi[patlaid][0] < patlamamaxframe -1)
			patlamalistesi[patlaid][0] += 1;
		else
		{
			patlamalistesi.erase(patlamalistesi.begin() + patlaid);
			break;
		}
	}
}

void gCanvas::patlamaUret(int gemiid)
{
	yenipatlama.push_back(0); // frame (index = 0)
	yenipatlama.push_back(dgemilistesi[gemiid][DGEMI_X]); // x
	yenipatlama.push_back(dgemilistesi[gemiid][DGEMI_Y]); // y
	yenipatlama.push_back(dgemilistesi[gemiid][DGEMI_W]); // w
	yenipatlama.push_back(dgemilistesi[gemiid][DGEMI_H]); // h
	patlamalistesi.push_back(yenipatlama);
	yenipatlama.clear();
	root->sesPlay(root->SES_PATLA);
}

void gCanvas::dmermiVsOyuncuGemi()
{
	for (int dmermi = 0; dmermi < dmermilistesi.size(); ++dmermi)
	{
		carpisan2.set(dmermilistesi[dmermi][DMERMI_X], dmermilistesi[dmermi][DMERMI_Y],
					  dmermilistesi[dmermi][DMERMI_X] + dmermilistesi[dmermi][DMERMI_W],
					  dmermilistesi[dmermi][DMERMI_Y] + dmermilistesi[dmermi][DMERMI_H]);
		for (int ogemi = 0; ogemi < ogemilistesi.size(); ++ogemi)
		{
			carpisan1.set(ogemilistesi[ogemi][OGEMI_X], ogemilistesi[ogemi][OGEMI_Y],
						  ogemilistesi[ogemi][OGEMI_X] + ogemilistesi[ogemi][OGEMI_W],
						  ogemilistesi[ogemi][OGEMI_Y] + ogemilistesi[ogemi][OGEMI_H]);
			if (carpisan2.intersects(carpisan1)) // --> karsilastirma operatoru, boolean deger dondurur
				// (dikdörtgenlerinin kesişip kesişmediğini kontrol eder. Eğer kesişiyorlarsa, çarpışma olmuş demektir.)
			{
				// geminin cani bittiyse yok et
				if (ogemilistesi[ogemi][OGEMI_CAN] > 0)
					ogemilistesi[ogemi][OGEMI_CAN]--;
				else
				{
					oPatlamaUret(ogemi);
					dmermilistesi.erase(dmermilistesi.begin() + dmermi);
					ogemilistesi.erase(ogemilistesi.begin() + ogemi);
					break;
				}
			}
		}
	}
}

void gCanvas::oPatlamaUret(int gemiid)
{
	yenipatlama.push_back(0);
	yenipatlama.push_back(ogemilistesi[gemiid][OGEMI_X]);
	yenipatlama.push_back(ogemilistesi[gemiid][OGEMI_Y]);
	yenipatlama.push_back(ogemilistesi[gemiid][OGEMI_W]);
	yenipatlama.push_back(ogemilistesi[gemiid][OGEMI_H]);
	patlamalistesi.push_back(yenipatlama);
	yenipatlama.clear();
	root->sesPlay(root->SES_PATLA);
}

void gCanvas::gemiFiyatSetup()
{
	gemifiyatresmi[0].loadImage("para_kapali.png");
	gemifiyatresmi[1].loadImage("para_acik.png");
	// bayrak konumlari
	gemifiyatx[0] = gemimarketix[0] + 20;
	gemifiyaty[0] = 15;
	gemifiyatx[1] = gemifiyatx[0];
	gemifiyaty[1] = gemifiyaty[0] + 265;
	gemifiyatx[2] = gemifiyatx[0];
	gemifiyaty[2] = gemifiyaty[1] + 265;
	gemifiyatx[3] = gemifiyatx[0];
	gemifiyaty[3] = gemifiyaty[2] + 265;
	para = 100;
	// gemi fiyatlari
	gemifiyati[SARIGEMI] = 15;
	gemifiyati[SIYAHGEMI] = 25;
	gemifiyati[YILDIZGEMI] = 30;
	gemifiyati[DIKENGEMI] = 50;
	// coin
	pararesmi.loadImage("coin_icon.png");
	pararesmix = 15;
	pararesmiy = 15;
	// para yeterliligini denetleme
	parayeterli[SARIGEMI] = true;
	parayeterli[SIYAHGEMI] = true;
	parayeterli[YILDIZGEMI] = true;
	parayeterli[DIKENGEMI] = true;

}

void gCanvas::gemiFiyatDraw()
{
	for (int sira = 0; sira < gemicesidi; ++sira)
	{
		gemifiyatresmi[parayeterli[sira]].draw(gemifiyatx[sira], gemifiyaty[sira], gemifiyatresmi[0].getWidth() / 2,
											   gemifiyatresmi[0].getHeight() / 2);
				parafont.drawText("" + gToStr(gemifiyati[sira]), gemifiyatx[sira], gemifiyaty[sira] +
						 	  	  parafont.getStringHeight("" + gToStr(gemifiyati[sira]))*1.5f);
		// para kontrolu
		if (para >= gemifiyati[sira])
			parayeterli[sira] = true;
		else
			parayeterli[sira] = false;
	}
	pararesmi.draw(pararesmix, pararesmiy);
	parafont.drawText(""+ gToStr(para), pararesmix + 110, pararesmiy + 35);
}

void gCanvas::oyunSonuSetup()
{
	oyunsonucu = -1;
	oyunsonu.loadImage("bitti/kazandiniz_arka.png");
	oyunsonux = 0;
	oyunsonuy = 0;
	oyunsonusonucu[KAYIP].loadImage("bitti/you_lose.png");
	oyunsonusonucu[KAZANC].loadImage("bitti/you_win.png");
	oyunsonusonucux = (getWidth() / 2) - (oyunsonusonucu[0].getWidth() / 2);
	oyunsonusonucuy = (getHeight() / 2) - (oyunsonusonucu[0].getHeight());
}

void gCanvas::oyunSonuDraw()
{
	if(oyunmodu != OYUN_BITTI)
		return;
	oyunsonu.draw(oyunsonux, oyunsonuy);
	oyunsonusonucu[oyunsonucu].draw(oyunsonusonucux, oyunsonusonucuy);
	restartbuton[restartbutondurum].draw(restartbutonx, restartbutony, restartbutonw, restartbutonh);
	exitbuton[exitbutondurum].draw(exitbutonx, exitbutony, exitbutonw, exitbutonh);
}

void gCanvas::restartbutonSetup()
{
	// restart butonu
	restartbutondurum = NORMAL;
	restartbuton[NORMAL].loadImage("bitti/bitti_yeniden.png");
	restartbuton[TIKLA].loadImage("bitti/bitti_yeniden_tiklandi.png");
	restartbutonw = restartbuton[restartbutondurum].getWidth();
	restartbutonh = restartbuton[restartbutondurum].getHeight();
	restartbutonx = (oyunsonu.getWidth() - restartbutonw) / 2;
	restartbutony = (oyunsonu.getHeight() + restartbutonh) / 2;
}

void gCanvas::restartbutonTikla(int farex, int farey, int hangimetod)
{
	if(farex > restartbutonx && farex < restartbutonx + restartbutonw
		&& farey > restartbutony && farey < restartbutony + restartbutonh)
		{
			if(hangimetod == PRESSED)
				restartbutondurum = TIKLA;
			if(hangimetod == RELEASED)
			{
				restartbutondurum = NORMAL;
				MenuCanvas* cnv = new MenuCanvas(root);
				appmanager->setCurrentCanvas(cnv);
			}
			if(hangimetod == DRAGGED)
				restartbutondurum = TIKLA;
		}
		else restartbutondurum = NORMAL;
}

void gCanvas::sesbariSetup()
{
	sesbari.loadImage("surebar.png");
	sesbariw = sesbari.getWidth();
	sesbarih = sesbari.getHeight();
	sesbarix = sesbutonx + sesbutonw / 2 - sesbariw / 2;
	sesbariy = sesbutony - sesbarih;
	// ses cubugu
	sescubukw = (sesbariw - 40) * (root->sesseviyesi / 10.0f);
	sescubukh = sesbarih - 30;
	sescubukx = sesbarix + 20;
	sescubuky = sesbariy + 15;
}

void gCanvas::sesbariDraw()
{
	if (oyunmodu == OYUN_SES)
	{
		// arka plan
		setColor(25, 25, 25, 75);
		gDrawRectangle(sesbarix, sesbariy, sesbariw, sesbarih, true);
		// ses seviyesi
		setColor(0, 255, 0, 255);
		gDrawRectangle(sescubukx, sescubuky, sescubukw, sescubukh, 1);
		setColor(255, 255, 255, 255);
		sesbari.draw(sesbarix, sesbariy);
	}
}

void gCanvas::sesbariTikla(int x, int y, int eventtype)
{
	if (y > sesbariy && y < sesbariy + sesbarih && x > sesbarix && x < sesbarix + sesbariw / 2) {
		// ses seviyesi degiskeni
		if (root->sesseviyesi > 0) {
			root->sesseviyesi -= 1;
			// ses cubugunu guncelle
			sescubukw = (sesbariw - 40) * (root->sesseviyesi / 10.0f);
			// ses nesnesine gonder
			root->sesSeviyesi(0, root->sesseviyesi); // bayrak
		}
	}
	if (y > sesbariy && y < sesbariy + sesbarih &&
		x > sesbarix + sesbariw / 2 && x < sesbarix + sesbariw)
	{
		if (root->sesseviyesi < 10) {
			root->sesseviyesi += 1;
			// ses cubugunu guncelle
			sescubukw = (sesbariw - 40) * (root->sesseviyesi / 10.0f);
			// ses nesnesine gonder
			root->sesSeviyesi(0, root->sesseviyesi); // bayrak
		}
	}
}

void gCanvas::exitbutonSetup() {
	// exit butonu
	exitbutondurum = NORMAL;
	exitbuton[NORMAL].loadImage("bitti/bitti_geri.png");
	exitbuton[TIKLA].loadImage("bitti/bitti_geri_tiklandi.png");
	exitbutonw = exitbuton[exitbutondurum].getWidth();
	exitbutonh = exitbuton[exitbutondurum].getHeight();
	exitbutonx = (oyunsonu.getWidth() - exitbutonw) / 4;
	exitbutony = (oyunsonu.getHeight() + exitbutonh) / 2;
}

void gCanvas::exitbutonTikla(int farex, int farey, int hangimetod) {
	if(farex > exitbutonx && farex < exitbutonx + exitbutonw
		&& farey > exitbutony && farey < exitbutony + exitbutonh)
		{
			if(hangimetod == PRESSED)
				exitbutondurum = TIKLA;
			if(hangimetod == RELEASED)
			{
				exitbutondurum = NORMAL;
				exit(0);
			}
			if(hangimetod == DRAGGED)
				exitbutondurum = TIKLA;
		}
		else exitbutondurum = NORMAL;
}







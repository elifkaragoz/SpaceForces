/*
 * MenuCanvas.cpp
 *
 *  Created on: 8 Þub 2024
 *      Author: Elif :)
 */

#include "MenuCanvas.h"
#include "gCanvas.h"

MenuCanvas::MenuCanvas(gApp* root) : gBaseCanvas(root)
{
	this->root = root;
	// TODO Auto-generated constructor stub
	muzikcalar.loadSound("menu_muzigi.mp3");
	sescalar[0].loadSound("click.wav");
	sescalar[1].loadSound("dusmanates.wav");
	sescalar[2].loadSound("patlama.mp3");
}

MenuCanvas::~MenuCanvas() {
	// TODO Auto-generated destructor stub
}

void MenuCanvas::setup()
{
	sesseviyesi = 5;
	map.loadImage("menu_background2.png");
	baslikYazi.loadFont("FreeSansBold.ttf", 60);
	playbutonSetup();
	hedefxtamam = false;
	hedefytamam = false;
	patlayangemiSetup();
	dpatlayangemiSetup();
	hareketliGemiSetup();
	patlamaefektiSetup();
	muzikPlay();
}

void MenuCanvas::update()
{
	patlayangemiUpdate();
	dpatlayangemiUpdate();
	hareketliGemiUpdate();
}

void MenuCanvas::draw()
{
	map.draw(0,0);
	patlayangemiDraw();
	dpatlayangemiDraw();
	hareketliGemiDraw();
	patlamaefektiDraw();
	playbutonDraw();
	setColor(255, 255, 150, 150);
	baslikYazi.drawText("Space ", getWidth() / 2.6, 300);
	setColor(200, 150, 120, 150);
	baslikYazi.drawText("Forces ", getWidth() / 2, 400);
	setColor(255, 255, 255, 255);
}

void MenuCanvas::keyReleased(int key) {

}
void MenuCanvas::keyPressed(int key) {
}

void MenuCanvas::charPressed(unsigned int codepoint) {
}

void MenuCanvas::mouseMoved(int x, int y) {
	//hareketliGemiMoved(x, y);
	mousex = x;
	mousey = y;
}

void MenuCanvas::mouseDragged(int x, int y, int button)
{
	//playbutonTikla(x, y, DRAGGED);
}

void MenuCanvas::mousePressed(int x, int y, int button)
{
	sesPlay(SES_CLICK);
	playbutonTikla(x, y, PRESSED);

}

void MenuCanvas::mouseReleased(int x, int y, int button)
{
	playbutonTikla(x, y, RELEASED);
}

void MenuCanvas::mouseScrolled(int x, int y) {
}

void MenuCanvas::mouseEntered() {
}

void MenuCanvas::mouseExited() {
}

void MenuCanvas::windowResized(int w, int h) {
}

void MenuCanvas::showNotify() {
}

void MenuCanvas::hideNotify() {
}

void MenuCanvas::playbutonTikla(int farex, int farey, int hangimetod)
{
	if(farex > playbutonx && farex < playbutonx + playbutonw
	&& farey > playbutony && farey < playbutony + playbutonh)
	{
		if(hangimetod == PRESSED)
			playbutondurum = TIKLA;
		if(hangimetod == RELEASED)
		{
			playbutondurum = NORMAL;
			muzikStop();
			gCanvas* cnv = new gCanvas(root);
			appmanager->setCurrentCanvas(cnv);
		}
		if(hangimetod == DRAGGED)
			playbutondurum = TIKLA;
	}
	else playbutondurum = NORMAL;
}

void MenuCanvas::playbutonSetup()
{
	playbutondurum = NORMAL;
	playbuton[NORMAL].loadImage("play.png");//NORMAL
	playbuton[TIKLA].loadImage("play_ticked.png");//TIKLA
	playbutonw = playbuton[playbutondurum].getWidth();
	playbutonh = playbuton[playbutondurum].getHeight();
	playbutonx = getWidth() / 2 - playbutonw / 2;
	playbutony = getHeight() / 2;
}

void MenuCanvas::playbutonDraw()
{
	setColor(255, 0, 0, 150);
	playbuton[playbutondurum].draw(playbutonx + 10, playbutony + 10);
	setColor(255, 255, 255, 255);
	playbuton[playbutondurum].draw((playbutonx + 10 * playbutondurum), (playbutony + 10 * playbutondurum));
}

void MenuCanvas::patlayangemiSetup()
{
	patlayangemi.loadImage("animasyon/ship_1_1.png");
	patlayangemiw = patlayangemi.getWidth();
	patlayangemih = patlayangemi.getHeight();
	patlayangemix = 10;
	patlayangemiy = ((getHeight() / 2) - (patlayangemih / 2));
	patlayangemihizx = ((getWidth() - patlayangemiw) / (120 * 2)); // hedef mesafesi (gelince patlayacak)
	patlayangemihizy = (-5 / 2);
	// patlayangemihizy = (-5 + 1) / (60 * 4);
	patlayangemiSayac = 0;
	patlayangemiHareketi = 0;
	patlayangemiRotate = 135.0f;
}

void MenuCanvas::patlayangemiDraw()
{
	patlayangemi.draw(patlayangemix, patlayangemiy, patlayangemiw, patlayangemih, patlayangemiRotate);
}

void MenuCanvas::patlayangemiUpdate()
{
	if (patlayangemiSayac < 120)
		patlayangemiSayac++;
	else
	{
		patlayangemiSayac = 0;
		patlayangemiHareketi++;
	}
	// yatay
	if (patlayangemix < (getWidth() - patlayangemiw))
		patlayangemix += patlayangemihizx;
	else
		hedefxtamam = true;
	// dikey hareketleri
	if (patlayangemiHareketi == 0)
		patlayangemiy += patlayangemihizy;
	if (patlayangemiHareketi == 1)
		patlayangemiy -= patlayangemihizy;
	if (patlayangemiHareketi >= 2)
	{
		hedefytamam = true;
		if (hedefxtamam == true && hedefytamam == true)
		{
			patlamaefektix = patlayangemix;
			patlamaefektiy = patlayangemiy;
			patlamaefektibasla = true;
			root->sesPlay(root->SES_PATLA);
			// gemi reset
			patlayangemiHareketi = 0;
			hedefxtamam = false;
			hedefytamam = false;
			patlayangemix = 10;
			patlayangemiy = ((getHeight() / 2) - (patlayangemih / 2));
			patlayangemiRotate = 135;
		}
	}
	// aci hareketi
	if (patlayangemiRotate < 225)
		patlayangemiRotate += (90.0f / (120.0f * 2)); // 90 / 240
}

void MenuCanvas::dpatlayangemiSetup()
{
	dpatlayangemi.loadImage("animasyon/ship_1_1.png");
	dpatlayangemiw = dpatlayangemi.getWidth();
	dpatlayangemih = dpatlayangemi.getHeight();
	dpatlayangemix = (getWidth() - dpatlayangemiw);
	dpatlayangemiy = ((map.getHeight() / 2) - 120);
	dpatlayangemihizx = ((getWidth() - dpatlayangemiw) / (120 * 2)); // hedef mesafesi (gelince patlayacak)
	dpatlayangemihizy = (- (-5 / 2));
	dpatlayangemiSayac = 0;
    dpatlayangemiHareketi = 0;
    dpatlayangemiRotate = 45.0f;
}

void MenuCanvas::dpatlayangemiDraw()
{
	dpatlayangemi.draw(dpatlayangemix, dpatlayangemiy, dpatlayangemiw, dpatlayangemih, dpatlayangemiRotate);
}

void MenuCanvas::dpatlayangemiUpdate()
{
	if (dpatlayangemiSayac < 120)
		dpatlayangemiSayac++;
	else
    {
	    dpatlayangemiSayac = 0;
	    dpatlayangemiHareketi++;
    }
	// yatay
    if (dpatlayangemix > 0)
	   dpatlayangemix -= dpatlayangemihizx;
    // dikey hareketleri
	if (dpatlayangemiHareketi == 0)
		dpatlayangemiy += dpatlayangemihizy;
	if (dpatlayangemiHareketi == 1)
		dpatlayangemiy -= dpatlayangemihizy;
	// aci hareketi
	if (dpatlayangemiRotate > - 45)
	    dpatlayangemiRotate -= (90.0f / (120.0f * 2)); // 90 / 240
}

void MenuCanvas::hareketliGemiSetup()
{
	hareketligemi.loadImage("animasyon/ship_2_1.png");
	hareketligemix = (getWidth() / 2);
	hareketligemiy = (getHeight() / 2);
	hareketligemiw = hareketligemi.getWidth();
	hareketligemih = hareketligemi.getHeight();
	hareketligemihiz = 10;
	hareketligemirotate = 0.0f;
}

void MenuCanvas::hareketliGemiDraw()
{
	hareketligemi.draw(hareketligemix, hareketligemiy, hareketligemiw, hareketligemih, hareketligemirotate);
}

void MenuCanvas::hareketliGemiUpdate()
{
	// update ile koordinatlarý alýnca ekrandan mouse çýktýðýnda da köþeleri takip edebiliyor.
	// geminin mouse'u takip etmesi
	hareketligemix += (mousex - (hareketligemix + (hareketligemi.getHeight() / 2))) * 0.12f; // 0.1f olmayýnca daha keskin takipler yapýyor. 0.1f koyunca kayýyormuþ gibi takip ediyor.
	hareketligemiy += (mousey - (hareketligemiy + (hareketligemi.getWidth() / 2))) * 0.12f;
}

void MenuCanvas::patlamaefektiSetup()
{
	patlamaefektibasla = false;
	patlamaefekti.loadImage("explosion.png");
	patlamaefektix = 0;
	patlamaefektiy = 0;
	patlamaefektiw = 64;
	patlamaefektih = 64;
	patlamaefektiframe = 0;
	// src resim dosyasýnýn koordinatlarýyla ilgilenir.
	patlamaefektisrc.set(64 * (patlamaefektiframe % 4), 64 * (patlamaefektiframe / 4),
						 64 * ((patlamaefektiframe % 4) + 1), 64 * ((patlamaefektiframe / 4) + 1));
	// dst hedef noktayla ilgilenir.
	patlamaefektidst.set(0, 0, 0, 0);
}

void MenuCanvas::patlamaefektiDraw()
{
	if (patlamaefektibasla != true)
		return;
	// cizdirilecek frame'i sec
	patlamaefektisrc.set(64 * (patlamaefektiframe % 4), 64 * (patlamaefektiframe / 4),
						 64 * ((patlamaefektiframe % 4) + 1), 64 * ((patlamaefektiframe / 4) + 1));
	patlamaefektidst.set(patlamaefektix, patlamaefektiy, patlamaefektix + patlayangemiw, patlamaefektiy + patlayangemih);
	patlamaefekti.drawSub(patlamaefektisrc, patlamaefektidst, 0.0f);
	if (patlamaefektiframe < 15 && patlamaefektiframe > 0)
		patlamaefektiframe += 1;
		//gLogi("kontrol") << patlamaefektiframe;
	else
	{
		//gLogi("kontrol") << "geldi1";
		patlamaefektiframe = 0;
		patlamaefektibasla = false;
	}
}

void MenuCanvas::muzikPlay()
{
	if (muzikcalar.isPlaying() == false)
		muzikcalar.play();
}

void MenuCanvas::muzikStop()
{
	if (muzikcalar.isPlaying() == true)
		muzikcalar.stop();
}

void MenuCanvas::sesPlay(int id)
{
	if (id < 0 || id >= sessayisi)
		return;
	if (sescalar[id].isPlaying() == false) // stop modunda olduðundan emin ol
		sescalar[id].play();
}

void MenuCanvas::sesSeviyesi(int id, int volume)
{
	if (volume < 0 || volume > 9)
		return;
	if (id < 0 || id >= sessayisi)
		return;
	sescalar[id].setVolume(volume * 0.1f); // herhangi bir sayiyi 0.1f ile çarparsan float olur
}

void MenuCanvas::sesStop(int id)
{
	if (id < 0 || id >= sessayisi)
		return;
	if (sescalar[id].isPlaying() == true)
		sescalar[id].stop();
}


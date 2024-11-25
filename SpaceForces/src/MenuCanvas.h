/*
 * MenuCanvas.h
 *
 *  Created on: 8 Þub 2024
 *      Author: Elif :)
 */

#ifndef SRC_MENUCANVAS_H_
#define SRC_MENUCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "gFont.h"

class MenuCanvas: public gBaseCanvas {
public:
	MenuCanvas(gApp* root);
	virtual ~MenuCanvas();

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
	static const int NORMAL = 0, TIKLA = 1;
	gImage map;

	//play button
	gImage playbuton[2];// [0]=>NORMAL [1]=>TIKLA
	int playbutonx, playbutony, playbutonw, playbutonh;
	int playbutondurum;
	void playbutonSetup();
	void playbutonDraw();
	void playbutonTikla(int farex, int farey, int hangimetod);
	gFont baslikYazi;
	//oyuncu gemi
	gImage patlayangemi;
	float patlayangemix, patlayangemiy, patlayangemiw, patlayangemih;
	float patlayangemihizx, patlayangemihizy;
	float patlayangemiRotate;
	int patlayangemiSayac;
	int patlayangemiHareketi;
	void patlayangemiSetup();
	void patlayangemiDraw();
	void patlayangemiUpdate();
	//düþman gemi
	gImage dpatlayangemi;
	float dpatlayangemix, dpatlayangemiy, dpatlayangemiw, dpatlayangemih;
	float dpatlayangemihizx, dpatlayangemihizy;
	float dpatlayangemiRotate;
	int dpatlayangemiSayac;
	int dpatlayangemiHareketi;
	void dpatlayangemiSetup();
	void dpatlayangemiDraw();
	void dpatlayangemiUpdate();
	// hareketli gemi
	gImage hareketligemi;
	int hareketligemix, hareketligemiy, hareketligemiw, hareketligemih, hareketligemihiz;
	float hareketligemirotate; // sadece tam sayýyla olmayacak tüm hareketler için dönecek
	int mousex, mousey;
	void hareketliGemiSetup();
	void hareketliGemiDraw();
	//void hareketliGemiMoved(int mousex, int mousey);
	void hareketliGemiUpdate();
	// patlama efekti
	gImage patlamaefekti;
	int patlamaefektix, patlamaefektiy, patlamaefektiw, patlamaefektih, patlamaefektiframe;
	gRect patlamaefektisrc, patlamaefektidst; // zoom yapmak içinde kullanýlýr.
	void patlamaefektiSetup();
	void patlamaefektiDraw();
	bool hedefxtamam, hedefytamam, patlamaefektibasla;
	gFmodSound muzikcalar;
	void muzikPlay();
	void muzikStop();
	static const int sessayisi = 3;
	gFmodSound sescalar[sessayisi];
	static const int SES_CLICK = 0, SES_ATES = 1, SES_PATLA = 2;
	int sesseviyesi;
	void sesPlay(int id);
	void sesSeviyesi(int id, int volume);
	void sesStop(int id);
};

#endif /* SRC_MENUCANVAS_H_ */

/*
 * gApp.cpp
 *
 *  Created on: May 6, 2024
 *      Author: elif
 */

#include "gApp.h"
#include "gCanvas.h"
#include "MenuCanvas.h"

gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
}

void gApp::setup()
{
	sesseviyesi = 5;
	sescalar[0].loadSound("click.wav");
	sescalar[1].loadSound("dusmanates.wav");
	sescalar[2].loadSound("patlama.mp3");

	muzikcalar.loadSound("menu_muzigi.mp3");

	MenuCanvas* cnv = new MenuCanvas(this);
	appmanager->setCurrentCanvas(cnv);
}

void gApp::update() {
}

void gApp::sesPlay(int id)
{
	if (id < 0 || id >= sessayisi)
		return;
	if (sescalar[id].isPlaying() == false) // stop modunda olduðundan emin ol
		sescalar[id].play();
}

void gApp::sesSeviyesi(int id, int volume)
{
	if (volume < 0 || volume > 9)
		return;
	if (id < 0 || id >= sessayisi)
		return;
	sescalar[id].setVolume(volume * 0.1f); // herhangi bir sayiyi 0.1f ile çarparsan float olur
}

void gApp::sesStop(int id)
{
	if (id < 0 || id >= sessayisi)
		return;
	if (sescalar[id].isPlaying() == true)
		sescalar[id].stop();
}

void gApp::muzikPlay()
{
	if (muzikcalar.isPlaying() == false)
		muzikcalar.play();
}

void gApp::muzikStop()
{
	if (muzikcalar.isPlaying() == true)
		muzikcalar.stop();
}












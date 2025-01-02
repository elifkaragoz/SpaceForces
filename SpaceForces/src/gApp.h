/*
 * gApp.h
 *
 *  Created on: 8 Þub 2024
 *      Author: elif
 */

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"

class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	static const int SES_CLICK = 0, SES_ATES = 1, SES_PATLA = 2;
	int sesseviyesi;
	void sesPlay(int id);
	void sesSeviyesi(int id, int volume);
	void sesStop(int id);
	void muzikPlay();
	void muzikStop();

private:
	static const int sessayisi = 3;
	gFmodSound sescalar[sessayisi];
	gFmodSound muzikcalar;
};

#endif /* GAPP_H_ */

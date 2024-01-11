#include <iostream>
#include <sl.h>
#include "menu.h"
#include <vector>
#include <random>
#include <Windows.h>
#include <string>
#include <chrono>

bool isCollisiont(bucket, ball1);
bool isCollisiont(bucket, ball2);
bool isCollisiont(bucket, ball3);
bool isCollisiont(bucket, bomb);

using namespace std;


// Assets Backsound
void backsound()
{
	int bgm = slLoadWAV("theme1.wav");
	slSoundLoop(bgm);
}

// Assets Sfx
void select()
{
	int sfx = slLoadWAV("select.wav");
	slSoundPlay(sfx);
}

void popup()
{
	int sfx = slLoadWAV("popup.wav");
	slSoundPlay(sfx);
}

void opengame()
{
	int sfx = slLoadWAV("opengame.wav");
	slSoundPlay(sfx);
}

// Assets Color
void normalcolor()
{
	slSetForeColor(1, 1, 1, 1);
}

void colorblack()
{
	slSetForeColor(0, 0, 0, 1);
}

void colorred()
{
	slSetForeColor(1, 0, 0, 1);
}

void colorgreen()
{
	slSetForeColor(0, 1, 0, 1);
}

void colorblue()
{
	slSetForeColor(0, 0, 1, 1);
}

void coloryellow()
{
	slSetForeColor(1, 1, 0, 1);
}

void colorpurple()
{
	slSetForeColor(1, 0, 1, 1);
}

void colorcyan()
{
	slSetForeColor(0, 1, 1, 1);
}

// Fungsi untuk memeriksa apakah titik (x, y) berada di dalam area persegi panjang dengan pusat (centerX, centerY) dan ukuran (width, height)
bool isPointInsideRect(int x, int y, int centerX, int centerY, int width, int height)
{
	int setengahLebar = width / 2;
	int setengahTinggi = height / 2;
	return x >= centerX - setengahLebar && x <= centerX + setengahLebar && y >= centerY - setengahTinggi && y <= centerY + setengahTinggi;
}

void drawGlitchText(int font, int fontSize, const string& text, double x, double y)
{
	// Original text
	slSetTextAlign(SL_ALIGN_LEFT);
	slSetFont(font, fontSize);
	coloryellow(); // Yellow Color
	slText(x, y, text.c_str());

	// Glitch effect 1
	slSetTextAlign(SL_ALIGN_LEFT);
	slSetFont(font, fontSize);
	coloryellow(); // Adjust color as needed
	slText(x - 2, y - 2, text.c_str());

	// Glitch effect 2
	slSetTextAlign(SL_ALIGN_LEFT);
	slSetFont(font, fontSize);
	coloryellow(); // Adjust color as needed
	slText(x + 2, y + 2, text.c_str());
}

int main()
{
	// Ukuran Windows
	slWindow(1920, 1080, "Catch The Ball", 1);
	//backsound();

	// Random
	random_device rd;

	// Background
	vector <int> imgBack(3);
	{
		imgBack[0] = slLoadTexture("background.jpg");
		imgBack[1] = slLoadTexture("ground.png");
		imgBack[2] = slLoadTexture("cloud2.png");
	}

	// Position Background
	Back bg;
	bg.posX1 = 0;
	bg.posX2 = 1920;
	bg.posY = 540;

	// Position Ground
	Ground gd;
	gd.posX = 960;
	gd.posY = 403;

	// Position Cloud
	Cloud c;
	c.posX = 1000;
	c.posY = 600;

	// Random Cloud
	vector <Cloud> clouds;
	for (int i = 0; i < 5; i++)
	{
		double x = c.posX * i / 4.0 + 100 * i;
		double y = gd.posY + 50 + static_cast<double>(rd() % 3) * 150 + 50;
		clouds.push_back({ x, y });
	}

	// Bird1 dari Kiri
	vector <int> Bird1Left;
	{
		Bird1Left.push_back(slLoadTexture("bird2_1.png"));
		Bird1Left.push_back(slLoadTexture("bird2_2.png"));
		Bird1Left.push_back(slLoadTexture("bird2_3.png"));
		Bird1Left.push_back(slLoadTexture("bird2_4.png"));
		Bird1Left.push_back(slLoadTexture("bird2_5.png"));
		Bird1Left.push_back(slLoadTexture("bird2_6.png"));
		Bird1Left.push_back(slLoadTexture("bird2_7.png"));
		Bird1Left.push_back(slLoadTexture("bird2_8.png"));
	}

	// Position Bird1
	Bird1 b1;
	b1.posX = -100;
	b1.posY = 720;

	// Random Bird1
	vector <Bird1> birds1;
	int numBird = rd() % 2 + 1;

	for (int i = 0; i < numBird; i++)
	{
		double x = b1.posX + i * 200;
		double y = b1.posY + static_cast<double>(rd() % 3);
		birds1.push_back({ x, y });
	}

	// Bird1
	int bird1Frame = 0;

	//Bird2 dari Kanan
	vector <int> Bird2Right;
	{
		Bird2Right.push_back(slLoadTexture("bird3_1.png"));
		Bird2Right.push_back(slLoadTexture("bird3_2.png"));
		Bird2Right.push_back(slLoadTexture("bird3_3.png"));
		Bird2Right.push_back(slLoadTexture("bird3_4.png"));
		Bird2Right.push_back(slLoadTexture("bird3_5.png"));
		Bird2Right.push_back(slLoadTexture("bird3_6.png"));
		Bird2Right.push_back(slLoadTexture("bird3_7.png"));
		Bird2Right.push_back(slLoadTexture("bird3_8.png"));
	}

	// Position Bird2
	Bird2 b2;
	b2.posX = 1940;
	b2.posY = 860;

	// Random Bird2
	vector <Bird2> birds2;
	int numBird2 = rd() % 2 + 1;

	for (int i = 0; i < numBird2; i++)
	{
		double x = b2.posX + i * 200;
		double y = b2.posY + static_cast<double>(rd() % 3);
		birds2.push_back({ x, y });
	}

	// Bird2
	int bird2Frame = 0;

	// All Birds
	double BirdDelay = 0.3;

	// Sun di Tengah
	vector <int> imgSun(2);
	{
		imgSun[0] = slLoadTexture("tengahansun.png");
		imgSun[1] = slLoadTexture("sampingansun.png");
	}

	// Position Sun
	Sun s;
	s.posX = 480;
	s.posY = 600;

	JariSun js;
	js.posX = 480;
	js.posY = 600;

	int duck = slLoadTexture("duck.png");

	// Position Duck
	Duck d;
	d.posX = 1930;
	d.posY = 290;

	// Jenis Font
	int font1 = slLoadFont("RoadPixel.ttf");
	int font2 = slLoadFont("Crima.ttf");
	int font3 = slLoadFont("game_over.ttf");
	int font4 = slLoadFont("pixel_lcd_7.ttf");
	int font5 = slLoadFont("PixelBook-Regular.ttf");

	// Position Font
	Font1 f1;
	f1.posX = 700;
	f1.posY = 550;

	// Teks Untuk Press
	string pressSpaceText = "Press Space To Continue";

	// Letak Coordinate
	int windowWidth = 960;
	int windowHeight = 105;

	double blinkTime = 0.0;

	double angle = 0;

	// load assets
	int imgBall1 = slLoadTexture("ball1.png");
	int imgBall2 = slLoadTexture("ball2.png");
	int imgBall3 = slLoadTexture("ball3.png");
	int imgBucket = slLoadTexture("bucket.png");
	int imgBomb = slLoadTexture("bomb.png");
	int imgHeart = slLoadTexture("heart.png");
	int imgBack1 = slLoadTexture("background2.jpg");
	int font = slLoadFont("catchtheball.ttf");
	int sndTheme = slLoadWAV("theme1.wav");
	int sndBomb = slLoadWAV("Bomb.wav");
	int sndCling = slLoadWAV("cling.wav");
	int sndGameover = slLoadWAV("gameover.wav");
	int GameOver = slLoadTexture("gameover.png");
	int Victory = slLoadTexture("victory.png");
	int sndVictory = slLoadWAV("victory.wav");
	slSoundLoop(sndTheme);

	const double gravity = -0.1;

	bool VictoryTriggered = false;
	bool gameOverTriggered = false;
	bool StopMovement = false;
	bool StopSound = false;
	bool GameStart = false;
	// jumlah tampilan score awal
	int Score = 0;
	int score = 3;
	// Background 
	back b;
	b.posX = 0;
	b.posY = 540;

	back bb;
	bb.posX = 1920;
	bb.posY = 540;

	back bbb;
	bbb.posX = 3840;
	bbb.posY = 540;

	ball1 bl;
	bl.posX = 1600;
	bl.posY = 1100;

	ball2 bll;
	bll.posX = 1600;
	bll.posY = 1100;

	ball3 blll;
	blll.posX = 1600;
	blll.posY = 1100;

	bomb bm;
	bm.posX = 1600;
	bm.posY = 1100;

	heart h;
	h.posX = 1800;
	h.posY = 1020;

	End e;
	e.posX = 960;
	e.posY = 550;

	// inisialisasi objek
	bucket bc{ 143, 143, 0, 0, 40 };
	bool isBall1Spawned = false;
	vector<ball1> balls1;
	if (!isBall1Spawned)
	{
		double y = (rd() % 700) + 1080;
		double x = (rd() % 1600) + 1;
		balls1.push_back({ x, y, 15 });
		isBall1Spawned = true;
	}

	bool isBall2Spawned = false;
	vector<ball2> balls2;
	if (!isBall2Spawned)
	{
		double y = (rd() % 700) + 1080;
		double x = (rd() % 1600) + 1;
		balls2.push_back({ x, y, 15 });
		isBall2Spawned = true;
	}

	bool isBall3Spawned = false;
	vector<ball3> balls3;
	if (!isBall3Spawned)
	{
		double y = (rd() % 700) + 1080;
		double x = (rd() % 1600) + 1;
		balls3.push_back({ x, y, 15 });
		isBall3Spawned = true;
	}

	bool isBombSpawned = false;
	vector<bomb> bombs;
	if (!isBombSpawned)
	{
		double y = (rd() % 700) + 1080;
		double x = (rd() % 1600) + 1;
		bombs.push_back({ x, y, 15 });
		isBall3Spawned = true;
	}

	slSetFont(font, 36);

	while (!slShouldClose())
	{
		

        // Periksa apakah tombol spasi ditekan
        if (slGetKey(' ') && !GameStart)
        {
			// Suara
			select();
			
            // Lakukan aksi yang diinginkan saat tombol spasi ditekan
            cout << "Space Pressed! Do something..." << endl;
            // Contoh: Keluar dari loop atau lakukan aksi lainnya
			GameStart = true;
			
            //break;

        }

		if (!GameStart)
		{
			// Background
			normalcolor(); // Default Color
			slSprite(imgBack1, b.posX, b.posY, 1920, 1080);
			slSprite(imgBack1, bb.posX, bb.posY, 1920, 1080);
			slSprite(imgBack1, bbb.posX, bbb.posY, 1920, 1080);

			b.posX -= 1;

			if (b.posX <= -1920)
				b.posX = 3840;

			bb.posX -= 1;

			if (bb.posX <= -1920)
				bb.posX = 3840;

			bbb.posX -= 1;

			if (bbb.posX <= -1920)
				bbb.posX = 3840;

			// Ground
			// slSprite(imgBack[1], gd.posX, gd.posY, 1920, 400);

			// Memanggil Duck
			normalcolor();
			slSprite(duck, d.posX, d.posY, 288, 64);

			d.posX -= 2.0; // -- Kecepatannya

			if (d.posX <= -140)
			{
				d.posX = 1925;
			}

			// Cloud
			for (Cloud& c : clouds)
			{
				c.posX -= 2;
				if (c.posX <= -200)
				{
					c.posX = 1920;
					c.posY = gd.posY + 50 + static_cast<double>(rd() % 3) * 150 + 100;
				}
				slSprite(imgBack[2], c.posX, c.posY, 238, 83);
			}

			// Bird1 random dari arah kiri
			for (Bird1& b1 : birds1)
			{
				b1.posX += 5;
				if (b1.posX >= 1930)
				{
					b1.posX = -50;
					b1.posY = 70 + static_cast<double>(rd() % 3) * 150 + 200;
				}
				slSprite(Bird1Left[bird1Frame], b1.posX, b1.posY, 62, 50);

				// Update animation frame dengan delay
				BirdDelay -= slGetDeltaTime();
				if (BirdDelay <= 0)
				{
					bird1Frame = (bird1Frame + 1) % Bird1Left.size();
					BirdDelay = 0.25; // Delay
				}

			}

			// Bird3 random dari arah kanan
			for (Bird2& b2 : birds2)
			{
				b2.posX -= 5;
				if (b2.posX <= -100)
				{
					b2.posX = 1940;
					b2.posY = 100 + static_cast<double>(rd() % 3) * 180 + 230;
				}
				slSprite(Bird2Right[bird2Frame], b2.posX, b2.posY, 62, 50);

				// Update animation frame dengan delay
				BirdDelay -= slGetDeltaTime();
				if (BirdDelay <= 0)
				{
					bird2Frame = (bird2Frame + 1) % Bird2Right.size();
					BirdDelay = 0.2; // Delay
				}

			}

			// Memanggil Sun
			slSprite(imgSun[0], s.posX, s.posY, 194, 194);

			// Rotate Jari Sun
			slPush();
			slTranslate(js.posX, js.posY);
			slRotate(angle);
			slSprite(imgSun[1], 0, 0, 332, 332);
			slPop();

			angle++;

			// Setting Font
			slSetTextAlign(SL_ALIGN_LEFT);
			slSetFont(font1, 114);

			// Input Text "Catch The Ball"
			drawGlitchText(font1, 124, "CATCH THE BALL", f1.posX, f1.posY);

			// Hitung waktu berkedip
			blinkTime += slGetDeltaTime();

			// Tampilkan teks dan berkedip
			if (static_cast<int>(blinkTime) % 2 == 0)
			{
				slSetTextAlign(SL_ALIGN_CENTER);
				slSetFont(font3, 130);
				colorblack();
				slText(windowWidth, windowHeight, "Press Space to Continue");
			}

			slRender();
		}
		else
		{
			slSetForeColor(1, 1, 1, 0.9);
			slSprite(imgBack1, b.posX, b.posY, 1920, 1080);
			slSprite(imgBack1, bb.posX, bb.posY, 1920, 1080);
			slSprite(imgBack1, bbb.posX, bbb.posY, 1920, 1080);

			if (!StopMovement)
			{
				b.posX -= 1;

				if (b.posX <= -1920)
					b.posX = 3840;

				bb.posX -= 1;

				if (bb.posX <= -1920)
					bb.posX = 3840;

				bbb.posX -= 1;

				if (bbb.posX <= -1920)
					bbb.posX = 3840;
			}

			slSetForeColor(1, 1, 1, 1);
			slSprite(imgHeart, h.posX, h.posY, 70, 60);

			if (!StopMovement)
			{
				slSetForeColor(1, 1, 1, 1);
				slSprite(imgBucket, bc.posX, bc.posY, 200, 80);
			}

			// Read input (mouse)
			if (slGetKey(SL_KEY_LEFT))
			{
				if (bc.posX - 10 >= 10)
				{
					bc.posX -= 10;
				}
				else
				{
					bc.posX = 10;
				}
			}

			if (slGetKey(SL_KEY_RIGHT))
			{
				if (bc.posX + 10 <= 1910)
				{
					bc.posX += 10;
				}
				else
				{
					bc.posX = 1910;
				}
			}

			// game state

			// update data
			// Bucket

			// Bom and Ball
			for (ball1& bl : balls1)
			{
				if (!StopMovement)
				{
					bl.posY -= 3;
					if (bl.posY <= 0)
					{
						bl.posY = 1100;
						bl.posX = (rd() % 1600) + 1;
					}

					if (isCollisiont(bc, bl))
					{
						Score += 10;
						bl.posY = 1100;
						bl.posX = (rd() % 1600) + 1;
						slSoundPlay(sndCling);
					}
				}
			}

			for (ball2& bll : balls2)
			{
				if (!StopMovement)
				{
					bll.posY -= 3.5;
					if (bll.posY <= 0)
					{
						bll.posY = 1100;
						bll.posX = (rd() % 1600) + 1;
					}

					if (isCollisiont(bc, bll))
					{
						Score += 15;
						bll.posY = 1100;
						bll.posX = (rd() % 1600) + 1;
						slSoundPlay(sndCling);
					}
				}
			}

			for (ball3& blll : balls3)
			{
				if (!StopMovement)
				{
					blll.posY -= 4;
					if (blll.posY <= 0)
					{
						blll.posY = 1100;
						blll.posX = (rd() % 1600) + 1;
					}

					if (isCollisiont(bc, blll))
					{
						Score += 20;
						blll.posY = 1100;
						blll.posX = (rd() % 1600) + 1;
						slSoundPlay(sndCling);
					}
				}
			}

			for (bomb& bm : bombs)
			{
				if (!StopMovement)
				{
					bm.posY -= 3;
					if (bm.posY <= 0)
					{
						bm.posY = 1100;
						bm.posX = (rd() % 1600) + 1;
					}

					if (isCollisiont(bc, bm))
					{
						score -= 1;
						bm.posY = 1100;
						bm.posX = (rd() % 1600) + 1;
						slSoundPlay(sndBomb);
					}
				}
			}
			// Render

			for (const ball1& bl : balls1)
				slSprite(imgBall1, bl.posX, bl.posY, 64, 64);

			for (const ball2& bll : balls2)
				slSprite(imgBall2, bll.posX, bll.posY, 64, 64);

			for (const ball3& blll : balls3)
				slSprite(imgBall3, blll.posX, blll.posY, 64, 64);

			for (const bomb& bm : bombs)
				slSprite(imgBomb, bm.posX, bm.posY, 64, 64);

			slSetForeColor(1, 1, 1, 1);
			slSetTextAlign(SL_ALIGN_RIGHT);
			slSetFont(font, 35);
			slText(450, 43, "Catch The Ball");

			slSetTextAlign(SL_ALIGN_LEFT);
			slSetFont(font, 45);
			slText(50, 1000, to_string(Score).c_str());

			slSetTextAlign(SL_ALIGN_LEFT);
			slSetFont(font, 45);
			slText(1850, 995, to_string(score).c_str());
			// jika Victory muncul gambar Victory dan menghentikan semua objeck dan sound
			if (Score >= 250)
			{
				slSetForeColor(1, 1, 1, 1);
				slSprite(Victory, e.posX, e.posY, 635, 766);
				slSoundStop(sndTheme);
				StopMovement = true;
			}
			// umtuk memunculkan sound Victory agar tidak berulang
			if (!VictoryTriggered && Score >= 250)
			{
				slSoundPlay(sndVictory);
				VictoryTriggered = true;
			}
			// jika Gameover muncul gambar Gameover dan menghentikan semua objeck dan sound
			if (score <= 0)
			{
				slSetForeColor(1, 1, 1, 1);
				slSprite(GameOver, e.posX, e.posY, 635, 766);
				slSoundStop(sndTheme);
				StopMovement = true;
			}
			// umtuk memunculkan sound Gameover agar tidak berulang
			if (!gameOverTriggered && score <= 0)
			{
				slSoundPlay(sndGameover);
				gameOverTriggered = true;
			}

			slRender();
		}
	}

	slClose();
	return 0;

}

// mengecek apakah ada collisiont / tabrakan
// antara Bucket dengan Bom dan Ball

bool isCollisiont(bucket bc, ball1 bl)
{
	double dx = bc.posX - bl.posX;
	double dy = bc.posY - bl.posY;
	double distrance = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	if (distrance <= bc.radius + bl.radius)
		return true;

	return false;
}

bool isCollisiont(bucket bc, ball2 bll)
{
	double dx = bc.posX - bll.posX;
	double dy = bc.posY - bll.posY;
	double distrance = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	if (distrance <= bc.radius + bll.radius)
		return true;

	return false;
}

bool isCollisiont(bucket bc, ball3 blll)
{
	double dx = bc.posX - blll.posX;
	double dy = bc.posY - blll.posY;
	double distrance = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	if (distrance <= bc.radius + blll.radius)
		return true;

	return false;
}

bool isCollisiont(bucket bc, bomb bm)
{
	double dx = bc.posX - bm.posX;
	double dy = bc.posY - bm.posY;
	double distrance = sqrt(pow(dx, 2.0) + pow(dy, 2.0));
	if (distrance <= bc.radius + bm.radius)
		return true;

	return false;
}
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <cmath>
using namespace std;
using namespace sf;

int w=1200, h=1200, pg, OK[9], i, j, gameMatrix[7][9], zoneMatrix[7][9], copyMatrix[7][9], fences[3][10], x, column = 0, counter[3], check[25], u[2][16];
int isLevelAvailable[100], Level[100], makeLevelReady;
int moves;
bool isMarked = false, isPut = false, isLevelPresed=false, reset=false, gameFinished=false, isNewGame=false;
bool putCow1=false, putCow2=false, putHorse1=false, putHorse2=false, putPig1=false, putSheep1=false, putPig2=false, putSheep2=false;
Texture t1, board, exitButtonImage, exitButtonFocused, background, back, levelBackground, infoImage;
Texture horizontalFence2, horizontalFence3, verticalFence2, verticalFence3, cornerFence1, cornerFence2, cornerFence3, cornerFence4;
Texture cow1, cow2, horse1, horse2, pig1, sheep1;
Sprite cow1Sprite, cow2Sprite, horse1Sprite, horse2Sprite, pig1Sprite, sheep1Sprite, pig2Sprite, sheep2Sprite; 
Sprite t1Sprite, exitButton, boardSprite, backgroundSprite, backSprite, levelBackgroundSprite, infoImageSprite;
Sprite horizontalFence2Sprite, horizontalFence3Sprite, verticalFence2Sprite, verticalFence3Sprite;
Sprite cornerFence1Sprite, cornerFence2Sprite, cornerFence3Sprite, cornerFence4Sprite;
RectangleShape rectangleL2, rectangleL3, rectangleL4, rectangleL5;
RectangleShape rectangleCorner1, rectangleCorner2, rectangleCorner3, rectangleCorner4;
RectangleShape rectangleEnd;
Text startText, descriptionText, settingsText, Levels, win, lose, backLevel1, backLevel2, resetGame, nextLevel;
Text rules1, rules2, rules3, rules4, rules5, rules6, rules7;
Text difficulty1, difficulty2, difficulty3, difficulty4, difficulty5, difficulty6, difficulty7, difficulty, credits;
Text SettingSound, settingNoSound, settingSong1, settingSong2, newGame, newGameDone;
Text L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L20, L21, L22, L23, L24, L25, L26, L27, L28;
Text L29, L30, L31, L32, L33, L34, L35, L36, L37, L38, L39, L40, L41, L42, L43, L44, L45, L46, L47, L48, L49, L50, L51, L52, L53, L54;
Text L55, L56, L57, L58, L59, L60;
Font font;
RenderWindow window;

bool checkMatrix(int gameMatrix[7][9], int zoneMatrix[7][9], int u[25])
{
    for (int i = 0; i <= 24; i++)
        check[i] = 0; 
    for (int i = 0; i <= 6; i++)
    {
        for (int j = 0; j <= 8; j++)
            if (gameMatrix[i][j] > 9)
            {
                check[gameMatrix[i][j]] = zoneMatrix[i][j];
                check[gameMatrix[i][j] - 10] = check[gameMatrix[i][j] - 10] + zoneMatrix[i][j];
            }
    } 
    for (i = 20; i <= 24; i++)
        if (check[i] != check[i - 10] && 2 * check[i] != check[i - 10])
            return false;
    for (int i = 10; i <= 24; i++)
        check[i] = 0;
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= 7; j++)
            if (gameMatrix[i][j] >= 20)
            {
                if (check[zoneMatrix[i][j]] == 0)
                    check[zoneMatrix[i][j]] = gameMatrix[i][j];
                else if (gameMatrix[i][j] != check[zoneMatrix[i][j]])
                    return false; 
            }                     
    int isEmpty[20] = {0}, maxZone = 1;
    for (int i = 1; i <= 5; i++)
        for (int j = 1; j <= 7; j++)
        {
            if (maxZone < zoneMatrix[i][j])
                maxZone = zoneMatrix[i][j];
            if (gameMatrix[i][j] >= 20 && gameMatrix[i][j] <= 24)
                isEmpty[zoneMatrix[i][j]] = 1; 
        }
    for (i = 1; i <= maxZone; i++)
        if (isEmpty[i] == 0)
            return false; 

    return true;
}

bool isInBounds(int x, int y, int typeFence)
{
    switch(typeFence)
    {
       case 1:{ if (x == 0 || x == 6 || y > 4)
            return false;
            break;
       }
       case 2:
       {
            if (x == 0 || x == 6 || y > 2)
            return false;
            break;
       }
       case 3:
       {
         if (y == 0 || y == 8 || x > 2)
            return false;  
            break;
       }
       case 4:
       {
           if (y == 0 || y == 8 || x >= 1)
            return false;
            break;
       }
       case 5:
       {
         if(x==0 || x==6 || y+4>=8) return false;
         break;
       }
       case 6:
       {
           if(x==0 || x+4>6 || y==0 || y==8) return false;
           break;
       }
       case 7:
       {
           if(y==0 || x>=4 || y+4>8 ) return false;
           break;
       }
       case 8:
       {
         if(x>=2) return false;
         break;
       }
       default: break;
    
    }
    return true;
}

void markZone(int x, int y, int zone)
{
    if (x < 7 && y < 9 && x >= 0 && y >= 0)
    {
        if (zoneMatrix[x][y] == 0 && gameMatrix[x][y] != -1)
        {
            zoneMatrix[x][y] = zone;
            markZone(x + 1, y, zone);
            markZone(x, y + 1, zone);
            markZone(x - 1, y, zone);
            markZone(x, y - 1, zone);
        }
    }
}

void markAllZones()
{
    for (int i = 0; i < 7; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            zoneMatrix[i][j] = 0;
        }
    }

    int zone = 1;
    for (int i = 1; i < 6; ++i)
    {
        for (int j = 1; j < 8; ++j)
        {
            if (zoneMatrix[i][j] == 0 && gameMatrix[i][j] != -1)
            {
                markZone(i, j, zone++);
            }
        }
    }
}

void initGameMatrix(int matrix[7][9])
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 9; j++)
            if (i == 0 || j == 0 || i == 6 || j == 8)
                matrix[i][j] = -1;
            else
                matrix[i][j] = 0;
}

void initZoneMatrix(int matrix[7][9])
{
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 9; j++)
            matrix[i][j] = 0;
}
void printMatrix(int matrix[7][9])
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 9; j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
}

void getAllowedFenceCoordinates(int u[2][16])
{
    u[0][0] = 412;
    u[1][0] = 118;
    u[0][1] = 600;
    u[1][1] = 118;
    u[0][2] = 788;
    u[1][2] = 118;
    u[0][3] = 224;
    u[0][4] = 412;
    u[0][5] = 600;
    u[0][6] = 788;
    u[0][7] = 972;
    for (i = 3; i <= 7; i++)
        u[1][i] = 304;
    for (i = 8; i <= 12; i++)
    {
        u[1][i] = 490;
        u[0][i] = u[0][i - 5];
    }
    for (i = 13; i <= 15; i++)
    {
        u[1][i] = 676;
        u[0][i] = u[0][i - 13];
    }
}

void getNewFenceCoordinates(int &x, int &y, float a, float b)
{
    getAllowedFenceCoordinates(u);
    float min = 10000, distance;
    int z;
    for (z = 0; z <= 15; z++)
    {
        distance = sqrt((a - u[0][z]) * (a - u[0][z]) + (b - u[1][z]) * (b - u[1][z]));
        if (distance < min)
        {
            x = u[0][z];
            y = u[1][z];
            min = distance;
        }
    }
}

void placeFence(int i, int j, int matrix[7][9], int z)
{
    int a;
    cout << i << " " << j << " " << z << endl;
    switch (z)
    {
    case 1:
    {
        for (a = 1; a <= 5 && j < 9 && i < 7; a++)
        {
            matrix[i][j] = -1;
            j++;
        }
        break;
    }
    case 2:
    {
        for (a = 1; a <= 7 && j < 9 && i < 7; a++)
        {
            matrix[i][j] = -1;
            j++;
        }
        break;
    }
    case 3:
    {
        for (a = 1; a <= 5 && j < 9 && i < 7; a++)
        {
            matrix[i][j] = -1;
            i++;
        }
        break;
    }
    case 4:
    {
        for (a = 1; a <= 7 && j < 9 && i < 7; a++)
        {
            cout << i << " " << j << endl;
            matrix[i][j] = -1;
            i++;
        }
        break;
    }
    case 5:
    {
        matrix[i+2][j+4]=-1;
        matrix[i+1][j+4]=-1;
        for (a = 1; a <= 5 && j < 9 && i < 7; a++)
        {
            matrix[i][j] = -1;
            j++;
        }
        break;

    }
    case 6:
    {
        matrix[i][j+1]=-1;
        matrix[i][j+2]=-1;
        for (a = 1; a <= 5 && j < 9 && i < 7; a++)
        {
            matrix[i][j] = -1;
            i++;
        }
        break;
    }
    case 7:
    {
        matrix[i][j]=-1;
        matrix[i+1][j]=-1;
        for (a = 1; a <= 5 && j < 9 && i < 7; a++)
        {
            matrix[i+2][j] = -1;
            j++;
        }
        break;
    }
    case 8:
    {
       matrix[i+4][j-2]=-1;
       matrix[i+4][j-1]=-1;
        for (a = 1; a <= 5 && j < 9 && i < 7; a++)
        {
            matrix[i][j] = -1;
            i++;
        }
    }
    default:
        break;
    }
}

void getMatrixCoordinates(int &i, int &j, int x, int y)
{
    i = 0, j = 0;
    while (x > 226)
    {
        j = j + 2;
        x = x - 188;
    }
    while (y > 118)
    {
        i = i + 2;
        y = y - 186;
    }
}

int main()
{
    isLevelAvailable[1]=1;
    for(i=2;i<=58;i++) isLevelAvailable[i]=1;
    RenderWindow window;
    Vector2i centrare((VideoMode::getDesktopMode().width / 2) - 600, (VideoMode::getDesktopMode().height / 2) - 600);
    window.create(VideoMode(w, h), "Smart Farmer");
    window.setPosition(centrare);
    SoundBuffer buttonPressed, victory, lost;
    Sound buttonSound, victorySound, lostSound;
    Music song1, song2;
    song1.setLoop(true);
    song2.setLoop(true);
    song1.openFromFile("song1.wav");
    song2.openFromFile("song2.wav");
    victory.loadFromFile("victory.wav");
    lost.loadFromFile("lose.wav");
    victorySound.setBuffer(victory);
    lostSound.setBuffer(lost);
    buttonPressed.loadFromFile("buttonSound.wav");
    buttonSound.setBuffer(buttonPressed);
    infoImage.loadFromFile("infoImage.jpg");
    infoImageSprite.setTexture(infoImage);
    infoImageSprite.setScale(2, 2.5);
    cornerFence1.loadFromFile("GardColt1.png");
    cornerFence1Sprite.setTexture(cornerFence1);
    cornerFence2.loadFromFile("GardColt2.png");
    cornerFence2Sprite.setTexture(cornerFence2);
    cornerFence3.loadFromFile("GardColt3.png");
    cornerFence3Sprite.setTexture(cornerFence3);
    cornerFence4.loadFromFile("GardColt4.png");
    cornerFence4Sprite.setTexture(cornerFence4);
    horizontalFence3.loadFromFile("GardOrizontalL3.png");
    horizontalFence3Sprite.setTexture(horizontalFence3);
    verticalFence2.loadFromFile("GardVerticalL2.png");
    verticalFence2Sprite.setTexture(verticalFence2);
    verticalFence3.loadFromFile("GardVerticalL3.png");
    verticalFence3Sprite.setTexture(verticalFence3);
    cow1.loadFromFile("vaca1.png");
    cow1Sprite.setTexture(cow1);
    cow2.loadFromFile("vaca2.png");
    cow2Sprite.setTexture(cow2);
    horse1.loadFromFile("cal1.png");
    horse1Sprite.setTexture(horse1);
    horse2.loadFromFile("cal2.png");
    horse2Sprite.setTexture(horse2);
    pig1.loadFromFile("porc1.png");
    pig1Sprite.setTexture(pig1);
    pig2Sprite.setTexture(pig1);
    sheep1.loadFromFile("oaie.png");
    sheep1Sprite.setTexture(sheep1);
    sheep2Sprite.setTexture(sheep1);
    sheep1Sprite.setScale(1.5, 1.5);
    sheep2Sprite.setScale(1.5, 1.5);
    pig1Sprite.setScale(1.5, 1.5);
    pig2Sprite.setScale(1.5, 1.5);
    horse1Sprite.setScale(1.5, 1.5);
    horse2Sprite.setScale(1.5, 1.5);
    cow1Sprite.setScale(1.5, 1.5);
    cow2Sprite.setScale(1.5, 1.5);
    t1.loadFromFile("background.jpg");
    t1Sprite.setTexture(t1);
    t1Sprite.setScale(0.63, 0.85);
    if (!exitButtonFocused.loadFromFile("exitButtonFocused.png"))
        cout << "Can't find the image" << endl;
    if (!exitButtonImage.loadFromFile("exitbutton.png"))
        cout << "Can't find the image" << endl;
    exitButton.setPosition(1075.00, 10.0);
    exitButton.setTexture(exitButtonImage);
    exitButton.setScale(0.2, 0.2);

    levelBackground.loadFromFile("levelBackground.jpg");
    levelBackgroundSprite.setTexture(levelBackground);
    levelBackgroundSprite.setScale(2, 3);

    board.loadFromFile("board.png");
    boardSprite.setTexture(board);
    boardSprite.setScale(1.5, 1.5);
    boardSprite.setPosition(60, -30);

    horizontalFence2.loadFromFile("GardOrizontalL2.png");
    horizontalFence2Sprite.setTexture(horizontalFence2);

    if (!font.loadFromFile("font.ttf"))
    {
        cout << "Nu s-a putut incarca fontul";
    }

    Levels.setFont(font);
    L1.setFont(font); L2.setFont(font); L3.setFont(font); L4.setFont(font); L5.setFont(font); L6.setFont(font); L7.setFont(font);
    L8.setFont(font); L9.setFont(font); L10.setFont(font); L11.setFont(font); L12.setFont(font); L13.setFont(font); L14.setFont(font);
    L15.setFont(font); L16.setFont(font); L17.setFont(font); L18.setFont(font); L19.setFont(font); L20.setFont(font);
    L21.setFont(font); L22.setFont(font); L23.setFont(font); L24.setFont(font); L25.setFont(font); L26.setFont(font); L27.setFont(font); 
    L28.setFont(font); L29.setFont(font); L30.setFont(font); L31.setFont(font); L32.setFont(font); L33.setFont(font); L34.setFont(font); 
    L35.setFont(font); L36.setFont(font); L37.setFont(font); L38.setFont(font); L39.setFont(font); L40.setFont(font); L41.setFont(font); 
    L42.setFont(font); L43.setFont(font); L44.setFont(font); L45.setFont(font); L46.setFont(font); L47.setFont(font); L48.setFont(font);
    L49.setFont(font); L50.setFont(font); L51.setFont(font); L52.setFont(font); L53.setFont(font); L54.setFont(font); L55.setFont(font); 
    L56.setFont(font); L57.setFont(font); L58.setFont(font); L59.setFont(font); L60.setFont(font);   

    Levels.setStyle(Text::Bold);
    L1.setStyle(Text::Bold); L2.setStyle(Text::Bold); L3.setStyle(Text::Bold); L4.setStyle(Text::Bold); L5.setStyle(Text::Bold); 
    L6.setStyle(Text::Bold); L7.setStyle(Text::Bold); L8.setStyle(Text::Bold); L9.setStyle(Text::Bold); L10.setStyle(Text::Bold); 
    L11.setStyle(Text::Bold); L12.setStyle(Text::Bold); L13.setStyle(Text::Bold); L14.setStyle(Text::Bold); L15.setStyle(Text::Bold); 
    L16.setStyle(Text::Bold); L17.setStyle(Text::Bold); L18.setStyle(Text::Bold); L19.setStyle(Text::Bold); L20.setStyle(Text::Bold);
    L21.setStyle(Text::Bold); L22.setStyle(Text::Bold); L23.setStyle(Text::Bold); L24.setStyle(Text::Bold); L25.setStyle(Text::Bold); 
    L26.setStyle(Text::Bold); L27.setStyle(Text::Bold); L28.setStyle(Text::Bold); L29.setStyle(Text::Bold); L30.setStyle(Text::Bold); 
    L31.setStyle(Text::Bold); L32.setStyle(Text::Bold); L33.setStyle(Text::Bold); L34.setStyle(Text::Bold); L35.setStyle(Text::Bold);
    L36.setStyle(Text::Bold); L37.setStyle(Text::Bold); L38.setStyle(Text::Bold); L39.setStyle(Text::Bold); L40.setStyle(Text::Bold); 
    L41.setStyle(Text::Bold); L42.setStyle(Text::Bold); L43.setStyle(Text::Bold); L44.setStyle(Text::Bold); L45.setStyle(Text::Bold);
    L46.setStyle(Text::Bold); L47.setStyle(Text::Bold); L48.setStyle(Text::Bold); L49.setStyle(Text::Bold); L50.setStyle(Text::Bold);   
    L51.setStyle(Text::Bold); L52.setStyle(Text::Bold); L53.setStyle(Text::Bold); L54.setStyle(Text::Bold); L55.setStyle(Text::Bold); 
    L56.setStyle(Text::Bold); L57.setStyle(Text::Bold); L58.setStyle(Text::Bold); L59.setStyle(Text::Bold); L59.setStyle(Text::Bold); 
    L60.setStyle(Text::Bold); 

    Levels.setString("Select a level");
    L1.setString("1"); L2.setString("2"); L3.setString("3"); L4.setString("4"); L5.setString("5"); L6.setString("6"); L7.setString("7"); 
    L8.setString("8"); L9.setString("9"); L10.setString("10"); L11.setString("11"); L12.setString("12"); L13.setString("13"); L14.setString("14");
    L15.setString("15"); L16.setString("16"); L17.setString("17"); L18.setString("18"); L19.setString("19"); L20.setString("20");
    L21.setString("21"); L22.setString("22"); L23.setString("23"); L24.setString("24"); L25.setString("25"); L26.setString("26"); L27.setString("27"); 
    L28.setString("28"); L29.setString("29"); L30.setString("30"); L31.setString("31"); L32.setString("32"); L33.setString("33"); L34.setString("34"); 
    L35.setString("35"); L36.setString("36"); L37.setString("37"); L38.setString("38"); L39.setString("39"); L40.setString("40"); L41.setString("41"); 
    L42.setString("42"); L43.setString("43"); L44.setString("44"); L45.setString("45"); L46.setString("46"); L47.setString("47"); L48.setString("48"); 
    L49.setString("49"); L50.setString("50"); L51.setString("51"); L52.setString("52"); L53.setString("53"); L54.setString("54"); L55.setString("55"); 
    L56.setString("56"); L57.setString("57"); L58.setString("58"); L59.setString("59"); L60.setString("60");

    Levels.setCharacterSize(120);
    L1.setCharacterSize(80); L2.setCharacterSize(80); L3.setCharacterSize(80); L4.setCharacterSize(80); L5.setCharacterSize(80); 
    L6.setCharacterSize(80); L7.setCharacterSize(80); L8.setCharacterSize(80); L9.setCharacterSize(80); L10.setCharacterSize(80); 
    L11.setCharacterSize(80); L12.setCharacterSize(80); L13.setCharacterSize(80); L14.setCharacterSize(80); L15.setCharacterSize(80); 
    L16.setCharacterSize(80); L17.setCharacterSize(80); L18.setCharacterSize(80); L19.setCharacterSize(80); L20.setCharacterSize(80); 
    L21.setCharacterSize(80); L22.setCharacterSize(80); L23.setCharacterSize(85); L24.setCharacterSize(85); L25.setCharacterSize(85); 
    L26.setCharacterSize(85); L27.setCharacterSize(85); L28.setCharacterSize(90); L29.setCharacterSize(90); L30.setCharacterSize(90); 
    L31.setCharacterSize(90); L32.setCharacterSize(90); L33.setCharacterSize(90); L34.setCharacterSize(90); L35.setCharacterSize(90); 
    L36.setCharacterSize(100); L37.setCharacterSize(100); L38.setCharacterSize(100); L39.setCharacterSize(100); L40.setCharacterSize(110); 
    L41.setCharacterSize(110); L42.setCharacterSize(110); L43.setCharacterSize(110); L44.setCharacterSize(110); L45.setCharacterSize(110);
    L46.setCharacterSize(110); L47.setCharacterSize(110); L48.setCharacterSize(110); L49.setCharacterSize(110); L50.setCharacterSize(110); 
    L51.setCharacterSize(110); L52.setCharacterSize(110); L53.setCharacterSize(110); L54.setCharacterSize(110); L55.setCharacterSize(110); 
    L56.setCharacterSize(130); L57.setCharacterSize(130); L58.setCharacterSize(130); L59.setCharacterSize(130); L60.setCharacterSize(160); 

    Levels.setPosition(260,20);
    L1.setPosition(25, 220); L2.setPosition(70, 260); L3.setPosition(120, 300); L4.setPosition(165, 345); L5.setPosition(220, 370);
    L6.setPosition(275, 385); L7.setPosition(325, 400); L8.setPosition(375, 415); L9.setPosition(430, 445); L10.setPosition(480,500);
    L11.setPosition(540, 465); L12.setPosition(600, 430); L13.setPosition(640, 370); L14.setPosition(685, 315); L15.setPosition(730, 250); 
    L16.setPosition(810, 250); L17.setPosition(855, 310); L18.setPosition(900, 370); L19.setPosition(930, 300); L20.setPosition(975, 240);
    L21.setPosition(1040, 140); L22.setPosition(1130, 160); L23.setPosition(1080, 250); L24.setPosition(1030, 330); L25.setPosition(1080, 410); 
    L26.setPosition(1030, 485); L27.setPosition(965, 550); L28.setPosition(900, 620); L29.setPosition(785, 610); L30.setPosition(675, 610); 
    L31.setPosition(585, 610); L32.setPosition(480, 610); L33.setPosition(380, 610); L34.setPosition(287, 625); L35.setPosition(170, 635); 
    L36.setPosition(20, 660);
    L37.setPosition(130, 695); L38.setPosition(225, 740); L39.setPosition(300, 800); L40.setPosition(410, 780); L41.setPosition(520, 760); 
    L42.setPosition(630, 750); L43.setPosition(740, 755); L44.setPosition(850, 735); L45.setPosition(960, 715); L46.setPosition(1070, 720); 
    L47.setPosition(1000, 800); L48.setPosition(885, 810); L49.setPosition(775, 835); L50.setPosition(660, 850); L51.setPosition(545, 875);
    L52.setPosition(440, 910); L53.setPosition(315, 940); L54.setPosition(200, 970); L55.setPosition(85, 1000); L56.setPosition(200, 1050); 
    L57.setPosition(380, 1030); L58.setPosition(550, 1010); L59.setPosition(730, 970); L60.setPosition(950, 980);   

    nextLevel.setFont(font);
    nextLevel.setStyle(Text::Bold);
    nextLevel.setString("Next level");
    nextLevel.setCharacterSize(60);
    nextLevel.setFillColor(Color::Black);
    nextLevel.setPosition(600, 530);

    credits.setFont(font);
    credits.setStyle(Text::Bold);
    credits.setString("Joc realizat de Chichirau Razvan si Corban Cosmin");
    credits.setCharacterSize(30);
    credits.setFillColor(Color::Black);
    credits.setPosition(475, 1150);

    difficulty.setFont(font); difficulty.setStyle(Text::Bold); difficulty.setCharacterSize(70); difficulty.setFillColor(Color::Black); difficulty.setPosition(10, 850);
    difficulty1.setFont(font); difficulty1.setStyle(Text::Bold); difficulty1.setCharacterSize(50); difficulty1.setFillColor(Color(11, 192, 41, 255)); difficulty1.setPosition(375, 750);
    difficulty2.setFont(font); difficulty2.setStyle(Text::Bold); difficulty2.setCharacterSize(50); difficulty2.setFillColor(Color::Green); difficulty2.setPosition(375, 800);
    difficulty3.setFont(font); difficulty3.setStyle(Text::Bold); difficulty3.setCharacterSize(50); difficulty3.setFillColor(Color::Yellow); difficulty3.setPosition(375, 850);
    difficulty4.setFont(font); difficulty4.setStyle(Text::Bold); difficulty4.setCharacterSize(50); difficulty4.setFillColor(Color(231, 238, 29, 255)); difficulty4.setPosition(375, 900);
    difficulty5.setFont(font); difficulty5.setStyle(Text::Bold); difficulty5.setCharacterSize(50); difficulty5.setFillColor(Color(223, 139, 14, 255)); difficulty5.setPosition(375, 950);
    difficulty6.setFont(font); difficulty6.setStyle(Text::Bold); difficulty6.setCharacterSize(50); difficulty6.setFillColor(Color::Red); difficulty6.setPosition(375, 1000);
    difficulty7.setFont(font); difficulty7.setStyle(Text::Bold); difficulty7.setCharacterSize(50); difficulty7.setFillColor(Color(175, 0, 0, 255)); difficulty7.setPosition(375, 1050);

    difficulty.setString("Dificultate");
    difficulty1.setString("Incepator: nivelele 1-7 (2 animale)");
    difficulty2.setString("Usor: nivelele 8-17 (3 animale)");
    difficulty3.setString("Mediu: nivelele 18-27 (4 animale)");
    difficulty4.setString("Greu: nivelele 28-37 (5 animale)");
    difficulty5.setString("Priceput: nivelele 38-47 (6 animale)");
    difficulty6.setString("Avansat: nivelele 48-57 (7 animale)");
    difficulty7.setString("Expert: nivelele 58-60 (8 animale)");



    rules1.setFont(font); rules1.setStyle(Text::Bold); rules1.setCharacterSize(50); rules1.setFillColor(Color(120, 15, 141, 255)); rules1.setPosition(90, 100);
    rules2.setFont(font); rules2.setStyle(Text::Bold); rules2.setCharacterSize(50); rules2.setFillColor(Color(120, 15, 141, 255)); rules2.setPosition(55, 150);
    rules3.setFont(font); rules3.setStyle(Text::Bold); rules3.setCharacterSize(50); rules3.setFillColor(Color(120, 15, 141, 255)); rules3.setPosition(55, 200);
    rules4.setFont(font); rules4.setStyle(Text::Bold); rules4.setCharacterSize(50); rules4.setFillColor(Color(120, 15, 141, 255)); rules4.setPosition(55, 250);
    rules5.setFont(font); rules5.setStyle(Text::Bold); rules5.setCharacterSize(50); rules5.setFillColor(Color(120, 15, 141, 255)); rules5.setPosition(55, 300);
    rules6.setFont(font); rules6.setStyle(Text::Bold); rules6.setCharacterSize(50); rules6.setFillColor(Color(120, 15, 141, 255)); rules6.setPosition(55, 350);
    rules7.setFont(font); rules7.setStyle(Text::Bold); rules7.setCharacterSize(50); rules7.setFillColor(Color(120, 15, 141, 255)); rules7.setPosition(55, 400);

    rules1.setString("Regulile jocului sunt simple. Sunt disponibile 3");
    rules2.setString("seturi de garduri dintre care puteti alege doar");
    rules3.setString("unul din fiecare set. Scopul jocului este de a");
    rules4.setString("imprejmui toate animalele de acelasi tip in acelasi");
    rules5.setString("spatiu. Nu este permis sa aveti 2 animale de tipuri");
    rules6.setString("diferite in aceeasi zona sau 2 animale de acelasi");
    rules7.setString("tip in zone diferite sau zone goale.");

   SettingSound.setFont(font); SettingSound.setStyle(Text::Bold); SettingSound.setCharacterSize(75); SettingSound.setFillColor(Color(212, 20, 180, 255)); SettingSound.setPosition(40, 700);
   settingNoSound.setFont(font); settingNoSound.setStyle(Text::Bold); settingNoSound.setCharacterSize(75); settingNoSound.setFillColor(Color(212, 20, 180, 255)); settingNoSound.setPosition(320, 700);
   settingSong1.setFont(font); settingSong1.setStyle(Text::Bold); settingSong1.setCharacterSize(75); settingSong1.setFillColor(Color(212, 20, 180, 255)); settingSong1.setPosition(650, 700);
   settingSong2.setFont(font); settingSong2.setStyle(Text::Bold); settingSong2.setCharacterSize(75); settingSong2.setFillColor(Color(212, 20, 180, 255)); settingSong2.setPosition(930, 700);
   newGame.setFont(font); newGame.setStyle(Text::Bold); newGame.setCharacterSize(70); newGame.setFillColor(Color::Blue); newGame.setPosition(470, 1000);
   newGameDone.setFont(font); newGameDone.setStyle(Text::Bold); newGameDone.setCharacterSize(70); newGameDone.setFillColor(Color::Blue); newGameDone.setPosition(20, 1080);

   SettingSound.setString("Sound:");
   settingNoSound.setString("No sound");
   settingSong1.setString("Song 1");
   settingSong2.setString("Song 2");
   newGame.setString("New Game");
   newGameDone.setString("Jocul dumneavoastra a fost resetat");

   resetGame.setFont(font);
   resetGame.setStyle(Text::Bold);
   resetGame.setString("RESET");
   resetGame.setFillColor(Color::Black);
   resetGame.setPosition(920, -10);
   resetGame.setCharacterSize(90);

   win.setFont(font);
   win.setStyle(Text::Bold);
   win.setString("Felicitari, ai castigat!");
   win.setCharacterSize(80);
   win.setFillColor(Color(204, 230, 32, 255));
   win.setPosition(245, 430);

   lose.setFont(font);
   lose.setStyle(Text::Bold);
   lose.setString("Din pacate ai pierdut!");
   lose.setCharacterSize(80);
   lose.setFillColor(Color(238, 15, 37, 255));
   lose.setPosition(245, 430);

   backLevel1.setFont(font);
   backLevel1.setStyle(Text::Bold);
   backLevel1.setString("Back");
   backLevel1.setCharacterSize(60);
   backLevel1.setFillColor(Color::Black);
   backLevel1.setPosition(400, 530);

   backLevel2.setFont(font);
   backLevel2.setStyle(Text::Bold);
   backLevel2.setString("Back");
   backLevel2.setCharacterSize(60);
   backLevel2.setFillColor(Color::Black);
   backLevel2.setPosition(535, 530);

    startText.setFont(font);
    startText.setStyle(Text::Bold);
    startText.setString("START");
    startText.setFillColor(Color::Red);
    startText.setCharacterSize(90);
    startText.setPosition(475.0, 1000.0);

    descriptionText.setFont(font);
    descriptionText.setStyle(Text::Bold);
    descriptionText.setString("Info");
    descriptionText.setFillColor(Color::Black);
    descriptionText.setCharacterSize(70);
    descriptionText.setPosition(50.0, 1000.0);

    settingsText.setFont(font);
    settingsText.setStyle(Text::Bold);
    settingsText.setString("Settings");
    settingsText.setFillColor(Color::Black);
    settingsText.setCharacterSize(70);
    settingsText.setPosition(900.0, 1000.0);

    if (!t1.loadFromFile("background.jpg"))
    {
        perror("Eroare cu mesajul:");
        window.close();
    }

    background.loadFromFile("backgroundStart.jpg");
    backgroundSprite.setTexture(background);
    backgroundSprite.setScale(0.65, 0.9);

    back.loadFromFile("back.png");
    backSprite.setTexture(back);
    backSprite.setScale(0.2, 0.2);
    backSprite.setPosition(10.0, 10.0);

    window.draw(t1Sprite);
    window.draw(exitButton);
    window.draw(startText);
    window.draw(descriptionText);
    window.draw(settingsText);
    window.display();

    song1.play();

    while (window.isOpen())
    {
        Event e;

        while (window.pollEvent(e))
        {

            switch (e.type)
            {
            case (Event::Closed):
            {
                window.close();
                break;
            }
            case (Event::KeyPressed):
            {
                if (e.key.code == Keyboard::Escape)
                    window.close();
                break;
            }

            case (Event::MouseMoved):
            {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if(reset==true){
                        for(i=1;i<=8;i++) OK[i]=0;
                        reset=false;
                    }
                if (isMarked == true && isPut == false)
                {
        
                    if (mousePos.x > 230 && mousePos.x < 974 && mousePos.y > 112 && mousePos.y < 678)
                    {
                        if (OK[1] == 1)
                        {
                            horizontalFence2Sprite.setScale(1.6, 2);
                            horizontalFence2Sprite.setPosition(mousePos.x, mousePos.y);
                            window.draw(horizontalFence2Sprite);
                        }
                        if (OK[2] == 1)
                        {
                            horizontalFence3Sprite.setScale(1.6, 2);
                            horizontalFence3Sprite.setPosition(mousePos.x - 45, mousePos.y - 45);
                            window.draw(horizontalFence3Sprite);
                        }
                        if (OK[3] == 1)
                        {
                            verticalFence2Sprite.setScale(1.6, 1.7);
                            verticalFence2Sprite.setPosition(mousePos.x - 70, mousePos.y - 40);
                            window.draw(verticalFence2Sprite);
                        }
                        if (OK[4] == 1)
                        {
                            verticalFence3Sprite.setScale(1.6, 1.6);
                            verticalFence3Sprite.setPosition(mousePos.x - 120, mousePos.y - 30);
                            window.draw(verticalFence3Sprite);
                        }
                        if (OK[5] == 1)
                        {
                            cornerFence1Sprite.setScale(1.6, 1.6);
                            cornerFence1Sprite.setPosition(mousePos.x-85, mousePos.y-20);
                            window.draw(cornerFence1Sprite);
                        }
                        if (OK[6] == 1)
                        {
                            cornerFence2Sprite.setScale(1.55, 1.55);
                            cornerFence2Sprite.setPosition(mousePos.x-40, mousePos.y-50);
                            window.draw(cornerFence2Sprite);
                        }
                        if (OK[7] == 1)
                        {
                            cornerFence3Sprite.setScale(1.55, 1.55);
                            cornerFence3Sprite.setPosition(mousePos.x-50, mousePos.y-50);
                            window.draw(cornerFence3Sprite);
                        }
                        if (OK[8] == 1)
                        {
                            cornerFence4Sprite.setScale(1.6, 1.55);
                            cornerFence4Sprite.setPosition(mousePos.x-210, mousePos.y);
                            window.draw(cornerFence4Sprite);
                        }
                        window.display();
                        window.clear();
                        window.draw(backgroundSprite);
                        window.draw(backSprite);
                        window.draw(boardSprite);
                        if(putCow1==true) window.draw(cow1Sprite);
                        if(putCow2==true) window.draw(cow2Sprite);
                        if(putHorse1==true) window.draw(horse1Sprite);
                        if(putHorse2==true) window.draw(horse2Sprite);
                        if(putSheep1==true) window.draw(sheep1Sprite);
                        if(putPig1==true) window.draw(pig1Sprite);
                        if(putPig2==true) window.draw(pig2Sprite);
                        if(putSheep2==true) window.draw(sheep2Sprite);
                        horizontalFence2Sprite.setScale(0.8, 1);
                        horizontalFence2Sprite.setPosition(105, 850);
                        horizontalFence3Sprite.setScale(0.8, 1);
                        horizontalFence3Sprite.setPosition(30, 950);
                        verticalFence2Sprite.setScale(0.8, 1);
                        verticalFence3Sprite.setScale(0.8, 1);
                        verticalFence2Sprite.setPosition(403, 827);
                        verticalFence3Sprite.setPosition(520, 780);
                        cornerFence1Sprite.setScale(0.4, 0.8);
                        cornerFence1Sprite.setPosition(690, 905);
                        cornerFence2Sprite.setScale(0.6, 0.4);
                        cornerFence2Sprite.setPosition(860, 775);
                        cornerFence3Sprite.setScale(0.4, 0.6);
                        cornerFence3Sprite.setPosition(1005, 920);
                        cornerFence4Sprite.setScale(0.6, 0.45);
                        cornerFence4Sprite.setPosition(860, 1060);
                        window.draw(rectangleL2);
                        window.draw(rectangleL3);
                        window.draw(rectangleL4);
                        window.draw(rectangleL5);
                        window.draw(rectangleCorner1);
                        window.draw(rectangleCorner2);
                        window.draw(rectangleCorner3);
                        window.draw(rectangleCorner4);
                        window.draw(cornerFence1Sprite);
                        window.draw(cornerFence2Sprite);
                        window.draw(cornerFence3Sprite);
                        window.draw(cornerFence4Sprite);
                        window.draw(verticalFence3Sprite);
                        window.draw(verticalFence2Sprite);
                        window.draw(horizontalFence2Sprite);
                        window.draw(horizontalFence3Sprite);
                        window.draw(resetGame);
                        i = 0;
                        while (fences[0][i] != 0)
                        {
                            switch (fences[0][i])
                            {
                            case 1:
                            {
                                horizontalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                horizontalFence2Sprite.setScale(1.6, 2);
                                window.draw(horizontalFence2Sprite);
                                break;
                            }
                            case 2:
                            {
                                horizontalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                horizontalFence3Sprite.setScale(1.6, 2);
                                window.draw(horizontalFence3Sprite);
                                break;
                            }
                            case 3:
                            {
                                verticalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                verticalFence2Sprite.setScale(1.6, 1.6);
                                window.draw(verticalFence2Sprite);
                                break;
                            }
                            case 4:
                            {
                                verticalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                verticalFence3Sprite.setScale(1.6, 1.6);
                                window.draw(verticalFence3Sprite);
                                break;
                            }
                            case 5:
                            {
                                cornerFence1Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence1Sprite.setScale(1.55, 1.6);
                                window.draw(cornerFence1Sprite);
                                break;
                            }
                            case 6:
                            {
                               cornerFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence2Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence2Sprite);
                                break; 
                            }
                            case 7:
                            {
                                cornerFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence3Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence3Sprite);
                                break;
                            }
                            case 8:
                            {
                                cornerFence4Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence4Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence4Sprite);
                                break;
                            }
                            }

                            i++;
                        }
                    }
                }

                break;
            }

            case (Event::MouseButtonPressed):
            {
                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                cout<<mousePos.x<<" "<<mousePos.y<<endl;
                if(pg==-2)
                {
                    if(newGame.getGlobalBounds().contains(mousePosF))
                    {
                        isLevelAvailable[1]=1;
                        for(i=2;i<=60;i++) isLevelAvailable[i]=0;
                        window.clear();
                        window.draw(t1Sprite);
                        window.draw(backSprite);
                        window.draw(SettingSound);
                        window.draw(settingNoSound);
                        window.draw(settingSong1);
                        window.draw(settingSong2);
                        window.draw(newGame);
                         window.draw(newGameDone);
                        window.display();
                        
                    }
                    if(settingNoSound.getGlobalBounds().contains(mousePosF))
                    {
                        song1.stop(); song2.stop();
                    }
                     if(settingSong1.getGlobalBounds().contains(mousePosF))
                    {
                        song1.stop(); song2.stop(); song1.play();
                    }
                    if(settingSong2.getGlobalBounds().contains(mousePosF))
                    {
                        song1.stop(); song2.stop(); song2.play();
                    }
                }
                if(pg==1)
                {
                    putCow1=false, putCow2=false, putHorse1=false, putHorse2=false, putPig1=false, putSheep1=false;
                    if(L1.getGlobalBounds().contains(mousePosF) && isLevelAvailable[1]==1)
                    {
                        makeLevelReady=2;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=21; gameMatrix[5][5]=22;
                        printMatrix(gameMatrix);
                        cow1Sprite.setPosition(552, 510); sheep1Sprite.setPosition(160, 160);
                        putSheep1=true; putCow1=true;

                    }
                    if(L2.getGlobalBounds().contains(mousePosF) && isLevelAvailable[2]==1)
                    {
                        makeLevelReady=3;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][7]=20; gameMatrix[3][5]=23;
                        printMatrix(gameMatrix);
                        pig1Sprite.setPosition(750, 140); horse1Sprite.setPosition(575, 325);
                        putPig1=true; putHorse1=true;
                    }
                    if(L3.getGlobalBounds().contains(mousePosF) && isLevelAvailable[3]==1)
                    {
                        makeLevelReady=4;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][7]=21; gameMatrix[5][1]=23;
                        printMatrix(gameMatrix);
                        sheep1Sprite.setPosition(735, 535); horse1Sprite.setPosition(200, 518);
                        putSheep1=true; putHorse1=true;
                    }
                    if(L4.getGlobalBounds().contains(mousePosF) && isLevelAvailable[4]==1)
                    {
                        makeLevelReady=5;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=20; gameMatrix[3][3]=22;
                        printMatrix(gameMatrix);
                        pig1Sprite.setPosition(378, 148); cow1Sprite.setPosition(365, 320);
                        putPig1=true; putCow1=true;
                    }
                    if(L5.getGlobalBounds().contains(mousePosF) && isLevelAvailable[5]==1)
                    {
                        makeLevelReady=6;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][7]=21; gameMatrix[5][1]=22;
                        printMatrix(gameMatrix);
                        sheep1Sprite.setPosition(720, 160); cow2Sprite.setPosition(225, 523);
                        putSheep1=true; putCow2=true;
                    }
                    if(L6.getGlobalBounds().contains(mousePosF) && isLevelAvailable[6]==1)
                    {
                        makeLevelReady=7;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][3]=23; gameMatrix[5][7]=22;
                        printMatrix(gameMatrix);
                        horse2Sprite.setPosition(395, 335); cow1Sprite.setPosition(745, 510);
                        putHorse2=true; putCow1=true;
                    }
                    if(L7.getGlobalBounds().contains(mousePosF) && isLevelAvailable[7]==1)
                    {
                        makeLevelReady=8;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[1][3]=21;
                        printMatrix(gameMatrix);
                        pig1Sprite.setPosition(188, 150); sheep1Sprite.setPosition(340, 170);
                        putPig1=true; putSheep1=true;
                    }
                    if(L8.getGlobalBounds().contains(mousePosF) && isLevelAvailable[8]==1)
                    {
                        makeLevelReady=9;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=23; gameMatrix[3][3]=22; gameMatrix[3][7]=21;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(198, 524); sheep1Sprite.setPosition(730, 345);
                        cow1Sprite.setPosition(367, 320);
                        putHorse1=true; putCow1=true; putSheep1=true;
                    }
                    if(L9.getGlobalBounds().contains(mousePosF) && isLevelAvailable[9]==1)
                    {
                        makeLevelReady=10;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[3][5]=20; gameMatrix[5][3]=22;
                        printMatrix(gameMatrix);
                        pig1Sprite.setPosition(185, 150); cow2Sprite.setPosition(410, 520);
                        pig2Sprite.setPosition(570, 325);
                        putPig2=true; putCow2=true; putPig1=true;
                    }
                    if(L10.getGlobalBounds().contains(mousePosF) && isLevelAvailable[10]==1)
                    {
                        makeLevelReady=11;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=23; gameMatrix[3][3]=20;
                        gameMatrix[3][5]=21;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(390, 145); sheep1Sprite.setPosition(530, 355);
                        pig1Sprite.setPosition(380, 338);
                        putPig1=true; putSheep1=true; putHorse1=true;
                    }
                    if(L11.getGlobalBounds().contains(mousePosF) && isLevelAvailable[11]==1)
                    {
                        makeLevelReady=12;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=22; gameMatrix[5][1]=21; gameMatrix[5][7]=23;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(220, 145); sheep1Sprite.setPosition(155, 540);
                        horse2Sprite.setPosition(770, 520);
                        putCow2=true;  putSheep1=true; putHorse2=true;
                    }
                    if(L12.getGlobalBounds().contains(mousePosF) && isLevelAvailable[12]==1)
                    {
                        makeLevelReady=13;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][5]=22; gameMatrix[3][3]=21; gameMatrix[5][3]=22;
                        printMatrix(gameMatrix);
                        cow1Sprite.setPosition(555, 130); cow2Sprite.setPosition(415, 525);
                        sheep1Sprite.setPosition(345, 355);
                        putCow2=true; putCow1=true; putSheep1=true;
                    }
                    if(L13.getGlobalBounds().contains(mousePosF) && isLevelAvailable[13]==1)
                    {
                        makeLevelReady=14;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][1]=23; gameMatrix[3][3]=20; gameMatrix[5][5]=23;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(575, 520); horse2Sprite.setPosition(210, 330);
                        pig1Sprite.setPosition(385, 335);
                        putHorse2=true;  putPig1=true; putHorse1=true;
                    }
                    if(L14.getGlobalBounds().contains(mousePosF) && isLevelAvailable[14]==1)
                    {
                        makeLevelReady=15;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][1]=21; gameMatrix[3][5]=22;  gameMatrix[5][5]=21;
                        printMatrix(gameMatrix);
                        sheep1Sprite.setPosition(545, 530);  sheep2Sprite.setPosition(170, 345);
                        cow1Sprite.setPosition(550, 320);
                        putSheep2=true; putCow1=true; putSheep1=true;
                    }
                    if(L15.getGlobalBounds().contains(mousePosF) && isLevelAvailable[15]==1)
                    {
                        makeLevelReady=16;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=22; gameMatrix[5][3]=20; gameMatrix[5][5]=21;
                        printMatrix(gameMatrix);
                        sheep1Sprite.setPosition(540, 530); cow1Sprite.setPosition(180, 510);
                        pig1Sprite.setPosition(385, 520);
                        putSheep1=true; putPig1=true; putCow1=true;
                    }
                    if(L16.getGlobalBounds().contains(mousePosF) && isLevelAvailable[16]==1)
                    {
                        makeLevelReady=17;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][5]=23; gameMatrix[1][1]=21;
                        gameMatrix[5][3]=21;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(575, 335); sheep1Sprite.setPosition(158, 165);
                        sheep2Sprite.setPosition(350, 535);
                        putHorse1=true; putSheep1=true; putSheep2=true;
                    }
                    if(L17.getGlobalBounds().contains(mousePosF) && isLevelAvailable[17]==1)
                    {
                        makeLevelReady=18;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][7]=23; gameMatrix[3][5]=22; gameMatrix[5][5]=20;
                        printMatrix(gameMatrix);
                        horse2Sprite.setPosition(775, 335); cow1Sprite.setPosition(550, 325);
                        pig1Sprite.setPosition(575, 520);
                        putHorse2=true; putCow1=true; putPig1=true;
                    }
                    if(L18.getGlobalBounds().contains(mousePosF) && isLevelAvailable[18]==1)
                    {
                        makeLevelReady=19;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[1][7]=21; gameMatrix[5][1]=22; gameMatrix[5][5]=23;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(575, 520); cow1Sprite.setPosition(175, 515);
                        pig1Sprite.setPosition(185, 150); sheep1Sprite.setPosition(718, 165);
                        putHorse1=true; putSheep1=true; putCow1=true; putPig1=true;
                    }
                    if(L19.getGlobalBounds().contains(mousePosF) && isLevelAvailable[19]==1)
                    {
                        makeLevelReady=20;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=22;  gameMatrix[3][3]=22;
                        gameMatrix[3][5]=20;  gameMatrix[5][3]=20;
                        printMatrix(gameMatrix);
                        cow1Sprite.setPosition(555, 320); cow2Sprite.setPosition(415, 515);
                        pig1Sprite.setPosition(380, 335); pig2Sprite.setPosition(185, 150);
                        putCow1=true; putCow2=true; putPig1=true; putPig2=true;
                    }
                    if(L20.getGlobalBounds().contains(mousePosF) && isLevelAvailable[20]==1)
                    {
                        makeLevelReady=21;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][3]=21; gameMatrix[1][7]=21; gameMatrix[1][5]=23; gameMatrix[3][1]=20;
                        printMatrix(gameMatrix);
                        sheep1Sprite.setPosition(715, 160); sheep2Sprite.setPosition(350, 535);
                        horse2Sprite.setPosition(585, 148); pig1Sprite.setPosition(190, 335);
                        putSheep1=true; putSheep2=true; putHorse2=true; putPig1=true;
                    }
                    if(L21.getGlobalBounds().contains(mousePosF) && isLevelAvailable[21]==1)
                    {
                        makeLevelReady=22;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][1]=22; gameMatrix[3][7]=22; gameMatrix[5][3]=20; gameMatrix[5][7]=21;
                        printMatrix(gameMatrix);
                        cow1Sprite.setPosition(175, 325); cow2Sprite.setPosition(785, 335);
                        pig1Sprite.setPosition(380, 525); sheep1Sprite.setPosition(720, 535);
                        putCow1=true; putCow2=true; putPig1=true; putSheep1=true;
                    }
                    if(L22.getGlobalBounds().contains(mousePosF) && isLevelAvailable[22]==1)
                    {
                        makeLevelReady=23;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=20; gameMatrix[3][3]=21; gameMatrix[3][5]=22; gameMatrix[1][7]=23;
                        printMatrix(gameMatrix);
                        cow1Sprite.setPosition(548, 320);
                        horse1Sprite.setPosition(765, 148);
                        pig1Sprite.setPosition(375, 150);
                        sheep1Sprite.setPosition(348, 350);
                        putCow1=true; putHorse1=true; putPig1=true; putSheep1=true;
                    }
                    if(L23.getGlobalBounds().contains(mousePosF) && isLevelAvailable[23]==1)
                    {
                        makeLevelReady=24;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix);  initGameMatrix(gameMatrix);
                        gameMatrix[3][1]=22; gameMatrix[5][3]=20; gameMatrix[5][5]=20; gameMatrix[1][5]=21;
                        printMatrix(gameMatrix);
                        cow1Sprite.setPosition(170, 320); sheep1Sprite.setPosition(545, 160);
                        pig1Sprite.setPosition(385, 525); pig2Sprite.setPosition(555, 525);
                        putCow1=true; putSheep1=true; putPig1=true; putPig2=true;
                    }
                    if(L24.getGlobalBounds().contains(mousePosF) && isLevelAvailable[24]==1)
                    {
                        makeLevelReady=25;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=23; gameMatrix[5][1]=21; gameMatrix[3][5]=22; gameMatrix[1][7]=20;
                        printMatrix(gameMatrix);
                        horse2Sprite.setPosition(200, 150); sheep1Sprite.setPosition(160, 545);
                        pig1Sprite.setPosition(755, 140); cow2Sprite.setPosition(600,333);
                        putHorse2=true; putSheep1=true; putPig1=true; putCow2=true;
                    }
                    if(L25.getGlobalBounds().contains(mousePosF) && isLevelAvailable[25]==1)
                    {
                        makeLevelReady=26;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[3][1]=21; gameMatrix[5][1]=22; gameMatrix[3][5]=23;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(570, 335); sheep1Sprite.setPosition(160, 355);
                        pig1Sprite.setPosition(180, 155); cow1Sprite.setPosition(179,515);
                        putHorse1=true; putSheep1=true; putPig1=true; putCow1=true;
                    }
                    if(L26.getGlobalBounds().contains(mousePosF) && isLevelAvailable[26]==1)
                    {
                        makeLevelReady=27;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=22; gameMatrix[3][3]=21; gameMatrix[5][5]=20; gameMatrix[3][7]=23;
                        printMatrix(gameMatrix);
                        horse2Sprite.setPosition(765, 335); sheep1Sprite.setPosition(355, 355);
                        pig1Sprite.setPosition(565, 525); cow2Sprite.setPosition(220, 520);
                        putHorse2=true; putSheep1=true; putPig1=true; putCow2=true;
                    }
                    if(L27.getGlobalBounds().contains(mousePosF) && isLevelAvailable[27]==1)
                    {
                        makeLevelReady=28;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=21; gameMatrix[5][3]=21; gameMatrix[3][1]=22;  gameMatrix[1][7]=23;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(765, 150); sheep1Sprite.setPosition(166, 166);
                        sheep2Sprite.setPosition(350, 528); cow1Sprite.setPosition(178, 325);
                        putHorse1=true; putSheep1=true; putSheep2=true; putCow1=true;
                    }
                    if(L28.getGlobalBounds().contains(mousePosF) && isLevelAvailable[28]==1)
                    {
                        makeLevelReady=29;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][5]=23; gameMatrix[1][7]=23; gameMatrix[1][1]=20; gameMatrix[3][3]=20; gameMatrix[5][1]=21;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(575,520); horse2Sprite.setPosition(765, 150);
                        sheep1Sprite.setPosition(166, 545); pig1Sprite.setPosition(178, 155);
                        pig2Sprite.setPosition(380, 330);
                        putHorse1=true; putSheep1=true; putHorse2=true; putPig1=true; putPig2=true;
                    }
                    if(L29.getGlobalBounds().contains(mousePosF) && isLevelAvailable[29]==1)
                    {
                        makeLevelReady=30;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=20; gameMatrix[5][5]=20; gameMatrix[3][3]=21; gameMatrix[1][1]=22; gameMatrix[5][7]=23;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(765,525); cow1Sprite.setPosition(180, 130);
                        sheep1Sprite.setPosition(350, 345);pig1Sprite.setPosition(180, 530);
                        pig2Sprite.setPosition(565, 520);
                        putHorse1=true; putSheep1=true; putCow1=true; putPig1=true; putPig2=true;
                    }
                    if(L30.getGlobalBounds().contains(mousePosF) && isLevelAvailable[30]==1)
                    {
                        makeLevelReady=31;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][7]=21; gameMatrix[5][5]=20; gameMatrix[5][3]=23; gameMatrix[1][3]=22; gameMatrix[1][7]=22;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(378,525); pig1Sprite.setPosition(565, 525);
                        sheep1Sprite.setPosition(725, 345); cow1Sprite.setPosition(370, 135);
                        cow2Sprite.setPosition(780, 150);
                        putHorse1=true; putSheep1=true; putCow1=true; putCow2=true; putPig1=true;
                    }
                    if(L31.getGlobalBounds().contains(mousePosF) && isLevelAvailable[31]==1)
                    {
                        makeLevelReady=32;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][3]=23; gameMatrix[3][5]=23; gameMatrix[3][1]=20; gameMatrix[1][5]=22; gameMatrix[3][7]=21;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(388,335); horse2Sprite.setPosition(578, 335);
                        sheep1Sprite.setPosition(725, 345);
                        cow1Sprite.setPosition(550, 135); pig1Sprite.setPosition(190, 340);
                        putHorse1=true; putSheep1=true; putCow1=true; putPig1=true; putHorse2=true;
                    }
                    if(L32.getGlobalBounds().contains(mousePosF) && isLevelAvailable[32]==1)
                    {
                        makeLevelReady=33;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[3][1]=20; gameMatrix[3][3]=22; gameMatrix[1][5]=23; gameMatrix[5][3]=23;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(388,520); horse2Sprite.setPosition(578, 150);
                        pig2Sprite.setPosition(190, 327); cow1Sprite.setPosition(370, 315);
                        pig1Sprite.setPosition(190, 147);
                        putHorse1=true; putPig2=true; putCow1=true; putPig1=true; putHorse2=true;
                    }
                    if(L33.getGlobalBounds().contains(mousePosF) && isLevelAvailable[33]==1)
                    {
                        makeLevelReady=34;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=21; gameMatrix[1][5]=21; gameMatrix[3][5]=20; gameMatrix[5][5]=22; gameMatrix[3][7]=22;
                        printMatrix(gameMatrix);
                        sheep1Sprite.setPosition(350,166); sheep2Sprite.setPosition(535, 166);
                        pig1Sprite.setPosition(575, 327); cow1Sprite.setPosition(555, 505);
                        cow2Sprite.setPosition(785, 330);
                        putSheep1=true; putCow2=true; putCow1=true; putPig1=true; putSheep2=true;
                    }
                    if(L34.getGlobalBounds().contains(mousePosF) && isLevelAvailable[34]==1)
                    {
                        makeLevelReady=35;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[5][3]=21; gameMatrix[3][3]=23; gameMatrix[1][7]=23; gameMatrix[5][7]=22;
                        printMatrix(gameMatrix);
                        horse1Sprite.setPosition(385,335); horse2Sprite.setPosition(765, 150);
                        pig1Sprite.setPosition(185, 155); cow1Sprite.setPosition(748, 520);
                        sheep1Sprite.setPosition(355, 545);
                        putSheep1=true; putCow1=true; putHorse1=true; putPig1=true; putHorse2=true;
                    }
                    if(L35.getGlobalBounds().contains(mousePosF) && isLevelAvailable[35]==1)
                    {
                        makeLevelReady=36;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][3]=20; gameMatrix[3][5]=20; gameMatrix[5][3]=21; gameMatrix[5][7]=21; gameMatrix[3][7]=22;
                        printMatrix(gameMatrix);
                        pig2Sprite.setPosition(565,335); sheep2Sprite.setPosition(740, 540);
                        pig1Sprite.setPosition(375, 340); cow1Sprite.setPosition(748, 330);
                        sheep1Sprite.setPosition(345, 545);
                        putSheep1=true; putCow1=true; putSheep2=true; putPig1=true; putPig2=true;
                    }
                    if(L36.getGlobalBounds().contains(mousePosF) && isLevelAvailable[36]==1)
                    {
                        makeLevelReady=37;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=23; gameMatrix[3][5]=23; gameMatrix[3][1]=21; gameMatrix[1][7]=20; gameMatrix[5][1]=20;
                        printMatrix(gameMatrix);
                        pig2Sprite.setPosition(750,150); horse2Sprite.setPosition(575, 328);
                        pig1Sprite.setPosition(185, 530); sheep1Sprite.setPosition(166, 340);
                        horse1Sprite.setPosition(195, 155);
                        putSheep1=true; putHorse1=true; putHorse2=true; putPig1=true; putPig2=true;
                    }
                    if(L37.getGlobalBounds().contains(mousePosF) && isLevelAvailable[37]==1)
                    {
                        makeLevelReady=38;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=21; gameMatrix[3][1]=21; gameMatrix[3][3]=22; gameMatrix[3][5]=22; gameMatrix[5][1]=23;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(600,330); horse1Sprite.setPosition(195, 525);
                        cow1Sprite.setPosition(365, 320); sheep1Sprite.setPosition(166, 165);
                        sheep2Sprite.setPosition(162, 348);
                        putSheep1=true; putHorse1=true; putSheep2=true; putCow1=true; putCow2=true;
                    }
                    if(L38.getGlobalBounds().contains(mousePosF) && isLevelAvailable[38]==1)
                    {
                        makeLevelReady=39;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=22; gameMatrix[5][1]=22; gameMatrix[3][3]=20; gameMatrix[5][3]=21; gameMatrix[1][7]=23;
                        gameMatrix[5][7]=20;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(220,520); horse1Sprite.setPosition(765, 150);
                        cow1Sprite.setPosition(180, 140); sheep1Sprite.setPosition(345, 545);
                        pig1Sprite.setPosition(380, 330); pig2Sprite.setPosition(760, 520);
                        putSheep1=true; putHorse1=true; putPig2=true; putCow1=true; putCow2=true; putPig1=true;
                    }
                    if(L39.getGlobalBounds().contains(mousePosF) && isLevelAvailable[39]==1)
                    {
                        makeLevelReady=40;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=21; gameMatrix[3][5]=21; gameMatrix[3][3]=20; gameMatrix[5][7]=20; gameMatrix[3][1]=22;
                        gameMatrix[5][1]=22;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(220,530); sheep2Sprite.setPosition(545, 345);
                        cow1Sprite.setPosition(180, 325); sheep1Sprite.setPosition(345, 166);
                        pig1Sprite.setPosition(380, 330); pig2Sprite.setPosition(760, 520);
                        putSheep1=true; putSheep2=true; putPig2=true; putCow1=true; putCow2=true; putPig1=true;
                    }
                    if(L40.getGlobalBounds().contains(mousePosF) && isLevelAvailable[40]==1)
                    {
                        makeLevelReady=41;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[1][3]=20; gameMatrix[3][3]=23; gameMatrix[5][1]=23; gameMatrix[5][5]=21;
                        gameMatrix[3][7]=22;
                        printMatrix(gameMatrix);
                        horse2Sprite.setPosition(200,515); cow1Sprite.setPosition(740, 320);
                        horse1Sprite.setPosition(385, 335); sheep1Sprite.setPosition(530, 545);
                        pig1Sprite.setPosition(180, 145); pig2Sprite.setPosition(380, 145);
                        putSheep1=true; putHorse2=true; putPig2=true; putHorse1=true; putCow1=true; putPig1=true;
                    }
                    if(L41.getGlobalBounds().contains(mousePosF) && isLevelAvailable[41]==1)
                    {
                        makeLevelReady=42;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=21; gameMatrix[1][5]=21; gameMatrix[5][7]=22; gameMatrix[1][7]=22; gameMatrix[3][3]=20;
                        gameMatrix[5][5]=23;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(775,525); cow1Sprite.setPosition(740, 135);
                        horse1Sprite.setPosition(385, 335); sheep1Sprite.setPosition(345, 166);
                        pig1Sprite.setPosition(560, 530); sheep2Sprite.setPosition(545, 166);
                        putSheep1=true; putCow2=true; putSheep2=true; putHorse1=true; putCow1=true; putPig1=true;
                    }
                    if(L42.getGlobalBounds().contains(mousePosF) && isLevelAvailable[42]==1)
                    {
                        makeLevelReady=43;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][1]=20; gameMatrix[5][1]=20; gameMatrix[5][5]=22; gameMatrix[3][5]=23; gameMatrix[1][7]=23;
                        gameMatrix[5][7]=21;
                        printMatrix(gameMatrix);
                        horse2Sprite.setPosition(765,140); cow1Sprite.setPosition(555, 515);
                        horse1Sprite.setPosition(570, 335); sheep1Sprite.setPosition(730, 545);
                        pig1Sprite.setPosition(190, 320); pig2Sprite.setPosition(190, 525);
                        putSheep1=true; putPig2=true; putHorse2=true; putHorse1=true; putCow1=true; putPig1=true;
                    }
                    if(L43.getGlobalBounds().contains(mousePosF) && isLevelAvailable[43]==1)
                    {
                        makeLevelReady=44;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=22; gameMatrix[5][5]=22; gameMatrix[1][3]=20; gameMatrix[1][7]=20; gameMatrix[3][5]=21;
                        gameMatrix[3][7]=21;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(730,345); cow1Sprite.setPosition(175, 135);
                        cow2Sprite.setPosition(610, 530); sheep1Sprite.setPosition(535, 345);
                        pig1Sprite.setPosition(390, 145); pig2Sprite.setPosition(755, 145);
                        putSheep1=true; putPig2=true; putSheep2=true; putCow2=true; putCow1=true; putPig1=true;
                    }
                    if(L44.getGlobalBounds().contains(mousePosF) && isLevelAvailable[44]==1)
                    {
                        makeLevelReady=45;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=21; gameMatrix[3][1]=21; gameMatrix[3][3]=20; gameMatrix[5][3]=23; gameMatrix[1][5]=22;
                        gameMatrix[5][7]=23;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(345,166); cow1Sprite.setPosition(550, 135);
                        horse2Sprite.setPosition(770, 520); sheep1Sprite.setPosition(166, 345);
                        horse1Sprite.setPosition(385, 525); pig1Sprite.setPosition(380, 335);
                        putSheep1=true; putHorse2=true; putSheep2=true; putHorse1=true; putCow1=true; putPig1=true;
                    }
                    if(L45.getGlobalBounds().contains(mousePosF) && isLevelAvailable[45]==1)
                    {
                        makeLevelReady=46;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=20; gameMatrix[1][7]=20; gameMatrix[3][3]=21; gameMatrix[3][5]=22; gameMatrix[3][7]=22;
                        gameMatrix[5][7]=21;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(732,535); cow1Sprite.setPosition(558, 320);
                        cow2Sprite.setPosition(785, 330); sheep1Sprite.setPosition(345, 345);
                        pig2Sprite.setPosition(748, 150); pig1Sprite.setPosition(190, 530);
                        putSheep1=true; putCow2=true; putSheep2=true; putPig2=true; putCow1=true; putPig1=true;
                    }
                    if(L46.getGlobalBounds().contains(mousePosF) && isLevelAvailable[46]==1)
                    {
                        makeLevelReady=47;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][3]=23; gameMatrix[3][5]=23; gameMatrix[5][3]=21; gameMatrix[5][7]=21; gameMatrix[1][1]=22;
                        gameMatrix[3][7]=22;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(732,535); cow1Sprite.setPosition(165, 140);
                        cow2Sprite.setPosition(785, 330); sheep1Sprite.setPosition(345, 545);
                        horse2Sprite.setPosition(580, 335); horse1Sprite.setPosition(380, 335);
                        putSheep1=true; putCow2=true; putSheep2=true; putHorse2=true; putCow1=true; putHorse1=true;
                    }
                    if(L47.getGlobalBounds().contains(mousePosF) && isLevelAvailable[47]==1)
                    {
                        makeLevelReady=48;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][1]=20; gameMatrix[5][5]=20; gameMatrix[1][3]=21; gameMatrix[3][3]=21; gameMatrix[3][5]=23;
                        gameMatrix[1][7]=23;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(345,345); pig1Sprite.setPosition(185, 140);
                        pig2Sprite.setPosition(570, 525); sheep1Sprite.setPosition(345, 166);
                        horse2Sprite.setPosition(767, 145); horse1Sprite.setPosition(565, 335);
                        putSheep1=true; putPig2=true; putSheep2=true; putHorse2=true; putPig1=true; putHorse1=true;
                    }
                    if(L48.getGlobalBounds().contains(mousePosF) && isLevelAvailable[48]==1)
                    {
                        makeLevelReady=49;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[3][5]=20; gameMatrix[3][1]=22; gameMatrix[3][3]=22; gameMatrix[5][1]=23;
                        gameMatrix[5][5]=23, gameMatrix[3][7]=21;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(415,335); pig1Sprite.setPosition(185, 150);
                        pig2Sprite.setPosition(565, 335); cow1Sprite.setPosition(180, 315);
                        horse2Sprite.setPosition(580, 523); horse1Sprite.setPosition(205, 523);
                        sheep1Sprite.setPosition(725, 354);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putPig2=true;
                    }
                    if(L49.getGlobalBounds().contains(mousePosF) && isLevelAvailable[49]==1)
                    {
                        makeLevelReady=50;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=23; gameMatrix[1][3]=23; gameMatrix[1][5]=21; gameMatrix[3][5]=21; gameMatrix[5][1]=22;
                        gameMatrix[3][3]=22, gameMatrix[5][5]=20;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(545,345); pig1Sprite.setPosition(565, 530);
                        cow2Sprite.setPosition(405, 335); cow1Sprite.setPosition(180, 520);
                        horse2Sprite.setPosition(205, 148); horse1Sprite.setPosition(390, 150);
                        sheep1Sprite.setPosition(535, 166);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putCow2=true;
                    }
                    if(L50.getGlobalBounds().contains(mousePosF) && isLevelAvailable[50]==1)
                    {
                        makeLevelReady=51;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=21; gameMatrix[1][5]=21; gameMatrix[3][1]=20; gameMatrix[5][5]=20; gameMatrix[3][3]=23;
                        gameMatrix[3][5]=23, gameMatrix[3][7]=22;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(540,160); pig1Sprite.setPosition(190, 330);
                        pig2Sprite.setPosition(568, 525); cow1Sprite.setPosition(740, 520);
                        horse2Sprite.setPosition(580, 330); horse1Sprite.setPosition(385, 330);
                        sheep1Sprite.setPosition(160, 160);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putPig2=true;
                    }
                    if(L51.getGlobalBounds().contains(mousePosF) && isLevelAvailable[51]==1)
                    {
                        makeLevelReady=52;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=20; gameMatrix[5][3]=20; gameMatrix[3][5]=21; gameMatrix[5][5]=21; gameMatrix[3][7]=23;
                        gameMatrix[5][7]=23, gameMatrix[1][3]=22;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(540,545); pig1Sprite.setPosition(200, 530);
                        pig2Sprite.setPosition(380, 530); cow1Sprite.setPosition(365, 140);
                        horse2Sprite.setPosition(765, 530); horse1Sprite.setPosition(765, 325);
                        sheep1Sprite.setPosition(545, 345);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putPig2=true;
                    }
                    if(L52.getGlobalBounds().contains(mousePosF) && isLevelAvailable[52]==1)
                    {
                        makeLevelReady=53;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][3]=21; gameMatrix[1][5]=21; gameMatrix[1][1]=23; gameMatrix[3][1]=23; gameMatrix[5][1]=22;
                        gameMatrix[5][5]=22, gameMatrix[5][7]=20;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(540,166); pig1Sprite.setPosition(755, 525);
                        cow2Sprite.setPosition(590, 530); cow1Sprite.setPosition(175, 520);
                        horse2Sprite.setPosition(200, 340); horse1Sprite.setPosition(200, 150);
                        sheep1Sprite.setPosition(345, 166);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putCow2=true;
                    }
                    if(L53.getGlobalBounds().contains(mousePosF) && isLevelAvailable[53]==1)
                    {
                        makeLevelReady=54;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=22; gameMatrix[1][3]=22; gameMatrix[1][5]=20; gameMatrix[1][7]=20; gameMatrix[3][1]=21;
                        gameMatrix[3][3]=21, gameMatrix[3][7]=23;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(166,345); pig1Sprite.setPosition(748, 155);
                        cow2Sprite.setPosition(410, 145); cow1Sprite.setPosition(175, 140);
                        pig2Sprite.setPosition(560, 155); horse1Sprite.setPosition(760, 340);
                        sheep1Sprite.setPosition(355, 355);
                        putSheep1=true; putSheep2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putCow2=true;
                    }
                    if(L54.getGlobalBounds().contains(mousePosF) && isLevelAvailable[54]==1)
                    {
                        makeLevelReady=55;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=21; gameMatrix[3][3]=21; gameMatrix[5][1]=23; gameMatrix[5][3]=23; gameMatrix[5][5]=20;
                        gameMatrix[1][7]=20, gameMatrix[3][7]=22;
                        printMatrix(gameMatrix);
                        sheep2Sprite.setPosition(345,345); pig1Sprite.setPosition(560, 525);
                        horse2Sprite.setPosition(390, 525); cow1Sprite.setPosition(740, 325);
                        pig2Sprite.setPosition(750, 145); horse1Sprite.setPosition(200, 525);
                        sheep1Sprite.setPosition(165, 160);
                        putSheep1=true; putSheep2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putHorse2=true;
                    }
                    if(L55.getGlobalBounds().contains(mousePosF) && isLevelAvailable[55]==1)
                    {
                        makeLevelReady=56;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[3][1]=20; gameMatrix[3][5]=23; gameMatrix[1][7]=23; gameMatrix[5][7]=21;
                        gameMatrix[5][1]=22, gameMatrix[5][5]=22;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(590,525); pig1Sprite.setPosition(183, 150);
                        horse2Sprite.setPosition(765, 147); cow1Sprite.setPosition(175, 510);
                        pig2Sprite.setPosition(183, 330); horse1Sprite.setPosition(573, 345);
                        sheep1Sprite.setPosition(730, 545);
                        putSheep1=true; putCow2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putHorse2=true;
                    }
                    if(L56.getGlobalBounds().contains(mousePosF) && isLevelAvailable[56]==1)
                    {
                        makeLevelReady=57;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[3][3]=21; gameMatrix[3][5]=21; gameMatrix[3][7]=23; gameMatrix[1][1]=22; gameMatrix[5][1]=22;
                        gameMatrix[5][3]=20, gameMatrix[5][7]=20;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(213,525); pig1Sprite.setPosition(373, 528);
                        sheep2Sprite.setPosition(545, 345); cow1Sprite.setPosition(185, 138);
                        pig2Sprite.setPosition(755, 528); horse1Sprite.setPosition(760, 335);
                        sheep1Sprite.setPosition(345, 345);
                        putSheep1=true; putCow2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putSheep2=true;
                    }
                    if(L57.getGlobalBounds().contains(mousePosF) && isLevelAvailable[57]==1)
                    {
                        makeLevelReady=58;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[5][1]=21; gameMatrix[5][3]=21; gameMatrix[5][5]=23; gameMatrix[5][7]=22; gameMatrix[1][7]=22;
                        gameMatrix[3][1]=23, gameMatrix[1][5]=20;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(780,515); pig1Sprite.setPosition(560, 150);
                        sheep2Sprite.setPosition(345, 545); cow1Sprite.setPosition(740, 138);
                        horse2Sprite.setPosition(578, 515); horse1Sprite.setPosition(200, 335);
                        sheep1Sprite.setPosition(166, 545);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true;
                    }
                    if(L58.getGlobalBounds().contains(mousePosF) && isLevelAvailable[58]==1)
                    {
                        makeLevelReady=59;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=23; gameMatrix[3][1]=23; gameMatrix[5][1]=21; gameMatrix[5][3]=21; gameMatrix[5][7]=20;
                        gameMatrix[3][7]=20; gameMatrix[1][7]=22; gameMatrix[3][3]=22;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(410,340); pig1Sprite.setPosition(750, 530);
                        sheep2Sprite.setPosition(340, 545); cow1Sprite.setPosition(735, 130);
                        horse2Sprite.setPosition(200, 329); horse1Sprite.setPosition(200, 155);
                        sheep1Sprite.setPosition(166, 545); pig2Sprite.setPosition(750, 335);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; putPig2=true;
                    }
                    if(L59.getGlobalBounds().contains(mousePosF) && isLevelAvailable[59]==1)
                    {
                        makeLevelReady=60;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][1]=20; gameMatrix[5][1]=20; gameMatrix[1][3]=21; gameMatrix[1][7]=21; gameMatrix[3][3]=22;
                        gameMatrix[5][7]=22; gameMatrix[5][3]=23; gameMatrix[5][5]=23;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(780,525); pig1Sprite.setPosition(188, 155);
                        sheep2Sprite.setPosition(720, 166); cow1Sprite.setPosition(365, 325);
                        horse2Sprite.setPosition(585, 530); horse1Sprite.setPosition(385, 530);
                        sheep1Sprite.setPosition(345, 166); pig2Sprite.setPosition(183, 525);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; putPig2=true;
                    }
                    if(L60.getGlobalBounds().contains(mousePosF) && isLevelAvailable[60]==1)
                    {
                        gameFinished=true;
                        isLevelPresed=true;
                        initZoneMatrix(zoneMatrix); initGameMatrix(gameMatrix);
                        gameMatrix[1][7]=20; gameMatrix[5][7]=20; gameMatrix[5][1]=21; gameMatrix[5][3]=21; gameMatrix[3][3]=22;
                        gameMatrix[3][1]=22; gameMatrix[1][1]=23; gameMatrix[3][5]=23;
                        printMatrix(gameMatrix);
                        cow2Sprite.setPosition(215,335); pig1Sprite.setPosition(750, 155);
                        sheep2Sprite.setPosition(345, 545); cow1Sprite.setPosition(365, 325);
                        horse2Sprite.setPosition(585, 335); horse1Sprite.setPosition(200, 150);
                        sheep1Sprite.setPosition(166, 545); pig2Sprite.setPosition(750, 525);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; putPig2=true;
                    }


                    if(isLevelPresed==true)
                    {
                        buttonSound.play();
                        pg=2;
                        reset=true;
                        isLevelPresed=false;
                        column = 0;
                        moves=0;
                        i=0;
                        while(fences[0][i]!=0)
                             {
                                 fences[0][i]=0;
                                 fences[1][i]=0;
                                 fences[2][i]=0;
                                 i++;
                             }
                             for(i=0;i<=6;i++) for(j=0;j<=8;j++) copyMatrix[i][j]=gameMatrix[i][j];
                        counter[0]=0;
                        counter[1]=0;
                        counter[2]=0;
                        Vector2i centrare((VideoMode::getDesktopMode().width / 2) - 600, (VideoMode::getDesktopMode().height / 2) - 600);
                        window.create(VideoMode(w, h), "Live game");
                        window.setPosition(centrare);
                        cornerFence1Sprite.setScale(0.4, 0.8);
                        cornerFence1Sprite.setPosition(690, 905);
                        cornerFence2Sprite.setScale(0.6, 0.4);
                        cornerFence2Sprite.setPosition(860, 775);
                        cornerFence3Sprite.setScale(0.4, 0.6);
                        cornerFence3Sprite.setPosition(1005, 920);
                        cornerFence4Sprite.setScale(0.6, 0.45);
                        cornerFence4Sprite.setPosition(860, 1060);
                        horizontalFence2Sprite.setScale(0.8, 1);
                        horizontalFence3Sprite.setScale(0.8, 1);
                        horizontalFence2Sprite.setPosition(105, 850);
                        horizontalFence3Sprite.setPosition(30, 950);
                        verticalFence2Sprite.setScale(0.8, 1);
                        verticalFence2Sprite.setPosition(403, 827);
                        verticalFence3Sprite.setScale(0.8, 1);
                        verticalFence3Sprite.setPosition(520, 780);
                        rectangleL2.setPosition(50, 820);
                        rectangleL2.setSize(Vector2f(320, 100));
                        rectangleL2.setFillColor(Color(0, 196, 33, 200));
                        rectangleL3.setPosition(50, 940);
                        rectangleL3.setSize(Vector2f(320, 100));
                        rectangleL3.setFillColor(Color(0, 196, 33, 200));
                        rectangleL4.setPosition(410, 780);
                        rectangleL4.setSize(Vector2f(100, 390));
                        rectangleL4.setFillColor(Color(0, 196, 33, 200));
                        rectangleL5.setPosition(540, 780);
                        rectangleL5.setSize(Vector2f(100, 390));
                        rectangleL5.setFillColor(Color(0, 196, 33, 200));
                        rectangleCorner1.setPosition(700, 905);
                        rectangleCorner1.setSize(Vector2f(140, 140));
                        rectangleCorner1.setFillColor(Color(0, 196, 33, 200));
                        rectangleCorner2.setPosition(850, 1050);
                        rectangleCorner2.setSize(Vector2f(140, 140));
                        rectangleCorner2.setFillColor(Color(0, 196, 33, 200));
                        rectangleCorner3.setPosition(1000, 905);
                        rectangleCorner3.setSize(Vector2f(140, 140));
                        rectangleCorner3.setFillColor(Color(0, 196, 33, 200));
                        rectangleCorner4.setPosition(850, 765);
                        rectangleCorner4.setSize(Vector2f(140, 140));
                        rectangleCorner4.setFillColor(Color(0, 196, 33, 200));
                        window.draw(backgroundSprite);
                        window.draw(backSprite);
                        window.draw(boardSprite);
                        window.draw(rectangleL2);
                        window.draw(rectangleL3);
                        window.draw(rectangleL4);
                        window.draw(rectangleL5);
                        window.draw(rectangleCorner1);
                        window.draw(rectangleCorner2);
                        window.draw(rectangleCorner3);
                        window.draw(rectangleCorner4);
                        window.draw(verticalFence2Sprite);
                        window.draw(verticalFence3Sprite);
                        window.draw(horizontalFence2Sprite);
                        window.draw(horizontalFence3Sprite);
                        window.draw(cornerFence1Sprite);
                        window.draw(cornerFence2Sprite);
                        window.draw(cornerFence3Sprite);
                        window.draw(cornerFence4Sprite);
                        window.draw(resetGame);
                        if(putCow1==true) window.draw(cow1Sprite);
                        if(putCow2==true) window.draw(cow2Sprite);
                        if(putHorse1==true) window.draw(horse1Sprite);
                        if(putHorse2==true) window.draw(horse2Sprite);
                        if(putSheep1==true) window.draw(sheep1Sprite);
                        if(putPig1==true) window.draw(pig1Sprite);
                        if(putPig2==true) window.draw(pig2Sprite);
                        if(putSheep2==true) window.draw(sheep2Sprite);
                        window.display();
                        
                    }
                }

                if((backLevel1.getGlobalBounds().contains(mousePosF) && pg==3) || (backLevel2.getGlobalBounds().contains(mousePosF) && pg==4) )
                    {   
                        buttonSound.play();
                        pg=1;
                        window.clear();
                        window.draw(levelBackgroundSprite);
                        if(isLevelAvailable[1]==1) L1.setFillColor(Color(44, 110, 255, 255));   else L1.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[2]==1) L2.setFillColor(Color(44, 110, 255, 255));   else L2.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[3]==1) L3.setFillColor(Color(44, 110, 255, 255));   else L3.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[4]==1) L4.setFillColor(Color(44, 110, 255, 255));   else L4.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[5]==1) L5.setFillColor(Color(44, 110, 255, 255));   else L5.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[6]==1) L6.setFillColor(Color(44, 110, 255, 255));   else L6.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[7]==1) L7.setFillColor(Color(44, 110, 255, 255));   else L7.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[8]==1) L8.setFillColor(Color(44, 110, 255, 255));   else L8.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[9]==1) L9.setFillColor(Color(44, 110, 255, 255));   else L9.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[10]==1) L10.setFillColor(Color(44, 110, 255, 255));   else L10.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[11]==1) L11.setFillColor(Color(44, 110, 255, 255));   else L11.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[12]==1) L12.setFillColor(Color(44, 110, 255, 255));   else L12.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[13]==1) L13.setFillColor(Color(44, 110, 255, 255));   else L13.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[14]==1) L14.setFillColor(Color(44, 110, 255, 255));   else L14.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[15]==1) L15.setFillColor(Color(44, 110, 255, 255));   else L15.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[16]==1) L16.setFillColor(Color(44, 110, 255, 255));   else L16.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[17]==1) L17.setFillColor(Color(44, 110, 255, 255));   else L17.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[18]==1) L18.setFillColor(Color(44, 110, 255, 255));   else L18.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[19]==1) L19.setFillColor(Color(44, 110, 255, 255));   else L19.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[20]==1) L20.setFillColor(Color(44, 110, 255, 255));   else L20.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[21]==1) L21.setFillColor(Color(44, 110, 255, 255));   else L21.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[22]==1) L22.setFillColor(Color(44, 110, 255, 255));   else L22.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[23]==1) L23.setFillColor(Color(44, 110, 255, 255));   else L23.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[24]==1) L24.setFillColor(Color(44, 110, 255, 255));   else L24.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[25]==1) L25.setFillColor(Color(44, 110, 255, 255));   else L25.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[26]==1) L26.setFillColor(Color(44, 110, 255, 255));   else L26.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[27]==1) L27.setFillColor(Color(44, 110, 255, 255));   else L27.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[28]==1) L28.setFillColor(Color(44, 110, 255, 255));   else L28.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[29]==1) L29.setFillColor(Color(44, 110, 255, 255));   else L29.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[30]==1) L30.setFillColor(Color(44, 110, 255, 255));   else L30.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[31]==1) L31.setFillColor(Color(44, 110, 255, 255));   else L31.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[32]==1) L32.setFillColor(Color(44, 110, 255, 255));   else L32.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[33]==1) L33.setFillColor(Color(44, 110, 255, 255));   else L33.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[34]==1) L34.setFillColor(Color(44, 110, 255, 255));   else L34.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[35]==1) L35.setFillColor(Color(44, 110, 255, 255));   else L35.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[36]==1) L36.setFillColor(Color(44, 110, 255, 255));   else L36.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[37]==1) L37.setFillColor(Color(44, 110, 255, 255));   else L37.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[38]==1) L38.setFillColor(Color(44, 110, 255, 255));   else L38.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[39]==1) L39.setFillColor(Color(44, 110, 255, 255));   else L39.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[40]==1) L40.setFillColor(Color(44, 110, 255, 255));   else L40.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[41]==1) L41.setFillColor(Color(44, 110, 255, 255));   else L41.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[42]==1) L42.setFillColor(Color(44, 110, 255, 255));   else L42.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[43]==1) L43.setFillColor(Color(44, 110, 255, 255));   else L43.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[44]==1) L44.setFillColor(Color(44, 110, 255, 255));   else L44.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[45]==1) L45.setFillColor(Color(44, 110, 255, 255));   else L45.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[46]==1) L46.setFillColor(Color(44, 110, 255, 255));   else L46.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[47]==1) L47.setFillColor(Color(44, 110, 255, 255));   else L47.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[48]==1) L48.setFillColor(Color(44, 110, 255, 255));   else L48.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[49]==1) L49.setFillColor(Color(44, 110, 255, 255));   else L49.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[50]==1) L50.setFillColor(Color(44, 110, 255, 255));   else L50.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[51]==1) L51.setFillColor(Color(44, 110, 255, 255));   else L51.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[52]==1) L52.setFillColor(Color(44, 110, 255, 255));   else L52.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[53]==1) L53.setFillColor(Color(44, 110, 255, 255));   else L53.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[54]==1) L54.setFillColor(Color(44, 110, 255, 255));   else L54.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[55]==1) L55.setFillColor(Color(44, 110, 255, 255));   else L55.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[56]==1) L56.setFillColor(Color(44, 110, 255, 255));   else L56.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[57]==1) L57.setFillColor(Color(44, 110, 255, 255));   else L57.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[58]==1) L58.setFillColor(Color(44, 110, 255, 255));   else L58.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[59]==1) L59.setFillColor(Color(44, 110, 255, 255));   else L59.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[60]==1) L60.setFillColor(Color(44, 110, 255, 255));   else L60.setFillColor(Color(0, 0, 0, 127));
                        window.draw(Levels);
                        window.draw(L1); window.draw(L2); window.draw(L3); window.draw(L4); window.draw(L5); window.draw(L6); 
                        window.draw(L7); window.draw(L8); window.draw(L9); window.draw(L10); window.draw(L11); window.draw(L12); 
                        window.draw(L13); window.draw(L14); window.draw(L15); window.draw(L16); window.draw(L17); window.draw(L18); 
                        window.draw(L19); window.draw(L20); window.draw(L21); window.draw(L22); window.draw(L23); window.draw(L24); 
                        window.draw(L25); window.draw(L26); window.draw(L27); window.draw(L28); window.draw(L29); window.draw(L30);
                        window.draw(L31); window.draw(L32); window.draw(L33); window.draw(L34); window.draw(L35); window.draw(L36); 
                        window.draw(L37); window.draw(L38); window.draw(L39); window.draw(L40); window.draw(L41); window.draw(L42); 
                        window.draw(L43); window.draw(L44); window.draw(L45); window.draw(L46); window.draw(L47); window.draw(L48);
                        window.draw(L49); window.draw(L50); window.draw(L51); window.draw(L52); window.draw(L53); window.draw(L54);   
                        window.draw(L55); window.draw(L56); window.draw(L57); window.draw(L58); window.draw(L59); window.draw(L60); 
                        backSprite.setScale(0.2, 0.2);
                        backSprite.setScale(0.2, 0.2);
                        backSprite.setPosition(10.0, 10.0);
                        window.draw(backSprite);
                        window.display();
                        putCow1=false, putCow2=false, putHorse1=false, putHorse2=false, putPig1=false, putSheep1=false, putPig2=false, putSheep2=false;

                    }

                if(pg==3)
                {
                    if(nextLevel.getGlobalBounds().contains(mousePosF))
                    {
                        buttonSound.play();
                        pg=2;
                        initGameMatrix(gameMatrix);
                        initZoneMatrix(zoneMatrix);
                        for(i=0;i<=9;i++){
                            fences[0][i]=0;
                            fences[1][i]=0;
                            fences[2][i]=0;
                        }
                        isMarked=false; putCow1=false; putCow2=false; putHorse1=false; putHorse2=false; putPig1=false; putPig2=false; putSheep1=false; putSheep2=false;
                        column = 0;
                        moves=0;
                        counter[0]=0;
                        counter[1]=0;
                        counter[2]=0;
                        for(i=0;i<=8;i++) OK[i]=0;
                        switch(makeLevelReady)
                        {
                          case 2:{
                        gameMatrix[1][7]=20; gameMatrix[3][5]=23;
                        pig1Sprite.setPosition(750, 140); horse1Sprite.setPosition(575, 325);
                        putPig1=true; putHorse1=true; makeLevelReady=3; break;
                          }
                         case 3:{
                        gameMatrix[5][7]=21; gameMatrix[5][1]=23;
                        sheep1Sprite.setPosition(735, 535); horse1Sprite.setPosition(200, 518);
                        putSheep1=true; putHorse1=true; makeLevelReady=4; break;
                         }
                         case 4:{
                        gameMatrix[1][3]=20; gameMatrix[3][3]=22;
                        pig1Sprite.setPosition(378, 148); cow1Sprite.setPosition(365, 320);
                        putPig1=true; putCow1=true; makeLevelReady=5; break;
                         }
                         case 5:{
                        gameMatrix[1][7]=21; gameMatrix[5][1]=22;
                        sheep1Sprite.setPosition(720, 160); cow2Sprite.setPosition(225, 523);
                        putSheep1=true; putCow2=true; makeLevelReady=6; break;
                         }
                         case 6:{
                        gameMatrix[3][3]=23; gameMatrix[5][7]=22;
                        horse2Sprite.setPosition(395, 335); cow1Sprite.setPosition(745, 510);
                        putHorse2=true; putCow1=true; makeLevelReady=7; break;
                         }
                         case 7:{
                        gameMatrix[1][1]=20; gameMatrix[1][3]=21;
                        pig1Sprite.setPosition(188, 150); sheep1Sprite.setPosition(340, 170);
                        putPig1=true; putSheep1=true; makeLevelReady=8; break;
                         }
                         case 8:{
                        gameMatrix[5][1]=23; gameMatrix[3][3]=22; gameMatrix[3][7]=21;
                        horse1Sprite.setPosition(198, 524); sheep1Sprite.setPosition(730, 345); cow1Sprite.setPosition(367, 320);
                        putHorse1=true; putCow1=true; putSheep1=true; makeLevelReady=9; break;
                         }
                         case 9:{
                        gameMatrix[1][1]=20; gameMatrix[3][5]=20; gameMatrix[5][3]=22;
                        pig1Sprite.setPosition(185, 150); cow2Sprite.setPosition(410, 520); pig2Sprite.setPosition(570, 325);
                        putPig2=true; putCow2=true; putPig1=true; makeLevelReady=10; break;
                         } 
                         case 10:{
                        gameMatrix[1][3]=23; gameMatrix[3][3]=20; gameMatrix[3][5]=21;
                        horse1Sprite.setPosition(390, 145); sheep1Sprite.setPosition(530, 355); pig1Sprite.setPosition(380, 338);
                        putPig1=true; putSheep1=true; putHorse1=true; makeLevelReady=11; break;
                         }
                         case 11:{
                        gameMatrix[1][1]=22; gameMatrix[5][1]=21; gameMatrix[5][7]=23;
                        cow2Sprite.setPosition(220, 145); sheep1Sprite.setPosition(155, 540); horse2Sprite.setPosition(770, 520);
                        putCow2=true;  putSheep1=true; putHorse2=true; makeLevelReady=12; break;
                         }
                         case 12:{
                        gameMatrix[1][5]=22; gameMatrix[3][3]=21; gameMatrix[5][3]=22;
                        cow1Sprite.setPosition(555, 130); cow2Sprite.setPosition(415, 525); sheep1Sprite.setPosition(345, 355);
                        putCow2=true; putCow1=true; putSheep1=true; makeLevelReady=13; break;
                         }
                         case 13:{
                        gameMatrix[3][1]=23; gameMatrix[3][3]=20; gameMatrix[5][5]=23;
                        horse1Sprite.setPosition(575, 520); horse2Sprite.setPosition(210, 330); pig1Sprite.setPosition(385, 335);
                        putHorse2=true; putPig1=true; putHorse1=true; makeLevelReady=14; break;
                         }
                         case 14:{
                        gameMatrix[3][1]=21; gameMatrix[3][5]=22;  gameMatrix[5][5]=21;
                        sheep1Sprite.setPosition(545, 530);  sheep2Sprite.setPosition(170, 345); cow1Sprite.setPosition(550, 320);
                        putSheep2=true; putCow1=true; putSheep1=true; makeLevelReady=15; break;
                         }
                         case 15:{
                        gameMatrix[5][1]=22; gameMatrix[5][3]=20; gameMatrix[5][5]=21;
                        sheep1Sprite.setPosition(540, 530); cow1Sprite.setPosition(180, 510); pig1Sprite.setPosition(385, 520);
                        putSheep1=true; putPig1=true; putCow1=true; makeLevelReady=16; break;
                         }
                         case 16:{
                        gameMatrix[3][5]=23; gameMatrix[1][1]=21; gameMatrix[5][3]=21;
                        horse1Sprite.setPosition(575, 335); sheep1Sprite.setPosition(158, 165); sheep2Sprite.setPosition(350, 535);
                        putHorse1=true; putSheep1=true; putSheep2=true; makeLevelReady=17; break;
                         }
                         case 17:{
                        gameMatrix[3][7]=23; gameMatrix[3][5]=22; gameMatrix[5][5]=20;
                        horse2Sprite.setPosition(775, 335); cow1Sprite.setPosition(550, 325); pig1Sprite.setPosition(575, 520);
                        putHorse2=true; putCow1=true; putPig1=true; makeLevelReady=18; break;
                         }
                         case 18:{
                        gameMatrix[1][1]=20; gameMatrix[1][7]=21; gameMatrix[5][1]=22; gameMatrix[5][5]=23;
                        horse1Sprite.setPosition(575, 520); cow1Sprite.setPosition(175, 515); pig1Sprite.setPosition(185, 150); sheep1Sprite.setPosition(718, 165);
                        putHorse1=true; putSheep1=true; putCow1=true; putPig1=true; makeLevelReady=19; break;
                         }
                         case 19:{
                        gameMatrix[1][1]=22;  gameMatrix[3][3]=22; gameMatrix[3][5]=20;  gameMatrix[5][3]=20;
                        cow1Sprite.setPosition(555, 320); cow2Sprite.setPosition(415, 515); pig1Sprite.setPosition(380, 335); pig2Sprite.setPosition(185, 150);
                        putCow1=true; putCow2=true; putPig1=true; putPig2=true; makeLevelReady=20; break;
                         }
                         case 20:{
                        gameMatrix[5][3]=21; gameMatrix[1][7]=21; gameMatrix[1][5]=23; gameMatrix[3][1]=20;
                        sheep1Sprite.setPosition(715, 160); sheep2Sprite.setPosition(350, 535); horse2Sprite.setPosition(585, 148); pig1Sprite.setPosition(190, 335);
                        putSheep1=true; putSheep2=true; putHorse2=true; putPig1=true; makeLevelReady=21; break;
                         }
                         case 21:{
                        gameMatrix[3][1]=22; gameMatrix[3][7]=22; gameMatrix[5][3]=20; gameMatrix[5][7]=21;
                        cow1Sprite.setPosition(175, 325); cow2Sprite.setPosition(785, 335); pig1Sprite.setPosition(380, 525); sheep1Sprite.setPosition(720, 535);
                        putCow1=true; putCow2=true; putPig1=true; putSheep1=true; makeLevelReady=22; break;
                         }
                         case 22:{
                        gameMatrix[1][3]=20; gameMatrix[3][3]=21; gameMatrix[3][5]=22; gameMatrix[1][7]=23;
                        cow1Sprite.setPosition(548, 320); horse1Sprite.setPosition(765, 148); pig1Sprite.setPosition(375, 150); sheep1Sprite.setPosition(348, 350);
                        putCow1=true; putHorse1=true; putPig1=true; putSheep1=true; makeLevelReady=23; break;
                         }
                         case 23:{
                        gameMatrix[3][1]=22; gameMatrix[5][3]=20; gameMatrix[5][5]=20; gameMatrix[1][5]=21;
                        cow1Sprite.setPosition(170, 320); sheep1Sprite.setPosition(545, 160); pig1Sprite.setPosition(385, 525); pig2Sprite.setPosition(555, 525);
                        putCow1=true; putSheep1=true; putPig1=true; putPig2=true; makeLevelReady=24; break;
                         }
                         case 24:{
                        gameMatrix[1][1]=23; gameMatrix[5][1]=21; gameMatrix[3][5]=22; gameMatrix[1][7]=20;
                        horse2Sprite.setPosition(200, 150); sheep1Sprite.setPosition(160, 545); pig1Sprite.setPosition(755, 140); cow2Sprite.setPosition(600,333);
                        putHorse2=true; putSheep1=true; putPig1=true; putCow2=true; makeLevelReady=25; break;
                         }
                         case 25:{
                        gameMatrix[1][1]=20; gameMatrix[3][1]=21; gameMatrix[5][1]=22; gameMatrix[3][5]=23;
                        horse1Sprite.setPosition(570, 335); sheep1Sprite.setPosition(160, 355); pig1Sprite.setPosition(180, 155); cow1Sprite.setPosition(179,515);
                        putHorse1=true; putSheep1=true; putPig1=true; putCow1=true; makeLevelReady=26; break;
                         }
                         case 26:{
                        gameMatrix[5][1]=22; gameMatrix[3][3]=21; gameMatrix[5][5]=20; gameMatrix[3][7]=23;
                        horse2Sprite.setPosition(765, 335); sheep1Sprite.setPosition(355, 355); pig1Sprite.setPosition(565, 525); cow2Sprite.setPosition(220, 520);
                        putHorse2=true; putSheep1=true; putPig1=true; putCow2=true; makeLevelReady=27; break;
                         }
                         case 27:{
                        gameMatrix[1][1]=21; gameMatrix[5][3]=21; gameMatrix[3][1]=22;  gameMatrix[1][7]=23;
                        horse1Sprite.setPosition(765, 150); sheep1Sprite.setPosition(166, 166); sheep2Sprite.setPosition(350, 528); cow1Sprite.setPosition(178, 325);
                        putHorse1=true; putSheep1=true; putSheep2=true; putCow1=true; makeLevelReady=28; break;
                         }
                         case 28:{
                        gameMatrix[5][5]=23; gameMatrix[1][7]=23; gameMatrix[1][1]=20; gameMatrix[3][3]=20; gameMatrix[5][1]=21;
                        horse1Sprite.setPosition(575,520); horse2Sprite.setPosition(765, 150); sheep1Sprite.setPosition(166, 545); pig1Sprite.setPosition(178, 155);
                        pig2Sprite.setPosition(380, 330);
                        putHorse1=true; putSheep1=true; putHorse2=true; putPig1=true; putPig2=true; makeLevelReady=29; break;
                         }
                         case 29:{
                        gameMatrix[5][1]=20; gameMatrix[5][5]=20; gameMatrix[3][3]=21; gameMatrix[1][1]=22; gameMatrix[5][7]=23;
                        horse1Sprite.setPosition(765,525); cow1Sprite.setPosition(180, 130); sheep1Sprite.setPosition(350, 345);pig1Sprite.setPosition(180, 530);
                        pig2Sprite.setPosition(565, 520);
                        putHorse1=true; putSheep1=true; putCow1=true; putPig1=true; putPig2=true; makeLevelReady=30; break;
                         }
                         case 30:{
                        gameMatrix[3][7]=21; gameMatrix[5][5]=20; gameMatrix[5][3]=23; gameMatrix[1][3]=22; gameMatrix[1][7]=22;
                        horse1Sprite.setPosition(378,525); pig1Sprite.setPosition(565, 525); sheep1Sprite.setPosition(725, 345); cow1Sprite.setPosition(370, 135);
                        cow2Sprite.setPosition(780, 150);
                        putHorse1=true; putSheep1=true; putCow1=true; putCow2=true; putPig1=true; makeLevelReady=31; break;
                         }
                         case 31:{
                        gameMatrix[3][3]=23; gameMatrix[3][5]=23; gameMatrix[3][1]=20; gameMatrix[1][5]=22; gameMatrix[3][7]=21;
                        horse1Sprite.setPosition(388,335); horse2Sprite.setPosition(578, 335); sheep1Sprite.setPosition(725, 345); cow1Sprite.setPosition(550, 135); 
                        pig1Sprite.setPosition(190, 340);
                        putHorse1=true; putSheep1=true; putCow1=true; putPig1=true; putHorse2=true; makeLevelReady=32; break;
                         }
                         case 32:{
                        gameMatrix[1][1]=20; gameMatrix[3][1]=20; gameMatrix[3][3]=22; gameMatrix[1][5]=23; gameMatrix[5][3]=23;
                        horse1Sprite.setPosition(388,520); horse2Sprite.setPosition(578, 150); pig2Sprite.setPosition(190, 327); cow1Sprite.setPosition(370, 315);
                        pig1Sprite.setPosition(190, 147);
                        putHorse1=true; putPig2=true; putCow1=true; putPig1=true; putHorse2=true; makeLevelReady=33; break;
                         }
                         case 33:{
                        gameMatrix[1][3]=21; gameMatrix[1][5]=21; gameMatrix[3][5]=20; gameMatrix[5][5]=22; gameMatrix[3][7]=22;
                        sheep1Sprite.setPosition(350,166); sheep2Sprite.setPosition(535, 166); pig1Sprite.setPosition(575, 327); cow1Sprite.setPosition(555, 505);
                        cow2Sprite.setPosition(785, 330);
                        putSheep1=true; putCow2=true; putCow1=true; putPig1=true; putSheep2=true; makeLevelReady=34; break;
                         }
                         case 34:{
                        gameMatrix[1][1]=20; gameMatrix[5][3]=21; gameMatrix[3][3]=23; gameMatrix[1][7]=23; gameMatrix[5][7]=22;
                        horse1Sprite.setPosition(385,335); horse2Sprite.setPosition(765, 150); pig1Sprite.setPosition(185, 155); cow1Sprite.setPosition(748, 520);
                        sheep1Sprite.setPosition(355, 545);
                        putSheep1=true; putCow1=true; putHorse1=true; putPig1=true; putHorse2=true; makeLevelReady=35; break;
                         }
                         case 35:{
                        gameMatrix[3][3]=20; gameMatrix[3][5]=20; gameMatrix[5][3]=21; gameMatrix[5][7]=21; gameMatrix[3][7]=22;
                        pig2Sprite.setPosition(565,335); sheep2Sprite.setPosition(740, 540); pig1Sprite.setPosition(375, 340); cow1Sprite.setPosition(748, 330);
                        sheep1Sprite.setPosition(345, 545);
                        putSheep1=true; putCow1=true; putSheep2=true; putPig1=true; putPig2=true; makeLevelReady=36; break;
                         }
                        case 36:{
                        gameMatrix[1][1]=23; gameMatrix[3][5]=23; gameMatrix[3][1]=21; gameMatrix[1][7]=20; gameMatrix[5][1]=20;
                        pig2Sprite.setPosition(750,150); horse2Sprite.setPosition(575, 328); pig1Sprite.setPosition(185, 530); sheep1Sprite.setPosition(166, 340);
                        horse1Sprite.setPosition(195, 155);
                        putSheep1=true; putHorse1=true; putHorse2=true; putPig1=true; putPig2=true; makeLevelReady=37; break;
                        }
                        case 37:{
                        gameMatrix[1][1]=21; gameMatrix[3][1]=21; gameMatrix[3][3]=22; gameMatrix[3][5]=22; gameMatrix[5][1]=23;
                        cow2Sprite.setPosition(600,330); horse1Sprite.setPosition(195, 525); cow1Sprite.setPosition(365, 320); sheep1Sprite.setPosition(166, 165);
                        sheep2Sprite.setPosition(162, 348);
                        putSheep1=true; putHorse1=true; putSheep2=true; putCow1=true; putCow2=true; makeLevelReady=38; break;
                        }
                        case 38:{
                        gameMatrix[1][1]=22; gameMatrix[5][1]=22; gameMatrix[3][3]=20; gameMatrix[5][3]=21; gameMatrix[1][7]=23; gameMatrix[5][7]=20;
                        cow2Sprite.setPosition(220,520); horse1Sprite.setPosition(765, 150); cow1Sprite.setPosition(180, 140); sheep1Sprite.setPosition(345, 545);
                        pig1Sprite.setPosition(380, 330); pig2Sprite.setPosition(760, 520);
                        putSheep1=true; putHorse1=true; putPig2=true; putCow1=true; putCow2=true; putPig1=true; makeLevelReady=39; break;
                        }
                        case 39:{
                        gameMatrix[1][3]=21; gameMatrix[3][5]=21; gameMatrix[3][3]=20; gameMatrix[5][7]=20; gameMatrix[3][1]=22; gameMatrix[5][1]=22;
                        cow2Sprite.setPosition(220,530); sheep2Sprite.setPosition(545, 345); cow1Sprite.setPosition(180, 325); sheep1Sprite.setPosition(345, 166);
                        pig1Sprite.setPosition(380, 330); pig2Sprite.setPosition(760, 520);
                        putSheep1=true; putSheep2=true; putPig2=true; putCow1=true; putCow2=true; putPig1=true; makeLevelReady=40; break;
                        }
                        case 40:{
                        gameMatrix[1][1]=20; gameMatrix[1][3]=20; gameMatrix[3][3]=23; gameMatrix[5][1]=23; gameMatrix[5][5]=21; gameMatrix[3][7]=22;
                        horse2Sprite.setPosition(200,515); cow1Sprite.setPosition(740, 320); horse1Sprite.setPosition(385, 335); sheep1Sprite.setPosition(530, 545);
                        pig1Sprite.setPosition(180, 145); pig2Sprite.setPosition(380, 145);
                        putSheep1=true; putHorse2=true; putPig2=true; putHorse1=true; putCow1=true; putPig1=true; makeLevelReady=41; break;
                        }
                        case 41:{
                        gameMatrix[1][3]=21; gameMatrix[1][5]=21; gameMatrix[5][7]=22; gameMatrix[1][7]=22; gameMatrix[3][3]=20; gameMatrix[5][5]=23;
                        cow2Sprite.setPosition(775,525); cow1Sprite.setPosition(740, 135); horse1Sprite.setPosition(385, 335); sheep1Sprite.setPosition(345, 166);
                        pig1Sprite.setPosition(560, 530); sheep2Sprite.setPosition(545, 166);
                        putSheep1=true; putCow2=true; putSheep2=true; putHorse1=true; putCow1=true; putPig1=true; makeLevelReady=42; break;
                        }
                        case 42:{
                        gameMatrix[3][1]=20; gameMatrix[5][1]=20; gameMatrix[5][5]=22; gameMatrix[3][5]=23; gameMatrix[1][7]=23;  gameMatrix[5][7]=21;
                        horse2Sprite.setPosition(765,140); cow1Sprite.setPosition(555, 515); horse1Sprite.setPosition(570, 335); sheep1Sprite.setPosition(730, 545);
                        pig1Sprite.setPosition(190, 320); pig2Sprite.setPosition(190, 525);
                        putSheep1=true; putPig2=true; putHorse2=true; putHorse1=true; putCow1=true; putPig1=true; makeLevelReady=43; break;
                        }
                        case 43:{
                        gameMatrix[1][1]=22; gameMatrix[5][5]=22; gameMatrix[1][3]=20; gameMatrix[1][7]=20; gameMatrix[3][5]=21; gameMatrix[3][7]=21;
                        sheep2Sprite.setPosition(730,345); cow1Sprite.setPosition(175, 135); cow2Sprite.setPosition(610, 530); sheep1Sprite.setPosition(535, 345);
                        pig1Sprite.setPosition(390, 145); pig2Sprite.setPosition(755, 145);
                        putSheep1=true; putPig2=true; putSheep2=true; putCow2=true; putCow1=true; putPig1=true; makeLevelReady=44; break;
                        }
                        case 44:{
                        gameMatrix[1][3]=21; gameMatrix[3][1]=21; gameMatrix[3][3]=20; gameMatrix[5][3]=23; gameMatrix[1][5]=22; gameMatrix[5][7]=23;
                        sheep2Sprite.setPosition(345,166); cow1Sprite.setPosition(550, 135); horse2Sprite.setPosition(770, 520); sheep1Sprite.setPosition(166, 345);
                        horse1Sprite.setPosition(385, 525); pig1Sprite.setPosition(380, 335);
                        putSheep1=true; putHorse2=true; putSheep2=true; putHorse1=true; putCow1=true; putPig1=true; makeLevelReady=45; break;
                        }
                        case 45:{
                        gameMatrix[5][1]=20; gameMatrix[1][7]=20; gameMatrix[3][3]=21; gameMatrix[3][5]=22; gameMatrix[3][7]=22; gameMatrix[5][7]=21;
                        sheep2Sprite.setPosition(732,535); cow1Sprite.setPosition(558, 320); cow2Sprite.setPosition(785, 330); sheep1Sprite.setPosition(345, 345);
                        pig2Sprite.setPosition(748, 150); pig1Sprite.setPosition(190, 530);
                        putSheep1=true; putCow2=true; putSheep2=true; putPig2=true; putCow1=true; putPig1=true; makeLevelReady=46; break;
                        }
                        case 46:{
                        gameMatrix[3][3]=23; gameMatrix[3][5]=23; gameMatrix[5][3]=21; gameMatrix[5][7]=21; gameMatrix[1][1]=22; gameMatrix[3][7]=22;
                        sheep2Sprite.setPosition(732,535); cow1Sprite.setPosition(165, 140); cow2Sprite.setPosition(785, 330); sheep1Sprite.setPosition(345, 545);
                        horse2Sprite.setPosition(580, 335); horse1Sprite.setPosition(380, 335);
                        putSheep1=true; putCow2=true; putSheep2=true; putHorse2=true; putCow1=true; putHorse1=true; makeLevelReady=47; break;
                        }
                        case 47:{
                        gameMatrix[3][1]=20; gameMatrix[5][5]=20; gameMatrix[1][3]=21; gameMatrix[3][3]=21; gameMatrix[3][5]=23; gameMatrix[1][7]=23;
                        sheep2Sprite.setPosition(345,345); pig1Sprite.setPosition(185, 140); pig2Sprite.setPosition(570, 525); sheep1Sprite.setPosition(345, 166);
                        horse2Sprite.setPosition(767, 145); horse1Sprite.setPosition(565, 335);
                        putSheep1=true; putPig2=true; putSheep2=true; putHorse2=true; putPig1=true; putHorse1=true; makeLevelReady=48; break;
                        }
                        case 48:{
                        gameMatrix[1][1]=20; gameMatrix[3][5]=20; gameMatrix[3][1]=22; gameMatrix[3][3]=22; gameMatrix[5][1]=23; gameMatrix[5][5]=23, gameMatrix[3][7]=21;
                        cow2Sprite.setPosition(415,335); pig1Sprite.setPosition(185, 150); pig2Sprite.setPosition(565, 335); cow1Sprite.setPosition(180, 315);
                        horse2Sprite.setPosition(580, 523); horse1Sprite.setPosition(205, 523); sheep1Sprite.setPosition(725, 354);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putPig2=true; makeLevelReady=49; break;
                        }
                        case 49:{
                        gameMatrix[1][1]=23; gameMatrix[1][3]=23; gameMatrix[1][5]=21; gameMatrix[3][5]=21; gameMatrix[5][1]=22; gameMatrix[3][3]=22, gameMatrix[5][5]=20;
                        sheep2Sprite.setPosition(545,345); pig1Sprite.setPosition(565, 530); cow2Sprite.setPosition(405, 335); cow1Sprite.setPosition(180, 520);
                        horse2Sprite.setPosition(205, 148); horse1Sprite.setPosition(390, 150); sheep1Sprite.setPosition(535, 166);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putCow2=true; makeLevelReady=50; break;
                        }
                        case 50:{
                        gameMatrix[1][1]=21; gameMatrix[1][5]=21; gameMatrix[3][1]=20; gameMatrix[5][5]=20; gameMatrix[3][3]=23; gameMatrix[3][5]=23, gameMatrix[3][7]=22;
                        sheep2Sprite.setPosition(540,160); pig1Sprite.setPosition(190, 330); pig2Sprite.setPosition(568, 525); cow1Sprite.setPosition(740, 520);
                        horse2Sprite.setPosition(580, 330); horse1Sprite.setPosition(385, 330); sheep1Sprite.setPosition(160, 160);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putPig2=true; makeLevelReady=51; break;
                        }
                        case 51:{
                        gameMatrix[5][1]=20; gameMatrix[5][3]=20; gameMatrix[3][5]=21; gameMatrix[5][5]=21; gameMatrix[3][7]=23; gameMatrix[5][7]=23, gameMatrix[1][3]=22;
                        sheep2Sprite.setPosition(540,545); pig1Sprite.setPosition(200, 530); pig2Sprite.setPosition(380, 530); cow1Sprite.setPosition(365, 140);
                        horse2Sprite.setPosition(765, 530); horse1Sprite.setPosition(765, 325); sheep1Sprite.setPosition(545, 345);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putPig2=true; makeLevelReady=52; break;
                        }
                        case 52:{
                        gameMatrix[1][3]=21; gameMatrix[1][5]=21; gameMatrix[1][1]=23; gameMatrix[3][1]=23; gameMatrix[5][1]=22; gameMatrix[5][5]=22, gameMatrix[5][7]=20;
                        sheep2Sprite.setPosition(540,166); pig1Sprite.setPosition(755, 525); cow2Sprite.setPosition(590, 530); cow1Sprite.setPosition(175, 520);
                        horse2Sprite.setPosition(200, 340); horse1Sprite.setPosition(200, 150); sheep1Sprite.setPosition(345, 166);
                        putSheep1=true; putSheep2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putCow2=true; makeLevelReady=53; break;
                        }
                        case 53:{
                        gameMatrix[1][1]=22; gameMatrix[1][3]=22; gameMatrix[1][5]=20; gameMatrix[1][7]=20; gameMatrix[3][1]=21; gameMatrix[3][3]=21, gameMatrix[3][7]=23;
                        sheep2Sprite.setPosition(166,345); pig1Sprite.setPosition(748, 155); cow2Sprite.setPosition(410, 145); cow1Sprite.setPosition(175, 140);
                        pig2Sprite.setPosition(560, 155); horse1Sprite.setPosition(760, 340); sheep1Sprite.setPosition(355, 355);
                        putSheep1=true; putSheep2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putCow2=true; makeLevelReady=54; break;
                        }
                        case 54:{
                        gameMatrix[1][1]=21; gameMatrix[3][3]=21; gameMatrix[5][1]=23; gameMatrix[5][3]=23; gameMatrix[5][5]=20; gameMatrix[1][7]=20, gameMatrix[3][7]=22;
                        sheep2Sprite.setPosition(345,345); pig1Sprite.setPosition(560, 525); horse2Sprite.setPosition(390, 525); cow1Sprite.setPosition(740, 325);
                        pig2Sprite.setPosition(750, 145); horse1Sprite.setPosition(200, 525); sheep1Sprite.setPosition(165, 160);
                        putSheep1=true; putSheep2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putHorse2=true; makeLevelReady=55; break;
                        }
                        case 55:{
                        gameMatrix[1][1]=20; gameMatrix[3][1]=20; gameMatrix[3][5]=23; gameMatrix[1][7]=23; gameMatrix[5][7]=21; gameMatrix[5][1]=22, gameMatrix[5][5]=22;
                        cow2Sprite.setPosition(590,525); pig1Sprite.setPosition(183, 150); horse2Sprite.setPosition(765, 147); cow1Sprite.setPosition(175, 510);
                        pig2Sprite.setPosition(183, 330); horse1Sprite.setPosition(573, 345); sheep1Sprite.setPosition(730, 545);
                        putSheep1=true; putCow2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putHorse2=true; makeLevelReady=56; break;
                        }
                        case 56:{
                        gameMatrix[3][3]=21; gameMatrix[3][5]=21; gameMatrix[3][7]=23; gameMatrix[1][1]=22; gameMatrix[5][1]=22; gameMatrix[5][3]=20, gameMatrix[5][7]=20;
                        cow2Sprite.setPosition(213,525); pig1Sprite.setPosition(373, 528); sheep2Sprite.setPosition(545, 345); cow1Sprite.setPosition(185, 138);
                        pig2Sprite.setPosition(755, 528); horse1Sprite.setPosition(760, 335); sheep1Sprite.setPosition(345, 345);
                        putSheep1=true; putCow2=true; putPig1=true; putPig2=true; putCow1=true; putHorse1=true; putSheep2=true; makeLevelReady=57; break;
                        }
                        case 57:{
                        gameMatrix[5][1]=21; gameMatrix[5][3]=21; gameMatrix[5][5]=23; gameMatrix[5][7]=22; gameMatrix[1][7]=22; gameMatrix[3][1]=23, gameMatrix[1][5]=20;
                        cow2Sprite.setPosition(780,515); pig1Sprite.setPosition(560, 150);  sheep2Sprite.setPosition(345, 545); cow1Sprite.setPosition(740, 138);
                        horse2Sprite.setPosition(578, 515); horse1Sprite.setPosition(200, 335);  sheep1Sprite.setPosition(166, 545);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; makeLevelReady=58; break;
                        }
                        case 58:{
                        gameMatrix[1][1]=23; gameMatrix[3][1]=23; gameMatrix[5][1]=21; gameMatrix[5][3]=21; gameMatrix[5][7]=20;  gameMatrix[3][7]=20; gameMatrix[1][7]=22; gameMatrix[3][3]=22;
                        cow2Sprite.setPosition(410,340); pig1Sprite.setPosition(750, 530); sheep2Sprite.setPosition(340, 545); cow1Sprite.setPosition(735, 130);
                        horse2Sprite.setPosition(200, 329); horse1Sprite.setPosition(200, 155); sheep1Sprite.setPosition(166, 545); pig2Sprite.setPosition(750, 335);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; putPig2=true; makeLevelReady=59; break;
                        }
                        case 59:{
                        gameMatrix[1][1]=20; gameMatrix[5][1]=20; gameMatrix[1][3]=21; gameMatrix[1][7]=21; gameMatrix[3][3]=22; gameMatrix[5][7]=22; gameMatrix[5][3]=23; gameMatrix[5][5]=23;
                        cow2Sprite.setPosition(780,525); pig1Sprite.setPosition(188, 155); sheep2Sprite.setPosition(720, 166); cow1Sprite.setPosition(365, 325);
                        horse2Sprite.setPosition(585, 530); horse1Sprite.setPosition(385, 530); sheep1Sprite.setPosition(345, 166); pig2Sprite.setPosition(183, 525);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; putPig2=true; makeLevelReady=60; break;
                        }
                        case 60:{
                        gameMatrix[1][7]=20; gameMatrix[5][7]=20; gameMatrix[5][1]=21; gameMatrix[5][3]=21; gameMatrix[3][3]=22;  gameMatrix[3][1]=22; gameMatrix[1][1]=23; gameMatrix[3][5]=23;
                        cow2Sprite.setPosition(215,335); pig1Sprite.setPosition(750, 155); sheep2Sprite.setPosition(345, 545); cow1Sprite.setPosition(365, 325);
                        horse2Sprite.setPosition(585, 335); horse1Sprite.setPosition(200, 150); sheep1Sprite.setPosition(166, 545); pig2Sprite.setPosition(750, 525);
                        putSheep1=true; putCow2=true; putPig1=true; putHorse2=true; putCow1=true; putHorse1=true; putSheep2=true; putPig2=true; gameFinished=true; break;
                        }
                         default: break;
                        }
                        for(i=0;i<=6;i++) for(j=0;j<=8;j++) copyMatrix[i][j]=gameMatrix[i][j];
                        window.clear();
                        rectangleCorner4.setFillColor(Color(0, 196, 33, 200));   
                            rectangleCorner3.setFillColor(Color(0, 196, 33, 200));
                            rectangleCorner2.setFillColor(Color(0, 196, 33, 200));
                            rectangleCorner1.setFillColor(Color(0, 196, 33, 200));
                            rectangleL2.setFillColor(Color(0, 196, 33, 200));
                            rectangleL3.setFillColor(Color(0, 196, 33, 200));
                            rectangleL4.setFillColor(Color(0, 196, 33, 200));
                            rectangleL5.setFillColor(Color(0, 196, 33, 200));
                            window.draw(backgroundSprite);
                            window.draw(backSprite);
                            window.draw(boardSprite);
                            if(putCow1==true) window.draw(cow1Sprite);
                            if(putCow2==true) window.draw(cow2Sprite);
                            if(putHorse1==true) window.draw(horse1Sprite);
                            if(putHorse2==true) window.draw(horse2Sprite);
                            if(putSheep1==true) window.draw(sheep1Sprite);
                            if(putPig1==true) window.draw(pig1Sprite);
                            if(putPig2==true) window.draw(pig2Sprite);
                            if(putSheep2==true) window.draw(sheep2Sprite);
                            horizontalFence2Sprite.setScale(0.8, 1);
                            horizontalFence2Sprite.setPosition(105, 850);
                            horizontalFence3Sprite.setScale(0.8, 1);
                            horizontalFence3Sprite.setPosition(30, 950);
                            verticalFence2Sprite.setScale(0.8, 1);
                            verticalFence3Sprite.setScale(0.8, 1);
                            verticalFence2Sprite.setPosition(403, 827);
                            verticalFence3Sprite.setPosition(520, 780);
                            cornerFence1Sprite.setScale(0.4, 0.8);
                            cornerFence1Sprite.setPosition(690, 905);
                            cornerFence2Sprite.setScale(0.6, 0.4);
                            cornerFence2Sprite.setPosition(860, 775);
                            cornerFence3Sprite.setScale(0.4, 0.6);
                            cornerFence3Sprite.setPosition(1005, 920);
                            cornerFence4Sprite.setScale(0.6, 0.45);
                            cornerFence4Sprite.setPosition(860, 1060);
                            window.draw(rectangleL2);
                            window.draw(rectangleL3);
                            window.draw(rectangleL4);
                            window.draw(rectangleL5);
                            window.draw(rectangleCorner1);
                            window.draw(rectangleCorner2);
                            window.draw(rectangleCorner3);
                            window.draw(rectangleCorner4);
                            window.draw(cornerFence1Sprite);
                            window.draw(cornerFence2Sprite);
                            window.draw(cornerFence3Sprite);
                            window.draw(cornerFence4Sprite);
                            window.draw(verticalFence3Sprite);
                            window.draw(verticalFence2Sprite);
                            window.draw(horizontalFence2Sprite);
                            window.draw(horizontalFence3Sprite);
                            window.draw(resetGame);
                            window.display();

                    }
                     
                }
                if (pg == 2)
                {
                    if(resetGame.getGlobalBounds().contains(mousePosF))
                    {
                        buttonSound.play();
                        initZoneMatrix(zoneMatrix);
                        for(i=0;i<=9;i++){
                            fences[0][i]=0;
                            fences[1][i]=0;
                            fences[2][i]=0;
                        }
                        isMarked=false;
                        column = 0;
                        moves=0;
                        counter[0]=0;
                        counter[1]=0;
                        counter[2]=0;
                        for(i=0;i<=8;i++) OK[i]=0;
                        for(i=0;i<=6;i++)
                               for(j=0;j<=8;j++) {gameMatrix[i][j]=copyMatrix[i][j];}

                            rectangleCorner4.setFillColor(Color(0, 196, 33, 200));   
                            rectangleCorner3.setFillColor(Color(0, 196, 33, 200));
                            rectangleCorner2.setFillColor(Color(0, 196, 33, 200));
                            rectangleCorner1.setFillColor(Color(0, 196, 33, 200));
                            rectangleL2.setFillColor(Color(0, 196, 33, 200));
                            rectangleL3.setFillColor(Color(0, 196, 33, 200));
                            rectangleL4.setFillColor(Color(0, 196, 33, 200));
                            rectangleL5.setFillColor(Color(0, 196, 33, 200));
                            window.clear();
                            window.draw(backgroundSprite);
                            window.draw(backSprite);
                            window.draw(boardSprite);
                            if(putCow1==true) window.draw(cow1Sprite);
                            if(putCow2==true) window.draw(cow2Sprite);
                            if(putHorse1==true) window.draw(horse1Sprite);
                            if(putHorse2==true) window.draw(horse2Sprite);
                            if(putSheep1==true) window.draw(sheep1Sprite);
                            if(putPig1==true) window.draw(pig1Sprite);
                            if(putPig2==true) window.draw(pig2Sprite);
                            if(putSheep2==true) window.draw(sheep2Sprite);
                            horizontalFence2Sprite.setScale(0.8, 1);
                            horizontalFence2Sprite.setPosition(105, 850);
                            horizontalFence3Sprite.setScale(0.8, 1);
                            horizontalFence3Sprite.setPosition(30, 950);
                            verticalFence2Sprite.setScale(0.8, 1);
                            verticalFence3Sprite.setScale(0.8, 1);
                            verticalFence2Sprite.setPosition(403, 827);
                            verticalFence3Sprite.setPosition(520, 780);
                            cornerFence1Sprite.setScale(0.4, 0.8);
                            cornerFence1Sprite.setPosition(690, 905);
                            cornerFence2Sprite.setScale(0.6, 0.4);
                            cornerFence2Sprite.setPosition(860, 775);
                            cornerFence3Sprite.setScale(0.4, 0.6);
                            cornerFence3Sprite.setPosition(1005, 920);
                            cornerFence4Sprite.setScale(0.6, 0.45);
                            cornerFence4Sprite.setPosition(860, 1060);
                            window.draw(rectangleL2);
                            window.draw(rectangleL3);
                            window.draw(rectangleL4);
                            window.draw(rectangleL5);
                            window.draw(rectangleCorner1);
                            window.draw(rectangleCorner2);
                            window.draw(rectangleCorner3);
                            window.draw(rectangleCorner4);
                            window.draw(cornerFence1Sprite);
                            window.draw(cornerFence2Sprite);
                            window.draw(cornerFence3Sprite);
                            window.draw(cornerFence4Sprite);
                            window.draw(verticalFence3Sprite);
                            window.draw(verticalFence2Sprite);
                            window.draw(horizontalFence2Sprite);
                            window.draw(horizontalFence3Sprite);
                            window.draw(resetGame);
                            window.display();
                            break;
                        
                        
                    }
                    if (mousePos.x > 230 && mousePos.x < 974 && mousePos.y > 112 && mousePos.y < 678)
                        int z = 1;
                    else
                    {
                        if (isMarked == true && isPut == false)
                        {
                            for (int i = 1; i <= 8; i++)
                                OK[i] = 0;
                            isMarked = false;
                            window.clear();
                            window.draw(backgroundSprite);
                            window.draw(backSprite);
                            window.draw(boardSprite);
                            if(putCow1==true) window.draw(cow1Sprite);
                            if(putCow2==true) window.draw(cow2Sprite);
                            if(putHorse1==true) window.draw(horse1Sprite);
                            if(putHorse2==true) window.draw(horse2Sprite);
                            if(putSheep1==true) window.draw(sheep1Sprite);
                            if(putPig1==true) window.draw(pig1Sprite);
                            if(putPig2==true) window.draw(pig2Sprite);
                            if(putSheep2==true) window.draw(sheep2Sprite);
                            horizontalFence2Sprite.setScale(0.8, 1);
                            horizontalFence2Sprite.setPosition(105, 850);
                            horizontalFence3Sprite.setScale(0.8, 1);
                            horizontalFence3Sprite.setPosition(30, 950);
                            verticalFence2Sprite.setScale(0.8, 1);
                            verticalFence3Sprite.setScale(0.8, 1);
                            verticalFence2Sprite.setPosition(403, 827);
                            verticalFence3Sprite.setPosition(520, 780);
                            cornerFence1Sprite.setScale(0.4, 0.8);
                            cornerFence1Sprite.setPosition(690, 905);
                            cornerFence2Sprite.setScale(0.6, 0.4);
                            cornerFence2Sprite.setPosition(860, 775);
                            cornerFence3Sprite.setScale(0.4, 0.6);
                            cornerFence3Sprite.setPosition(1005, 920);
                            cornerFence4Sprite.setScale(0.6, 0.45);
                            cornerFence4Sprite.setPosition(860, 1060);
                            window.draw(rectangleL2);
                            window.draw(rectangleL3);
                            window.draw(rectangleL4);
                            window.draw(rectangleL5);
                            window.draw(rectangleCorner1);
                            window.draw(rectangleCorner2);
                            window.draw(rectangleCorner3);
                            window.draw(rectangleCorner4);
                            window.draw(cornerFence1Sprite);
                            window.draw(cornerFence2Sprite);
                            window.draw(cornerFence3Sprite);
                            window.draw(cornerFence4Sprite);
                            window.draw(verticalFence3Sprite);
                            window.draw(verticalFence2Sprite);
                            window.draw(horizontalFence2Sprite);
                            window.draw(horizontalFence3Sprite);
                            window.draw(resetGame);
                            i = 0;
                            while (fences[0][i] != 0)
                            {
                                switch (fences[0][i])
                                {
                                case 1:
                                {
                                    horizontalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                    horizontalFence2Sprite.setScale(1.6, 2);
                                    window.draw(horizontalFence2Sprite);
                                    break;
                                }
                                case 2:
                                {
                                    horizontalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                    horizontalFence3Sprite.setScale(1.6, 2);
                                    window.draw(horizontalFence3Sprite);
                                    break;
                                }
                                case 3:
                                {
                                    verticalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                    verticalFence2Sprite.setScale(1.6, 1.6);
                                    window.draw(verticalFence2Sprite);
                                    break;
                                }
                                case 4:
                                {
                                    verticalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                    verticalFence3Sprite.setScale(1.6, 1.6);
                                    window.draw(verticalFence3Sprite);
                                    break;
                                }
                                case 5:
                            {
                                cornerFence1Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence1Sprite.setScale(1.55, 1.6);
                                window.draw(cornerFence1Sprite);
                                break;
                            }
                            case 6:
                            {
                               cornerFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence2Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence2Sprite);
                                break; 
                            }
                            case 7:
                            {
                                cornerFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence3Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence3Sprite);
                                break;
                            }
                            case 8:
                            {
                                cornerFence4Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence4Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence4Sprite);
                                break;
                            }
                                }

                                i++;
                            }
                            window.display();
                        }
                    }

                    if (mousePos.x >= 52 && mousePos.x <= 366 && mousePos.y >= 824 && mousePos.y <= 916)
                    {
                        OK[1] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 54 && mousePos.x <= 366 && mousePos.y >= 944 && mousePos.y <= 1032)
                    {
                        OK[2] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 414 && mousePos.x <= 504 && mousePos.y >= 784 && mousePos.y <= 1166)
                    {
                        OK[3] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 546 && mousePos.x <= 634 && mousePos.y >= 784 && mousePos.y <= 1164)
                    {
                        OK[4] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 704 && mousePos.x <= 814 && mousePos.y >= 830 && mousePos.y <= 1022)
                    {
                        OK[5] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 854 && mousePos.x <= 980 && mousePos.y >= 770 && mousePos.y <= 892)
                    {
                        OK[6] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 1004 && mousePos.x <= 1134 && mousePos.y >= 912 && mousePos.y <= 1044)
                    {
                        OK[7] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (mousePos.x >= 856 && mousePos.x <= 984 && mousePos.y >= 1056 && mousePos.y <= 1186)
                    {
                        OK[8] = 1;
                        isMarked = true;
                        isPut = false;
                        break;
                    }
                    if (isMarked == true)
                    {
                        int xWindow, yWindow, iMatrix, jMatrix;
                        getNewFenceCoordinates(xWindow, yWindow, mousePos.x, mousePos.y);
                        getMatrixCoordinates(iMatrix, jMatrix, xWindow, yWindow);
                        int z = 1;
                        while (OK[z] == 0 && z <= 7)
                            z++;
                        if (z == 1 || z == 2)
                            i = 0;
                          else if (z == 3 || z == 4)
                            i = 1;
                            else i=2;
                        if (isInBounds(iMatrix, jMatrix, z) != 0 && counter[i] == 0)
                        {

                            if (OK[1] == 1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleL2.setFillColor(Color(205, 53, 53, 220));
                                rectangleL3.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 1;
                                fences[1][column] = xWindow;
                                fences[2][column] = yWindow;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                horizontalFence2Sprite.setPosition(xWindow, yWindow);
                                horizontalFence2Sprite.setScale(1.6, 2);
                                isPut = true;
                                isMarked = false;
                                counter[0]++;
                                moves++;
                            }
                            else if (OK[2] == 1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleL2.setFillColor(Color(205, 53, 53, 220));
                                rectangleL3.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 2;
                                fences[1][column] = xWindow - 42;
                                fences[2][column] = yWindow - 44;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                horizontalFence3Sprite.setPosition(xWindow - 42, yWindow - 44);
                                horizontalFence3Sprite.setScale(1.6, 2);
                                isPut = true;
                                isMarked = false;
                                counter[0]++;
                                moves++;
                            }
                            else if (OK[3] == 1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleL4.setFillColor(Color(205, 53, 53, 220));
                                rectangleL5.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 3;
                                fences[1][column] = xWindow - 98;
                                fences[2][column] = yWindow;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                verticalFence2Sprite.setPosition(xWindow - 98, yWindow);
                                verticalFence2Sprite.setScale(1.6, 1.6);
                                isPut = true;
                                isMarked = false;
                                counter[1]++;
                                moves++;
                            }
                            else if (OK[4] == 1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleL4.setFillColor(Color(205, 53, 53, 220));
                                rectangleL5.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 4;
                                fences[1][column] = xWindow - 125;
                                fences[2][column] = yWindow;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                verticalFence3Sprite.setPosition(xWindow - 125, yWindow);
                                verticalFence3Sprite.setScale(1.6, 1.6);
                                isPut = true;
                                isMarked = false;
                                counter[1]++;
                                moves++;
                            }
                            else if(OK[5]==1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleCorner1.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner2.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner3.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner4.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 5;
                                fences[1][column] = xWindow-80;
                                fences[2][column] = yWindow-10;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                cornerFence1Sprite.setPosition(xWindow-80, yWindow-10);
                                cornerFence1Sprite.setScale(1.55, 1.6);
                                isPut = true;
                                isMarked = false;
                                counter[2]++;
                                moves++;
                            }
                            else if(OK[6]==1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleCorner1.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner2.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner3.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner4.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 6;
                                fences[1][column] = xWindow-57;
                                fences[2][column] = yWindow-18;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                cornerFence2Sprite.setPosition(xWindow-57, yWindow-18);
                                cornerFence2Sprite.setScale(1.55, 1.55);
                                isPut = true;
                                isMarked = false;
                                counter[2]++;
                                moves++;
                            }
                            else if(OK[7]==1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleCorner1.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner2.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner3.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner4.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 7;
                                fences[1][column] = xWindow-53;
                                fences[2][column] = yWindow;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                cornerFence3Sprite.setPosition(xWindow-53, yWindow);
                                cornerFence3Sprite.setScale(1.55, 1.55);
                                isPut = true;
                                isMarked = false;
                                counter[2]++;
                                moves++;
                            }
                            else if(OK[8]==1)
                            {
                                window.clear();
                                window.draw(backgroundSprite);
                                window.draw(backSprite);
                                window.draw(boardSprite);
                                rectangleCorner1.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner2.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner3.setFillColor(Color(205, 53, 53, 220));
                                rectangleCorner4.setFillColor(Color(205, 53, 53, 220));
                                fences[0][column] = 8;
                                fences[1][column] = xWindow-220;
                                fences[2][column] = yWindow+25;
                                column++;
                                placeFence(iMatrix, jMatrix, gameMatrix, z);
                                markAllZones();
                                cornerFence4Sprite.setPosition(xWindow-220, yWindow+25);
                                cornerFence4Sprite.setScale(1.55, 1.55);
                                isPut = true;
                                isMarked = false;
                                counter[2]++;
                                moves++;
                            }
                            for (int i = 1; i <= 8; i++)
                                OK[i] = 0;
                            window.draw(cornerFence4Sprite);
                            window.draw(rectangleL2);
                            window.draw(rectangleL3);
                            window.draw(rectangleL4);
                            window.draw(rectangleL5);
                            window.draw(rectangleCorner1);
                            window.draw(rectangleCorner2);
                            window.draw(rectangleCorner3);
                            window.draw(rectangleCorner4);
                            horizontalFence2Sprite.setPosition(105, 850);
                            horizontalFence2Sprite.setScale(0.8, 1);
                            window.draw(horizontalFence2Sprite);
                            horizontalFence3Sprite.setPosition(30, 950);
                            horizontalFence3Sprite.setScale(0.8, 1);
                            window.draw(horizontalFence3Sprite);
                            verticalFence2Sprite.setScale(0.8, 1);
                            verticalFence3Sprite.setScale(0.8, 1);
                            verticalFence2Sprite.setPosition(403, 827);
                            verticalFence3Sprite.setPosition(520, 780);
                            window.draw(verticalFence3Sprite);
                            window.draw(verticalFence2Sprite);
                            cornerFence1Sprite.setScale(0.4, 0.8);
                            cornerFence1Sprite.setPosition(690, 905);
                            cornerFence2Sprite.setScale(0.6, 0.4);
                            cornerFence2Sprite.setPosition(860, 775);
                            cornerFence3Sprite.setScale(0.4, 0.6);
                            cornerFence3Sprite.setPosition(1005, 920);
                            cornerFence4Sprite.setScale(0.6, 0.45);
                            cornerFence4Sprite.setPosition(860, 1060);
                            window.draw(cornerFence1Sprite);
                            window.draw(cornerFence2Sprite);
                            window.draw(cornerFence3Sprite);
                            window.draw(cornerFence4Sprite);
                            window.draw(resetGame);
                            if(putCow1==true) window.draw(cow1Sprite);
                            if(putCow2==true) window.draw(cow2Sprite);
                            if(putHorse1==true) window.draw(horse1Sprite);
                            if(putHorse2==true) window.draw(horse2Sprite);
                            if(putSheep1==true) window.draw(sheep1Sprite);
                            if(putPig1==true) window.draw(pig1Sprite);
                            if(putPig2==true) window.draw(pig2Sprite);
                            if(putSheep2==true) window.draw(sheep2Sprite);
                            i = 0;
                                while (fences[0][i] != 0)
                                {
                                    switch (fences[0][i])
                                    {
                                    case 1:
                                    {
                                        horizontalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                        horizontalFence2Sprite.setScale(1.6, 2);
                                        window.draw(horizontalFence2Sprite);
                                        break;
                                    }
                                    case 2:
                                    {
                                        horizontalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                        horizontalFence3Sprite.setScale(1.6, 2);
                                        window.draw(horizontalFence3Sprite);
                                        break;
                                    }
                                    case 3:
                                    {
                                        verticalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                        verticalFence2Sprite.setScale(1.6, 1.6);
                                        window.draw(verticalFence2Sprite);
                                        break;
                                    }
                                    case 4:
                                    {
                                        verticalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                        verticalFence3Sprite.setScale(1.6, 1.6);
                                        window.draw(verticalFence3Sprite);
                                        break;
                                    }
                                    case 5:
                            {
                                cornerFence1Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence1Sprite.setScale(1.55, 1.6);
                                window.draw(cornerFence1Sprite);
                                break;
                            }
                            case 6:
                            {
                               cornerFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence2Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence2Sprite);
                                break; 
                            }
                            case 7:
                            {
                                cornerFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence3Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence3Sprite);
                                break;
                            }
                            case 8:
                            {
                                cornerFence4Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence4Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence4Sprite);
                                break;
                            }
                                    }

                                    i++;
                                }
                           if(checkMatrix(gameMatrix, zoneMatrix, check)==0 && moves==3) 
                           {
                             pg=4;
                             lostSound.play();
                             rectangleEnd.setPosition(0,450);
                             rectangleEnd.setSize(Vector2f(1200, 175));
                             rectangleEnd.setFillColor(Color(22, 15, 238, 200));
                             window.draw(rectangleEnd);
                             window.draw(lose);
                             window.draw(backLevel2);
                             

                           }
                           if(checkMatrix(gameMatrix, zoneMatrix, check)==1)
                            {
                             isLevelAvailable[makeLevelReady]=1;
                             pg=3;
                             victorySound.play();
                             rectangleEnd.setPosition(0,450);
                             rectangleEnd.setSize(Vector2f(1200, 175));
                             rectangleEnd.setFillColor(Color(22, 15, 238, 200));
                             window.draw(rectangleEnd);
                             if(gameFinished==false){
                             window.draw(win);
                             window.draw(backLevel1);
                             window.draw(nextLevel);
                                        }
                              else {gameFinished=false;
                                  window.draw(win);
                                  window.draw(backLevel2);
                              }
                             
                           }
                    window.display();
                        
                        }
                        else
                        {
                            window.clear();
                            window.draw(backgroundSprite);
                            window.draw(backSprite);
                            window.draw(boardSprite);
                            cornerFence1Sprite.setScale(0.4, 0.8);
                            cornerFence1Sprite.setPosition(690, 905);
                            cornerFence2Sprite.setScale(0.6, 0.4);
                            cornerFence2Sprite.setPosition(860, 775);
                            cornerFence3Sprite.setScale(0.4, 0.6);
                            cornerFence3Sprite.setPosition(1005, 920);
                            cornerFence4Sprite.setScale(0.6, 0.45);
                            cornerFence4Sprite.setPosition(860, 1060);
                            verticalFence2Sprite.setScale(0.8, 1);
                            verticalFence3Sprite.setScale(0.8, 1);
                            verticalFence2Sprite.setPosition(403, 827);
                            verticalFence3Sprite.setPosition(520, 780);
                            horizontalFence3Sprite.setScale(0.8, 1);
                            horizontalFence2Sprite.setScale(0.8, 1);
                            horizontalFence3Sprite.setPosition(30, 950);
                            horizontalFence2Sprite.setPosition(105, 850);
                            if(putCow1==true) window.draw(cow1Sprite);
                            if(putCow2==true) window.draw(cow2Sprite);
                            if(putHorse1==true) window.draw(horse1Sprite);
                            if(putHorse2==true) window.draw(horse2Sprite);
                            if(putSheep1==true) window.draw(sheep1Sprite);
                            if(putPig1==true) window.draw(pig1Sprite);
                            if(putPig2==true) window.draw(pig2Sprite);
                            if(putSheep2==true) window.draw(sheep2Sprite);
                            window.draw(rectangleL2);
                            window.draw(rectangleL3);
                            window.draw(rectangleL4);
                            window.draw(rectangleL5);
                            window.draw(rectangleCorner1);
                            window.draw(rectangleCorner2);
                            window.draw(rectangleCorner3);
                            window.draw(rectangleCorner4);
                            window.draw(horizontalFence2Sprite);
                            window.draw(horizontalFence3Sprite);
                            window.draw(verticalFence2Sprite);
                            window.draw(verticalFence3Sprite);
                            window.draw(cornerFence1Sprite);
                            window.draw(cornerFence2Sprite);
                            window.draw(cornerFence3Sprite);
                            window.draw(cornerFence4Sprite);
                            window.draw(resetGame);
                            i = 0;
                            while (fences[0][i] != 0)
                            {
                                switch (fences[0][i])
                                    {
                                    case 1:
                                    {
                                        horizontalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                        horizontalFence2Sprite.setScale(1.6, 2);
                                        window.draw(horizontalFence2Sprite);
                                        break;
                                    }
                                    case 2:
                                    {
                                        horizontalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                        horizontalFence3Sprite.setScale(1.6, 2);
                                        window.draw(horizontalFence3Sprite);
                                        break;
                                    }
                                    case 3:
                                    {
                                        verticalFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                        verticalFence2Sprite.setScale(1.6, 1.6);
                                        window.draw(verticalFence2Sprite);
                                        break;
                                    }
                                    case 4:
                                    {
                                        verticalFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                        verticalFence3Sprite.setScale(1.6, 1.6);
                                        window.draw(verticalFence3Sprite);
                                        break;
                                    }
                                    case 5:
                            {
                                cornerFence1Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence1Sprite.setScale(1.55, 1.6);
                                window.draw(cornerFence1Sprite);
                                break;
                            }
                            case 6:
                            {
                               cornerFence2Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence2Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence2Sprite);
                                break; 
                            }
                            case 7:
                            {
                                cornerFence3Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence3Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence3Sprite);
                                break;
                            }
                            case 8:
                            {
                                cornerFence4Sprite.setPosition(fences[1][i], fences[2][i]);
                                cornerFence4Sprite.setScale(1.55, 1.55);
                                window.draw(cornerFence4Sprite);
                                break;
                            }
                                    }
                                i++;
                            }

                            window.display();
                            isPut = false;
                            isMarked = false;
                            cout << "pozitie incorecta" << endl;
                            for (int i = 1; i <= 8; i++)
                                OK[i] = 0;
                        }

                        cout << endl;
                        printMatrix(gameMatrix);
                        cout << endl;
                        printMatrix(zoneMatrix);
                        cout << endl;
                    }
                }

                if (exitButton.getGlobalBounds().contains(mousePosF))
                {
                    if (pg == 0)
                        window.close();
                }
                if (backSprite.getGlobalBounds().contains(mousePosF))
                {
                    if (pg != 0)
                    {
                        buttonSound.play();
                        moves=0;
                        for (i = 0; i <= 9; i++)
                        {
                            fences[0][i] = 0;
                            fences[1][i] = 0;
                            fences[2][i] = 0;
                        }
                        for (i = 1; i <= 8; i++)
                            OK[i] = 0;

                        for (i = 0; i <= 2; i++)
                            counter[i] = 0;
                        Vector2i centrare((VideoMode::getDesktopMode().width / 2) - 600, (VideoMode::getDesktopMode().height / 2) - 600);
                        window.create(VideoMode(w, h), "Smart Farmer");
                        window.setPosition(centrare);
                        window.draw(t1Sprite);
                        window.draw(exitButton);
                        window.draw(startText);
                        window.draw(descriptionText);
                        window.draw(settingsText);
                        window.display();
                        isPut = false;
                        isMarked = false;
                        putCow1=false, putCow2=false, putHorse1=false, putHorse2=false, putPig1=false, putSheep1=false, putPig2=false, putSheep2=false;
                        pg = 0;
                        column = 0;
                    }
                }
                if (startText.getGlobalBounds().contains(mousePosF))
                {
                    if (pg == 0)
                    {
                        buttonSound.play();
                        moves=0;
                        pg=1;
                        window.clear();
                        window.draw(levelBackgroundSprite);
                        Levels.setStyle(Text::Bold);
                        Levels.setString("Select a level");
                        Levels.setFillColor(Color::Blue);
                        Levels.setCharacterSize(120);
                        Levels.setPosition(260.0, 20.0);
                        if(isLevelAvailable[1]==1) L1.setFillColor(Color(44, 110, 255, 255));   else L1.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[2]==1) L2.setFillColor(Color(44, 110, 255, 255));   else L2.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[3]==1) L3.setFillColor(Color(44, 110, 255, 255));   else L3.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[4]==1) L4.setFillColor(Color(44, 110, 255, 255));   else L4.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[5]==1) L5.setFillColor(Color(44, 110, 255, 255));   else L5.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[6]==1) L6.setFillColor(Color(44, 110, 255, 255));   else L6.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[7]==1) L7.setFillColor(Color(44, 110, 255, 255));   else L7.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[8]==1) L8.setFillColor(Color(44, 110, 255, 255));   else L8.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[9]==1) L9.setFillColor(Color(44, 110, 255, 255));   else L9.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[10]==1) L10.setFillColor(Color(44, 110, 255, 255));   else L10.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[11]==1) L11.setFillColor(Color(44, 110, 255, 255));   else L11.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[12]==1) L12.setFillColor(Color(44, 110, 255, 255));   else L12.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[13]==1) L13.setFillColor(Color(44, 110, 255, 255));   else L13.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[14]==1) L14.setFillColor(Color(44, 110, 255, 255));   else L14.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[15]==1) L15.setFillColor(Color(44, 110, 255, 255));   else L15.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[16]==1) L16.setFillColor(Color(44, 110, 255, 255));   else L16.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[17]==1) L17.setFillColor(Color(44, 110, 255, 255));   else L17.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[18]==1) L18.setFillColor(Color(44, 110, 255, 255));   else L18.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[19]==1) L19.setFillColor(Color(44, 110, 255, 255));   else L19.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[20]==1) L20.setFillColor(Color(44, 110, 255, 255));   else L20.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[21]==1) L21.setFillColor(Color(44, 110, 255, 255));   else L21.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[22]==1) L22.setFillColor(Color(44, 110, 255, 255));   else L22.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[23]==1) L23.setFillColor(Color(44, 110, 255, 255));   else L23.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[24]==1) L24.setFillColor(Color(44, 110, 255, 255));   else L24.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[25]==1) L25.setFillColor(Color(44, 110, 255, 255));   else L25.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[26]==1) L26.setFillColor(Color(44, 110, 255, 255));   else L26.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[27]==1) L27.setFillColor(Color(44, 110, 255, 255));   else L27.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[28]==1) L28.setFillColor(Color(44, 110, 255, 255));   else L28.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[29]==1) L29.setFillColor(Color(44, 110, 255, 255));   else L29.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[30]==1) L30.setFillColor(Color(44, 110, 255, 255));   else L30.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[31]==1) L31.setFillColor(Color(44, 110, 255, 255));   else L31.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[32]==1) L32.setFillColor(Color(44, 110, 255, 255));   else L32.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[33]==1) L33.setFillColor(Color(44, 110, 255, 255));   else L33.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[34]==1) L34.setFillColor(Color(44, 110, 255, 255));   else L34.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[35]==1) L35.setFillColor(Color(44, 110, 255, 255));   else L35.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[36]==1) L36.setFillColor(Color(44, 110, 255, 255));   else L36.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[37]==1) L37.setFillColor(Color(44, 110, 255, 255));   else L37.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[38]==1) L38.setFillColor(Color(44, 110, 255, 255));   else L38.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[39]==1) L39.setFillColor(Color(44, 110, 255, 255));   else L39.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[40]==1) L40.setFillColor(Color(44, 110, 255, 255));   else L40.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[41]==1) L41.setFillColor(Color(44, 110, 255, 255));   else L41.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[42]==1) L42.setFillColor(Color(44, 110, 255, 255));   else L42.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[43]==1) L43.setFillColor(Color(44, 110, 255, 255));   else L43.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[44]==1) L44.setFillColor(Color(44, 110, 255, 255));   else L44.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[45]==1) L45.setFillColor(Color(44, 110, 255, 255));   else L45.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[46]==1) L46.setFillColor(Color(44, 110, 255, 255));   else L46.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[47]==1) L47.setFillColor(Color(44, 110, 255, 255));   else L47.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[48]==1) L48.setFillColor(Color(44, 110, 255, 255));   else L48.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[49]==1) L49.setFillColor(Color(44, 110, 255, 255));   else L49.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[50]==1) L50.setFillColor(Color(44, 110, 255, 255));   else L50.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[51]==1) L51.setFillColor(Color(44, 110, 255, 255));   else L51.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[52]==1) L52.setFillColor(Color(44, 110, 255, 255));   else L52.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[53]==1) L53.setFillColor(Color(44, 110, 255, 255));   else L53.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[54]==1) L54.setFillColor(Color(44, 110, 255, 255));   else L54.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[55]==1) L55.setFillColor(Color(44, 110, 255, 255));   else L55.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[56]==1) L56.setFillColor(Color(44, 110, 255, 255));   else L56.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[57]==1) L57.setFillColor(Color(44, 110, 255, 255));   else L57.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[58]==1) L58.setFillColor(Color(44, 110, 255, 255));   else L58.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[59]==1) L59.setFillColor(Color(44, 110, 255, 255));   else L59.setFillColor(Color(0, 0, 0, 127));
                        if(isLevelAvailable[60]==1) L60.setFillColor(Color(44, 110, 255, 255));   else L60.setFillColor(Color(0, 0, 0, 127));
                        window.draw(Levels);
                        window.draw(L1); window.draw(L2); window.draw(L3); window.draw(L4); window.draw(L5); window.draw(L6); 
                        window.draw(L7); window.draw(L8); window.draw(L9); window.draw(L10); window.draw(L11); window.draw(L12); 
                        window.draw(L13); window.draw(L14); window.draw(L15); window.draw(L16); window.draw(L17); window.draw(L18); 
                        window.draw(L19); window.draw(L20); window.draw(L21); window.draw(L22); window.draw(L23); window.draw(L24); 
                        window.draw(L25); window.draw(L26); window.draw(L27); window.draw(L28); window.draw(L29); window.draw(L30);
                        window.draw(L31); window.draw(L32); window.draw(L33); window.draw(L34); window.draw(L35); window.draw(L36); 
                        window.draw(L37); window.draw(L38); window.draw(L39); window.draw(L40); window.draw(L41); window.draw(L42); 
                        window.draw(L43); window.draw(L44); window.draw(L45); window.draw(L46); window.draw(L47); window.draw(L48);
                        window.draw(L49); window.draw(L50); window.draw(L51); window.draw(L52); window.draw(L53); window.draw(L54);   
                        window.draw(L55); window.draw(L56); window.draw(L57); window.draw(L58); window.draw(L59); window.draw(L60); 
                        backSprite.setScale(0.2, 0.2);
                        backSprite.setPosition(10.0, 10.0);
                        window.draw(backSprite);
                        window.display();

                        
                    }
                }
                if (descriptionText.getGlobalBounds().contains(mousePosF) && pg==0)
                {
                    buttonSound.play();
                    pg=-1;
                    window.clear();
                    window.draw(infoImageSprite);
                    window.draw(backSprite);
                    window.draw(rules1);  window.draw(rules2); window.draw(rules3); window.draw(rules4); window.draw(rules5);
                    window.draw(rules6); window.draw(rules7); 
                    window.draw(difficulty1); window.draw(difficulty2); window.draw(difficulty3); window.draw(difficulty4);
                    window.draw(difficulty5); window.draw(difficulty6); window.draw(difficulty7);
                    window.draw(difficulty);
                    window.draw(credits);
                    window.display();

                }

                if (settingsText.getGlobalBounds().contains(mousePosF) && pg==0)
                {
                     buttonSound.play();
                    pg=-2;
                    window.clear();
                    window.draw(t1Sprite);
                    window.draw(backSprite);
                    window.draw(SettingSound);
                    window.draw(settingNoSound);
                    window.draw(settingSong1);
                    window.draw(settingSong2);
                    window.draw(newGame);
                    window.display();
                }

                break;
            }
            default:
                break;
            }
        }
    }

    return 0;
}

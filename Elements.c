#include "include.h"

Pieces *piece = NULL;
Zappeurs zappeur[5];
Lasers laser[6];
Missiles missile[6];
Jetons jeton;

static const unsigned char vehiculesX[2][12] = {
{4,1,2,0,2,1,6,1,5,1,5},
{16,22,21,7,11,22,20,20,26,17,16,16}
};
static const unsigned char vehiculesY[2][12] = {
{1,1,1,1,1,1,1,1,1,1,1,1},
{14,17,18,14,15,12,19,16,13,17,14,14}
};

static const unsigned char coinPatterns[19][45] = { // patterns des pieces
{0xE3, 0x8E, 0x38, 0xE3, 0x80, 0x71, 0xC7, 0x1C, 0x71, 0xC0, 0x38, 0xE3, 0x8E, 0x38, 0xE0, 0x71, 0xC7, 0x1C, 0x71, 0xC0, 0xE3, 0x8E, 0x38, 0xE3, 0x80},
{0x7F, 0x00, 0x1F, 0xC0, 0xFF, 0x80, 0x3F, 0xE0, 0x7F, 0x00, 0x1F, 0xC0, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x3F, 0x80, 0x00},
{0x73, 0x3A, 0x4E, 0xA8, 0x84, 0x93, 0x50, 0xA8, 0x84, 0x92, 0xDE, 0xA8, 0x84, 0x92, 0x42, 0x00, 0x73, 0x3A, 0x5E, 0xA8},
{0x63, 0x00, 0x94, 0x80, 0x94, 0x00, 0xE4, 0x00, 0x84, 0x80, 0x83, 0x00},
{0x7F, 0x00, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x1F, 0xC0},
{0xAA, 0xA0, 0x55, 0x40, 0xAA, 0xA0, 0x55, 0x40, 0xAA, 0xA0, 0x55, 0x40, 0xAA, 0xA0, 0x55, 0x40, 0xAA, 0xA0},
{0x7F, 0xFC, 0xFF, 0xFE, 0x7F, 0xFC},
{0x6C, 0xFE, 0xFE, 0x7C, 0x38, 0x10},
{0xF0, 0x03, 0xC0, 0x00, 0x08, 0x04, 0x20, 0x00, 0x04, 0x08, 0x10, 0x00, 0x02, 0x10, 0x08, 0x00, 0x01, 0xE0, 0x07, 0x80},
{0xFC, 0x00, 0x1F, 0x80, 0x03, 0x80, 0xE0, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x03, 0x80, 0xE0, 0x00, 0xFC, 0x00, 0x1F, 0x80},
{0xF3, 0x39, 0xC9, 0x94, 0xA5, 0x29, 0xE7, 0xBD, 0xEF, 0x94, 0xA9, 0x42, 0xF4, 0xA5, 0x22},
{0x03, 0x00, 0x03, 0x00, 0x18, 0x60, 0x18, 0x60, 0xC3, 0x0C, 0xC3, 0x0C, 0x18, 0x60, 0x18, 0x60, 0x03, 0x00, 0x03, 0x00},
{0x02, 0x00, 0x03, 0x00, 0x03, 0x80, 0xFF, 0xC0, 0xFF, 0xE0, 0xFF, 0xC0, 0x03, 0x80, 0x03, 0x00, 0x02, 0x00},
{0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x3E, 0x0F, 0x80, 0x00, 0x07, 0xC0, 0x00, 0x7C, 0x00, 0xF8, 0x00, 0x00, 0x03, 0xE0},
{0xAA, 0xAA, 0xAA, 0xA0, 0xAA, 0xAA, 0xAA, 0xA0},
{0xF8, 0x7C, 0x3E, 0x1F, 0x3E, 0x7C, 0xF8},
{0xFF, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x1F, 0xE0},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFE}
};

static const unsigned short laserPattern[17][8] = { // patterns des lasers

    {70,70,0,0,0,0,50,130}, // niveau facile
    {0,0,70,70,0,0,50,130},
    {0,0,0,0,70,70,50,130},
    {0,0,70,70,70,70,50,130},
    {70,70,70,70,0,0,50,130},
    {0,0,70,0,0,0,50,130},
    {0,0,0,70,0,0,50,130},
    {70,0,0,0,0,0,50,288},
    {0,0,0,0,0,70,50,288},

    {70,70,140,140,210,210,50,260}, // niveau moyen
    {70,999,140,140,999,70,50,190},
    {70,0,0,0,0,70,50,130},
    {70,110,150,190,230,270,30,300},
    {70,70,170,170,120,120,50,220},

    {120,210,310,260,160,70,30,340}, // niveau difficile
    {70,0,0,0,0,70,50,164},
    {70,0,0,0,0,70,50,312},
};

int pieceCount, lastPiece;
int chosenPattern;
int vehiculeHitbox;

int RTC_getTicks()
{
     return (*SysCall)(0, 0, 0, 0, 0x3B);
}

void setFPS(int fpsNB) // régulateur de fps
{
    static unsigned int fps, fpsCount;

    do fps = RTC_getTicks();
    while(fps < fpsCount+fpsNB);

    fpsCount = RTC_getTicks();
}

void dsInfo(int distance, int money)
{
    char text[6];

    sprintf(text,"%dM",distance);
    PrintMini(0,0,text,0);
    sprintf(text,"%d$",money);
    PrintMini(0,6,text,0);
}

void changerHitbox(int newHitbox)
{
    vehiculeHitbox = newHitbox;
}

void initAll()
{
    initPieces();
    initZappeurs();
    initLasers();
    initMissiles();
    initBox();
    initJeton();
}

void initPieces()
{
    pieceCount = 0;
    lastPiece = 0;
}
int genererPieces()
{
    int largeur, hauteur, tabIndex, pattern, allocSize, maxY, randY, c;

    pieceCount = 0;
    maxY = 0;
    pattern = rand() %19;

    switch(pattern) // on choisit aleatoirement un pattern et on definit ses proprietes
    {
        case 0: largeur = 5, hauteur = 5, allocSize = 90; break;
        case 1: largeur = 4, hauteur = 6, allocSize = 69; break;
        case 2: largeur = 4, hauteur = 5, allocSize = 65; break;
        case 3: largeur = 2, hauteur = 6, allocSize = 21; break;
        case 4: largeur = 4, hauteur = 9, allocSize = 69; break;
        case 5: largeur = 2, hauteur = 9, allocSize = 50; break;
        case 6: largeur = 2, hauteur = 3, allocSize = 41; break;
        case 7: largeur = 1, hauteur = 6, allocSize = 27; break;
        case 8: largeur = 4, hauteur = 5, allocSize = 25; break;
        case 9: largeur = 4, hauteur = 9, allocSize = 49; break;
        case 10: largeur = 3, hauteur = 5, allocSize = 63; break;
        case 11: largeur = 2, hauteur = 10, allocSize = 36; break;
        case 12: largeur = 2, hauteur = 9, allocSize = 43; break;
        case 13: largeur = 5, hauteur = 4, allocSize = 35; break;
        case 14: largeur = 4, hauteur = 2, allocSize = 28; break;
        case 15: largeur = 1, hauteur = 7, allocSize = 35; break;
        case 16: largeur = 5, hauteur = 3, allocSize = 32; break;
        case 17: largeur = 1, hauteur = 5, allocSize = 40; break;
        case 18: largeur = 5, hauteur = 5, allocSize = 34; break;
    }
        piece = (Pieces*)realloc(piece, allocSize * sizeof(Pieces)); // on realloue la memoire

        randY = rand()%(61-6*hauteur);

        for(tabIndex = 0; tabIndex < (hauteur*largeur); tabIndex++) // algorithme qui definit les coordonnes de chaque piece
            for(c=0; c<8; c++)
                if((coinPatterns[pattern][tabIndex]>> (7-c)) & 1)
                {
                    piece[pieceCount].x = 130 + (tabIndex%largeur)*(6*8) + c*6;
                    piece[pieceCount].y = tabIndex/largeur * 6 + randY;
                    if (130 + (tabIndex%largeur)*(6*8) + c*6 > maxY) // on identifie la piece la plus eloignee de l'ecran
                    {
                        maxY = 130 + (tabIndex%largeur)*(6*8) + c*6;
                        lastPiece = pieceCount;
                    }
                    piece[pieceCount].active = ((sauvegarde->gadgetEquipe[0] == 7 || sauvegarde->gadgetEquipe[1] == 7)&&rand()%2==1?2:1);
                    piece[pieceCount].attract = 0;
                    pieceCount++;
                }

    return largeur * 48;
}
void afficherPieces()
{
    int w;

    for (w=0; w < pieceCount; w++)
        if (piece[w].active != 0) // si la piece existe
        {
            ML_bmp_8_and_cl(moneyAlpha,piece[w].x,piece[w].y); // masque alpha de la pièce
            ML_bmp_8_or_cl(piece[w].active==1?money:money2,piece[w].x,piece[w].y); // si la piece n'a pas ete deja prise par barry, on l'affiche
        }
}
int deplacerPieces(int yBarry, float vitesseDecalage, int vehicule)
{
    int w;

    if (lastPiece == 0) return 0;
    if (piece[lastPiece].x<-6) // si la dernière piece est en dehors de l'ecran
    return 0;
    else for (w=0; w < pieceCount; w++) // on decale toutes les pieces vers la gauche
    {
        if (piece[w].attract) // si la piece est attiree
        {
            if (piece[w].x+2 > 8 + (int)((vehiculesX[1][vehiculeHitbox] - vehiculesX[0][vehiculeHitbox])/2)) piece[w].x-=piece[w].attract;
            else if (piece[w].x+2 < 8 + (int)((vehiculesX[1][vehiculeHitbox] - vehiculesX[0][vehiculeHitbox])/2)) piece[w].x+=piece[w].attract;

            if (piece[w].y+2 > yBarry + (int)((vehiculesY[1][vehiculeHitbox] - vehiculesY[0][vehiculeHitbox])/2)) piece[w].y-=piece[w].attract;
            else if (piece[w].y+2 < yBarry + (int)((vehiculesY[1][vehiculeHitbox] - vehiculesY[0][vehiculeHitbox])/2)) piece[w].y+=piece[w].attract;

        }
        else piece[w].x-= vitesseDecalage;
        if (vehicule <= 0 || vehicule > 6)
        {
            if ((piece[w].x <= 8 + 6 + vehiculesX[1][vehiculeHitbox] && piece[w].x >= 8 - 6 + vehiculesX[0][vehiculeHitbox] - 5 && piece[w].y <= 6 + yBarry + vehiculesY[1][vehiculeHitbox] && piece[w].y >= yBarry + vehiculesY[0][vehiculeHitbox] - 5 - 6) && (sauvegarde->gadgetEquipe[0] == 10 || sauvegarde->gadgetEquipe[1] == 10)) // si on a l'atout aimant a piece et que la piece est assez proche
            piece[w].attract = 1;
        }
        else
        {
            if ((piece[w].x <= 8 + 10 + vehiculesX[1][vehiculeHitbox] && piece[w].x >= 8 - 10 + vehiculesX[0][vehiculeHitbox] - 5 && piece[w].y <= 10 + yBarry + vehiculesY[1][vehiculeHitbox] && piece[w].y >= yBarry + vehiculesY[0][vehiculeHitbox] - 5 - 10) && sauvegarde->ameliorationAimant[vehicule-1] == 2) // si on a l'atout aimant a piece et que la piece est assez proche
            piece[w].attract = 2;
        }
    }
    return 1;
}
void collisionPieces(int yBarry, int xBarry, int *nbPieces, int vehicule)
{

    int w;

    for (w=0; w < pieceCount; w++)
        if (piece[w].x <= xBarry + vehiculesX[1][vehiculeHitbox] && piece[w].x >= xBarry + vehiculesX[0][vehiculeHitbox] - 5 && piece[w].y <= yBarry + vehiculesY[1][vehiculeHitbox] && piece[w].y >= yBarry + vehiculesY[0][vehiculeHitbox] - 5) // si la piece touche barry
        {
            if (piece[w].active != 0)
            {
                if (vehicule > 6) sauvegarde->statMort[4] += 1;
                *nbPieces += (piece[w].active==1?1:2)*(sauvegarde->doublePiece==1?2:1); // on gagne de l'argent
            }
            piece[w].active = 0; // on indique que la piece a etee touchee
        }
}

void initZappeurs()
{
    int w;

    for (w = 0; w < 5; w++)
        zappeur[w].exists = 0;
}
int genererZappeurs(int vehicule) // calcule aleatoirement la position des zappeurs afin que barry puisse passer au dessus ou en dessous
{
    int w, x, type, change, taille;

    for (w = 0; zappeur[w].exists && w < 5; w++); // on cherche la prochaine place du tableau disponible

    if (w == 5) return 0; // si il y a trop de zappeurs a l'ecran on arrete

    zappeur[w].exists = 1; // on indique qu'il existe
    zappeur[w].active = 1;
    zappeur[w].pos = rand()%2;
    zappeur[w].x1 = 128;

    if (zappeur[w].pos == 0) // zappeur horizontal
    {
        if (vehicule <= 0 || vehicule == 2 || vehicule == 3) zappeur[w].y1 = rand()%51;
        else zappeur[w].y1 = rand()%2==1?rand()%31:rand()%(51+1-39)+39;
        zappeur[w].y2 = zappeur[w].y1;
        zappeur[w].x2 = zappeur[w].x1 + rand()%(vehicule <= 0 || vehicule == 2 || vehicule == 3?40:15)+10;
    }
    else if (vehicule <= 0 || vehicule == 2 || vehicule == 3)
    {
        type = rand()%3;
        for (x=0;x<6&&!missile[x].exists;x++);
        if (x!=6) type = 2;

        if (type == 0)
        {
            taille = rand()%(36+1-18)+(vehicule==3?14:18);
            zappeur[w].y1 = rand()%(53-taille+1-17)+17;
            zappeur[w].y2 = zappeur[w].y1 + taille;
        }
        else if (type == 1)
        {
            taille = rand()%(36+1-18)+(vehicule==3?14:18);
            zappeur[w].y2 = rand()%(36+1-taille)+taille;
            zappeur[w].y1 = zappeur[w].y2 - taille;
        }
        else if (type == 2)
        {
            taille = rand()%(19+1-18)+(vehicule==3?14:18);
            zappeur[w].y1 = rand()%(36-taille+1-17)+17;
            zappeur[w].y2 = zappeur[w].y1 + taille;
        }

        //zappeur[w].x2 = zappeur[w].x1 + (zappeur[w].pos==1?0:zappeur[w].y2-zappeur[w].y1);
        zappeur[w].x2 = zappeur[w].x1;
    }
    else
    {
        type = rand()%2;

        if (type == 0)
        {
            taille = 14;
            zappeur[w].y1 = 39;
            zappeur[w].y2 = zappeur[w].y1 + taille;
        }
        else if (type == 1)
        {
            taille = rand()%(30+1-18)+18;
            zappeur[w].y2 = rand()%(30+1-taille)+taille;
            zappeur[w].y1 = zappeur[w].y2 - taille;
        }

        //zappeur[w].x2 = zappeur[w].x1 + (zappeur[w].pos==1?0:zappeur[w].y2-zappeur[w].y1);
        zappeur[w].x2 = zappeur[w].x1;

        /*if (rand()%2 && zappeur[w].pos==2)
        {
            change = zappeur[w].x2;
            zappeur[w].x2 = zappeur[w].x1;
            zappeur[w].x1 = change;
        }*/
    }

    return abs((int)(zappeur[w].x2-zappeur[w].x1));
}
int deplacerZappeurs(float vitesseDecalage)
{
    int w, zapNumber = 0;

    for (w = 0; w < 5; w++)
        if (zappeur[w].exists) // si le zappeur existe
        {
            if (zappeur[w].x1 < -10 && zappeur[w].x2 < -10) // si il est en dehors de l'ecran on indique qu'il n'existe plus
            {
                zappeur[w].exists = 0;
                continue;
            }
            zappeur[w].x1-=vitesseDecalage; // sinon on le deplace
            zappeur[w].x2-=vitesseDecalage;
            zapNumber++;

            if ((sauvegarde->gadgetEquipe[0] == 12 || sauvegarde->gadgetEquipe[1] == 12) && (zappeur[w].x1 == 60 || zappeur[w].x1 == 61) && rand()%4==1) zappeur[w].active = 0;
        }

    return zapNumber;
}
void afficherZappeurs(int lvl)
{
    int v, w;

    for (v = 0; v < 5; v++)
    {
        if (zappeur[v].exists) // si le zappeur existe
        {
            if (zappeur[v].active)
            {
                for (w=0; w < 4; w++)
                    if (zappeur[v].pos)
                    {
                        ML_line(zappeur[v].x1+w+3,zappeur[v].y1+5,zappeur[v].x2+w+3,zappeur[v].y2+4,ML_WHITE); // affichage du "zap"
                        if (lvl%6<3) ML_line(zappeur[v].x1+w+3,zappeur[v].y1+5,zappeur[v].x2+w+3,zappeur[v].y2+4,ML_BLACK);
                    }
                    else
                    {
                        ML_line(zappeur[v].x1+3,zappeur[v].y1+w+3,zappeur[v].x2+3,zappeur[v].y2+w+3,ML_WHITE);
                        if (lvl%6<3) ML_line(zappeur[v].x1+3,zappeur[v].y1+w+3,zappeur[v].x2+3,zappeur[v].y2+w+3,ML_BLACK);
                    }
            }

            ML_bmp_16_and_cl(zapAlpha,zappeur[v].x1,zappeur[v].y1); // masque alpha des zappeurs
            ML_bmp_16_and_cl(zapAlpha,zappeur[v].x2,zappeur[v].y2);
            ML_bmp_16_or_cl(zap,zappeur[v].x1,zappeur[v].y1); // affichage des extremites des zappeurs
            ML_bmp_16_or_cl(zap,zappeur[v].x2,zappeur[v].y2);
        }
    }
}

int collisionZappeurs(int yBarry, int xBarry, int vehicule)
{
    int w, x2, y2, x1, y1;

    for (w = 0; w < 5; w++)
    {
        if (zappeur[w].exists)
        {
            /*if (zappeur[w].pos != 2)
            {*/
                if (zappeur[w].x1 + 2 <= xBarry + vehiculesX[1][vehiculeHitbox] && zappeur[w].x2 + 2 >= xBarry + vehiculesX[0][vehiculeHitbox] - 7 && zappeur[w].y1 + 2 <= yBarry + vehiculesY[1][vehiculeHitbox] && zappeur[w].y2 + 2 >= yBarry + vehiculesY[0][vehiculeHitbox] - 7 && zappeur[w].active && zappeur[w].exists)
                {
                    if (vehicule == 0)
                    {
                        sauvegarde->statMort[0] += 1;
                        if (sauvegarde->succes[5] != -1) sauvegarde->succes[5] = 0;
                    }
                    return 1;
                }
            /*}
            else
            {
                y1 = yBarry + vehiculesY[0][vehiculeHitbox];
                x1 = zappeur[w].x1+(y1-zappeur[w].y1);
                y2 = yBarry + vehiculesY[1][vehiculeHitbox];
                x2 = zappeur[w].x1+(y2-zappeur[w].y1);

                if ((x1 < xBarry + vehiculesX[1][vehiculeHitbox] && x1 > xBarry + vehiculesX[0][vehiculeHitbox] && y1 < zappeur[w].y2 + 2 && y1 > zappeur[w].y1 + 2)/*||(x2 < xBarry + vehiculesX[1][vehiculeHitbox] && x2 > xBarry + vehiculesX[0][vehiculeHitbox] && y2 < zappeur[w].y2 + 2 && y2 > zappeur[w].y1 + 2))
                {
                    if (vehicule == 0)
                    {
                        sauvegarde->statMort[0] += 1;
                        if (sauvegarde->succes[5] != -1) sauvegarde->succes[5] = 0;
                    }
                    return 1;
                }*/
        }
    }
    return 0;
}

void initLasers()
{
    int w;

    for (w = 0; w < 6; w++)
    {
        laser[w].exists = 0;
        laser[w].phase = 0;
        laser[w].y = 11*w;
    }
}
void genererLasers(int level)
{
    int w, a = 9;

    if (level >= 3 && level < 7) a += 5;
    else if (level >= 7) a += 8;

    chosenPattern = rand()%a; // on genere aleatoirement le paterne du laser selon la difficulte

    for (w = 0; w < 6; w++)
        if (laserPattern[chosenPattern][w]) laser[w].exists = 1;
}
int activerLasers(int *temps) //  on active les lasers en fonction de leurs paternes
{
    int a = 0, w;

    if (chosenPattern == 7)
    {
        if (laser[0].exists) a = 1;
        if (*temps < 8 || *temps > 280) laser[0].phase = 0;
        else if (*temps%96<48) laser[0].phase = 1;
        else laser[0].phase = 2;
        if (*temps < 280 && *temps > 8) laser[0].y+=0.20;
        if (*temps > 296)
        {
            *temps = -1;
            initLasers();
        }
    }
    else if (chosenPattern == 8)
    {
        if (laser[5].exists) a = 1;
        if (*temps < 8 || *temps > 280) laser[5].phase = 0;
        else if (*temps%96<48) laser[5].phase = 1;
        else laser[5].phase = 2;
        if (*temps < 280 && *temps > 8) laser[5].y-=0.20;
        if (*temps > 296)
        {
            *temps = -1;
            initLasers();
        }
    }
    else if (chosenPattern == 15)
    {
        if (laser[0].exists && laser[5].exists) a = 1;
        if (*temps < 16 || *temps > 156) laser[0].phase = 0, laser[5].phase = 0;
        else if (*temps%160>64) laser[0].phase = 1, laser[5].phase = 1;
        else laser[0].phase = 2, laser[5].phase = 2;
        if (*temps < 156 && *temps > 32) laser[0].y+=0.20, laser[5].y-=0.20;
        if (*temps > 172)
        {
            *temps = -1;
            initLasers();
        }
    }
    else if (chosenPattern == 16)
    {
        if (laser[0].exists && laser[5].exists) a = 1;
        if (*temps < 16 || *temps > 304) laser[0].phase = 0, laser[5].phase = 0;
        else if (*temps%160>64) laser[0].phase = 1, laser[5].phase = 1;
        else laser[0].phase = 2, laser[5].phase = 2;
        if (*temps < 156 && *temps > 32) laser[0].y+=0.20, laser[5].y-=0.20;
        else if (*temps > 180 && *temps < 304) laser[0].y-=0.20, laser[5].y+=0.20;
        if (*temps > 312)
        {
            *temps = -1;
            initLasers();
        }
    }
    else
    {
        for (w = 0; w < 6; w++)
        {
            if (laser[w].exists) // on definit les phases de chaque laser
            {
                if (laserPattern[chosenPattern][w]-50 == *temps) laser[w].phase = 1;
                else if (laserPattern[chosenPattern][w] == *temps) laser[w].phase = 2;
                else if (laserPattern[chosenPattern][7]-7 == *temps || laserPattern[chosenPattern][w]+laserPattern[chosenPattern][6] == *temps) laser[w].phase = 0;
                a = 1;
            }
        }

        if (laserPattern[chosenPattern][7]+8 == *temps) // si le paterne est finit on arrete
        {
            *temps = -1;
            initLasers();
        }
    }

    return a;
}
void afficherLasers(temps)
{
    int w;

    for (w = 0; w < 6; w++) // dessin des rayons
    {
        if (laser[w].phase == 1) // si le laser est a sa premiere phase
        {
            ML_rectangle(6,laser[w].y+1,121,laser[w].y+5,1,ML_BLACK,ML_CHECKER); // animation du laser
            if (temps%6<3) ML_rectangle(6,laser[w].y+1,121,laser[w].y+5,1,ML_TRANSPARENT,ML_XOR);
        }
        else if (laser[w].phase == 2) // si le laser est a sa deuxieme phase
        {
            ML_rectangle(6,laser[w].y+1,121,laser[w].y+5,1,ML_CHECKER,ML_BLACK); // animation du laser
            if (temps%6<3) ML_rectangle(6,laser[w].y+1,121,laser[w].y+5,1,ML_XOR,ML_TRANSPARENT);
        }
    }

    for (w = 0; w < 6; w++) // dessin des canons
    {
        if (laser[w].exists) // si le laser existe
        {
            ML_bmp_8_and_cl(laserGaucheAlpha,temps<=8?-8+temps:temps>laserPattern[chosenPattern][7]?laserPattern[chosenPattern][7]-temps:0,laser[w].y); // masque alpha des canons du laser
            ML_bmp_8_and_cl(laserDroiteAlpha,temps<=8?128-temps:temps>laserPattern[chosenPattern][7]?temps-laserPattern[chosenPattern][7]+120:120,laser[w].y);
            ML_bmp_8_or_cl(laserGauche,temps<=8?-8+temps:temps>laserPattern[chosenPattern][7]?laserPattern[chosenPattern][7]-temps:0,laser[w].y); // affichage des canons, animation d'entrée et de sortie
            ML_bmp_8_or_cl(laserDroite,temps<=8?128-temps:temps>laserPattern[chosenPattern][7]?temps-laserPattern[chosenPattern][7]+120:120,laser[w].y);
        }
    }
}
int collisionLasers(int yBarry, int vehicule)
{
    int w;

    for (w = 0; w < 6; w++)
        if (laser[w].exists && laser[w].phase == 2 && laser[w].y + 1 <= yBarry + vehiculesY[1][vehiculeHitbox] && laser[w].y + 1 >= yBarry + vehiculesY[0][vehiculeHitbox] - 5)
        {
            if (vehicule == 0)
            {
                sauvegarde->statMort[1] += 1;
                if (sauvegarde->succes[5] != -1) sauvegarde->succes[5] = 0;
            }
            return 1; // si la piece touche barry
        }


    return 0;
}

void initMissiles()
{
    int w;

    for (w = 0; w <6; w++)
        missile[w].exists = 0;
}
int genererMissiles(int yBarry)
{
    int w;

    for (w = 0; missile[w].exists && w < 6; w++); // on regarde s'il y a pas trop de missiles a l'ecran
    if (w == 6) return 0;

    missile[w].exists = 1;
    missile[w].temps = 0;
    missile[w].x = 128;
    missile[w].y = yBarry; // on definit le missile aux coordonees de barry
}
int deplacerMissiles(int yBarry)
{
    int w, a = 0, phase1, phase2;

    phase1 = 40;
    phase2 = 60;

    for (w = 0; w < 6; w++)
    {
        if (missile[w].exists == 0) continue;
        a = 1;
        missile[w].temps++;

        if (missile[w].temps < phase1) // si le missile est pas encore lance il suit barry
        {
            if (missile[w].y < yBarry) missile[w].y++;
            else if (missile[w].y > yBarry) missile[w].y--;
        }
        else if (missile[w].temps >= phase2) // sinon on le bloque et on le fait avancer
        {
            if (missile[w].exists == 1)
            {
                missile[w].x -= 4;

                if (missile[w].x < -16) // si le missile sort de l'ecran on le supprime
                    missile[w].exists = 0;

                if (missile[w].x == 60 && rand()%2==1 && (sauvegarde->gadgetEquipe[0] == 4 || sauvegarde->gadgetEquipe[1] == 4)) missile[w].exists = 2;
            }
            else
            {
                missile[w].x -= 3;
                missile[w].exists += 1;
                if (missile[w].exists > 10) missile[w].exists = 0;
            }
        }
    }

    return a;
}
void afficherMissiles(int lvl)
{
    int w, phase1, phase2;

    phase1 = 40;
    phase2 = 60;

    for (w = 0;w < 6; w++)
    {
        if (missile[w].exists == 1) // si le missile existe
        {
            if (missile[w].temps < phase1) // affichage du point d'exclamation
                ML_bmp_8_or(warning1,119,missile[w].y);
            else if (missile[w].temps >= phase1 && missile[w].temps < phase2) // clignotement du point d'exclamation
            {
                ML_bmp_8_or(warning1,119,missile[w].y);
                if (missile[w].temps%3) ML_rectangle(121,missile[w].y-1,124,missile[w].y+8,1,ML_XOR,ML_XOR);
            }
            else if (missile[w].temps >= phase2)
            {
                ML_bmp_16_and_cl(missile[w].temps%6<3?missile1Alpha:missile2Alpha,missile[w].x,missile[w].y); // masque alpha du missile
                ML_bmp_16_or_cl(missile[w].temps%6<3?missile1:missile2,missile[w].x,missile[w].y); // affichage et aimation du missile
                if (lvl%8<4) ML_rectangle(missile[w].x+1,missile[w].y+2,missile[w].x+8,missile[w].y+6,1,ML_BLACK,ML_BLACK);
            }
        }
        else if (missile[w].exists > 1)
        {
            ML_filled_circle(missile[w].x+7,missile[w].y+4,missile[w].exists,ML_WHITE);
            ML_circle(missile[w].x+7,missile[w].y+4,missile[w].exists,ML_BLACK);
        }
    }
}
int collisionMissiles(int yBarry, int xBarry, int vehicule)
{
    int w;

    for (w = 0; w < 6; w++)
    {
        if (missile[w].exists == 1 && missile[w].x <= xBarry + vehiculesX[1][vehiculeHitbox] && missile[w].x >= xBarry + vehiculesX[0][vehiculeHitbox] - 8 && missile[w].y <= yBarry + vehiculesY[1][vehiculeHitbox] && missile[w].y >= yBarry + vehiculesY[0][vehiculeHitbox] - 7) // si la piece touche barry
        {
            if (vehicule == 0)
            {
                sauvegarde->statMort[2] += 1;
                if (sauvegarde->succes[5] != -1) sauvegarde->succes[5] += 1;
            }
            return 1;
        }
    }

    return 0;
}

void initBox()
{
    boxPos = -13;
}
void genererBox()
{
    boxPos = 140;
}
int deplacerBox(float vitesseDecalage, int *isBox)
{
    if (boxPos <= -13)
    {
        *isBox = 0;
         return 0;
    }
    else
    {
        boxPos -= vitesseDecalage;
        return 1;
    }
}
void afficherBox()
{
    ML_rectangle(boxPos+1,7,boxPos+12,18,1,ML_WHITE,ML_WHITE);
    ML_bmp_or_cl(box,boxPos,6,14,14);
}
int collisionBox(int yBarry)
{
    if (boxPos <= 8 + vehiculesX[1][vehiculeHitbox] && boxPos >= 8 + vehiculesX[0][vehiculeHitbox] - 14 && 6 <= yBarry + vehiculesY[1][vehiculeHitbox] && 6 >= yBarry + vehiculesY[0][vehiculeHitbox] - 14)
    {
        boxPos = -13;
        return 1;
    }
    else
        return 0;
}

void initJeton()
{
    jeton.x = -20;
}
void genererJeton(int type)
{
    if (jeton.x == -20)
    {
        jeton.yMid = rand()%(26)+10;
        jeton.x = 128;
        jeton.first = type;
    }
}
int deplacerJeton(int yBarry, int *isJeton)
{
    static int yPos = 32;

    if (jeton.x > -20) // si le jeton est a l'ecran
    {
        jeton.x -= (jeton.first?0.7:1); // on deplace le jeton

        if (jeton.first == 1) // si le gadget jeton offert est equipe
        {
            if (yPos < yBarry) yPos++; // le gadget suit le joueur
            else if (yPos > yBarry) yPos--;
            jeton.y = yPos+2*sin(((float)(jeton.x)/4)); // oscillation du jeton
        }
        else if (sauvegarde->gadgetEquipe[0] == 8 || sauvegarde->gadgetEquipe[1] == 8) // si le gadget jeton magnetique est equipe
        {
            if (yPos < yBarry) yPos++; // le jeton suit le joueur
            else if (yPos > yBarry) yPos--;
            jeton.y = yPos+5*sin(((float)(jeton.x)/4)); // oscillation du jeton
        }
        else jeton.y = 10*sin(((float)(jeton.x)/4))+jeton.yMid; // sinon on fait juste osciller le jeton
    }
    else if (jeton.x < -20)
    {
        jeton.x++;
    }
    else return 0, *isJeton = 0;

    return 1;
}
void afficherJeton(int nbJetons)
{
    char jetonText[3];

    sprintf(jetonText,"+%d",nbJetons); // on convertit le nombre de jetons en texte

    ML_bmp_16_and_cl(jetonSPAlpha,jeton.x,jeton.y); // masque alpha du jeton
    ML_bmp_16_or_cl(jetonSP,jeton.x,jeton.y); // affichage du jeton

    if (jeton.x < -20) PrintXY(12+vehiculesX[1][vehiculeHitbox],jeton.yHit,jetonText,0); // si on a touche un jeton on affiche le nombre de jetons
}
int collisionJeton(int yBarry, int xBarry, int vehicule, int *totalJetons)
{
    if (jeton.x <= xBarry + vehiculesX[1][vehiculeHitbox] && jeton.x >= xBarry + vehiculesX[0][vehiculeHitbox] - 12 && jeton.y <= yBarry + vehiculesY[1][vehiculeHitbox] && jeton.y >= yBarry + vehiculesY[0][vehiculeHitbox] - 12) // si le jeton touche barry
    {
        *totalJetons += 1;
        if (vehicule > 6) sauvegarde->statMort[5] += 1;
        jeton.yHit = jeton.y; // on indique le point de collision
        jeton.x = -40;
    }
}

void collision(int *vehicule, int *collisionPhase, int *tempsLaser, float vehicDistance[6], int *tempsBox)
{
    reinitElements(&tempsLaser);

    if (*vehicule)
    {
        *tempsBox = 800;
        if (vehicDistance[*vehicule-1] > sauvegarde->statVehicule[*vehicule-1]) sauvegarde->statVehicule[*vehicule-1] = vehicDistance[*vehicule-1];
        if (vehicDistance[*vehicule-1] >= 800 && sauvegarde->succes[14] != -1 && *vehicule == 2) sauvegarde->succes[14] = -1, nouveauSucces = 1;
        *vehicule = 0;
        initValues();
    }
    else *vehicule = 9;

    *collisionPhase = 1;
}

void reinitElements(int *tempsLaser)
{
    int w;

    for (w = 0; w < 5; w++)
        zappeur[w].active = 0; // on desactive les zappeurs a l'ecran

    for (w = 0; w < 6; w++)
    {
        if (missile[w].exists) missile[w].exists = 2; // on desactive les missiles a l'ecran
        if (laser[w].exists && *tempsLaser < laserPattern[chosenPattern][7]-7) *tempsLaser = laserPattern[chosenPattern][7]-7; // on desactive les lasers a l'ecran
    }

    initBox();
}

void collisionAnim(int *collisionPhase)
{
    if (*collisionPhase == 1 || *collisionPhase == 2 || *collisionPhase == 5 || *collisionPhase == 6) ML_rectangle(0,0,127,63,1,ML_XOR,ML_XOR);

    *collisionPhase += 1;
    if (*collisionPhase == 7) *collisionPhase = 0;
}

int exitAnim()
{
    int u = 36;

    do
    {
        ML_rectangle(30,15,97,49,1,ML_BLACK,ML_WHITE);
        ML_rectangle(35,34,63,42,1,ML_BLACK,ML_TRANSPARENT);
        ML_rectangle(65,34,93,42,1,ML_BLACK,ML_TRANSPARENT);
        PrintMini(47,22,"QUITTER ?",0);
        PrintMini(41,36,"NON",0);
        PrintMini(73,36,"OUI",0);
        ML_rectangle(u,35,u+26,41,1,ML_XOR,ML_XOR);
        ML_display_vram();

        if (IsKeyDown(KEY_CTRL_LEFT) && u != 36) u = 36;
        if (IsKeyDown(KEY_CTRL_RIGHT) && u == 36) u = 66;
    }
    while (IsKeyUp(KEY_CTRL_EXE));

    if (u == 36) return 0;
    else return 1;
}

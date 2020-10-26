#ifndef _ELEMENTS
#define _ELEMENTS

static int SysCallCode[] = {0xD201422B,0x60F20000,0x80010070};
static int (*SysCall)(int R4, int R5, int R6, int R7, int FNo ) = (void*)&SysCallCode;

int RTC_getTicks();

typedef struct
{
    float x;
    int y, active, attract;
}Pieces;

typedef struct
{
    float x1, y1;
    float x2, y2;
    int exists, pos, active;
}Zappeurs;

typedef struct
{
    float y;
    int exists, phase;
}Lasers;

typedef struct
{
    int x, y;
    int exists, temps;
}Missiles;

typedef struct
{
    float x, y, yMid;
    float first, yHit;
}Jetons;

static float boxPos;

void dsInfo(int distance, int money);

void changerHitbox(int newHitbox);

void initAll();

void initPieces();
int genererPieces();
void afficherPieces();
int deplacerPieces(int yBarry, float vitesseDecalage, int vehicule);
void collisionPieces(int yBarry, int xBarry, int *nbPieces, int vehicule);

void initZappeurs();
int genererZappeurs(int vehicule);
int deplacerZappeurs(float vitesseDecalage);
void afficherZappeurs(int lvl);
int collisionZappeurs(int yBarry, int xBarry, int vehicule);

void initLasers();
void genererLasers(int level);
int activerLasers(int *temps);
void afficherLasers(int temps);
int collisionLasers(int yBarry, int vehicule);

void initMissiles();
int genererMissiles(int yBarry);
int deplacerMissiles(int yBarry);
void afficherMissiles(int lvl);
int collisionMissiles(int yBarry, int xBarry, int vehicule);

void initBox();
void genererBox();
int deplacerBox(float vitesseDecalage, int *isBox);
void afficherBox();
int collisionBox(int yBarry);

void initJeton();
void genererJeton(int type);
int deplacerJeton(int yBarry, int *isJeton);
void afficherJeton(int nbJetons);
int collisionJeton(int yBarry, int xBarry, int vehicule, int *totalJetons);

void collision(int *vehicule, int *collisionPhase, int *tempsLasers, float vehicDistance[6], int *tempsBox);
void collisionAnim(int *phase);

void reinitElements();
int exitAnim();
#endif

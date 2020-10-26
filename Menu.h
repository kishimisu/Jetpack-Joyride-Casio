#ifndef _MENU
#define _MENU

typedef struct
{
    unsigned char gadget[12];
    unsigned char gadgetEquipe[2];

    unsigned char tenue[6];
    unsigned char tenueEquipe;

    unsigned char jetpack[4];
    unsigned char jetpackEquipe;

    unsigned char ameliorationNoir[6];
    unsigned char ameliorationAimant[6];

    unsigned short int statScore[3];
    unsigned short int statVehicule[6];
    int statTotal[7];
    unsigned short int statMort[6];

    char succes[22];

    char doublePiece;
    char longueurAvance;

    int argent;

    unsigned char contrast;

}Save;

int choix();
int runGameUntilExit();
int mainMenu();
int deadMenu(int score, int nbPiece);
void barryMenu();
void jetpackMenu();
void ameliorationMenu();
void optionMenu();
void gadgetMenu();
void statMenu();
void succesMenu();
int tirageMenu(int *nbJetons, int *nbPiece, int coeur);

int isAllBought();

Save *loadSaveFile(void);
void createSaveFile(Save *save);
void saveFile();

#endif

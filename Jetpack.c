#include "include.h"

Save *sauvegarde = NULL;
char nouveauSucces = 0;

int AddIn_main(int isAppli, unsigned short OptionNum)
{
    ML_bmp_or(jetpackJoyride,0,0,128,64);
    ML_display_vram();
    Sleep(1500);

    sauvegarde = loadSaveFile(); // on charge la sauvegarde du joueur
    ML_set_contrast(sauvegarde->contrast);

    while (choix() != 1);

    saveFile();
    ML_set_contrast(get_default_contrast());
}

int jetpackMainGame()
{
    int y = 8, x = 45, w;
    int vehicule = 0;
    int tempsLasers = 0, tempsMissiles = 0, tempsPieces = 0, tempsBox = 0, lasers = 0;
    int niveau = 0, sleep = 40;
    int isDead = 0, saveGadgetsUsed = 0, collisionPhase = 0, firstDeath = 0, isJeton = 0, isBox = 0, coeur = 0;
    int nbPieces = 0, nbJetons = 0, totalJetons = 0;
    float  decalageElements = -60, scrolling = 0, vitesseDecalage = 2;
    float lvl = 0, deadDistance = 0, vehicDistance[6] = {0,0,0,0,0,0};

    nouveauSucces = 0;
    if (sauvegarde->succes[15] != -1) sauvegarde->succes[15] = 0;

    srand(RTC_getTicks()); // on initialise l'aleatoire

    initAll(); // on initialise tous les pieges

    sauvegarde->statTotal[0] += 1;

    if (sauvegarde->gadgetEquipe[0] == 9 || sauvegarde->gadgetEquipe[1] == 9)
    {
        sauvegarde->statTotal[4] += 1;
        vehicule = rand()%6+1; // si on a le gadget tour gratuit
        if (vehicule == 4 && sauvegarde->succes[15] != -1) sauvegarde->succes[15] += 1;
    }
    if (sauvegarde->gadgetEquipe[0] == 5 || sauvegarde->gadgetEquipe[1] == 5) genererJeton(1), isJeton = 1; // generation des jetons de tirage
    if (sauvegarde->longueurAvance == 1) sauvegarde->longueurAvance = 0, vehicule = -1, vitesseDecalage = 7;

    while (1) // boucle principale, tout le jeu est dedans
    {
        if (IsKeyDown(KEY_CTRL_EXIT)) if (exitAnim() == 1) break;

        ML_clear_vram();

        //sequence dessin :D

        ML_bmp_or(fond,0,0,128,51); // dessin du fond
        ML_bmp_or(sol,0,52,128,12); // dessin du sol

        ML_horizontal_scroll(-scrolling); // scrolling

        if (deplacerPieces(x, vitesseDecalage, vehicule)) // si des pieces sont a l'ecran
        {
            afficherPieces(); // on affiche les pieces restantes
            collisionPieces(x, y, &nbPieces, vehicule); // test des collisions
        }
        if (deplacerZappeurs(vitesseDecalage)) // si des zappeurs sont a l'ecran
        {
            afficherZappeurs(lvl); // on affiche les zappeurs
            if (collisionZappeurs(x,y,vehicule) && vehicule < 7 && vehicule != -1) collision(&vehicule, &collisionPhase, &tempsLasers, vehicDistance, &tempsBox); // test des collisions
        }
        if (activerLasers(&tempsLasers)) // si les lasers sont actives
        {
            tempsLasers++;
            afficherLasers(tempsLasers); // on affiche les lasers
            if (collisionLasers(x,vehicule) && vehicule < 7 && vehicule != -1) collision(&vehicule, &collisionPhase, &tempsLasers, vehicDistance, &tempsBox); // test des collisions
        }
        if (deplacerBox(vitesseDecalage, &isBox)) // si la boite est a l'ecran
        {
            afficherBox(); // on l'affiche
            if (collisionBox(x) && vehicule == 0)// test des collisions
            {
                sauvegarde->statTotal[4] += 1;
                vehicule = rand()%6+1; // on change de vehicule
                if (vehicule == 4 && sauvegarde->succes[15] != -1) sauvegarde->succes[15] += 1;
                initValues();
                reinitElements(&tempsLasers);
                isBox = 0;
                lasers = 0;
                vehicDistance[vehicule-1] = 0;
            }
        }
        if (deplacerJeton(x, &isJeton))
        {
            if (collisionJeton(x,y,vehicule,&totalJetons))
                nbJetons++, isJeton = 0;
            afficherJeton(nbJetons);
        }
        if (deplacerMissiles(x)) // si des missiles sont a l'ecran
        {
            afficherMissiles(lvl); // on les affiche
            if (collisionMissiles(x,y,vehicule) && vehicule < 7 && vehicule != -1) collision(&vehicule, &collisionPhase, &tempsLasers, vehicDistance, &tempsBox); // test des collisions
        }

        dsInfo((int)(lvl), nbPieces); // affichage de la distance et de l'argent

        afficherVehicule(y,x,lvl,vehicule); // on affiche le vehicule actuel

        if (collisionPhase) collisionAnim(&collisionPhase); // animation de collision eventuelle

        ML_display_vram(); // on affiche tout le resultat

        setFPS(2); // 42 fps

        if (rand()%((int)(sleep*0.7)) == 1 && decalageElements >= 0 && tempsLasers <= 0 && lasers == 0 && vehicule == 0 && isBox == 0 && tempsBox == 0) // generation de la mystery box
        {
            decalageElements = -32;
            genererBox();
            isBox = 1;
        }

        if (decalageElements >= 0 && tempsLasers <= 0 && lasers == 0) // generation des 2 paternes importants : zappeurs et pieces
        {
            if(rand()%(vehicule==0?4:2) == 1 && tempsPieces <= 0)
            {
                decalageElements = -genererPieces();
                tempsPieces = -decalageElements + 128;
            }
            else decalageElements = -genererZappeurs(vehicule) - 60 + (lvl<10000?lvl/400:20);
        }

        if (rand()%(sleep*8) == 1 && tempsMissiles > 10 && tempsLasers <= 0 && lasers == 0 && vehicule < 7 && vehicule != -1) // generation aleatoire des missiles
        {
            genererMissiles(x);
            tempsMissiles = 0;
        }

        if (rand()%(sleep*40) == 1 && tempsLasers <= 0 && vehicule == 0) // generation aleatoire des lasers
            lasers = 1;

        if (lasers == 1 && decalageElements >= 120) // les lasers doivent etre seuls a l'ecran
        {
            genererLasers(niveau);
            lasers = 0;
        }

        if (rand()%(sleep*30)==1 && isJeton == 0)
            genererJeton(0), isJeton = 1;

        changerHitbox(choixHitbox(y,vehicule)); // on gere la hitbox du vehicule

        isDead = deplacerVehicule(&y,&x,40,lvl,vehicule); // on gere le deplacement du vehicule actuel

        if (isDead) // si on est mort
        {
            if (saveGadgetsUsed == 0 && (sauvegarde->gadgetEquipe[0] == 2 ||sauvegarde->gadgetEquipe[1] == 2)) vehicule = 7;
            else if (saveGadgetsUsed < 2 && (sauvegarde->gadgetEquipe[0] == 6 || sauvegarde->gadgetEquipe[1] == 6)) vehicule = 8;
            else vehicule = 9;

            if (saveGadgetsUsed == 0 || saveGadgetsUsed == 2) initValues();

            if (isDead == 2) vitesseDecalage -= 0.02;
            if (vitesseDecalage < 0) vitesseDecalage = 0;

            saveGadgetsUsed++;
        }

        if ((vehicule == 0 || (vehicule == 9 && vitesseDecalage >= 2)) && y != 8)
        {
            if (y > 8)
            {
                y -= vitesseDecalage;
                if (y < 8) y = 8;
            }
            else if (y < 8)
            {
                y += vitesseDecalage;
                if (y > 8) y = 8;
            }
        }

        if (vehicule == -1 && lvl > 750 && vitesseDecalage > 2)
        {
            vitesseDecalage -= 1;
            if (vitesseDecalage == 2)
            {
                vehicule = 0;
                vitesseDecalage = 2;
                if (sauvegarde->gadgetEquipe[0] == 9 || sauvegarde->gadgetEquipe[1] == 9)
                {
                    sauvegarde->statTotal[4] += 1;
                    vehicule = rand()%6+1; // si on a le gadget tour gratuit
                    if (vehicule == 3 && sauvegarde->succes[15] != -1) sauvegarde->succes[15] += 1;
                    if (x == 0) x = 2;
                }
                reinitElements();
            }
        }

        if (vitesseDecalage == 0)
        {
            SaveDisp(1);
            // stats
            initValues();
            if (firstDeath == 0)
            {
                firstDeath = 1;
                if (nbJetons == 0 && nbPieces == 0 && sauvegarde->succes[1] != -1) sauvegarde->succes[1] = -1, nouveauSucces = 1;
            }

            switch (tirageMenu(&nbJetons, &nbPieces,coeur)) // tirage final
            {
                case 0: // si on a plus de jetons
                    if (totalJetons > sauvegarde->statScore[2]) sauvegarde->statScore[2] = totalJetons;
                    if (nbPieces > sauvegarde->statScore[1]) sauvegarde->statScore[1] = nbPieces; // stats
                        sauvegarde->statTotal[3] += totalJetons;
                    sauvegarde->statTotal[2] += nbPieces;
                    sauvegarde->statMort[3] += deadDistance;
                    for (w = 0; w < 6; w++)
                    {
                        sauvegarde->statVehicule[w] += vehicDistance[w];
                    }

                    if (sauvegarde->statMort[4] >= 20 && sauvegarde->succes[4] != -1) sauvegarde->succes[4] = -1, nouveauSucces = 1;

                    return deadMenu(lvl,nbPieces);
                    break;
                    case 1:
                        RestoreDisp(1);
                        ML_display_vram();
                        Sleep(750);
                        collisionPhase = 1;
                        vitesseDecalage = 2;
                        vehicule = 9;
                        reinitElements();
                    break;
                    case 2:
                        RestoreDisp(1);
                        ML_display_vram();
                        Sleep(750);
                        collisionPhase = 1;
                        vitesseDecalage = 3;
                        vehicule = 9;
                        reinitElements();
                    break;
                    case 3:
                        RestoreDisp(1);
                        ML_display_vram();
                        Sleep(750);
                        collisionPhase = 1;
                        vitesseDecalage = 4;
                        vehicule = 9;
                        reinitElements();
                    break;
                    case 4:
                        coeur = 1;
                        vitesseDecalage = 2;
                        vehicule = 0;
                        reinitElements();
                    break;
            }
        }

        // on gere toutes les variables diverses et variees

        niveau = (int)(lvl/400)+1; // calcul du niveau (le secteur)
        if (sleep >= 14) sleep = 40 - niveau*3;

        if (tempsBox > 0) tempsBox--;

        if (tempsPieces > 0) tempsPieces--;
        tempsMissiles++;

        decalageElements+=vitesseDecalage;

        scrolling +=vitesseDecalage;
        if (scrolling > 128) scrolling -= 128; // decalage du scrolling

        if (vitesseDecalage < 2) y+=vitesseDecalage;

        lvl+=vitesseDecalage/2;

        if (vehicule > 6) deadDistance += vitesseDecalage/2;
        else if (vehicule > 0) vehicDistance[vehicule-1] += vitesseDecalage/2;

        Sleep(sleep/(vehicule>=1&&vehicule<=6?2.5:vehicule>6?2:1.5)); // un peu de calme dans ce monde de brutes :p
      }

    sauvegarde = loadSaveFile();

    return 0;
}

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section
#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section

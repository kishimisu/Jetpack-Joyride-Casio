#include "include.h"

FONTCHARACTER PathName[]={'\\','\\','f','l','s','0','\\','J','E','T','P','A','C','K','.','s','a','v',0};
int handle;

int choix()
{
    switch(mainMenu()) // menu principal
    {
        case 0: return 1; break;
        case 26: barryMenu(); break;
        case 50: gadgetMenu(); break;
        case 56: jetpackMenu(); break;
        case 80: statMenu(); break;
        case 86: ameliorationMenu(); break;
        case 110: succesMenu(); break;
        case 116: optionMenu(); break;
        case 140: return runGameUntilExit(); break;
    }
    saveFile();
    return 0;
}

int runGameUntilExit()
{
    char value;

    do
    {
        value = jetpackMainGame();
    }while (value == 2);

    return value;
}

int mainMenu()
{
    int u = 9, v = 17, exeDown = 1, exitDown = 1, timer = 0, animation = 0, sixTouch = 0, waitTime = 0;

    do
    {
        ML_clear_vram();

        ML_bmp_or(menu,0,0,128,64); // on dessine le menu
        ML_rectangle(u,v,u+19,v+19,1,ML_BLACK,ML_XOR); // on affiche le curseur

        if (animation != 0)
        {
            if (animationSucces(animation == 2?-1:9) == 1) animation = 0;
            else animation = 1;
        }

        ML_display_vram();

        if (timer>1200 && sauvegarde->succes[0] != -1) sauvegarde->succes[0] = -1, animation = 2;

        if (IsKeyDown(KEY_CTRL_RIGHT) && u < 99 && waitTime == 0) u += 30, waitTime = 2; // on gere les entrees utilisateur
        if (IsKeyDown(KEY_CTRL_LEFT) && u > 9 && waitTime == 0) u -= 30, waitTime = 2;
        if (IsKeyDown(KEY_CTRL_DOWN) && v < 37 && waitTime == 0) v += 24, waitTime = 2;
        if (IsKeyDown(KEY_CTRL_UP) && v > 17 && waitTime == 0) v -= 24, waitTime = 2;
        if (IsKeyDown(KEY_CHAR_6)) sixTouch = 10;
        if (IsKeyDown(KEY_CHAR_9) && sixTouch > 0 && sauvegarde->succes[18] != -1) sauvegarde->succes[18] = -1, animation = 2;

        if (IsKeyDown(KEY_CTRL_EXIT) && exitDown == 0) return 0;

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
        if (IsKeyUp(KEY_CTRL_EXIT)) exitDown = 0;
        if (waitTime != 0) waitTime--;

        timer++;
        if (sixTouch > 0)sixTouch--;

        Sleep(60);
    }while (IsKeyUp(KEY_CTRL_EXE) || exeDown == 1);

    return u+v;
}

int deadMenu(int score, int nbPiece)
{
    int u = 30;
    char scoreText[8], pieceText[5], animation = 0, newRecord = 0, exeDown = 1;

    if (sauvegarde->succes[5] >= 3) sauvegarde->succes[5] = -1, nouveauSucces = 1;
    if (sauvegarde->statMort[0] + sauvegarde->statMort[1] >= 99 && sauvegarde->succes[6] != -1) sauvegarde->succes[6] = -1, nouveauSucces = 1;
    if (score >= 2000 && sauvegarde->succes[7] != -1) sauvegarde->succes[7] = -1, nouveauSucces = 1;
    if (score >= 5000 && sauvegarde->succes[8] != -1) sauvegarde->succes[8] = -1, nouveauSucces = 1;
    if (sauvegarde->tenueEquipe == 1 && sauvegarde->jetpackEquipe == 2  && sauvegarde->succes[9] != -1) sauvegarde->succes[9] = -1, nouveauSucces = 1;
    if (score == 800 && sauvegarde->succes[12] != -1) sauvegarde->succes[12] = -1, nouveauSucces = 1;
    if (sauvegarde->succes[15] >= 2) sauvegarde->succes[15] = -1, nouveauSucces = 1;
    else sauvegarde->succes[15] = 0;
    if (nbPiece == 69 && sauvegarde->succes[17] != -1)sauvegarde->succes[17] = -1, nouveauSucces = 1;
    if (sauvegarde->statTotal[1] + score >= 50000 && sauvegarde->succes[19] != -1) sauvegarde->succes[19] = -1, nouveauSucces = 1;
    if (sauvegarde->statVehicule[0] > 10000 && sauvegarde->succes[20] != -1) sauvegarde->succes[20] = -1, nouveauSucces = 1;
    if (sauvegarde->statTotal[2] >= 50000 && sauvegarde->succes[21] != -1) sauvegarde->succes[21] = -1, nouveauSucces = 1;

    if (score > sauvegarde->statScore[0]) sauvegarde->statScore[0] = score, newRecord = 1;
    sauvegarde->statTotal[1] += score;
    sauvegarde->argent += nbPiece;
    sauvegarde->doublePiece = 0;

    saveFile();

    sprintf(scoreText,"%d M",score);
    sprintf(pieceText,"%d $",nbPiece);


    if (nouveauSucces == 1) animation = 2;

    do
    {
        ML_clear_vram();

        ML_bmp_or(menuMort,0,0,128,64);
        if (sauvegarde->gadgetEquipe[0] != 0) ML_bmp_16_or(gadgets[sauvegarde->gadgetEquipe[0]-1],72,4); // on affiche les sprites des objets equipes
        if (sauvegarde->gadgetEquipe[1] != 0) ML_bmp_16_or(gadgets[sauvegarde->gadgetEquipe[1]-1],92,4);
        PrintXY(8,22,scoreText,0);
        PrintXY(18,35,pieceText,0);
        ML_bmp_8_or(money,8,36);
        if (newRecord) ML_bmp_or(record,9,48,42,8);
        ML_rectangle(60,u,120,u+12,1,ML_BLACK,ML_XOR);

        if (IsKeyDown(KEY_CTRL_DOWN) && u != 46) u += 16;
        if (IsKeyDown(KEY_CTRL_UP) && u != 30) u -= 16;

        if (animation != 0)
        {
            if (animationSucces(animation == 2?-1:2) == 1) animation = 0;
            else animation = 1;
        }

        ML_display_vram();
        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
    }while (IsKeyUp(KEY_CTRL_EXE) || exeDown == 1);

    return (u==30?0:2);
}

void barryMenu()
{
    int x, u = 13, v = 13, vNext = 13, selection = 0, exeDown = 1, keyDown = 0, animation = 0;

    int barryCosts[6] = {0,10000,7000,9000,6000,7000};
    char *barryCostsText[6] = {"","10000$","7000$","9000$","6000$","7000$"};
    char *barryNames[6] = {"BARRY","BARRY COSTUME","BARRY KID","NINJA BARRY","SPACE BARRY","BARRY LEE"};
    char totalMoney[7];

    do
    {
        sprintf(totalMoney,"%d$",sauvegarde->argent); // on transforme notre argent en texte pour pouvoir l'afficher

        ML_clear_vram();

        for(x = 0; x < 6; x++) // il y a 12 tenues
        {

            ML_rectangle(0,v+19*x,127,v+19+19*x,1,ML_BLACK,ML_WHITE); // premier rectangle "gadget"
            PrintMini(23,v+8+19*x,barryNames[x],0); // nom du premier gadget

            if (sauvegarde->tenue[x] == 0) PrintMini(101,v+8+19*x,barryCostsText[x],0); // si on n'a pas achete le premier gadget
            else if (sauvegarde->tenue[x] == 1) PrintMini(100,v+8+19*x,"Equiper",0); // si on a achete le premier gadget

            ML_bmp_16_or(barryDown1[x],5,v+2+19*x); // on affiche les sprites des gadgets

            if (sauvegarde->tenue[x] == 2) // si le premier gadget est equipe
            {
                ML_bmp_8_and(gadgetOKAlpha,11,v+10+19*x); // on affiche un petit signe
                ML_bmp_8_or(gadgetOK,11,v+10+19*x);
            }
        }

        ML_rectangle(0,u,127,u+19,1,ML_BLACK,ML_XOR); // on affiche la position du curseur
        ML_rectangle(1,u,19,u+19,1,ML_BLACK,ML_XOR); // on rÈinverse la zone du sprite pour qu'elle soit normale

        ML_rectangle(27,0,98,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete
        ML_line(80,14,92,14,ML_WHITE); // on fait des petits dessins

        ML_bmp_or(tenuesTop,0,0,128,16); // on affiche le sprite de l'entete
        ML_bmp_16_or(barryDown1[sauvegarde->tenueEquipe],81,0); // on affiche les sprites des objets equipes

        PrintMini(102,2,totalMoney,2); // on affiche notre argent

        if (animation != 0)
        {
            if (animationSucces(animation == 2?-1:1) == 1) animation = 0;
            else animation = 1;
        }

        ML_display_vram();

        if (vNext == v && keyDown == 0) // on gere le clavier ainsi que la position des elements a l'ecran
        {
            if (IsKeyDown(KEY_CTRL_DOWN) && selection != 5)
            {
                if (u > 30)
                {
                    vNext -= 19;
                    u += 19;
                }
                else
                {
                    u += 19;
                    keyDown = 6;
                }
                selection++;
            }
            else if (IsKeyDown(KEY_CTRL_UP) && selection != 0)
            {
                if (u < 20)
                {
                    vNext += 19;
                    u -= 19;
                }
                else
                {
                    u -= 19;
                    keyDown = 6;
                }
                selection--;
            }
            else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0) // si on appuie sur EXE
            {
                exeDown = 1; // pour Èviter l'appui constant sur EXE

                if (sauvegarde->tenue[selection] == 0) // si on a pas achete le gadget
                {
                    if (sauvegarde->argent >= barryCosts[selection]) // si on a assez d'argent et si il est debloque
                    {
                        sauvegarde->statTotal[6] += barryCosts[selection];
                        sauvegarde->argent -= barryCosts[selection]; // on paye
                        sauvegarde->tenue[selection] = 1; // on indique qu'il est debloque

                        if (sauvegarde->statTotal[6] >= 50000 && sauvegarde->succes[21] != -1) sauvegarde->succes[21] = -1, animation = 2;
                        if (sauvegarde->succes[3] != -1 && isAllBought() == 1) sauvegarde->succes[3] = -1, animation = 2;
                    }
                }
                else if (sauvegarde->tenue[selection] == 1) // si il est debloque
                {
                    sauvegarde->tenue[sauvegarde->tenueEquipe] = 1;
                    sauvegarde->tenue[selection] = 2;
                    sauvegarde->tenueEquipe = selection;
                }
            }
        }
        else
        {
            if (v < vNext) u += (vNext-v>2?3:1), v += (vNext-v>2?3:1);
            else if (v > vNext) u -= (v-vNext>2?3:1), v -= (v-vNext>2?3:1);

            if (keyDown > 0) keyDown--;
        }

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

    }while (IsKeyUp(KEY_CTRL_EXIT));
}

void jetpackMenu()
{
    int x, u = 13, v = 13, vNext = 13, selection = 0, exeDown = 1, keyDown = 0, animation = 0;

    int jetpackCosts[4] = {0,40000,50000,20000};
    char *jetpackCostsText[4] = {"","40000$","50000$","20000$"};
    char *jetpackNames[5] = {"MACHINEGUN JETPACK","LASER JETPACK","REACTEUR JETPACK","GUITARE JETPACK","MACHINEGUN JETPA..."};
    char totalMoney[7];

    do
    {
        sprintf(totalMoney,"%d$",sauvegarde->argent); // on transforme notre argent en texte pour pouvoir l'afficher

        ML_clear_vram();

        for(x = 0; x < 4; x++) // il y a 12 tenues
        {

            ML_rectangle(0,v+19*x,127,v+19+19*x,1,ML_BLACK,ML_WHITE); // premier rectangle "gadget"
            PrintMini(23,v+8+19*x,jetpackNames[x],0); // nom du premier gadget

            if (sauvegarde->jetpack[x] == 0) PrintMini(101,v+8+19*x,(x==0&&sauvegarde->jetpack[x]!=2?jetpackCostsText[4]:jetpackCostsText[x]),0); // si on n'a pas achete le premier gadget
            else if (sauvegarde->jetpack[x] == 1) PrintMini(100,v+8+19*x,"Equiper",0); // si on a achete le premier gadget

            switch (x)
            {
                case 0: ML_bmp_or(jetpack1[0],5,v+2+19*x,8,16); break; // on affiche les sprites des jetpacks
                case 1: ML_bmp_or(jetpack2,5,v+2+19*x,8,16); break;
                case 2: ML_bmp_or(jetpack3[1],5,v+2+19*x,8,16); break;
                case 3: ML_bmp_or(jetpack4,5,v+2+19*x,8,16); break;
            }

            if (sauvegarde->jetpack[x] == 2) // si le premier gadget est equipe
            {
                ML_bmp_8_and(gadgetOKAlpha,11,v+10+19*x); // on affiche un petit signe
                ML_bmp_8_or(gadgetOK,11,v+10+19*x);
            }
        }

        ML_rectangle(0,u,127,u+19,1,ML_BLACK,ML_XOR); // on affiche la position du curseur
        ML_rectangle(1,u,19,u+19,1,ML_BLACK,ML_XOR); // on rÈinverse la zone du sprite pour qu'elle soit normale

        ML_rectangle(27,0,98,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete
        ML_line(85,14,97,14,ML_WHITE); // on fait des petits dessins

        ML_bmp_or(jetpacksTop,0,0,128,16); // on affiche le sprite de l'entete
        ML_bmp_or((sauvegarde->jetpackEquipe==0?jetpack1[0]:sauvegarde->jetpackEquipe==1?jetpack2:sauvegarde->jetpackEquipe==2?jetpack3[1]:jetpack4),sauvegarde->jetpackEquipe==0?85:86,0,8,16); // on affiche les sprites des objets equipes
        PrintMini(102,2,totalMoney,2); // on affiche notre argent

        if (animation != 0)
        {
            if (animationSucces(animation == 2?-1:1) == 1) animation = 0;
            else animation = 1;
        }

        ML_display_vram();

        if (vNext == v && keyDown == 0) // on gere le clavier ainsi que la position des elements a l'ecran
        {
            if (IsKeyDown(KEY_CTRL_DOWN) && selection != 3)
            {
                if (u > 30)
                {
                    vNext -= 19;
                    u += 19;
                }
                else
                {
                    u += 19;
                    keyDown = 6;
                }
                selection++;
            }
            else if (IsKeyDown(KEY_CTRL_UP) && selection != 0)
            {
                if (u < 20)
                {
                    vNext += 19;
                    u -= 19;
                }
                else
                {
                    u -= 19;
                    keyDown = 6;
                }
                selection--;
            }
            else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0) // si on appuie sur EXE
            {
                exeDown = 1; // pour Èviter l'appui constant sur EXE

                if (sauvegarde->jetpack[selection]== 0) // si on a pas achete le gadget
                {
                    if (sauvegarde->argent >= jetpackCosts[selection]) // si on a assez d'argent et si il est debloque
                    {
                        sauvegarde->statTotal[6] += jetpackCosts[selection];
                        sauvegarde->argent -= jetpackCosts[selection]; // on paye
                        sauvegarde->jetpack[selection] = 1; // on indique qu'il est debloque

                        if (((sauvegarde->jetpack[1]!=0&&sauvegarde->jetpack[2]!=0)||(sauvegarde->jetpack[1]!=0&&sauvegarde->jetpack[3]!=0)||(sauvegarde->jetpack[2]!=0&&sauvegarde->jetpack[3]!=0))&&sauvegarde->succes[13]!=-1)
                            sauvegarde->succes[13]=-1, animation = 2;
                        if (sauvegarde->statTotal[6] >= 50000 && sauvegarde->succes[21] != -1) sauvegarde->succes[21] = -1, animation = 2;
                        if (sauvegarde->succes[3] != -1 && isAllBought() == 1) sauvegarde->succes[3] = -1, animation = 2;

                    }
                }
                else if (sauvegarde->jetpack[selection] == 1) // si il est debloque
                {
                    sauvegarde->jetpack[sauvegarde->jetpackEquipe] = 1;
                    sauvegarde->jetpack[selection] = 2;
                    sauvegarde->jetpackEquipe = selection;
                }
            }
        }
        else
        {
            if (v < vNext) u += (vNext-v>2?3:1), v += (vNext-v>2?3:1);
            else if (v > vNext) u -= (v-vNext>2?3:1), v -= (v-vNext>2?3:1);

            if (keyDown > 0) keyDown--;
        }

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

    }while (IsKeyUp(KEY_CTRL_EXIT));
}

void ameliorationMenu()
{
    int x, u = 22, v = 14, vNext = 14, position = 0, selection = 1, exeDown = 1, keyDown = 0, animation = 0;

    char *vehicules[6] = {"Becane d'enfer", "Teleporteur Mega Dement", "Gravity Guy", "Oiseau Benef", "P'tit Ecraseur", "Mr Calin"};
    char *ameliorationNames[13] = {"BECANE NOIRE", "BECANE AIMANTEE", "TELEPORTEUR NOIR", "TELEPORTEUR AIMANTE", "GRAVITY GUY NOIR", "GRAVITY GUY AIMANTE", "OISEAU NOIR", "OISEAU AIMANTE", "P'TIT ECRASEUR NOIR", "P'TIT ECRASEUR AIMANTE", "MR CALIN NOIR", "MR CALIN AIMANTE", "P'TIT ECRASEUR AI..."};
    char totalMoney[7];

    do
    {
        sprintf(totalMoney,"%d$",sauvegarde->argent); // on transforme notre argent en texte pour pouvoir l'afficher

        ML_clear_vram();

        for(x = 0; x < 6; x++) // il y a 6 secteurs
        {

            ML_rectangle(0,v+46*x,127,v+8+46*x,1,ML_BLACK,ML_WHITE); // le rectange "secteur"

            PrintMini(2,v+2+46*x,vehicules[x],0); // on affiche le nom du secteur
            ML_rectangle(0,v+8+46*x,127,v+27+46*x,1,ML_BLACK,ML_WHITE); // premier rectangle "gadget"
            ML_rectangle(0,v+27+46*x,127,v+46+46*x,1,ML_BLACK,ML_WHITE); // deuxieme rectange "gadget"
            PrintMini(x==3?27:x==5?28:20,v+15+46*x,ameliorationNames[2*x],0); // nom du premier gadget
            PrintMini(20,v+34+46*x,ameliorationNames[2*x+1==9&&sauvegarde->ameliorationAimant[5]!=2?12:2*x+1],0); // nom du deuxieme gadget

            if (sauvegarde->ameliorationNoir[x] == 0) PrintMini(101,v+15+46*x,"10000$",0); // si on n'a pas achete le premier gadget
            else if (sauvegarde->ameliorationNoir[x] == 1) PrintMini(100,v+15+46*x,"Equiper",0); // si on a achete le premier gadget
            if (sauvegarde->ameliorationAimant[x] == 0) PrintMini(101,v+34+46*x,"10000$",0); // si on n'a pas achete le deuxieme gadget
            else if (sauvegarde->ameliorationAimant[x] == 1) PrintMini(100,v+34+46*x,"Equiper",0); // si on a achete le deuxieme gadget

            switch (x)
            {
                case 0: ML_bmp_or(moto[1],1,v+8+46*x,24,24); break;
                case 1: ML_bmp_or(teleporteur1[1],5,v+10+46*x,8,16); break;
                case 2: ML_bmp_or(gravitySolUp1[1],2,v+9+46*x,16,24); break;
                case 3: ML_bmp_or(bird1[1],1,v+11+46*x,24,16); break;
                case 4: ML_bmp_or_cl(tank1[1],-3,v+7+46*x,24,24); break;
                case 5: ML_bmp_or_cl(dragonMilieu[1],-2,v+10+46*x,28,16); break;
            }

            ML_bmp_16_or(gadgets[9],2,v+29+46*x);

            if (sauvegarde->ameliorationNoir[x] == 2) // si le premier gadget est equipe
            {
                ML_bmp_8_and(gadgetOKAlpha,13,v+18+46*x); // on affiche un petit signe
                ML_bmp_8_or(gadgetOK,13,v+18+46*x);
            }
            if (sauvegarde->ameliorationAimant[x] == 2) // si le deuxieme gadget est equipe
            {
                ML_bmp_8_and(gadgetOKAlpha,13,v+37+46*x); // on affiche le petit signe
                ML_bmp_8_or(gadgetOK,13,v+37+46*x);
            }
        }

        ML_rectangle(0,u,127,u+19,1,ML_BLACK,ML_XOR); // on affiche la position du curseur

        ML_rectangle(26,0,100,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete

        ML_bmp_or(ameliorationsTop,0,0,128,16); // on affiche le sprite de l'entete

        PrintMini(102,2,totalMoney,2); // on affiche notre argent

        if (animation != 0)
        {
            if (animationSucces(animation == 2?-1:1) == 1) animation = 0;
            else animation = 1;
        }

        ML_display_vram();

        if (vNext == v && keyDown == 0) // on gere le clavier ainsi que la position des elements a l'ecran
        {
            if (IsKeyDown(KEY_CTRL_DOWN))
            {
                if (position == 0)
                {
                    if (selection%2<1)
                        vNext = v-8;
                    else keyDown = 6;

                    u += (selection%2<1?27:19);
                    position = 1;
                    selection++;
                }
                else if (v > -201)
                {
                    vNext = v-(selection%2<1?27:19);
                    u += (selection%2<1?27:19);
                    selection++;
                }
            }
            else if (IsKeyDown(KEY_CTRL_UP))
            {
                if (position == 1)
                {
                    if (selection%2>=1)
                        vNext = v+8;
                    else keyDown = 6;

                    u -= (selection%2<1?19:27);
                    position = 0;
                    selection--;
                }
                else if (v < 14)
                {
                    vNext = v+(selection%2<1?19:27);
                    u -= (selection%2<1?19:27);
                    selection--;
                }
            }
            else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0) // si on appuie sur EXE
            {
                exeDown = 1; // pour Èviter l'appui constant sur EXE

                if (selection%2==1)
                {
                    if (sauvegarde->ameliorationNoir[(selection-1)/2] == 0) // si on a pas achete le gadget
                    {
                        if (sauvegarde->argent >= 10000) // si on a assez d'argent et si il est debloque
                        {
                            sauvegarde->statTotal[6] += 10000;
                            sauvegarde->argent -= 10000; // on paye
                            sauvegarde->ameliorationNoir[(selection-1)/2] = 1; // on indique qu'il est debloque

                            if (sauvegarde->succes[16] != -1) sauvegarde->succes[16] = -1, animation = 2;
                            if (sauvegarde->statTotal[6] >= 50000 && sauvegarde->succes[21] != -1) sauvegarde->succes[21] = -1, animation = 2;
                            if (sauvegarde->succes[3] != -1 && isAllBought() == 1) sauvegarde->succes[3] = -1, animation = 2;
                        }
                    }
                    else if (sauvegarde->ameliorationNoir[(selection-1)/2] == 1) // si il est debloque
                    {
                        sauvegarde->ameliorationNoir[(selection-1)/2] = 2;
                    }
                    else if (sauvegarde->ameliorationNoir[(selection-1)/2] == 2) // si il est equipe
                    {
                        sauvegarde->ameliorationNoir[(selection-1)/2] = 1; // on l'enleve
                    }
                }
                else
                {
                    if (sauvegarde->ameliorationAimant[selection/2-1] == 0) // si on a pas achete le gadget
                    {
                        if (sauvegarde->argent >= 10000) // si on a assez d'argent et si il est debloque
                        {
                            sauvegarde->statTotal[6] += 10000;
                            sauvegarde->argent -= 10000; // on paye
                            sauvegarde->ameliorationAimant[selection/2-1] = 1; // on indique qu'il est debloque
                            if (sauvegarde->statTotal[6] >= 50000 && sauvegarde->succes[21] != -1) sauvegarde->succes[21] = -1, animation = 2;
                            if (sauvegarde->succes[3] != -1 && isAllBought() == 1) sauvegarde->succes[3] = -1, animation = 2;
                        }
                    }
                    else if (sauvegarde->ameliorationAimant[selection/2-1] == 1) // si il est debloque
                    {
                        sauvegarde->ameliorationAimant[selection/2-1] = 2;
                    }
                    else if (sauvegarde->ameliorationAimant[selection/2-1] == 2) // si il est equipe
                    {
                        sauvegarde->ameliorationAimant[selection/2-1] = 1; // on l'enleve
                    }
                }
            }
        }
        else
        {
            if (v < vNext) u += (vNext-v>2?3:1), v += (vNext-v>2?3:1);
            else if (v > vNext) u -= (v-vNext>2?3:1), v -= (v-vNext>2?3:1);

            if (keyDown > 0) keyDown--;
        }

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

    }while (IsKeyUp(KEY_CTRL_EXIT));
}

void optionMenu()
{
    char *gadgetsNames[12] = {"AIR BARRYS", "INSTA-BALLE", "CEINTURE DE GRAVITE", "BROUILLEUR DE MISSILES", "JETON OFFERT", "GLACOMATIQUE", "GEMMOLOGIE", "JETONS MAGNETIQUES", "TOUR GRATUIT", "AIMANT A PIECES", "DERNIERE CHANCE", "DEZAPPEUR"};
    char *gadgetDesc1[12] = {"Sautez plus haut !","Rebondissez avant votre","Rejoignez le sol plus", "Desarmez certains missiles", "Un jeton offert des le","Glissez sur le ventre","Des pieces qui comptent","Les jetons sont aimantes","Demarrez chaque partie avec","Les pieces viennent d'elles","Gagnez a coup sur votre","Desactivez certains"};
    char *gadgetDesc2[12] = {"","mort afin de gagner","rapidement que d'habitude !","afin qu'ils explosent avant", "demarrage d'une partie, si","a votre mort pour","double ! De quoi en ravir","a vous ! Attrapez les","un vehicule !","memes vers vous ! Fini","dernier jeton de tirage !","zappeurs ! Ideal si vous"};
    char *gadgetDesc3[12] = {"","quelques metres de plus !","","de vous atteindre !","c'est pas beau ca !","augmenter votre score !","certains !","tous !","","les acrobaties !","Tricheur, va !","avez des difficultees !"};

    int x,y, u = 16, exeDown = 1, exitDown = 1,a,b;

    unsigned char actualContrast;
    int scrolling, plusDown, minusDown, gadgetsCount, leftDown, rightDown;
    float v;
    char contrastChar[2];

    char *sectionNames[4] = {"CONTRASTE","GADGETS","CREDITS","REINITIALISER"};

    do
    {
        ML_clear_vram();

        for(x = 0; x < 4; x++) // il y a 12 tenues
        {
            PrintMini(x==0?44:x==1?49:x==2?49:37,18+12*x,sectionNames[x],0); // nom du premier gadget
            ML_rectangle(30,16+12*x,96,24+12*x,1,ML_BLACK,ML_TRANSPARENT); // premier rectangle "gadget"
        }

        ML_rectangle(31,u+1,95,u+7,1,ML_BLACK,ML_XOR); // on affiche la position du curseur

        ML_rectangle(39,0,88,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete

        ML_bmp_or(optionsTop,0,0,128,16); // on affiche le sprite de l'entete

        ML_display_vram();

        if (IsKeyDown(KEY_CTRL_DOWN) && u != 52) u += 12;
        else if (IsKeyDown(KEY_CTRL_UP) && u != 16) u -= 12;
        else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0) // si on appuie sur EXE
        {
            exeDown = 1;

            switch (u)
            {

            case 16:
                actualContrast = sauvegarde->contrast;
                do
                {
                    ML_clear_vram();

                    ML_bmp_or(fond,0,0,128,51);
                    for (x=0;x<4;x++) ML_line(83,x+5,113,x+25,ML_CHECKER);
                    ML_bmp_16_and_cl(zapAlpha,80,2); // masque alpha des zappeur
                    ML_bmp_16_and_cl(zapAlpha,110,22);
                    ML_bmp_16_or_cl(zap,80,2); // affichage des extremites des zappeurs
                    ML_bmp_16_or_cl(zap,110,22);

                    ML_horizontal_scroll(-scrolling);

                    ML_bmp_and(jetpack1Alpha,8,30,8,16); // masque alpha du jetpack
                    if (scrolling&1) ML_bmp_or(jetpack1[0],8,30,8,16); // affichage de la premiere animation du jetpack
                    else ML_bmp_or(jetpack1[1],8,30,8,16); // 2eme animation du jetpack
                    ML_bmp_and_cl(barryAlpha[0],15,30,10,16); // masque alpha de barry en l'air
                    ML_bmp_or(barryUp[0],15,30,10,16); // affichage de barry en l'air

                    ML_rectangle(0,47,127,63,1,ML_BLACK,ML_WHITE);

                    ML_line(30,52,97,52,ML_BLACK);

                    sprintf(contrastChar,"%d",actualContrast);
                    PrintMini(67/30*(actualContrast-get_default_contrast())+127/2-6,50,contrastChar,0);
                    PrintMini(5,50,"[-]",0);
                    PrintMini(114,50,"[+]",0);
                    PrintMini(34,57,"[EXIT]     [EXE]",0);

                    ML_display_vram();

                    if ((IsKeyDown(KEY_CHAR_PLUS) || IsKeyDown(KEY_CTRL_RIGHT)) && plusDown == 0 && actualContrast < get_default_contrast() + 15) plusDown = 1, actualContrast++, ML_set_contrast(actualContrast);
                    else if ((IsKeyDown(KEY_CHAR_MINUS) || IsKeyDown(KEY_CTRL_LEFT)) && minusDown == 0 && actualContrast > get_default_contrast() - 15) minusDown = 1, actualContrast--, ML_set_contrast(actualContrast);

                    if (IsKeyUp(KEY_CHAR_PLUS) && IsKeyUp(KEY_CTRL_RIGHT)) plusDown = 0;
                    if (IsKeyUp(KEY_CHAR_MINUS) && IsKeyUp(KEY_CTRL_LEFT)) minusDown = 0;

                    scrolling += 3;
                    if (scrolling > 128) scrolling -= 128; // decalage du scrolling

                    if (IsKeyDown(KEY_CTRL_EXIT))
                    {
                        ML_set_contrast(sauvegarde->contrast);
                        break;
                    }
                    else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0)
                    {
                        sauvegarde->contrast = actualContrast;
                        break;
                    }

                    if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

                }while (1);
            break;
            case 28:
                    a=0;
                    b=1;
                    exeDown = 1;
                    u = 4;
                    v = 16;
                    gadgetsCount = 0;
                    for (x = 0; x < 12; x++) // on regarde le nombre de gadgets deja achetes
                        if (sauvegarde->gadget[x] != 0) gadgetsCount++;
                    do
                    {
                        ML_clear_vram();

                        for (y = 0; y < 2; y++)
                        for (x = 0; x < 6; x++)
                        {
                            ML_bmp_16_or(gadgets[x+y*6],x*21+3,y*20+15);
                            if (gadgetsCount < (x+y*6)/2) ML_rectangle(x*21+4,y*20+16,x*21+4+13,y*20+16+13,1,ML_CHECKER,ML_CHECKER);
                        }

                        ML_rectangle(u,v,u+13,v+13,1,ML_XOR,ML_XOR);

                        ML_display_vram();

                        if (IsKeyDown(KEY_CTRL_LEFT) && u != 4 && leftDown == 0) u -= 21, leftDown = 1,a--;
                        if (IsKeyDown(KEY_CTRL_RIGHT) && u != 109 && rightDown == 0) u += 21, rightDown = 1,a++;
                        if (IsKeyDown(KEY_CTRL_UP) && v != 16 && plusDown == 0) v -= 20, plusDown = 1,b--;
                        if (IsKeyDown(KEY_CTRL_DOWN) && v != 36 && minusDown == 0) v += 20, minusDown = 1,b++;

                        if (IsKeyUp(KEY_CTRL_LEFT)) leftDown = 0;
                        if (IsKeyUp(KEY_CTRL_RIGHT)) rightDown = 0;
                        if (IsKeyUp(KEY_CTRL_UP)) plusDown = 0;
                        if (IsKeyUp(KEY_CTRL_DOWN)) minusDown = 0;
                        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

                        if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0 && ((b==1?a:a+6) < (gadgetsCount+1)*2))
                        {
                            ML_rectangle(0,0,128,64,1,ML_BLACK,ML_BLACK);
                            exeDown = 1;
                            ML_rectangle(5,5,123,59,1,ML_BLACK,ML_WHITE);
                            PrintMini(10,10,gadgetsNames[b==1?a:a+6],0);
                            PrintMini(10,25,gadgetDesc1[b==1?a:a+6],0);
                            PrintMini(10,35,gadgetDesc2[b==1?a:a+6],0);
                            PrintMini(10,45,gadgetDesc3[b==1?a:a+6],0);
                            ML_display_vram();
                            do
                            {
                                if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
                            }while(IsKeyUp(KEY_CTRL_EXE) || exeDown == 1);
                            exeDown = 1;
                        }

                    }while (IsKeyUp(KEY_CTRL_EXIT));
            break;
            case 40:
                v = 0.0;
                do
                {
                    ML_clear_vram();

                    PrintMini(35,v+5,"=== CREDITS ===",0);
                    PrintMini(0,v+20,"PROJET :",0);
                    PrintMini(0,v+35,"DEVELOPPEUR : DRAKALEX007",0);
                    PrintMini(0,v+45,"GRAPHISTE : DRAKALEX007",0);
                    PrintMini(0,v+55,"LANCEMENT DU PROJET : 06/06/14",0);
                    PrintMini(0,v+65,"SORTIE DU JEU : 20/01/2015",0);
                    PrintMini(0,v+80,"INFORMATIONS :",0);
                    PrintMini(0,v+95,"LIGNES DE CODE : 3692",0);
                    PrintMini(0,v+105,"SPRITES : 364",0);
                    PrintMini(0,v+120,"REMERCIEMENTS SPECIAUX :",0);
                    PrintMini(0,v+135,"LEPHENIXNOIR !",0);
                    PrintMini(0,v+145,"DARKSTORM",0);
                    PrintMini(0,v+155,"EIYERON",0);
                    PrintMini(0,v+165,"LA COMMUNAUTE PLANETE CASIO",0);
                    PrintMini(0,v+175,"POUR SON SOUTIEN :",0);
                    PrintMini(0,v+185,"WWW.PLANET-CASIO.COM",0);
                    PrintMini(0,v+200,"N'HESITEZ PAS A ALLER NOTER",0);
                    PrintMini(0,v+210,"MON JEU SUR LE SITE !",0);
                    PrintMini(0,v+220,"AN ENGLISH VERSION OF THE",0);
                    PrintMini(0,v+230,"GAME AVAILABLE ON THE SITE !",0);
                    PrintMini(0,v+250,"BON COURS DE MATHS !",0);
                    PrintMini(98,v+260,"[EXIT]",0);

                    ML_display_vram();

                    if (v>-205) v-=(IsKeyDown(KEY_CTRL_EXE)||IsKeyDown((KEY_CTRL_DOWN))?0.8:0.2);
                    if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
                    else exeDown = 1;
                    if (IsKeyDown(KEY_CTRL_EXIT)) break;
                }
                while (IsKeyUp(KEY_CTRL_EXE) || exeDown == 1 || v>-175);
                break;
            case 52:
                u = 16;
                do
                {
                    ML_clear_vram();

                    PrintMini(8,5,"ATTENTION: ETES VOUS SUR DE",0);
                    PrintMini(8,13,"  VOULOIR TOUT SUPPRIMER ?",0);
                    PrintMini(8,21," TOUTES VOS DONNEES SERONT",0);
                    PrintMini(8,29,"      REINITIALISEES.",0);
                    PrintMini(20,47,"ANNULER",0);
                    PrintMini(89,47,"OUI",0);

                    ML_rectangle(3,2,124,36,1,ML_BLACK,ML_TRANSPARENT);
                    ML_rectangle(15,45,55,53,1,ML_BLACK,ML_TRANSPARENT);
                    ML_rectangle(74,45,114,53,1,ML_BLACK,ML_TRANSPARENT);
                    ML_rectangle(u,46,u+38,52,1,ML_XOR,ML_XOR);

                    ML_display_vram();

                    if (IsKeyDown(KEY_CTRL_RIGHT) && u == 16) u += 59;
                    else if (IsKeyDown(KEY_CTRL_LEFT) && u != 16) u -= 59;
                    else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0)
                    {
                        if (u != 16)
                        {
                            exeDown = 1;

                            ML_clear_vram(); // on efface l'écran
                            Bfile_DeleteFile(PathName); // on supprime le fichier de sauvegarde existant
                            sauvegarde = loadSaveFile(); // on réécrit un fichier de sauvegarde vierge et on le charge
                            PrintMini(30,20,"JEU REINITIALISE",0);
                            ML_rectangle(22,12,103,32,1,ML_BLACK,ML_TRANSPARENT);
                            PrintMini(58,44,"OK",0);
                            ML_rectangle(52,41,71,51,1,ML_BLACK,ML_XOR);
                            ML_display_vram();

                            do
                            {
                                if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
                            }
                            while(IsKeyUp(KEY_CTRL_EXE) || exeDown == 1);
                        }
                        break;
                    }

                    if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
                }
                while (1);
                break;
            }
            exeDown = 1;
            exitDown = 1;
            u=16;
        }

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
        if (IsKeyUp(KEY_CTRL_EXIT)) exitDown = 0;

        Sleep(100);

    }while (IsKeyUp(KEY_CTRL_EXIT) || exitDown == 1);
}

void gadgetMenu()
{
    int x, u = 22, v = 14, vNext = 14, position = 0, selection = 1, exeDown = 1, keyDown = 0, animation = 0;

    int gadgetsCount = 0;
    int gadgetsCost[12] = {2000,2500,3000,4000,5000,3000,7500,4500,5000,6000,5500,6000};
    char *gadgetsCostText[12] = {"2000$","2500$","3000$","4000$","5000$","3000$","7500$","4500$","5000$","6000$","5500$","6000$"};
    char *secteurs[6] = {"Secteur 1: Preparation", "Secteur 2: Recherche", "Secteur 3: Developpement", "Secteur 4: Production", "Secteur 5: Amelioration", "Secteur 6: Extermination"};
    char *gadgetsNames[14] = {"AIR BARRYS", "INSTA-BALLE", "CEINTURE DE GRAVITE", "BROUILLEUR DE MISSI..", "JETON OFFERT", "GLACOMATIQUE", "GEMMOLOGIE", "JETONS MAGNETIQUES", "TOUR GRATUIT", "AIMANT A PIECES", "DERNIERE CHANCE", "DEZAPPEUR", "BROUILLEUR DE MIS..", "BROUILLEUR DE MISSILES"};
    char totalMoney[7];

    do
    {
        gadgetsCount = 0;
        for (x = 0; x < 12; x++) // on regarde le nombre de gadgets deja achetes
            if (sauvegarde->gadget[x] != 0) gadgetsCount++;

        sprintf(totalMoney,"%d$",sauvegarde->argent); // on transforme notre argent en texte pour pouvoir l'afficher

        ML_clear_vram();

        for(x = 0; x < 6; x++) // il y a 6 secteurs
        {

            ML_rectangle(0,v+46*x,127,v+8+46*x,1,ML_BLACK,ML_WHITE); // le rectange "secteur"

            if (gadgetsCount >= x) // si le secteur est debloque
            {
                PrintMini(2,v+2+46*x,secteurs[x],0); // on affiche le nom du secteur
                ML_rectangle(0,v+8+46*x,127,v+27+46*x,1,ML_BLACK,ML_WHITE); // premier rectangle "gadget"
                ML_rectangle(0,v+27+46*x,127,v+46+46*x,1,ML_BLACK,ML_WHITE); // deuxieme rectange "gadget"
                PrintMini(20,v+15+46*x,gadgetsNames[2*x],0); // nom du premier gadget
                PrintMini(20,v+34+46*x,gadgetsNames[2*x+1==3&&sauvegarde->gadget[3]?sauvegarde->gadget[3]==1?12:13:2*x+1],0); // nom du deuxieme gadget

                if (sauvegarde->gadget[2*x] == 0) PrintMini(105,v+15+46*x,gadgetsCostText[2*x],0); // si on n'a pas achete le premier gadget
                else if (sauvegarde->gadget[2*x] == 1) PrintMini(100,v+15+46*x,"Equiper",0); // si on a achete le premier gadget
                if (sauvegarde->gadget[2*x+1] == 0) PrintMini(105,v+34+46*x,gadgetsCostText[2*x+1],0); // si on n'a pas achete le deuxieme gadget
                else if (sauvegarde->gadget[2*x+1] == 1) PrintMini(100,v+34+46*x,"Equiper",0); // si on a achete le deuxieme gadget
            }
            else // sinon
            {
                PrintMini(2,v+2+46*x,"Achetez plus de gadgets",0); // on indique qu'il faut plus de gadgets
                ML_rectangle(0,v+8+46*x,127,v+27+46*x,1,ML_BLACK,ML_CHECKER); // on barre le nom et le prix des deux gadgets
                ML_rectangle(0,v+27+46*x,127,v+46+46*x,1,ML_BLACK,ML_CHECKER);
            }

            ML_bmp_16_or(gadgets[2*x],2,v+10+46*x); // on affiche les sprites des gadgets
            ML_bmp_16_or(gadgets[2*x+1],2,v+29+46*x);

            if (sauvegarde->gadget[2*x] == 2) // si le premier gadget est equipe
            {
                ML_bmp_8_and(gadgetOKAlpha,13,v+18+46*x); // on affiche un petit signe
                ML_bmp_8_or(gadgetOK,13,v+18+46*x);
            }
            if (sauvegarde->gadget[2*x+1] == 2) // si le deuxieme gadget est equipe
            {
                ML_bmp_8_and(gadgetOKAlpha,13,v+37+46*x); // on affiche le petit signe
                ML_bmp_8_or(gadgetOK,13,v+37+46*x);
            }
        }

        ML_rectangle(0,u,127,u+19,1,ML_BLACK,ML_XOR); // on affiche la position du curseur

        ML_rectangle(27,0,98,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete

        ML_bmp_or(gadgetTop,0,0,128,16); // on affiche le sprite de l'entete
        if (sauvegarde->gadgetEquipe[0] != 0) ML_bmp_16_or(gadgets[sauvegarde->gadgetEquipe[0]-1],69,0); // on affiche les sprites des objets equipes
        if (sauvegarde->gadgetEquipe[1] != 0) ML_bmp_16_or(gadgets[sauvegarde->gadgetEquipe[1]-1],84,0);

        ML_line(71,14,83,14,ML_WHITE); ML_line(86,14,98,14,ML_WHITE);// on fait des petits dessins

        PrintMini(102,2,totalMoney,2); // on affiche notre argent

        if (animation != 0)
        {
            if (animationSucces(animation == 2?-1:1) == 1) animation = 0;
            else animation = 1;
        }

        ML_display_vram();

        if (vNext == v && keyDown == 0) // on gere le clavier ainsi que la position des elements a l'ecran
        {
            if (IsKeyDown(KEY_CTRL_DOWN))
            {
                if (position == 0)
                {
                    if (selection%2<1)
                        vNext = v-8;
                    else keyDown = 6;

                    u += (selection%2<1?27:19);
                    position = 1;
                    selection++;
                }
                else if (v > -201)
                {
                    vNext = v-(selection%2<1?27:19);
                    u += (selection%2<1?27:19);
                    selection++;
                }
            }
            else if (IsKeyDown(KEY_CTRL_UP))
            {
                if (position == 1)
                {
                    if (selection%2>=1)
                        vNext = v+8;
                    else keyDown = 6;

                    u -= (selection%2<1?19:27);
                    position = 0;
                    selection--;
                }
                else if (v < 14)
                {
                    vNext = v+(selection%2<1?19:27);
                    u -= (selection%2<1?19:27);
                    selection--;
                }
            }
            else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0) // si on appuie sur EXE
            {
                exeDown = 1; // pour Èviter l'appui constant sur EXE

                if (sauvegarde->gadget[selection-1] == 0) // si on a pas achete le gadget
                {
                    if (sauvegarde->argent >= gadgetsCost[selection-1] && gadgetsCount*2+2 >= selection) // si on a assez d'argent et si il est debloque
                    {
                        sauvegarde->statTotal[6] += gadgetsCost[selection-1];
                        sauvegarde->argent -= gadgetsCost[selection-1]; // on paye
                        sauvegarde->gadget[selection-1] = 1; // on indique qu'il est debloque

                        if (sauvegarde->statTotal[6] >= 50000 && sauvegarde->succes[21] != -1) sauvegarde->succes[21] = -1, animation = 2;
                        if (sauvegarde->succes[3] != -1 && isAllBought() == 1) sauvegarde->succes[3] = -1, animation = 2;
                    }
                }
                else if (sauvegarde->gadget[selection-1] == 1) // si il est debloque
                {
                    if (sauvegarde->gadgetEquipe[0] == 0)
                    {
                       sauvegarde->gadget[selection-1] = 2;
                       sauvegarde->gadgetEquipe[0] = selection;
                    }
                    else if (sauvegarde->gadgetEquipe[1] == 0)
                    {
                       sauvegarde->gadget[selection-1] = 2;
                       sauvegarde->gadgetEquipe[1] = selection;
                    }
                }
                else if (sauvegarde->gadget[selection-1] == 2) // si il est equipe
                {
                    sauvegarde->gadget[selection-1] = 1; // on l'enleve

                    if (sauvegarde->gadgetEquipe[0] == selection) sauvegarde->gadgetEquipe[0] = 0;
                    else if (sauvegarde->gadgetEquipe[1] == selection) sauvegarde->gadgetEquipe[1] = 0;
                }
            }
        }
        else
        {
            if (v < vNext) u += (vNext-v>2?3:1), v += (vNext-v>2?3:1);
            else if (v > vNext) u -= (v-vNext>2?3:1), v -= (v-vNext>2?3:1);

            if (keyDown > 0) keyDown--;
        }

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

    }while (IsKeyUp(KEY_CTRL_EXIT));
}

void statMenu()
{
    int x, y, u = 22, v = 14, vNext = 14, selection = 0, actualPos = 0, totalCount = 0, waitTime = 0;

    char *secteurs[4] = {"MEILLEURS SCORES", "MEILLEURS SCORES EN VEHICULES", "TOTAL", "MORTS"};
    char *statNames[22] = {"Meilleur Score", "Max pieces collectees", "Max jetons collectes", "Becane d'enfer", "Teleporteur", "Gravity Guy",  "Oiseau Benef", "P'tit Ecraseur", "Mr Calin", "Total de parties", "Distance parcourue", "Total de pieces", "Total de jetons", "Total de vehicules", "Prix du tirage final", "Pieces depensees", "Mort par zappeur", "Mort par laser", "Mort par missile", "Distance etant mort", "Pieces etant mort", "Jetons etant mort"};
    char statCount[4] = {3,6,7,6};
    char stat[8];

    do
    {
        totalCount = 0;

        ML_clear_vram();

        for(x = 0; x < 4; x++) // il y a 6 secteurs
        {
            if (x != 0) totalCount += statCount[x-1];

            ML_rectangle(0,v+8*(totalCount+2*x),127,v+8*(totalCount+2*x+1),1,ML_BLACK,ML_WHITE); // le rectange "secteur"
            PrintMini(2,v+2+8*(totalCount+2*x),secteurs[x],0); // on affiche le nom du secteur

            for (y = 0; y < statCount[x]; y++)
            {
                sprintf(stat,"%d",x==0?sauvegarde->statScore[y]:x==1?sauvegarde->statVehicule[y]:x==2?sauvegarde->statTotal[y]:sauvegarde->statMort[y]);

                ML_rectangle(0,v+8*(totalCount+2*x+y+1),127,v+8*(totalCount+2*x+y+2),1,ML_BLACK,ML_WHITE);
                PrintMini(99,v+2+8*(totalCount+2*x+y+1),stat,0);
                PrintMini(2,v+2+8*(totalCount+2*x+y+1),statNames[totalCount+y],0);
            }
        }

        ML_rectangle(0,u,127,u+8,1,ML_BLACK,ML_XOR); // on affiche la position du curseur

        ML_rectangle(26,0,100,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete

        ML_bmp_or(statistiquesTop,0,0,128,16); // on affiche le sprite de l'entete

        ML_display_vram();

        if (vNext == v) // on gere le clavier ainsi que la position des elements a l'ecran
        {
            if (IsKeyDown(KEY_CTRL_DOWN) && !(actualPos == 3 && statCount[actualPos]-1 == selection) && waitTime == 0)
            {
                waitTime = 8;
                u += (selection==statCount[actualPos]-1?24:8);
                if (u > 40 && !(actualPos == 3 && selection > 3))
                {
                    vNext += 40 - u;
                }
                selection++;

                if (statCount[actualPos]-1 < selection) selection = 0, actualPos++;
            }
            else if (IsKeyDown(KEY_CTRL_UP) && !(actualPos == 0 && selection == 0) && waitTime == 0)
            {
                waitTime = 8;
                u -= (selection == 0?24:8);

                if (u < 22)
                {
                    vNext -= u - 22;
                }
                selection--;

                if (selection < 0) selection = statCount[actualPos-1]-1, actualPos--;
            }
        }
        else
        {
            if (v < vNext) u += (vNext-v>1?2:1), v += (vNext-v>1?2:1);
            else if (v > vNext) u -= (v-vNext>1?2:1), v -= (v-vNext>1?2:1);
        }

        if (waitTime != 0) waitTime--;

    }while (IsKeyUp(KEY_CTRL_EXIT));
}

void succesMenu()
{
    int x, u = 13, v = 13, vNext = 13, selection = 0, exeDown = 1, keyDown = 0, selectionExpanded = 99;

    int succesNb = 0;
    char succesNumber[1];
    char *succesNames[22] = {"PATIENT","HIPPIE","HALF WAY","BILL GATES","GOLD DIGGER","LIKE A TOAST","ELECTRICIEN","PILOTE DE LIGNE","PILOTE DE CHASSE","TROP DE BILLEY","MALCHANCEUX","DICTATEUR","PRECIS","COLLECTIONNEUR","CRAZY FREAKING SKILLS","AMI DES BETES","PIMP MY RIDE","MMHH... WELL","JE CONNAIS LE CREATEUR","MARATHON","I'M A BIKER","SHOPPING ADDICT"};
    char *succesDesc1[22] = {"Regardez la planque","Faites une partie sans","Debloquez 10 succes","Achetez tout les","Collectez 20 pieces en","Faites vous exploser par","Faites vous cramer","Depassez 2km","Depassez 5km","Faites une partie en","Perdez le tirage final","Gagnez 3 explosions","Faites un score","Achetez 2 jetpacks","Faites plus de 800m","Obtenez l'oiseau benef","Achetez un vehicule noir","Obtenez un score","Oh...","Volez sur 50km","Faites 10km avec la","Depensez plus de 50000"};
    char *succesDesc2[22] = {"pendant 2 minutes","collecter de piece/jeton","","elements du jeu","etant mort","un missile 3 fois de suite","(zappeur ou laser) 99 fois","","","costume et en reacteur","100 fois","atomiques","d'exactement 800m","","avec le teleporteur","2 fois en une partie","","d'exactement 69 pieces","You touch my tralala...","","becane d'enfer","pieces"};

    for (x = 0; x < 22; x++)
        if (sauvegarde->succes[x]==-1) succesNb++;

    if (succesNb >= 10 && sauvegarde->succes[2] != -1) sauvegarde->succes[2] = -1, succesNb++;

    sprintf(succesNumber,"%d / 22",succesNb);

    do
    {
        ML_clear_vram();

        for(x = 0; x < 22; x++) // il y a 12 tenues
        {

            ML_rectangle(0,x>selectionExpanded?v+19*x+10:v+19*x,127,x==selectionExpanded?v+29+19*x:x>selectionExpanded?v+29+19*x+10:v+19+19*x,1,ML_BLACK,ML_WHITE); // premier rectangle "gadget"
            PrintMini(23,x>selectionExpanded?v+12+19*x:v+2+19*x,succesNames[x],0); // nom du premier

            if (!((x==4 || x==7 || x==11 || x==13 || x==16 || x==18 || x==21) && sauvegarde->succes[x] != -1))
            {
                PrintMini(23,x>selectionExpanded?v+20+19*x:v+10+19*x,succesDesc1[x],0);
                if (x==selectionExpanded)PrintMini(23,v+19+19*x,succesDesc2[x],0);
            }
            else
            {
                PrintMini(23,x>selectionExpanded?v+20+19*x:v+10+19*x,"Succes secret",0);
            }

            if (x!=2&&x!=7&&x!=8&&x!=13&&x!=16&&x!=19&&!((x==4 || x==7 || x==11 || x==13 || x==16 || x==18 || x==21) && sauvegarde->succes[x] != -1)) ML_bmp_8_or(expand[x==selectionExpanded],120,x==selectionExpanded?v+25+19*x:x>selectionExpanded?v+22+19*x:v+12+19*x);
            ML_bmp_16_or(x==18&&sauvegarde->succes[x]==-1?sixtyNine:trophee[sauvegarde->succes[x]==-1],3,(x>selectionExpanded?v+12+19*x:v+2+19*x)+(x==18&&sauvegarde->succes[x]==-1?1:0));
        }

        ML_rectangle(0,u,127,u+(selection==selectionExpanded?29:19),1,ML_BLACK,ML_XOR); // on affiche la position du curseur
        ML_rectangle(1,u,20,u+19,1,ML_BLACK,ML_XOR); // on rÈinverse la zone du sprite pour qu'elle soit normale

        ML_rectangle(39,0,88,13,1,ML_WHITE,ML_WHITE); // on gere la transparence de l'en-tete

        ML_bmp_or(succesTop,0,0,128,16); // on affiche le sprite de l'entete

        PrintMini(97,2,succesNumber,2);

        ML_display_vram();

        if (vNext == v && keyDown == 0) // on gere le clavier ainsi que la position des elements a l'ecran
        {
            if (IsKeyDown(KEY_CTRL_DOWN) && selection != 21)
            {
                selectionExpanded = 99;
                if (u > 30)
                {
                    vNext -= 19;
                    u += 19;
                }
                else
                {
                    u += 19;
                    keyDown = 6;
                }
                selection++;
            }
            else if (IsKeyDown(KEY_CTRL_UP) && selection != 0)
            {
                selectionExpanded = 99;
                if (u < 20)
                {
                    vNext += 19;
                    u -= 19;
                }
                else
                {
                    u -= 19;
                    keyDown = 6;
                }
                selection--;
            }
            else if (IsKeyDown(KEY_CTRL_EXE) && exeDown == 0) // si on appuie sur EXE
            {
                exeDown = 1; // pour Èviter l'appui constant sur EXE

                if (selection!=2&&selection!=7&&selection!=8&&selection!=13&&selection!=16&&selection!=19&&!((selection==4 || selection==7 || selection==11 || selection==13 || selection==16 || selection==18 || selection==21) && sauvegarde->succes[selection] != -1)) selectionExpanded = selectionExpanded!=99?99:selection;
            }
        }
        else
        {
            if (v < vNext) u += (vNext-v>2?3:1), v += (vNext-v>2?3:1);
            else if (v > vNext) u -= (v-vNext>2?3:1), v -= (v-vNext>2?3:1);

            if (keyDown > 0) keyDown--;
        }

        if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;

    }while (IsKeyUp(KEY_CTRL_EXIT));
}

int tirageMenu(int *nbJetons, int *nbPiece, int coeur)
{
    char *lotNames[10] = {"PETIT BONUS","GROS BONUS","JACKPOT","PETITE EXPLOSION","GROSSE EXPLOSION","BOMBE ATOMIQUE","SECONDE CHANCE","JETONS BONUS","DOUBLE PIECE","LONGUEUR D'AVANCE"};
    char *lotDesc1[10] = {"+100 pieces !","+500 pieces !","+1000 pieces !","Augmente legerement","Augmente ton score !","Boost ton score !","Repars comme si de","Allez, cadeau !", "Pieces doublees a la", "750m d'avance a la"};
    char *lotDesc2[10] = {"","","","ton score !","","","rien n'etait !","","prochaine partie !","prochaine partie !"};
    char jetonsChar[2];
    char pieceChar[4];
    int exeDown = 1;
    int phase = 0, phaseTimer = 0, spinPhase = 0;
    int lots[3][3],i,lotsFinaux[3]={ -1,-1,-1 };
    unsigned proba;

    for(i=0;i<9;i++) lots[i/3][i%3] = rand() % 10;

    if (*nbJetons == 0) return 0;

    while (*nbJetons != 0 || spinPhase != 276) // tant qu'il y a encore des jetons
    {
        if(lotsFinaux[0]==-1 && lotsFinaux[1]==-1 && lotsFinaux[2]==-1)
        {
            proba = rand() % ((sauvegarde->gadgetEquipe[0]==11||sauvegarde->gadgetEquipe[1]==11)&&*nbJetons==1?220:367);
            for(i=0;i<3;i++)
            {
                if(proba > 220) lotsFinaux[i] = rand()%10;
                else if(proba > 170) lotsFinaux[i] = 0;
                else if(proba > 140) lotsFinaux[i] = 1;
                else if(proba > 130) lotsFinaux[i] = 2;
                else if(proba >  90) lotsFinaux[i] = 3;
                else if(proba >  70) lotsFinaux[i] = 4;
                else if(proba >  65) lotsFinaux[i] = 5;
                else if(proba >  55) lotsFinaux[i] = (coeur==0?6:rand()%5);
                else if(proba >  40) lotsFinaux[i] = 7;
                else if(proba >  25) lotsFinaux[i] = (sauvegarde->doublePiece==0?8:rand()%8);
                else lotsFinaux[i] = (sauvegarde->longueurAvance==0?9:rand()%8);
            }
        }

        ML_clear_vram(); // on efface l'écran

        sprintf(jetonsChar,*nbJetons<10?"0%d":"%d",*nbJetons); // on convertit le int de jetons en char

        PrintXY(99,6,jetonsChar,0); // on affiche le nombre de jetons restants
        ML_bmp_and(tirageFinalAlpha,0,0,128,64); // masque alpha du menu
        ML_bmp_or(tirageFinal,0,0,128,64); // affichage du menu

        if (phase) phaseTimer++; // si l'animation du levier est lancée, on a un timer qui change la position du levier toutes les 20 frames
        if (phaseTimer > 10) // si le timer est à 20 et qu'on est pas resté appuyé sur EXE (sinon il reste bloqué en bas)
        {
            phase++; // on affiche le prochain sprite du levier
            phaseTimer = 0; // on recommence le timer
        }
        if (phase == 8) // si l'animation du levier est finie
        {
            spinPhase += 19; // on lance l'animation de l'arret de la roulette
            phase = 0; // on réinitialise l'animation du levier
        }

        if(spinPhase >= 0)
        {
            int i,j;
            int x, y1, y2;

            for(i=0;i<3;i++) for(j=0;j<3;j++)
            {
                x = 4+24*i;
                y1 = 4+(spinPhase%19) + 19*j;
                if(spinPhase > 86 + 38*i && spinPhase < 276 + 38*i) y1 = 14 + 19*j;
                y2 = y1 + 16;
                if(y2>61) y2 = 61;
                if(y2<21) continue;

                ML_bmp_or(((sauvegarde->gadgetEquipe[0]==11||sauvegarde->gadgetEquipe[1]==11)&&*nbJetons==1?trefle:lotsTirage[lots[i][j]])+(y1<21)*(3*(21-y1)),x,y1<21?21:y1,17,y2-(y1<21?21:y1));
            }

            spinPhase++;
            if(spinPhase==19) spinPhase = 0;
            if(spinPhase==352) spinPhase = 0;
            if(!(spinPhase%19)) for(i=0;i<3;i++)
            {
                if(spinPhase > 86 + 38*i && spinPhase < 276 + 38*i) continue;
                lots[i][2] = lots[i][1],
                lots[i][1] = lots[i][0],
                lots[i][0] = rand()%10;
                if(spinPhase==57+38*i) lots[i][0] = lotsFinaux[i];
            }
            Sleep(5);
        }

        if(spinPhase == 276)
        {
            int lot = lots[0][1];
            (*nbJetons)--;
            if(lots[1][1] != lot) lot = -1;
            else if(lots[2][1] != lot) lot = -1;

            if(lot+1)
            {
                unsigned int key;
                sauvegarde->statTotal[5] += 1;
                if (lot == 5 && sauvegarde->succes[11] != -1)
                {
                    sauvegarde->succes[11] += 1;
                    if (sauvegarde->succes[11] >= 3) sauvegarde->succes[11] = -1, nouveauSucces = 1;
                }
                ML_rectangle(17,15,110,50,1,ML_BLACK,ML_WHITE);
                ML_bmp_or(lotsTirage[lotsFinaux[0]],20,16,17,17);
                PrintMini(39,22,lotNames[lotsFinaux[0]],0);
                PrintMini(25,35,lotDesc1[lotsFinaux[0]],0);
                PrintMini(25,43,lotDesc2[lotsFinaux[0]],0);
                ML_display_vram();
                do{if (IsKeyUp(KEY_CTRL_EXE)) exeDown = 0;
                }while (IsKeyUp(KEY_CTRL_EXE) || exeDown == 1);
                exeDown = 1;
                switch (lot)
                {
                    case 0: *nbPiece += 100; break;
                    case 1: *nbPiece += 500; break;
                    case 2: *nbPiece += 1000; break;
                    case 3: return 1; break;
                    case 4: return 2; break;
                    case 5: return 3; break;
                    case 6: return 4; break;
                    case 7: *nbJetons += 3; break;
                    case 8: sauvegarde->doublePiece = 1; break;
                    case 9: sauvegarde->longueurAvance = 1; break;
                }
            }
            else
            {
                if (sauvegarde->succes[10] != -1)
                {
                    sauvegarde->succes[10] += 1;
                    if (sauvegarde->succes[10] >= 100) sauvegarde->succes[10] = -1, nouveauSucces = 1;
                }
            }

            lotsFinaux[0] = -1, lotsFinaux[1] = -1, lotsFinaux[2] = -1;
        }

        ML_bmp_or(levier[phase>4?8-phase:phase],92,phase==0?19:27,16,phase==0||phase==4?36:28); // on dessine le levier
        if (phase == 0) ML_line(98,20,102,20,ML_WHITE); // dans deux cas précis il faut rajouter une bande blanche
        else if (phase == 4) ML_line(98,61,102,61,ML_WHITE);

        if (phase == 0 && spinPhase == 0)
        {
            if (IsKeyDown(KEY_CTRL_SHIFT))
            {
                ML_rectangle(20,20,107,45,1,ML_BLACK,ML_WHITE);
                ML_bmp_16_or_cl(jetonSP,35,26);
                PrintMini(52,30,"x   =",0);
                PrintMini(57,30,jetonsChar,0);
                sprintf(pieceChar,"%d",*nbJetons*50);
                PrintMini(74,30,pieceChar,0);
                ML_bmp_8_or_cl(money,88,29);
                ML_display_vram();
                while(IsKeyUp(KEY_CTRL_EXE));
                *nbPiece += *nbJetons*50;
                *nbJetons = 0;
                break;
            }
            if (IsKeyDown(KEY_CTRL_EXE)) phase = 1; // si on appuie sur exe, que le levier est à sa position initiale et que la roulette est toujours en train de tourner
        }

        ML_display_vram(); // on affiche l'écran
    }

    return 0;
}

int isAllBought()
{
    char x;

    for (x = 0; x < 6; x++)
    {
        if (sauvegarde->tenue[x] == 0 || sauvegarde->ameliorationAimant[x] == 0 || sauvegarde->ameliorationNoir[x] == 0 || sauvegarde->gadget[2*x] == 0 || sauvegarde->gadget[2*x+1] == 0) return 0;
    }
    for (x = 0; x < 4; x++)
        if (sauvegarde->jetpack[x] == 0) return 0;

    return 1;
}

Save *loadSaveFile(void) // fonction qui charge la sauvegare
 {
     Save *save = (Save*)malloc(sizeof(Save)); // on crée un fichier de sauvegarde temporaire qui sera retourné

     handle = Bfile_OpenFile(PathName,_OPENMODE_READ); // on ouvre le fichier en mode lecture

     if (handle < 0) // si ça a échoué
     {
         createSaveFile(save); // on crée le fichier de sauvegarde
         handle = Bfile_OpenFile(PathName,_OPENMODE_READ); // on l'ouvre en mode lecture
     }

     Bfile_ReadFile(handle,save,sizeof(Save),0); // on charge la sauvegarde
     Bfile_CloseFile(handle); // on ferme le fichier

     return save; // on retourne la fichier sauvegarde temporaire
 }

 void createSaveFile(Save *save) // fonction qui crée un nouveau fichier de sauvegarde quand il n'existe pas
 {
     save->tenue[0] = 2;
     save->jetpack[0] = 2;
     save->contrast = get_default_contrast();

     Bfile_CreateFile(PathName,sizeof(Save)); // on crée le fichier
     handle = Bfile_OpenFile(PathName,_OPENMODE_WRITE); // on l'ouvre en mode écriture
     Bfile_WriteFile(handle,save,sizeof(Save)); // on écrit dedans notre sauvegarde
     Bfile_CloseFile(handle); // on le ferme
 }
 void saveFile() // fichier qui enregistre les données sauvegardées
 {
     Bfile_DeleteFile(PathName); // on supprime le fichier existant
     Bfile_CreateFile(PathName,sizeof(Save)); // on en recrée un
     handle = Bfile_OpenFile(PathName,_OPENMODE_WRITE); // on l'ouvre en mode écriture
     Bfile_WriteFile(handle,sauvegarde,sizeof(Save)); // on écrit dedans notre sauvegarde
     Bfile_CloseFile(handle); // on ferme le fichier
 }

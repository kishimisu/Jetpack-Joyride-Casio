#include "include.h"

static int exeStatut = 0, position = 8, direction = 0;
static float angle = 0, phase = 0;

static float vitesse = 0, acceleration = 0;
static int frameDepart = 0, frameActuelle = 0;

void initValues()
{
    exeStatut = 0, phase = 0, position = 8, direction = 0;
    angle = 0;

    vitesse = 0, acceleration = 0;
    frameDepart = 0, frameActuelle = 0;
}

int deplacerVehicule(int *x, int *y, int speed, int frame, int vehicule)
{
    if (vehicule == -1) // 750m d'avance
    {
        if (IsKeyDown(KEY_CTRL_SHIFT)) acceleration = (0.4); //si on appuie sur EXE on monte
        else acceleration = (*y?-0.4:-1); // si y vaut 0, acceleration vaut -1 sinon elle vaut -0.01

        vitesse += acceleration; //la vitesse depend de l'acceleration et y depend de la vitesse
        *y-=vitesse;

        if (*y > 45) *y = 45; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 45 || *y == 0 ) // si on touche le sol ou le plafond
        {
            vitesse = 0; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
        }
    }
    if (vehicule == 0) // barry
    {
        if (IsKeyDown(KEY_CTRL_SHIFT)) acceleration = (0.01*(float)(speed)), vitesse = (*y==45&&sauvegarde->gadgetEquipe[0]==1||sauvegarde->gadgetEquipe[1]==1?2:vitesse); //si on appuie sur EXE on monte
        else acceleration = (*y?-((sauvegarde->gadgetEquipe[0]==3||sauvegarde->gadgetEquipe[1]==3?0.015:0.01)*(float)(speed)):-1); // si y vaut 0, acceleration vaut -1 sinon elle vaut -0.01

        vitesse += acceleration; //la vitesse depend de l'acceleration et y depend de la vitesse
        *y-=vitesse;

        if (*y > 45) *y = 45; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 45 || *y == 0 ) // si on touche le sol ou le plafond
        {
            if (*y == 0) *y = 1;
            vitesse = 0; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
        }
    }
    if (vehicule == 1) // moto
    {
        if(IsKeyDown(KEY_CTRL_SHIFT) && *y==44 && exeStatut == 0) // si on appuie sur exe et qu'on est au sol
        {
            acceleration = 0;
            vitesse = 2.5;
            frameDepart = frame;
            exeStatut = 1;
        }
        frameActuelle = frame - frameDepart;
        if((IsKeyUp(KEY_CTRL_SHIFT) && frameActuelle>2) || frameActuelle>8) acceleration = -0.3; // determination de la hauteur du saut

        vitesse += acceleration; //la vitesse depend de l'acceleration et y depend de la vitesse
        *y-=vitesse;

        if (*y > 44) *y = 44; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 44 || *y == 0 ) // si on touche le sol ou le plafond
        {
            vitesse = 0; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
        }
    }
    if (vehicule == 2) // teleporteur
    {
        if (phase == 0) // si il n'y a pas de teleportation en cours
        {
            if (position > 53) direction = 1; // si la droite sort de l'ecran, on change de sens
            else if (position < 8) direction = 0;

            position += (direction?-2:2); // on definit le sens de la droite

            if (IsKeyDown(KEY_CTRL_SHIFT) && *x < 50 && exeStatut == 0)
            {
                phase = 1; // si on appuye sur EXE et qu'on est pas trop eloigne, on commence l'animation de teleportation
                exeStatut = 1;
            }
        }
        else phase++;

        if (*x > 8) *x -= 2; // on ramene le teleporteur a sa position initiale

        if (phase == 5) // a cette animation on fait la teleportation
        {
            *y = position-8;
            *x += 45;
        }
        else if (phase == 9) phase = 0; // la derniere animation reinitialise tout
    }
    if (vehicule == 3) // gravity guy
    {
        if (IsKeyDown(KEY_CTRL_SHIFT) && exeStatut == 0) // si on appuie sur exe
        {
            direction = (direction?0:1); // on change la gravite
            exeStatut =  1;
        }

        if (direction) acceleration = (0.01*(float)(speed));
        else acceleration = (*y?-(0.01*(float)(speed)):-1);

        vitesse += acceleration; //la vitesse depend de l'acceleration et y depend de la vitesse
        *y-=vitesse;

        if (*y > 44) *y = 44; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 44 || *y == 0 ) // si on touche le sol ou le plafond
        {
            vitesse = 0; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
        }
    }
    if (vehicule == 4) // oiseau
    {
        if (phase != 0) phase++; // on gere l'animation de l'oiseau
        if (phase > 6) phase = 0;

        if (IsKeyDown(KEY_CTRL_SHIFT) && exeStatut == 0) // si on appuie sur exe
        {
            vitesse = 0; // on reinitialise la vitesse
            acceleration = (0.06*(float)(speed)); // on donne un impulsion
            exeStatut = 1;
            phase = 1; // premiere phase de l'animation
        }
        else acceleration = (*y?-(0.01*(float)(speed)):-1);

        vitesse += acceleration; //la vitesse depend de l'acceleration et y depend de la vitesse
        *y-=vitesse;

        if (*y > 46) *y = 46; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 46 || *y == 0 ) // si on touche le sol ou le plafond
        {
            vitesse = 0; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
        }
    }
    if (vehicule == 5) // ptit ecraseur
    {
        if(IsKeyDown(KEY_CTRL_SHIFT) && *y == 40 && exeStatut == 0) // si on appuie sur exe et qu'on est au sol
        {
            acceleration = -(0.01*(float)(speed)); // on accelere
            vitesse = 4.; // on augmente la vitesse d'un coup : impulsion
            exeStatut = 1;
        }
        else acceleration = (*y?(IsKeyDown(KEY_CTRL_SHIFT)?-(0.001*(float)(speed)):-0.4):-0.4); // sinon on redescent Ë† -0.4, ou 0.005 si on appuie sur exe

        vitesse += acceleration; //la vitesse depend de l'acceleration et x depend de la vitesse
        *y-=vitesse;

        if (*y > 40) *y = 40; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 40 || *y == 0 ) // si on touche le sol ou le plafond
        {
            vitesse = -1; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
            if (*y == 0) *y = 1;
        }
    }

    if (vehicule == 6) // dragon
    {
        *x = 0;
        acceleration = (IsKeyDown(KEY_CTRL_SHIFT) ? -0.005 : +0.005) * ((float)speed);
        if(!*y && IsKeyDown(KEY_CTRL_SHIFT)) vitesse = -1;
        vitesse += acceleration;
        *y -= vitesse;

        if (*y > 45) *y = 45;
        else if (*y < 0) *y = 0 + IsKeyDown(KEY_CTRL_SHIFT);
        if (*y == 45 || *y == 0 ) vitesse = 0;

        phase = (vitesse != 0 ? (vitesse < 0 ? -1 : 1) : 0);
    }

    if (vehicule == 7) // insta-ball
    {
        acceleration = -(0.01*(float)(speed));

        vitesse += acceleration; //la vitesse depend de l'acceleration et x depend de la vitesse
        *y-=vitesse;

        if (*y > 44) *y = 44; // pour ne pas sortir de l'ecran
        if (*y == 44) // si on touche le sol ou le plafond
        {
            vitesse = 5;
            phase++;
        }

        if (phase == 5)
        {
            phase = 0;
            return 1;
        }

        angle+=0.004; // rotation de la balle
        if (angle > 360) angle = 0;
    }

    if (vehicule == 8) // glacomatique
    {
        if (phase == 0) *y = 45;

        phase++;
        if (phase > 120) return 1;
    }

    if (vehicule == 9)
    {
        acceleration = -(0.005*(float)(speed));

        vitesse += acceleration; //la vitesse depend de l'acceleration et x depend de la vitesse
        *y-=vitesse;

        if (*y > 50) *y = 50; // pour ne pas sortir de l'ecran
        else if (*y < 0) *y = 0;

        if (*y == 50 || *y == 0 ) // si on touche le sol ou le plafond
        {
            vitesse = -1; // la vitesse s'arrete
            acceleration = -!*y; // l'acceleration devient negative
            if (*y == 0) *y = 1;
            else
            {
                vitesse = 4-phase/2;
            }
            phase++;
        }

        if (phase > 6) return 2;
        if (*y == 50) return 1;
        else return 0;
    }

    if (IsKeyUp(KEY_CTRL_SHIFT)) exeStatut = 0; // pour eviter l'appui constant sur EXE

    return 0;
}
void afficherVehicule(int x, int y, int lvl, int vehicule)
{
    if (vehicule == -1 || vehicule == 0)
    {
        /* Tous les lvl%8<4 servent a alterner le pied droit et le pied gauche
         lorsque barry est au sol pour donner une impression de marche.
         Tous les 5 tours de boucle il change de pied. */

        ML_filled_ellipse(x+10,60,5*y/45+2,y/45+1,ML_BLACK); // dessin de l'ombre

        switch (sauvegarde->jetpackEquipe) // on affiche le jetpack et ses animations
        {
            case 0:
                ML_bmp_and(jetpack1Alpha,x,lvl%8<4&&y==45?y-1:y,8,16); // masque alpha du jetpack
                if (lvl&1 && IsKeyDown(KEY_CTRL_SHIFT)) ML_bmp_or(jetpack1[0],x,lvl%8<4&&y==45?y-1:y,8,16); // affichage de la premiere animation du jetpack
                else ML_bmp_or(jetpack1[1],x,lvl%8<4&&y==45?y-1:y,8,16); // 2eme animation du jetpack
            break;
            case 1:
                if (IsKeyDown(KEY_CTRL_SHIFT))
                {
                    ML_rectangle(x+2,y+13,x+6,60,1,ML_CHECKER,ML_BLACK); // animation du laser
                    if (lvl%6<3) ML_rectangle(x+2,y+13,x+6,60,1,ML_XOR,ML_TRANSPARENT);
                }
                ML_bmp_and(jetpack2Alpha,x,lvl%8<4&&y==45?y-1:y,8,16);
                ML_bmp_or(jetpack2,x,lvl%8<4&&y==45?y-1:y,8,16);
            break;
            case 2:
                if (IsKeyDown(KEY_CTRL_SHIFT))
                {
                    ML_bmp_and(jetpack3Alpha[(lvl%4<2)+1],x,lvl%8<4&&y==45?y-1:y,8,16);
                    ML_bmp_or(jetpack3[(lvl%4<2)+1],x,lvl%8<4&&y==45?y-1:y,8,16);
                }
                else
                {
                    ML_bmp_and(jetpack3Alpha[0],x,lvl%8<4&&y==45?y-1:y,8,16);
                    ML_bmp_or(jetpack3[0],x,lvl%8<4&&y==45?y-1:y,8,16);
                }
            break;
            case 3:
                ML_bmp_and(jetpack4Alpha,x,lvl%8<4&&y==45?y-1:y,8,16);
                ML_bmp_or(jetpack4,x,lvl%8<4&&y==45?y-1:y,8,16);
            break;
        }

        if (y == 45) // si on touche le sol
        {
            ML_bmp_and_cl((lvl%8<4?barryDown1Alpha[sauvegarde->tenueEquipe]:barryDown2Alpha[sauvegarde->tenueEquipe]),x+7,lvl%8<4?y-1:y,10,16); // masque alpha de barry au sol
            ML_bmp_or((lvl%8<4?barryDown1[sauvegarde->tenueEquipe]:barryDown2[sauvegarde->tenueEquipe]),x+7,lvl%8<4?y-1:y,10,16); // affichage de barry au sol
        }
        else //sinon, si on est en l'air
        {
            ML_bmp_and_cl(barryAlpha[sauvegarde->tenueEquipe],x+7,y,10,16); // masque alpha de barry en l'air
            ML_bmp_or(barryUp[sauvegarde->tenueEquipe],x+7,y,10,16); // affichage de barry en l'air
        }
    }
    if (vehicule == 1)
    {
        ML_filled_ellipse(x+12,60,5*y/45+6,y/45+1,ML_BLACK);

        if (y == 44) // si la moto est au sol
        {
            ML_bmp_and(motoAlpha,x,y-1,24,24); // masque alpha de la moto au sol
            ML_bmp_or(lvl%8<4||sauvegarde->ameliorationNoir[0]==2?moto[sauvegarde->ameliorationNoir[0]==2]:moto2,x,y-1,24,24); // affichage de la moto au sol
        }
        else
        {
            ML_bmp_and(motoAirAlpha,x,y,24,24); // masque alpha de la moto en l'air
            ML_bmp_or(motoAir[sauvegarde->ameliorationNoir[0]==2],x,y,24,24); // affichage de la moto en l'air
        }
    }
    if (vehicule == 2)
    {
        ML_filled_ellipse(x+3,60,5*y/45+2,y/45+1,ML_BLACK);

        ML_line(x+5,y+8,x+45,position,ML_BLACK);

        if (phase == 0) // phase initiale du teleporteur
        {
            ML_bmp_and(teleporteur1Alpha,x,y,8,16);
            ML_bmp_or(teleporteur1[sauvegarde->ameliorationNoir[1]==2],x,y,8,16);
        }
        else if (phase == 1 || phase == 2 || phase == 7 || phase == 8) // premiere animation
        {
            ML_bmp_and(teleporteur2Alpha,x,y,8,16);
            ML_bmp_or(teleporteur2[sauvegarde->ameliorationNoir[1]==2],x,y,8,16);
        }
        else if (phase >= 3 || phase <= 6) // deuxieme animation
        {
            ML_bmp_and(teleporteur3Alpha,x,y,8,16);
            ML_bmp_or(teleporteur3[sauvegarde->ameliorationNoir[1]==2],x,y,8,16);
        }
    }
    if (vehicule == 3)
    {
        ML_filled_ellipse(x+7,60,5*y/45+2,y/45+1,ML_BLACK);

        if (y == 0 && direction == 1) // si on est au plafond
        {
            ML_bmp_and((lvl%8<4?gravitySolDown1Alpha:gravitySolDown2Alpha),x,lvl%8<4?y+1:y,16,24); // masque alpha au plafond
            ML_bmp_or((lvl%8<4?gravitySolDown1[sauvegarde->ameliorationNoir[2]==2]:gravitySolDown2[sauvegarde->ameliorationNoir[2]==2]),x,lvl%8<4?y-1:y,16,24); // animation un pied sur l'autre au plafond
        }
        else if (y == 44 && direction == 0) // si on est au sol
        {
            ML_bmp_and((lvl%8<4?gravitySolUp1Alpha:gravitySolUp2Alpha),x,lvl%8<4?y-1:y,16,24); // masque alpha au sol
            ML_bmp_or((lvl%8<4?gravitySolUp1[sauvegarde->ameliorationNoir[2]==2]:gravitySolUp2[sauvegarde->ameliorationNoir[2]==2]),x,lvl%8<4?y-1:y,16,24); // animation un pied sur l'autre au sol
        }
        else // si on est en "gravitation" (dans les airs)
        {
            ML_bmp_and(direction?gravitySolDown1Alpha:gravitySolUp1Alpha,x,y,16,24); // masque alpha en l'air haut ou bas
            ML_bmp_or(direction?gravitySolDown1[sauvegarde->ameliorationNoir[2]==2]:gravitySolUp1[sauvegarde->ameliorationNoir[2]==2],x,y,16,24); // animation en l'air haut ou bas
        }
    }
    if (vehicule == 4)
    {
        ML_filled_ellipse(x+12,60,5*y/45+6,y/45+1,ML_BLACK);

        if (phase == 0) // phase normale de l'oiseau, ailes vers le haut
        {
            ML_bmp_and(bird1Alpha,x,y,24,16); // masque alpha ailes vers le haut
            ML_bmp_or(bird1[sauvegarde->ameliorationNoir[3]==2],x,y,24,16); // animation ailes vers le haut
        }
        else if (phase == 1 || phase == 2 || phase == 5 || phase == 6) // phase du milieu, ailes au milieu
        {
            ML_bmp_and(bird1Alpha,x,y,24,16); // masque alpha ailes au milieu
            ML_bmp_or(bird2[sauvegarde->ameliorationNoir[3]==2],x,y,24,16); // animation ailes au milieu
        }
        else // derniere phase, ailes vers le bas
        {
            ML_bmp_and(bird1Alpha,x,y,24,16); // masque alpha ailes vers le bas
            ML_bmp_or(bird3[sauvegarde->ameliorationNoir[3]==2],x,y,24,16); // animation ailes vers le bas
        }
    }
    if (vehicule == 5)
    {
        ML_filled_ellipse(x+13,60,5*y/45+6,y/45+1,ML_BLACK);

        if (y == 40) // si on est au sol
        {
            ML_bmp_and(lvl%16<8?tank1Alpha:tank2Alpha,x,lvl%24<12?y-1:y,24,24); // masque alpha au sol
            ML_bmp_or(lvl%16<8?tank1[sauvegarde->ameliorationNoir[4]==2]:tank2[sauvegarde->ameliorationNoir[4]==2],x,lvl%24<12?y-1:y,24,24); // animation de marche
        }
        else if (IsKeyDown(KEY_CTRL_SHIFT)) // si est en l'air et qu'on appuie sur exe
        {
            ML_bmp_and(lvl%6<3?tank4Alpha:tank5Alpha,x,y,24,24); // masque alpha en l'air avec les flammes
            ML_bmp_or(lvl%6<3?tank4[sauvegarde->ameliorationNoir[4]==2]:tank5[sauvegarde->ameliorationNoir[4]==2],x,y,24,24); // animation des flames
        }
        else // si on est en l'air et qu'on appuie pas sur exe
        {
            ML_bmp_and(tank3Alpha,x,y,24,24); // masque alpha en l'air
            ML_bmp_or(tank3[sauvegarde->ameliorationNoir[4]==2],x,y,24,24); // animation en l'air
        }
    }

    if (vehicule == 6)
    {
        if(phase == -1)
            ML_bmp_and(dragonBasAlpha,x,y,28,16),
            ML_bmp_or(dragonBas[sauvegarde->ameliorationNoir[5]==2],x,y,28,16);
        else if(phase == 0)
            ML_bmp_and(dragonMilieuAlpha,x,y,28,16),
            ML_bmp_or(dragonMilieu[sauvegarde->ameliorationNoir[5]==2],x,y,28,16);
        else if(phase == 1)
            ML_bmp_and(dragonHautAlpha,x,y,28,16),
            ML_bmp_or(dragonHaut[sauvegarde->ameliorationNoir[5]==2],x,y,28,16);
    }

    if (vehicule == 7)
    {
        ML_filled_ellipse(x+8,60,5*y/45+6,y/45+1,ML_BLACK);

        ML_filled_circle(x+9,y+9,9,ML_WHITE);
        ML_circle(x+9,y+9,9,ML_BLACK);
        ML_line(x+9-cos(angle*180/3.14)*8,y+9-sin(angle*180/3.14)*8,x+9+cos(angle*180/3.14)*9,y+9+sin(angle*180/3.14)*9,ML_BLACK);
    }

    if (vehicule == 8)
    {
        ML_filled_ellipse(x+7,60,5*y/45+1,y/45,ML_BLACK);

        ML_bmp_16_and(glaceAlpha[(int)(phase/30)],x,y);
        ML_bmp_16_or(glace[(int)(phase/30)],x,y);
    }

    if (vehicule == 9)
    {
        ML_filled_ellipse(x+10,60,5*y/45+2,y/45+1,ML_BLACK); // dessin de l'ombre

        if (phase == 0)
        {
            ML_bmp_and_cl(barryAlpha[sauvegarde->tenueEquipe],x+7,y,10,16); // masque alpha de barry en l'air
            ML_bmp_or(barryUp[sauvegarde->tenueEquipe],x+7,y,10,16); // affichage de barry en l'air
        }
        else
        {
            Disp_Bitmap(barryAlpha[sauvegarde->tenueEquipe],x+7,y,10,16,0);
            Disp_Bitmap(barryUp[sauvegarde->tenueEquipe],x+7,y,10,16,1);
        }
    }
}

int animationSucces(int time)
{
    static unsigned char frame = -1;

    if (time == -1) frame = -1, time = 1;
    frame += (time == 9?2:1);
    if (time == 9) time = 1;

    ML_bmp_and_cl(succesAlpha,11,frame<=20*time?frame/time-20:frame<80*time?0:(80*time-frame)/time,105,20);
    ML_bmp_or_cl(succes,11,frame<=20*time?frame/time-20:frame<80*time?0:(80*time-frame)/time,105,20);

    if (frame > 100*time)
    {
        frame = 0;
        return 1;
    }
    else return 0;
}

int choixHitbox(int y, int vehic)
{
    switch (vehic)
    {
        case -1: return 0; break;
        case 0: return 0; break;
        case 1:
            if (y == 44) return 1;
            else return 2;
            break;
        case 2: return 3; break;
        case 3: return 4; break;
        case 4: return 5; break;
        case 5:
            if (y == 40) return 6;
            else return 7;
            break;
        case 6: return 8; break;
        case 7: return 9; break;
        case 8: return 10; break;
        case 9: return 11; break;
    }
}

void dsInt(int x, int y, int nb)
{
    char text[10];
    sprintf(text,"%d",nb);
    PrintXY(x,y,text,1);
}

unsigned char get_default_contrast() // Function from Eiyeron
{
	int model = *(int*)0x80000300;

	switch(model) {
		case 0 :
			return 0; // EMULATOR
		break;

		case 0x33363246:
			return 162; // G35
		break;

		case 0x34393542:
			return 168; // G75
		break;

		case 0x80005D7C :
			return 170; // G85
		break;

		case 0x34393342 :
			return 168; // G95
		break;

		default :
			return 165; // UNDEFINED
		break;
	}
}

void Disp_Bitmap(const unsigned char *b, int x, int y, int w, int h, int m) // function from Lephenixnoir
{
  unsigned char v;
  int i,j;

  for(i=0;i<h;i++) for(j=0;j<w;j++)
  {
    v = (*(b+i*(((w-1)>>3)+1)+(j>>3))>>(~j&7))&1;

    switch(m)
    {
/* Sprite normal
      case 0: if(!v) Bdisp_SetPoint_VRAM(x+j,y+i,0); break;
      case 1: if(v)  Bdisp_SetPoint_VRAM(x+j,y+i,1); break;
      case 2: if(v)  Bdisp_SetPoint_VRAM(x+j,y+i,!Bdisp_GetPoint_VRAM(x+j,y+i)); break;
*/
/* Sprite a 90° */
      case 0: if(!v) Bdisp_SetPoint_VRAM(x+w-1-i,y+j,0); break;
      case 1: if(v)  Bdisp_SetPoint_VRAM(x+w-1-i,y+j,1); break;
      case 2: if(v)  Bdisp_SetPoint_VRAM(x+w-1-i,y+j,!Bdisp_GetPoint_VRAM(x+j,y+i)); break;
    }
  }
}

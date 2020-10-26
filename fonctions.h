#ifndef _FONCTIONS_H
#define _FONCTIONS_H

void initValues();
int deplacerVehicule(int *x, int *y, int speed, int frame, int vehicule);
void afficherVehicule(int x, int y, int lvl, int vehicule);
int animationSucces(int time);
int choixHitbox(int y, int vehic);
void dsInt(int x, int y, int nb);
unsigned char get_default_contrast();
void Disp_Bitmap(const unsigned char *b, int x, int y, int w, int h, int m);

#endif

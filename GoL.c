#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define H 50
#define W 200
//#define CHANCE 45

int main(void) {
    srand(time(NULL));

    int grille[H][W];
    int next_grille[H][W];
    int age[H][W] = {0}; //To change the color of cells with their lifetime

    char buffer[H][W+1];

    memset(grille, 0, sizeof grille);
    for (int k = 0; k < 6; k++) {
        int cy = rand() % H, cx = rand() % W;
        int r = 4 + rand() % 8;
        for (int i = -r; i <= r; i++)
            for (int j = -r * 2; j <= r * 2; j++) {
                int y = cy + i, x = cx + j;
                if (y < 0 || y >= H || x < 0 || x >= W) continue;
                double d = sqrt(i*i*4.0 + j*j) / (r * 2.0);
                if (d < 1.0 && rand() % 100 < 50) grille[y][x] = 1;
            }
    }

    /*for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
      double dy = (i - H / 2.0) * 2.0;  
      double dx = j - W / 2.0;
      double dist = sqrt(dx*dx + dy*dy) / (W / 2.5);
      double p = dist < 1.0 ? 0.55 * (1.0 - dist * 0.5) : 0.0;
      grille[i][j] = (rand() / (double)RAND_MAX) < p;
      }
      }*/

    /*for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
      if (rand()%100 < CHANCE) {
      grille[i][j] = 1;
      } else {
      grille[i][j] = 0;
      }



    //grille[i][j] = 0;
    }
    }*/

    /* Planneur
       grille[4][2] = 1;
       grille[4][3] = 1;
       grille[4][4] = 1;
       grille[3][4] = 1;
       grille[2][3] = 1;
       */

    //grille[10][10] = 1; grille[10][11] = 1; grille[10][12] = 1; grille[11][12] = 1; grille[12][11] = 1;

    printf("\x1b[?1049h");
    printf("\x1b[?25l");

    while (1) {
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int voisins = 0;

                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (x+dx < 0 || y+dy < 0 || x+dx >= W || y+dy >= H) continue;
                        if (dx == 0 && dy == 0) continue;
                        if (grille[y+dy][x+dx] == 1) voisins += 1;
                    }
                }

                if (grille[y][x] == 1 && (voisins == 2 || voisins == 3)) {
                    next_grille[y][x] = 1;
                } else if (grille[y][x] == 0 && voisins == 3) {
                    next_grille[y][x] = 1;
                } else {
                    next_grille[y][x] = 0;
                }
            }
        }

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (grille[y][x] == next_grille[y][x]) {
                    if (grille[y][x] == 0) {
                        continue;
                    } else {
                        age[y][x] ++;
                    }
                } else {
                    age[y][x] = 0;
                }
            }
        }

        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                grille[i][j] = next_grille[i][j];
            }
        }

        // With # (basic)
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (grille[i][j] == 1) {
                    buffer[i][j] = '#';
                }
                if (grille[i][j] == 0) {
                    buffer[i][j] = ' ';
                }
            }
            buffer[i][W] = '\0';
        }


        printf("\x1b[H");
        (void)buffer;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (grille[i][j]) {
                    int a = age[i][j] > 50 ? 50 : age[i][j];
                    int r = 255 * a / 50;
                    int g = 255 - r;
                    int b = 80;
                    printf("\x1b[38;2;%d;%d;%dm#", r, g, b);
                } else {
                    putchar(' ');
                }
            }
            printf("\x1b[0m\n");
        }
        //        for (int i = 0; i < H; i++) printf("%s\n", age_buffer[i]);

        usleep(200000);
    }
}

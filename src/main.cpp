#include <iostream>
#include <pthread.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "class_RGB.h"

// Résolution (taille) de l'écran OpenCV
#define WIDTH  1024
#define HEIGHT 1024

//nombre de thread
#define NB_THREAD 4

//repère complexe
double X = -1;
double Y = 1;

// Conversion du repère de l'écran vers le repère complexe
#define convertx(x) ((x)*(Y*2)/WIDTH - Y)
#define converty(y) ((y)*(X*2)/HEIGHT - X)

// nombre complexe C
long double re = 0.285 , im = 0.01;

// nombre d'itérations
int it = 300;

// Variable HSV pour la coloration
double H;
double S = 0.80;
double V = 0.80;

// un nouvel objet image (ordre des couleurs : bleu/vert/rouge)
cv::Mat newImg(WIDTH, HEIGHT, CV_8UC3);
// déclaration de la couleur noir
cv::Vec3b black(0,0,0);

void* julia(void* arg)
{
  int id = *((int*) arg);

  long double x, y, tmp;

  int p;

  for (int i = 0 ; i < HEIGHT ; i++){

    //si la ligne modulo le nombre de thread est égale à l'id du thread, on lance le calcul. Si non, on ne fait rien (permet de partager le travail).
    if (i%NB_THREAD == id){
      for (int j = 0 ; j < WIDTH ; j++){
        x = convertx(i);
        y = converty(j);
        p = 0;

        while (x*x + y*y < 4 && p < it)
        {
          tmp = (x * x - y * y) + re;
          y = (x * y + x * y) + im;
          x = tmp;
          p++;
        }

        //calcul de la couleur en HSV
        H = (p*360)/it;
        HSV data = HSV(H,S,V);

        //conversion de HSV vers RGB
        RGB value = HSVToRGB(data);
        cv::Vec3b color(value.R, value.G, value.B);

        if(p!=it)
        {
          newImg.at<cv::Vec3b>(cv::Point(i, j)) = color;
        }
        else
        {
          newImg.at<cv::Vec3b>(cv::Point(i, j)) = black;
        }
      }
    }
  }
  return NULL;
}


int main(int argc, char* argv[])
{
  pthread_t julia_fract[NB_THREAD];
  int julia_id[NB_THREAD];


  refresh:
  for (int i = 0 ; i < NB_THREAD ; i++) {
    julia_id[i] = i;
    pthread_create(&julia_fract[i], NULL, julia, (void*) &julia_id[i]);
  }

  //Gestion des entrées
  while(char key = cvWaitKey(66)) {
    switch(key){
      case 'a':
      for (int i = 0 ; i < NB_THREAD ; i++) {
        pthread_cancel(julia_fract[i]);
      }

      /* Modification de la fractale à la volée */
      re = re + 0.01;
      goto refresh;
      break;
      case 'z':
      for (int i = 0 ; i < NB_THREAD ; i++) {
        pthread_cancel(julia_fract[i]);
      }
      re = re - 0.01;
      goto refresh;
      break;
      case 'o':
      for (int i = 0 ; i < NB_THREAD ; i++) {
        pthread_cancel(julia_fract[i]);
      }
      im = im + 0.01;
      goto refresh;
      break;
      case 'p':
      for (int i = 0 ; i < NB_THREAD ; i++) {
        pthread_cancel(julia_fract[i]);
      }
      im = im - 0.01;
      goto refresh;
      break;

      /* Zoom/Dezomm */
      case 'd':
      for (int i = 0 ; i < NB_THREAD ; i++) {
        pthread_cancel(julia_fract[i]);
      }
      X += 0.05;
      Y -= 0.05;
      goto refresh;
      break;
      case 's':
      for (int i = 0 ; i < NB_THREAD ; i++) {
        pthread_cancel(julia_fract[i]);
      }
      X -= 0.05;
      Y += 0.05;
      goto refresh;
      break;

      default:
      break;
    }
    if (key == 'q')
    break;
    imshow("image", newImg);
  }
  cvDestroyWindow("image"); // ferme la fenêtre

  return 0;
}

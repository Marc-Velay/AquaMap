#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include<opencv/cv.h>


/*
 * COMPILE USING gcc -Wall testOpenCV.c -o test `pkg-config --libs --cflags opencv` 
 */


/*
void cvShowImage(const char* name, const CvArr* image)

int cvNamedWindow(const char* name, int flags=CV_WINDOW_AUTOSIZE )
void cvDestroyWindow(const char* name)
int cvWaitKey(int delay=0 )
*/


int main(int argc, char** argv) {
  
  IplImage* img=cvLoadImage("ImagesPST/G0020024.JPG", CV_LOAD_IMAGE_GRAYSCALE);

  if (img==NULL){
    printf("Caramba, pas vu pas pris!\n");
    return(1);
  }

//Partie traitement d'image ici

  cvNamedWindow("Exemple image", CV_WINDOW_NORMAL);

  cvShowImage("Exemple image",img);

  cvWaitKey(0);

  cvReleaseImage(&img);

  cvDestroyWindow("Exemple image");

  return(0);
}














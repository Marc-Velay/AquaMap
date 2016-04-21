#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <dirent.h>


#define abs(x) ((x) > 0 ? (x) : -(x))
#define IMGCONST 3

int h, s, v, tolerance = 10;
IplImage* img = NULL;

//gcc -Wall color.c `pkg-config --libs --cflags opencv` -o color


char* concat(char *s1, char *s2)
{
	char *result = malloc(strlen(s1)+strlen(s2)+1);
	//in real code you would check for errors in malloc here but
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}


void getObjectColor(int event, int x, int y, int flags, void *param ) {

    // Vars
    CvScalar pixel;
    IplImage *hsv;

    if(event == CV_EVENT_LBUTTONUP) {

        // Get the hsv image
        hsv = cvCloneImage(img);
        cvCvtColor(img, hsv, CV_BGR2HSV);

        // Get the selected pixel
        pixel = cvGet2D(hsv, y*IMGCONST, x*IMGCONST);

        // Change the value of the tracked color with the color of the selected pixel
        h = (int)pixel.val[0];
        s = (int)pixel.val[1];
        v = (int)pixel.val[2];

		printf("h: %d, s: %d, v: %d \n", h, s, v);
        // Release the memory of the hsv image
            cvReleaseImage(&hsv);

    }

}


int main(int argc, char** argv)
{
	IplImage* dst = NULL;
	IplImage* dst2 = NULL;
	IplImage* dst3 = NULL;
	IplImage* dst4 = NULL;
	IplImage* dst5 = NULL;
	IplImage* dst6 = NULL;
	IplImage* sum = NULL;
	IplImage* sum1 = NULL;

	int row,col;
	int x, y;

	char* cha = "./temp/";//le dossier de base
	DIR* rep = NULL;
	struct dirent* fichierLu = NULL; /* Déclaration d'un pointeur vers la structure dirent. */
	struct dirent* repLu = NULL; /* Déclaration d'un pointeur vers la structure dirent. */
	char* ch;
	rep = opendir(cha);
	if (rep == NULL)
	{
		exit(1);
	}
	while((repLu = readdir(rep)) != NULL )
	{
		while (strcmp(repLu->d_name, ".") == 0 || strcmp(repLu->d_name, "..") == 0)
		{
			repLu = readdir(rep);
			printf("%s\n",repLu->d_name);
		}
		DIR* rep2 = NULL;
		ch = concat(cha,repLu->d_name);
		rep2 = opendir(ch);
		char* c = ch;
		c = concat(ch,"/");
		while ((fichierLu = readdir(rep2)) != NULL )
		{
			if (strcmp(fichierLu->d_name, ".") != 0 && strcmp(fichierLu->d_name, "..") != 0)
			{
				printf("Le fichier lu s'appelle %s\n", fichierLu->d_name);

				ch = concat(c,fichierLu->d_name);
				img = cvLoadImage( ch , CV_LOAD_IMAGE_COLOR);
				if (img == NULL)
				{
					printf("Oups j'ai eu un problème.\n");
					return -1;
				}


				int sommeX=0, sommeY=0;
				int nbPixel =0;

				IplImage *hsv;
				hsv = cvCloneImage(img);
				cvCvtColor(img, hsv, CV_BGR2HSV);
				IplImage *hsv2;
				hsv2 = cvCloneImage(img);
				cvCvtColor(img, hsv2, CV_BGR2HSV);
				IplImage *hsv3;
				hsv3 = cvCloneImage(img);
				cvCvtColor(img, hsv3, CV_BGR2HSV);

				IplImage* imgG = cvCloneImage(img);

				unsigned char *pRowC, *pRowG;
				for(row=0; row<img->height; row++)
				{
					pRowC = (unsigned char*)(img->imageData + row * img->widthStep);
					pRowG = (unsigned char*)(imgG->imageData + row * imgG->widthStep);
					for(col=0; col<img->width; col++)
					{
						// pour le canal B de imgG
						*(pRowG + col*imgG->nChannels*sizeof(unsigned char)) = (unsigned char) (*(pRowC + col*img->nChannels*sizeof(unsigned char))* 0.114 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+1)* 0.587 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+2)* 0.299);
						// pour le canal G de imgG
						*(pRowG + col*imgG->nChannels*sizeof(unsigned char) + 1) = (unsigned char) (*(pRowC + col*img->nChannels*sizeof(unsigned char))* 0.114 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+1)* 0.587 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+2)* 0.299);
						// pour le canal R de imgG
						*(pRowG + col*imgG->nChannels*sizeof(unsigned char) + 2) = (unsigned char) (*(pRowC + col*img->nChannels*sizeof(unsigned char))* 0.114 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+1)* 0.587 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+2)* 0.299);
					}
				}

				IplImage* imgE = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
				cvSobel(imgG, imgE, 1, 0, 3);
				cvThreshold(imgE,imgG,90,255,3);
				cvErode(imgG,imgE,NULL,1);
				cvThreshold(imgE,imgG,100,255,3);
				cvDilate(imgG,imgE,NULL,5);


				IplImage *maskrouge;
				maskrouge = cvCreateImage(cvGetSize(img), img->depth, 1);
				IplImage *maskbleu;
				maskbleu = cvCreateImage(cvGetSize(img), img->depth, 1);
				IplImage *maskblanc;
				maskblanc = cvCreateImage(cvGetSize(img), img->depth, 1);
				IplImage *maskSobel;
				maskSobel = cvCreateImage(cvGetSize(img), img->depth, 1);

				int h1=140, s1=35, v1=200;
				int h2=100, s2=185, v2=250;
				int h3=350, s3=83;
				cvInRangeS(hsv, cvScalar(h1 -tolerance, s1 - tolerance, v1 -tolerance, 0.0), cvScalar(h1 + tolerance, s1 + tolerance, v1 + tolerance, 0.0), maskrouge);
				cvInRangeS(hsv2, cvScalar(h2 -tolerance, s2 - tolerance, v2 -tolerance, 0.0), cvScalar(h2 + tolerance, s2 + tolerance, v2 +tolerance, 0.0), maskbleu);
				cvInRangeS(hsv3, cvScalar(h3 -tolerance, s3 - tolerance, 0, 0.0), cvScalar(h3 + tolerance, s3 + tolerance, 255, 0.0), maskblanc);
				cvInRangeS(imgE, cvScalar(1, 0, 0, 0.0), cvScalar(255, 255, 255, 0.0), maskSobel);

				IplConvKernel *kernel;
				kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE, NULL);


				cvDilate(maskrouge, maskrouge, kernel, 1);
				cvDilate(maskbleu, maskbleu, kernel, 1);
				cvErode(maskbleu, maskbleu, kernel, 1);
				//cvDilate(maskblanc, maskblanc, kernel, 1);
				//cvDilate(maskrouge, maskrouge, kernel, 1);
				//cvDilate(maskbleu, maskbleu, kernel, 1);
				//cvDilate(maskblanc, maskblanc, kernel, 1);
				//cvErode(maskrouge, maskrouge, kernel, 1);

				// affichage

				dst = cvCreateImage(cvSize( img->width / IMGCONST, img->height / IMGCONST ), img->depth,img->nChannels );
				dst2 = cvCreateImage(cvSize( maskrouge->width / 6, maskrouge->height / 6 ), maskrouge->depth,maskrouge->nChannels );
				dst3 = cvCreateImage(cvSize( maskbleu->width / 6, maskbleu->height / 6 ), maskbleu->depth,maskbleu->nChannels );
				sum = cvCreateImage(cvSize(maskrouge->width, maskrouge ->height), maskrouge->depth, maskrouge->nChannels);
				sum1 = cvCreateImage(cvSize(maskrouge->width, maskrouge ->height), maskrouge->depth, maskrouge->nChannels);

				cvAddWeighted(maskbleu, 1.0, maskrouge, 1.0, 0.0, sum1);
				cvAddWeighted(maskSobel, 1.0, sum1, 1.0, 0.0, sum);
				//cvAdd(imgE, sum1, sum, NULL);

				for(x = 0; x < sum->width; x++) {
					for(y = 0; y < sum->height; y++) {

						// If its a tracked pixel, count it to the center of gravity's calcul
						if(((uchar *)(sum->imageData + y*sum->widthStep))[x] == 255) {
							sommeX += x;
							sommeY += y;
							(nbPixel)++;
						}
					}
				}
				printf("%d \n", nbPixel);

				IplImage* result = cvCloneImage(img);

				// Draw an object (circle) centered on the calculated center of gravity
				if (nbPixel > 800) {
					CvPoint objectPos = cvPoint(sommeX/nbPixel, sommeY/nbPixel);
					cvDrawCircle(result, objectPos, 150, CV_RGB(255, 0, 0), 10, 8, 0);
				}
				nbPixel=0;

				dst4 = cvCreateImage(cvSize( maskblanc->width / 6, maskblanc->height / 6 ), result->depth, result->nChannels );
				dst5 = cvCreateImage(cvSize( sum->width / 6, sum->height / 6 ), sum->depth, sum->nChannels );
				dst6 = cvCreateImage(cvSize( imgE->width / 6, imgE->height / 6 ), imgE->depth, imgE->nChannels );
				cvResize(img, dst, CV_INTER_AREA );
				cvResize(maskrouge, dst2, CV_INTER_AREA );
				cvResize(maskbleu, dst3, CV_INTER_AREA );
				cvResize(result, dst4, CV_INTER_AREA );
				cvResize(sum, dst5, CV_INTER_AREA );
				cvResize(imgE, dst6, CV_INTER_AREA );

				/*cvShowImage("Originale",dst);
				cvShowImage("Rouge",dst2);
				cvShowImage("Bleu",dst3);
				cvShowImage("Blanc",dst4);
				cvShowImage("Sobel", dst6);
				cvShowImage("Result",dst5);
				cvMoveWindow("Originale" ,0,0);
				cvMoveWindow("Rouge" ,dst->width,dst->height+60);
				cvMoveWindow("Bleu" ,dst->width,0);
				cvMoveWindow("Blanc" ,0,dst->height+60);
				cvMoveWindow("Sobel" ,dst->width *2,dst->height+60);
				cvMoveWindow("Result", dst->width *2, 0);*/



				cvSetMouseCallback("Originale", getObjectColor, NULL);
				//cvWaitKey(0);


				cvReleaseImage(&maskrouge);
				cvReleaseImage(&maskbleu);
				cvReleaseImage(&maskblanc);
				cvReleaseImage(&maskSobel);
				cvReleaseImage(&imgE);
				cvReleaseImage(&result);
				cvReleaseImage(&imgG);
				cvReleaseImage(&hsv);
				cvReleaseImage(&hsv2);
				cvReleaseImage(&hsv3);
				cvReleaseImage(&img);
				cvReleaseImage(&dst);
				cvReleaseImage(&dst2);
				cvReleaseImage(&dst3);
				cvReleaseImage(&dst4);
				cvReleaseImage(&dst5);
				cvReleaseImage(&dst6);
				cvReleaseImage(&sum);
				cvReleaseImage(&sum1);
				cvDestroyWindow("Originale");
				cvDestroyWindow("Rouge");
				cvDestroyWindow("Bleu");
				cvDestroyWindow("Blanc");
				cvDestroyWindow("Sobel");
				cvDestroyWindow("Result");
				//cvDestroyWindow("test5");
			}
		}
	}


	if (closedir(rep) == -1)
		exit(-1);

	return 0;
}

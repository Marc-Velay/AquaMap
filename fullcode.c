#include <stdio.h>
#include <opencv/highgui.h>
#include <dirent.h>

/*
//gcc -Wall test.c `pkg-config --libs --cflags opencv` -o h

int main(int argc, char** argv)
{
int row,col;

	IplImage* img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (img==NULL)
	{
		printf("File moi une image la prochaine fois.\n");
		return(1);
	}
	printf("arg2 = %s mais arg 1 = %s",argv[2],argv[1]);

	IplImage* imgG = cvCloneImage(img);
	if (argv[2]==NULL)
	{
		printf("Je peux pas sauvegarder !!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		return(1);
	}
	else 
		printf("Image sauvegardé\n");
	
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



	IplImage* dst = NULL;
	IplImage* dst2 = NULL;
	dst = cvCreateImage(cvSize( imgG->width / 5, imgG->height / 5 ), imgG->depth,imgG->nChannels );
    dst2 = cvCreateImage(cvSize( imgG->width / 5, imgG->height / 5 ), imgG->depth,imgG->nChannels );
	
	// traitements

	IplImage* imgE = cvCloneImage(img);
	cvSobel(imgG, imgE, 1, 0, 3);
	cvThreshold(imgE,imgG,80,255,3);
	cvErode(imgG,imgE,NULL,1);
	cvThreshold(imgE,imgG,100,255,3);
	cvDilate(imgG,imgE,NULL,5);
	

	//redimentionnement et sauvegardes
	
	cvResize(img, dst, CV_INTER_AREA );
	cvResize(imgE, dst2, CV_INTER_AREA );
	cvSaveImage(argv[2], imgE, 0);
	//cvSaveImage("image base.jpg", img, 0);


	// affichage
	cvShowImage("test2",dst);
	cvNamedWindow("test1", CV_WINDOW_AUTOSIZE);
	cvShowImage("test1",dst2);
	
	cvWaitKey(0);

	cvReleaseImage(&imgG);
	cvReleaseImage(&imgE);
	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	cvReleaseImage(&dst2);
	cvDestroyWindow("test1");
	cvDestroyWindow("test2");

	return(0);
} */


char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


	int main(int argc, char** argv)
	{
		char* ch = "./montpellier_mission_images_floues_nettes/";
	    DIR* rep = NULL;
	    struct dirent* fichierLu = NULL; /* Déclaration d'un pointeur vers la structure dirent. */

	    rep = opendir(ch);
	    if (rep == NULL)
	    {
	        exit(1);
	    }
	    fichierLu = readdir(rep); /* On lit le premier répertoire du dossier. */
	    printf("%s\n",fichierLu->d_name);

	    DIR* rep2 = NULL;
	    ch =(concat(ch,fichierLu->d_name));
	    rep2 = opendir(ch);
	    fichierLu = readdir(rep2); /* On lit le premier "truc" */
	    printf("%s\n",fichierLu->d_name);
	    if (closedir(rep) == -1)
	        exit(-1);

	    return 0;
	}

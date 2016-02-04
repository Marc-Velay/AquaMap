#include <opencv/highgui.h>

//gcc -Wall test.c `pkg-config --libs --cflags opencv` -o h

int main(int argc, char** argv)
{

	int row,col,x;
	int threshold = 100;
	IplImage* img = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	IplImage* imgG = cvCloneImage(img);
	IplImage* imgF = cvCloneImage(img);
	
	if (img==NULL)
	{
		printf("File moi une image.");
		return(1);
	}
	
	unsigned char *pRowC, *pRowG;
	for(row=0; row<img->height; row++)
	{
		pRowC = (unsigned char*)(img->imageData + row * img->widthStep);
		pRowG = (unsigned char*)(imgF->imageData + row * imgF->widthStep);
		for(col=0; col<img->width; col++)
		{
			// pour le canal B de imgG 
			*(pRowG + col*imgF->nChannels*sizeof(unsigned char)) = (unsigned char) (*(pRowC + col*img->nChannels*sizeof(unsigned char))* 0.114 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+1)* 0.587 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+2)* 0.299);
			// pour le canal G de imgG 
			*(pRowG + col*imgF->nChannels*sizeof(unsigned char) + 1) = (unsigned char) (*(pRowC + col*img->nChannels*sizeof(unsigned char))* 0.114 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+1)* 0.587 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+2)* 0.299);
			// pour le canal R de imgG 
			*(pRowG + col*imgF->nChannels*sizeof(unsigned char) + 2) = (unsigned char) (*(pRowC + col*img->nChannels*sizeof(unsigned char))* 0.114 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+1)* 0.587 + *(pRowC + col*img->nChannels*sizeof(unsigned char)+2)* 0.299);
		}
	}

	x = cvLaplace(imgG, imgF, 3);
	printf("%d",x);
	cvShowImage("image",img);
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	cvShowImage("La place image",imgF);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&imgG);
	cvDestroyWindow("image");
	cvDestroyWindow("La place image");
	return 0;
}
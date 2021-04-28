#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void insertionSort(int newimageData[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = newimageData[i];
        j = i - 1;

        /* Move elements of new imageData[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && newimageData[j] > key)
        {
            newimageData[j + 1] = newimageData[j];
            j = j - 1;
        }
        newimageData[j + 1] = key;
    }
}
int main()
{
	clock_t start, stop;
	start=clock();
	FILE* fp = fopen("images/lena_salt.bmp", "rb");   //read the file//

	unsigned char *imageData; // to store the image information
	unsigned char *newimageData; // to store the new image information, i.e. the negative image
        unsigned char imageHeader[54]; // to get the image header
        unsigned char colorTable[1024]; // to get the colortable

	int i,j,window[9]; // loop counter variables

	fread(imageHeader, sizeof(unsigned char), 54, fp); // read the 54-byte from fp to imageHeader


	// extract image height and width from imageHeader
        int width = *(int*)&imageHeader[18];
        int height = *(int*)&imageHeader[22];
	int bitDepth = *(int*)&imageHeader[28];

        int imgDataSize = width * height; // calculate image size

        imageData = (unsigned char*)malloc (imgDataSize * sizeof(unsigned char)); // allocate the block of memory as big as the image size
        newimageData = (unsigned char*)malloc (imgDataSize * sizeof(unsigned char));

	if(bitDepth <= 8)	// COLOR TABLE Present
		fread(colorTable, sizeof(unsigned char), 1024, fp); // read the 1024-byte from fp to colorTable


	fread( imageData, sizeof(unsigned char), imgDataSize, fp);

	printf("%d w: %d\n",height,width);
	//Calculate the mean of the image
	for(i = 0; i < height; i++){
	      for(j = 0; j < width; j++){
		    newimageData[i*width + j] =imageData[i*width + j];
		 }
	}
    for(i = 1; i < height-1; i++)
    {
        for(j = 1; j < width; j++)
        {
            //neighbor pixel values are stored in window including this pixel
            window[0] = newimageData[i*width + j - 1];
            window[1] = newimageData[i*width + j + 0];
            window[2] = newimageData[i*width + j + 1];
            window[3] = newimageData[i*width + j + 2];
            window[4] = newimageData[i*width + j + 3];
            window[5] = newimageData[i*width + j + 4];
            window[6] = newimageData[i*width + j + 1];
            window[7] = newimageData[i*width + j + 5];
            window[8] = newimageData[i*width + j + 6];

            //sort window newimageData
            insertionSort(window,9);
            //put the median to the new newimageData
            newimageData[i*width + j]=window[4];
        }
    }

	FILE *fo = fopen("images/filtered.bmp", "wb");// Name of output file

        fwrite(imageHeader, sizeof(unsigned char), 54, fo); // write the header back.

	if(bitDepth <= 8)	// COLOR TABLE Present
        	fwrite(colorTable, sizeof(unsigned char), 1024, fo); // write the color table back

        fwrite( newimageData, sizeof(unsigned char), imgDataSize, fo); // write the values of the negative image.

        fclose(fo);
	fclose(fp);
        stop = clock();
	double d = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
        printf("%lf\n",d);

}

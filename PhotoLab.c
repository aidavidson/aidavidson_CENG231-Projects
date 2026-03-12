/* PhotoLab.c: Assignment 5 for CENG231/L, Fall 2025                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** global definitions ***/
#define WIDTH  800              /* image width */
#define HEIGHT 450              /* image height */
#define SLEN    80              /* maximum length of file names */
#define KERNELW  3

/*** function declarations ***/

/* Load an image from a file */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT],
        unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* Save a processed image */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT],
        unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* Age the image */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
                unsigned char B[WIDTH][HEIGHT]);

/* Change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

/* Reverse the image color */
void Negative(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

/* Flip an image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

void pixelFlip(unsigned char C[WIDTH][HEIGHT], int x, int y, int destY);

/* Mirror an image horizontally */
void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

/* Exchange G and B color channels */
void ExGB(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

/* [Bonus] Sharpen an image */
void Sharpen(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

unsigned char Clamp(int value);

int pixelMod(unsigned char C[WIDTH][HEIGHT], int y, int x, int sharpenKernel[KERNELW][KERNELW], int kernelRadius );

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];

    /* Please extend the AutoTest function for the other DIP operations */
    AutoTest(R, G, B);

    printf("---------------------------------------------\n");
    printf("All DIP Operations:\n");
    printf("- Age the image \n");
    printf("- Change the color image to black and white\n");
    printf("- Make a negative of the image\n");
    printf("- Flip the image vertically\n");
    printf("- Mirror the image horizontally\n");
    printf("- Exchange the green and blue color channels\n");
    printf("- Sharpen the image\n ");
    printf("---------------------------------------------\n");
    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File)
    {
     	printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0)
    {
     	printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH)
    {
     	printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT)
    {
     	printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255)
    {
     	printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File))
    {
     	printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++)
    {
     	for (x = 0; x < WIDTH; x++)
        {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File))
    {
     	printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File)
    {
     	printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++)
    {
     	for (x = 0; x < WIDTH; x++)
        {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File))
    {
     	printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/ceng231/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~ceng231/bin/pnmtojpeg.sh %s", fname_ext);
    if (system(SysCmd) != 0)
    {
     	printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/* Example: age the image to make it look like old picture */
void Aging(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    int x, y;

    for( y = 0; y < HEIGHT; y++ )
        {
	for( x = 0; x < WIDTH; x++ )
        {
            B[x][y] = ( R[x][y]+G[x][y]+B[x][y] )/5;
            R[x][y] = (unsigned char) (B[x][y]*1.6);
            G[x][y] = (unsigned char) (B[x][y]*1.6);
            }
    }
}

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{

    /*loads initial Image*/
    LoadImage("argyros", R, G, B);
    /*applies filter of aging on the three arrays of R,G and B values*/
    Aging(R, G, B);
    /*saves image post filter*/
    SaveImage("aging", R, G, B);
    /*prints to show that the filter was tested*/
    printf("Aging tested!\n\n");



    /*loads initial Image*/
    LoadImage("argyros", R, G, B);
    /*applies the black and white filter onto the three arrays of R, G and B values making each channel the same value*/
    BlackNWhite(R, G, B);
    /*saves the image post filter*/
    SaveImage("bw", R, G, B);
    /*prints to show that the filter was tested*/
    printf("Black and White tested!\n\n");



    /*loads initial Image*/
    LoadImage("argyros", R, G, B);
    /* applies the negative filter subtracting each of the R, G, and B channels from 255 to flip their value*/
    Negative(R, G, B);
    /* saves the image post filter*/
    SaveImage("negative", R, G, B);
    /* prints to show the filter was tested*/
    printf("Negative tested!\n\n");



    /*loads initial image*/
    LoadImage("argyros", R, G, B);
    /*applies the vflip filter taking the pixel at the top and exchanging them for their counterpart at the bottom half this image */
    /*indexes from the top half to the bottom half*/
    VFlip(R, G, B);
    /* saves the image post filter*/
    SaveImage("vflip", R, G, B);
    /* prints to show the filter was tested*/
    printf("Vertical Flip tested!\n\n");



    /*loads initial Image*/
    LoadImage("argyros", R, G, B);
    /* takes the left half of the image and overwrites the right half of the image with it */
    HMirror(R, G, B);
    /* saves the image post filter*/
    SaveImage("hmirror", R, G, B);
    /* prints to show the filter was tested*/
    printf("Horizontal Mirror tested!\n\n");



    /*loads initial Image*/
    LoadImage("argyros", R, G, B);
    /* applies the exchange G and B value filter which takes the G value of each pixel using temp exchange holds it */
    /* while taking the B value moving it into the G value*/
    ExGB(R, G, B);
    /* saves the image post filter*/
    SaveImage("xGB", R, G, B);
    /* prints to show the filter was tested*/
    printf("Exchange Green and Blue tested!\n\n");



    /*loads initial Image*/
    LoadImage("argyros", R, G, B);
    /*applies the sharpen filter taking the Center Pixel E which is then subracted by all other pixels surrounding it*/
    Sharpen(R, G, B);
    /* saves the image post filter*/
    SaveImage("sharpen", R, G, B);
    /* prints to show the filter was tested*/
    printf("Sharpen tested!\n\n");
}

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/


void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
                 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT])
{
        int x,y,pixelValue = 0;
        for(y = 0; y < HEIGHT; y++)
        {
                for(x = 0; x < WIDTH; x++)
                {

                        pixelValue = (R[x][y] + G[x][y] + B[x][y])/3;
                        R[x][y] = pixelValue;
                        G[x][y] = pixelValue;
                        B[x][y] = pixelValue;

                }/*rof*/

        }/*rof*/

}/*etihWNkcalB*/

void Negative(unsigned char R[WIDTH][HEIGHT],
              unsigned char G[WIDTH][HEIGHT],
              unsigned char B[WIDTH][HEIGHT]){
        int x,y = 0;
        for(y = 0; y < HEIGHT; y++)
        {
                for(x = 0; x < WIDTH; x++)
                {
                        R[x][y] = 255 - R[x][y];
                        G[x][y] = 255 - G[x][y];
                        B[x][y] = 255 - B[x][y];
                }/*rof*/
        }/*rof*/
}/*evitageN*/

/*function should swap each identified pixel by their counterpart on the opposite half of the image*/
void VFlip(unsigned char R[WIDTH][HEIGHT],
           unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT])
{
        int x, y, destY;
        for(y = 0; y < HEIGHT/2; y++)
        {
                for(x = 0; x < WIDTH; x++)
                {
                        destY = HEIGHT - y - 1;

                        pixelFlip(R,x,y,destY);
                        pixelFlip(G,x,y,destY);
                        pixelFlip(B,x,y,destY);

                }/*rof*/

        }/*rof*/

}/*plifV*/

void pixelFlip(unsigned char C[WIDTH][HEIGHT], int x, int y, int destY)
{

        int temp = C[x][y];
        C[x][y] = C[x][destY];
        C[x][destY] = temp;

}/*pilFlexip*/

void HMirror(unsigned char R[WIDTH][HEIGHT],
           unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT])
{

        int x, y, destX;

        for(y = 0; y < HEIGHT; y++)
        {

                for(x = 0; x < WIDTH/2; x++)
                {

                        destX = WIDTH - x - 1;

                        R[destX][y] = R[x][y];
                        G[destX][y] = G[x][y];
                        B[destX][y] = B[x][y];

                }/*rof*/

        }/*rof*/

}/*rorriMh*/








void Sharpen(unsigned char R[WIDTH][HEIGHT],
             unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT])
{
        /*sharpen filter kernel which allows code to hold any value to manipulate the image*/
        int sharpenKernel[KERNELW][KERNELW] =
                                  {{-1,-1,-1},
                                   {-1, 9,-1},
                                   {-1,-1,-1}};
        /*for loop variables and the radius of the kernel*/
        int x , y = 0;
        int kernelRadius = 1;

        /*temporary R, G and B arrays so the original content passed in isn't destroyed*/
        unsigned char tempR[WIDTH][HEIGHT],tempG[WIDTH][HEIGHT], tempB[WIDTH][HEIGHT];

        /* both for loops will start at kernel radius and end at the max minus kernel Radius for edge handling*/
        /* this is because the kernel utilises surrounding values to manipulate the center pixel*/
        /* if the pixel is on the edge of the image there aren't 8 surrounding pixels which would mess*/
        /* up the algorithm*/
        for(y = kernelRadius; y < HEIGHT - kernelRadius; y++)
        {

                for(x = kernelRadius; x < WIDTH - kernelRadius; x++)
                {
                        /*Clamp: the temporary array of R,G, and B channels are set to the clamped off value of pixel*/
                        /* mod the clamp will ensure that if any pixel is over or undersaturated over 255 or under 0*/
                        /* they are then set to the min or max value depending on their saturation*/
                        /*pixelMod: used to use the kernel on all pixels surrounding the center to get the total sum*/\
                        /* for this function to run the color channel, kernel, radius and loop values are necessary*/
                        tempR[x][y] = Clamp(pixelMod(R,y,x,sharpenKernel, kernelRadius));
                        tempG[x][y] = Clamp(pixelMod(G,y,x,sharpenKernel, kernelRadius));
                        tempB[x][y] = Clamp(pixelMod(B,y,x,sharpenKernel, kernelRadius));

                }/*rof*/

        }/*rof*/

        /*because the current array is already filled with current pixel values the temp arrays should refill the*/
        /*current values except for the edges which is why*/
        for(y = kernelRadius; y < HEIGHT - kernelRadius; y++)
        {

                for(x = kernelRadius; x < WIDTH - kernelRadius; x++)
                {
                        /*assigns the array values to the temps within the right range*/
                        R[x][y] = tempR[x][y];
                        G[x][y] = tempG[x][y];
                        B[x][y] = tempB[x][y];

                }/*rof*/

        }/*rof*/

}/*neprhS*/

//type stuff

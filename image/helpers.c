#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double av;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
         av = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
         image[i][j].rgbtBlue = round(av);
         image[i][j].rgbtGreen = round(av);
         image[i][j].rgbtRed = round(av);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    double temp;
    int red,blue,green;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j ++)
        {
           temp = 0.189 * image[i][j].rgbtBlue + 0.769 * image[i][j].rgbtGreen + 0.393 * image[i][j].rgbtRed;
           if(temp > 255)
           {
             red = 255;
           }
           else
           {
             red = round(temp);
           }
           temp = 0.168 * image[i][j].rgbtBlue + 0.686 * image[i][j].rgbtGreen + 0.349 * image[i][j].rgbtRed;
           if(temp > 255)
           {
            green = 255;
           }
           else
           {
            green = round(temp);
           }
           temp = 0.131 * image[i][j].rgbtBlue + 0.534 * image[i][j].rgbtGreen + 0.272 * image[i][j].rgbtRed;
           if(temp > 255)
           {
            blue = 255;
           }
           else
           {
            blue = round(temp);
           }
           image[i][j].rgbtRed = red;
           image[i][j].rgbtBlue = blue;
           image[i][j].rgbtGreen = green;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width-1-j];
            image[i][width-1-j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int blue = 0;
    int red = 0;
    int green = 0;
    double tmp = 0;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {

            if(i == 0 && j == 0)
            {
                for(int z = 0; z <= 1; z++)
            {
                for(int t = 0; t <= 1; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i == 0 && j == width - 1)
            {
                for(int z = 0 ; z <= 1; z++)
            {
                for(int t = -1; t <= 0; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i == 0 && j > 0 && j < width - 1)
            {
                for(int z = 0 ; z <= 1; z++)
            {
                for(int t = -1; t <= 1; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i > 0 && i < height - 1 && j == 0)
            {
                for(int z = -1 ; z <= 1; z++)
            {
                for(int t = 0; t <= 1; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i > 0 && i < height - 1 && j == width - 1)
            {
                for(int z = -1 ; z <= 1; z++)
            {
                for(int t = -1; t <= 0; t++)
            {
                blue = blue + copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;

            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i > 0 && i < height - 1 && j > 0 && j < width - 1)
            {
                for(int z = -1 ; z <= 1; z++)
            {
                for(int t = -1; t <= 1; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i == height - 1 && j == width - 1)
            {
                for(int z = -1; z <= 0; z++)
            {
                for(int t = -1; t <= 0; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else if(i == height - 1 && j > 0 && j < width - 1)
            {
                for(int z = -1; z <= 0; z++)
            {
                for(int t = -1; t <= 1; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }

            else
            {
                for(int z = -1; z <= 0; z++)
            {
                for(int t = 0; t <= 1; t++)
            {
                blue += copy[i+z][j+t].rgbtBlue;
                green += copy[i+z][j+t].rgbtGreen;
                red += copy[i+z][j+t].rgbtRed;
                tmp++;
            }
            }
            image[i][j].rgbtBlue = round(blue / tmp);
            image[i][j].rgbtGreen = round(green / tmp);
            image[i][j].rgbtRed = round(red / tmp);
            red = 0;
            blue = 0;
            green = 0;
            tmp = 0;
            }


        }
    }
    return;
}

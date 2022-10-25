/// @file main.c
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION 
#include "./headers/stb_image.h" 
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "./headers/stb_image_write.h"
#include <math.h>

/**
* Delete a quarter of the image
* @param[in] image the input image
* @param[in] width the width of the image
* @param[in] height the height of the image
* @param[in] channel the channel of the image */

unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char *subtract_image(unsigned char *image, unsigned char *observed_image, int width, int height, int channel)
{
	unsigned subtraction_channel = 0;
	unsigned char *temp_array = uc_arrayNew_1d(width * height * channel);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channel; k++)
			{
				if (abs(observed_image[i * width * channel + j * channel + 1] - image[i * width * channel + j * channel + 1]) <= 79 &&
					abs(observed_image[i * width * channel + j * channel + 2] - image[i * width * channel + j * channel + 2]) <= 79 &&
					abs(observed_image[i * width * channel + j * channel + 3] - image[i * width * channel + j * channel + 3]) <= 79)
				{
					temp_array[i * width * channel + j * channel + k] = 0;
		
				}
				else
				{
					temp_array[i * width * channel + j * channel + k] = observed_image[i * width * channel + j * channel + k];
				} 
			}
		}	
	}
	return temp_array;
}
unsigned char* add_background(unsigned char* no_background, unsigned char* new_background, int width, int height, int channel)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < channel; k++)
			{
				if (no_background[i * width * channel + j * channel + k] != 0)
				{
					new_background[i * width * channel + j * channel + k] = no_background[i * width * channel + j * channel + k];
				}
			}
		}
	}
	return new_background;
}

int main()
{
	// declare variables
	int width, height, channel;
	char path_img[] = "./images/background.jpg";
	char path_observed_image[] = "./images/foreground.jpg";
	char path_new_background[] = "./images/new_background.jpg";
	char save_path[] = "./images/new_image.png";
	char save_path1[] = "./images/new_image2.png";

	// read image data
	unsigned char *image = stbi_load(path_img, &width, &height, &channel, 0);
	unsigned char *observed_image = stbi_load(path_observed_image, &width, &height, &channel, 0);
	unsigned char *new_background = stbi_load(path_new_background, &width, &height, &channel, 0);

	if (image == NULL)
	{
		printf("\nError in loading the image\n");
		exit(1);
	}
	printf("Width = %d\nHeight = %d\nChannel = %d\n", width , height , channel);

	// save image
	unsigned char *no_background = subtract_image(image, observed_image, width, height, channel);
	stbi_write_png(save_path, width, height, channel, no_background, width * channel);
    unsigned char *new_image = add_background(no_background, new_background, width, height, channel);
    stbi_write_png(save_path1, width, height, channel, new_image, width * channel);
    printf("New image saved to %s\n", save_path);
}

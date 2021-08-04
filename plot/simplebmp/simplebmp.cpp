// The MIT License (MIT)
//
// Copyright (c) 2015 Vallentin
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Date Created: January 03, 2014
// Last Modified: January 05, 2014
//
// Author: Vallentin <mail@vallentinsource.com>
//
// Github: https://github.com/VallentinSource/SimpleBMP

#include "simplebmp.h"

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <cstring>

int SimpleBMP::save(const int width, const int height, const unsigned char *pixels, const char *path)
{
	unsigned char bmp_file_header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, };
	unsigned char bmp_info_header[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, };
	unsigned char bmp_pad[3] = { 0, 0, 0, };
	const int size = 54 + width * height * 3;
	bmp_file_header[2] = static_cast<unsigned char>(size);
	bmp_file_header[3] = static_cast<unsigned char>(size >> 8);
	bmp_file_header[4] = static_cast<unsigned char>(size >> 16);
	bmp_file_header[5] = static_cast<unsigned char>(size >> 24);
	bmp_info_header[4] = static_cast<unsigned char>(width);
	bmp_info_header[5] = static_cast<unsigned char>(width >> 8);
	bmp_info_header[6] = static_cast<unsigned char>(width >> 16);
	bmp_info_header[7] = static_cast<unsigned char>(width >> 24);
	bmp_info_header[8] = static_cast<unsigned char>(height);
	bmp_info_header[9] = static_cast<unsigned char>(height >> 8);
	bmp_info_header[10] = static_cast<unsigned char>(height >> 16);
	bmp_info_header[11] = static_cast<unsigned char>(height >> 24);
	FILE *file = fopen(path, "wb");
	if (file)
	{
		fwrite(bmp_file_header, 1, 14, file);
		fwrite(bmp_info_header, 1, 40, file);
		for (int i = 0; i < height; i++)
		// for (int i = (height - 1); i >= 0; i--)
		{
			fwrite(pixels + (width * i * 3), 3, width, file);
			fwrite(bmp_pad, 1, ((4 - (width * 3) % 4) % 4), file);
		}
		fclose(file);
		return SIMPLEBMP_NO_ERROR;
	}
	return SIMPLEBMP_FOPEN_ERROR;
}

int SimpleBMP::load(int *width, int *height, unsigned char **pixels, const char *path)
{
	FILE *file = fopen(path, "rb");
	if (file)
	{
		unsigned char bmp_file_header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, };
		unsigned char bmp_info_header[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, };
		unsigned char bmp_pad[3] = { 0, 0, 0, };
		memset(bmp_file_header, 0, sizeof(bmp_file_header));
		memset(bmp_info_header, 0, sizeof(bmp_info_header));
		fread(bmp_file_header, sizeof(bmp_file_header), 1, file);
		fread(bmp_info_header, sizeof(bmp_info_header), 1, file);
		if ((bmp_file_header[0] != 'B') || (bmp_file_header[1] != 'M'))
		{
			fclose(file);
			return SIMPLEBMP_INVALID_SIGNATURE;
		}
		if ((bmp_info_header[14] != 24) && (bmp_info_header[14] != 32))
		{
			fclose(file);
			return SIMPLEBMP_INVALID_BITS_PER_PIXEL;
		}
		int w = (bmp_info_header[4] + (bmp_info_header[5] << 8) + (bmp_info_header[6] << 16) + (bmp_info_header[7] << 24));
		int h = (bmp_info_header[8] + (bmp_info_header[9] << 8) + (bmp_info_header[10] << 16) + (bmp_info_header[11] << 24));
		unsigned char *p = new unsigned char[w * h * 3];
		for (int i = 0; i < h; i++)
		// for (int i = (h - 1); i >= 0; i--)
		{
			fread(p + (w * i * 3), 3, w, file);
			fread(bmp_pad, 1, (4 - (w * 3) % 4) % 4, file);
		}
		(*width) = w;
		(*height) = h;
		(*pixels) = p;
		fclose(file);
		return SIMPLEBMP_NO_ERROR;
	}
	return SIMPLEBMP_FOPEN_ERROR;
}

void SimpleBMP::setPixel(const int width, const int height, unsigned char *pixels, const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)
{
	pixels[x * 3 + (height - y - 1) * width * 3 + 2] = red;
	pixels[x * 3 + (height - y - 1) * width * 3 + 1] = green;
	pixels[x * 3 + (height - y - 1) * width * 3 + 0] = blue;
}

void SimpleBMP::getPixel(const int width, const int height, const unsigned char *pixels, const int x, const int y, unsigned char *red, unsigned char *green, unsigned char *blue)
{
	if (red)   { (*red)   = pixels[x * 3 + (height - y - 1) * width * 3 + 2]; }
	if (green) { (*green) = pixels[x * 3 + (height - y - 1) * width * 3 + 1]; }
	if (blue)  { (*blue)  = pixels[x * 3 + (height - y - 1) * width * 3 + 0]; }
}

void SimpleBMP::setRGB(const int width, const int height, unsigned char *pixels, const int x, const int y, const int rgb)
{
	const int red = ((rgb >> 16) & 0xFF);
	const int green = ((rgb >> 8) & 0xFF);
	const int blue = (rgb & 0xFF);
	// const int alpha = ((rgb >> 24) & 0xFF);
	SimpleBMP::setPixel(width, height, pixels, x, y, static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue));
}

int SimpleBMP::getRGB(const int width, const int height, const unsigned char *pixels, const int x, const int y)
{
	unsigned char red, green, blue;

	SimpleBMP::getPixel(width, height, pixels, x, y, &red, &green, &blue);

	return ((red << 16) | (green << 8) | blue);
	// return ((alpha << 24) | (red << 16) | (green << 8) | blue);
}

// SimpleBMP::SimpleBMP(void) {}

SimpleBMP::SimpleBMP(const int square)
{
	this->setSize(square, square);
}

SimpleBMP::SimpleBMP(const int a_width, const int a_height, unsigned char *a_pPixels)
{
	if (a_pPixels)
	{
		this->setPixels(a_width, a_height, pixels);
	}
	else
	{
		this->setSize(a_width, a_height);
	}
}

SimpleBMP::~SimpleBMP(void)
{
	this->destroy();
}


int SimpleBMP::save(const char *path) const
{
	return SimpleBMP::save(this->width, this->height, this->pixels, path);
}

int SimpleBMP::load(const char *path)
{
	this->destroy();

	return SimpleBMP::load(&this->width, &this->height, &this->pixels, path);
}

void SimpleBMP::setPixel(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue) { SimpleBMP::setPixel(this->width, this->height, this->pixels, x, y, red, green, blue); }
void SimpleBMP::getPixel(const int x, const int y, unsigned char *red, unsigned char *green, unsigned char *blue) const { SimpleBMP::getPixel(this->width, this->height, this->pixels, x, y, red, green, blue); }
void SimpleBMP::setRGB(const int x, const int y, const int rgb) { SimpleBMP::setRGB(this->width, this->height, this->pixels, x, y, rgb); }
int SimpleBMP::getRGB(const int x, const int y) const { return SimpleBMP::getRGB(this->width, this->height, this->pixels, x, y); }

void SimpleBMP::setPixels(unsigned char *a_pPixels)
{
	for (int i = (this->width * this->height * 3 - 1); i >= 0; i--)
	{
		this->pixels[i] = a_pPixels[i];
	}
}

void SimpleBMP::setPixels(const int a_width, const int a_height, unsigned char *a_pPixels)
{
	this->setSize(a_width, a_height);
	this->setPixels(a_pPixels);
}

void SimpleBMP::setSize(const int a_width, const int a_height)
{
	this->destroy();
	this->width = a_width;
	this->height = a_height;
	this->pixels = new unsigned char[a_width * a_height * 3]; // 3 = R, G, B
}

void SimpleBMP::destroy(void)
{
	this->width = 0;
	this->height = 0;
	if (this->pixels)
	{
		delete[] this->pixels;

		this->pixels = nullptr;
	}
}

bool SimpleBMP::isValid(void) const
{
	if (this->pixels)
	{
		return ((this->width > 0) && (this->height > 0));
	}
	return true;
}

bool SimpleBMP::isValid(const int x, const int y) const
{
	if (this->isValid())
	{
		if (x < 0) { return false; }
		if (y < 0) { return false; }
		if (x >= this->width) { return false; }
		if (y >= this->height) { return false; }
		return true;
	}
	return false;
}

int SimpleBMP::getWidth(void) const { return this->width; }
int SimpleBMP::getHeight(void) const { return this->height; }
unsigned char* SimpleBMP::getPixels(void) const { return this->pixels; }

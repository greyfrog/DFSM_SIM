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

#ifndef SIMPLEBMP_H
#define SIMPLEBMP_H

#define SIMPLEBMP_NO_ERROR 0
#define SIMPLEBMP_FOPEN_ERROR 1

#define SIMPLEBMP_INVALID_SIGNATURE 2
#define SIMPLEBMP_INVALID_BITS_PER_PIXEL 3

class SimpleBMP
{
public:
	static int save(const int width, const int height, const unsigned char *pixels, const char *path);

	// Example:
	//
	// int width = 0, height = 0;
	// unsigned char *pixels = nullptr;
	//
	// bool success = simplebmp_load(&width, &height, &pixels);
	static int load(int *width, int *height, unsigned char **pixels, const char *path);

	static void setPixel(const int width, const int height, unsigned char *pixels, const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue);
	static void getPixel(const int width, const int height, const unsigned char *pixels, const int x, const int y, unsigned char *red, unsigned char *green, unsigned char *blue);

	static void setRGB(const int width, const int height, unsigned char *pixels, const int x, const int y, const int rgb);
	static int getRGB(const int width, const int height, const unsigned char *pixels, const int x, const int y);

	int width = 0, height = 0;
	unsigned char *pixels = nullptr;

	SimpleBMP(void) {}
	SimpleBMP(const int square);
	SimpleBMP(const int width, const int height, unsigned char *pixels = nullptr);
	~SimpleBMP(void);

	int save(const char *path) const;
	int load(const char *path);

	void setPixel(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue);
	void getPixel(const int x, const int y, unsigned char *red, unsigned char *green, unsigned char *blue) const;

	void setRGB(const int x, const int y, const int rgb);
	int getRGB(const int x, const int y) const;

	void setPixels(unsigned char *pixels);
	void setPixels(const int width, const int height, unsigned char *pixels);

	void setSize(const int width, const int height);

	void destroy(void);

	bool isValid(void) const;
	bool isValid(const int x, const int y) const;

	int getWidth(void) const;
	int getHeight(void) const;

	unsigned char* getPixels(void) const;
};

#endif

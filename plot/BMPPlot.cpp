/*! \file BMPPlot.cpp
 *  \brief Implementierung der Klasse BMPPlot
 *  \author cpr
 *  \date 2020
 */

#include "BMPPlot.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace sgnl;


BMPPlot::BMPPlot(const std::string& a_title) :
	plot_(1280, 800),
	nSignals_(0)
{
	if(font_.load("./myCode/plot/font/font.bmp") != 0)
	{
		cerr << "BMPPlot: Failed to load font.bmp in folder myCode/plot/font/font.bmp"
			 << endl;
	}
	fontSizeX_ = font_.getWidth()/69;
	fontSizeY_ = font_.getHeight();
	plotText(Coordinate{fontSizeX_, fontSizeY_}, a_title);
}

void BMPPlot::add(
	const std::string& a_signalName,
	const std::vector<sgnl::Volt>& a_signal)
{
	plotSignalName(a_signalName);
	plotAxes(a_signal.size());
	plotSignalValues(a_signal);
	nSignals_++;
	return;
}

void BMPPlot::setTimeTicks(sgnl::Nanoseconds a_spacing)
{
	plotText(
		Coordinate{2*fontSizeX_, fontSizeY_*3 + (fontSizeY_*(1+nSignals_*4))},
		"t/ns");
	for(int x=100; x<1280; x+=a_spacing)
	{
		plotPixels(
			bresenhamLine(
				Coordinate{x, fontSizeY_*3 + (fontSizeY_*(1+nSignals_*4))},
				Coordinate{x, 799}),
			Color{50, 50, 50});
		plotText(
			Coordinate{x+1, fontSizeY_*3 + (fontSizeY_*(1+nSignals_*4))},
			std::to_string(x-100));
	}
	return;
}

bool BMPPlot::save() const
{
	time_t tim = time(nullptr);
	struct tm stim = *localtime(&tim);
	stringstream ss;
	ss << std::put_time(&stim, "%d-%m-%Y_%H-%M-%S");
	string filename = "./Plots/";
	filename += ss.str();
	filename += ".bmp";
	if(plot_.save(filename.c_str()) != 0)
	{
		cerr << "Failed to save bitmap image " << filename << "." << endl;
		return false;
	}
	cout << "Bitmap image " << filename << " saved." << endl;
	return true;
}

void BMPPlot::plotPixels(
	const std::vector<Coordinate>& a_pixels,
	Color a_color)
{
	for(int i=0; i<static_cast<int>(a_pixels.size()); i++)
	{
		plot_.setPixel(
			a_pixels[i].x_,
			a_pixels[i].y_,
			a_color.r_,
			a_color.g_,
			a_color.b_);
	}
	return;
}

void BMPPlot::plotText(
	Coordinate a_where,
	const std::string& a_text)
{
	unsigned char r=0;
	unsigned char g=0;
	unsigned char b=0;

	for(int c=0; c<static_cast<int>(a_text.size()); c++)
	{
		int xf = 0;
		char ch = a_text[c];
		if(ch>='a' && ch<='z')
		{
			xf = fontSizeX_*(ch-'a');
		}
		else if(ch>='A' && ch<='Z')
		{
			xf = 26*fontSizeX_ + (fontSizeX_*(ch-'A'));
		}
		else if(ch>='0' && ch<='9')
		{
			xf = 59*fontSizeX_ + (fontSizeX_*(ch-'0'));
		}
		else if(ch == ' ')
		{
			xf = 52*fontSizeX_;
		}
		else if(ch == ',')
		{
			xf = 53*fontSizeX_;
		}
		else if(ch == '.')
		{
			xf = 54*fontSizeX_;
		}
		else if(ch == ':')
		{
			xf = 55*fontSizeX_;
		}
		else if(ch == '/')
		{
			xf = 56*fontSizeX_;
		}
		else if(ch == '_')
		{
			xf = 57*fontSizeX_;
		}
		else if(ch == '-')
		{
			xf = 58*fontSizeX_;
		}
		else
		{
			xf = 52*fontSizeX_;
		}
		for(int y=0; y<fontSizeY_; y++)
		{
			for(int x=0; x<fontSizeX_; x++)
			{
				// Copy over pixel data from font bitmap to plot
				font_.getPixel(
					xf+x,
					y,
					&r,
					&g,
					&b);
				plot_.setPixel(
					a_where.x_+x+c*fontSizeX_,
					a_where.y_+y,
					r,
					g,
					b);
			}
		}
	}
	return;
}

void BMPPlot::plotSignalName(const std::string& a_signalName)
{
	plotText(
		Coordinate{2*fontSizeX_, fontSizeY_*6 + (fontSizeY_*(1+nSignals_*4))},
		a_signalName);
	return;
}

void BMPPlot::plotSignalValues(const std::vector<sgnl::Volt>& a_values)
{
	if(a_values.size()<2)
		return;
	int offs_y = fontSizeY_*6 + (fontSizeY_*(1+nSignals_*4)) + 2*fontSizeY_;
	double scale = 3*fontSizeY_/HIGH_LEVEL_VOLT;

	int y_deadband_upper
		= offs_y-static_cast<int>((HIGH_THRESH_VOLT+DEADBAND_VOLT/2)*scale);
	int y_deadband_lower
		= offs_y-static_cast<int>((HIGH_THRESH_VOLT-DEADBAND_VOLT/2)*scale);

	for(int y=y_deadband_upper; y<=y_deadband_lower; y++)
	{
		plotPixels(bresenhamLine(
				Coordinate{100, y},
				Coordinate{100+static_cast<int>(a_values.size()), y}),
			Color{75, 75, 75});
	}

	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	for(int i=1; i<static_cast<int>(a_values.size()); i++)
	{
		vector<Coordinate> linePixels = bresenhamLine(
			Coordinate{100+i-1, offs_y-static_cast<int>(a_values[i-1]*scale)},
			Coordinate{100+i, offs_y-static_cast<int>(a_values[i]*scale)});
		for(int k=0; k<static_cast<int>(linePixels.size()); k++)
		{
			Volt interpValue = (offs_y-linePixels[k].y_)/scale;
			if(interpValue >= HIGH_THRESH_VOLT+DEADBAND_VOLT/2)
			{
				r = 75;
				g = 255;
				b = 75;
			}
			else if(interpValue < HIGH_THRESH_VOLT+DEADBAND_VOLT/2
				 && interpValue > HIGH_THRESH_VOLT-DEADBAND_VOLT/2)
			{
				r = 255;
				g = 255;
				b = 75;
			}
			else
			{
				r = 255;
				g = 75;
				b = 75;
			}
			plot_.setPixel(linePixels[k].x_, linePixels[k].y_, r, g, b);
		}
	}
	return;
}

void BMPPlot::plotAxes(sgnl::Nanoseconds a_length)
{
	int offs_y = fontSizeY_*6 + (fontSizeY_*(1+nSignals_*4));
	plotPixels(
		bresenhamLine(
			Coordinate{100, offs_y-1*fontSizeY_},
			Coordinate{100, offs_y+2*fontSizeY_}),
		Color{255, 255, 255});
	plotPixels(
		bresenhamLine(
			Coordinate{100, offs_y+2*fontSizeY_},
			Coordinate{100+a_length, offs_y+2*fontSizeY_}),
		Color{255, 255, 255});
	return;
}

std::vector<BMPPlot::Coordinate> BMPPlot::bresenhamLine(
	Coordinate a_start,
	Coordinate a_end) const
{
	vector<Coordinate> ret;
	int x1 = a_start.x_;
	int y1 = a_start.y_;
	int x2 = a_end.x_;
	int y2 = a_end.y_;
    int dx =  abs(x2-x1);
    int sx = x1<x2 ? 1 : -1;
	int dy = -abs(y2-y1);
	int sy = y1<y2 ? 1 : -1;
	int err = dx+dy;
	int e2 = 0;
	while(true)
	{
	   	ret.push_back(Coordinate{x1, y1});
	    if (x1==x2 && y1==y2)
	    {
	    	break;
	    }
	    e2 = 2*err;
	    if (e2 > dy)
	    {
	    	err += dy;
	    	x1 += sx;
	    }
	    if (e2 < dx)
	    {
	    	err += dx;
	    	y1 += sy;
	    }
	}
	return ret;
}

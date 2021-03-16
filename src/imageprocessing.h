#pragma once
#include <vector>
#include <fstream>
using namespace std;
struct pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
struct imageinfo
{
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short xorigin;
	short yorigin;
	short width;
	short height;
	char bitsperpixel;
	char imagedescriptor;
};
class image
{
	imageinfo info;
	vector<pixel> pixels;
	float normalize(const unsigned char& p) const;
	float clamp(const float& p) const;
	unsigned char denormalize(const float& p) const;
public:
	image();
	image(imageinfo& info, vector<pixel>& pixels);
	image(ifstream& file);
	image(image& b, image& g, image& r);
	void write(ofstream& ofile) const;
	image operator*(image& rhs) const; //multiply blending mode
	image operator-(image& rhs) const; //subtract blending mode
	image operator/(image& rhs) const; //screen blending mode??? uhhh idk if the analogy works here since there's a divide blending mode, but whatever
	image operator%(image& rhs) const; //overlay or something idk at this point
	
	bool operator==(image& rhs) const;
	image add(unsigned int channel,unsigned char amount) const;
	image scale(unsigned int channel, float amount) const;
	image* split() const;
	image rotate180() const;
};

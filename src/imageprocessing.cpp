#include "imageprocessing.h"
#include <vector>
using namespace std;

image::image()
{
}

image::image(imageinfo& info, vector<pixel>& pixels)
{
	this->info = info;
	this->pixels = pixels;
}

image::image(ifstream& file)
{
	imageinfo info;
	file.read(&info.idlength, 1);
	file.read(&info.colormaptype, 1);
	file.read(&info.datatypecode, 1);
	file.read((char*)&info.colormaporigin, 2);
	file.read((char*)&info.colormaplength, 2);
	file.read(&info.colormapdepth, 1);
	file.read((char*)&info.xorigin, 2);
	file.read((char*)&info.yorigin, 2);
	file.read((char*)&info.width, 2);
	file.read((char*)&info.height, 2);
	file.read(&info.bitsperpixel, 1);
	file.read(&info.imagedescriptor, 1);
	vector<pixel> pixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		file.read((char*)&pixels[i].blue, 1);
		file.read((char*)&pixels[i].green, 1);
		file.read((char*)&pixels[i].red, 1);
	}
	file.close();
	this->info = info;
	this->pixels = pixels;
}

image::image(image& b, image& g, image& r)
{
	imageinfo info = b.info;
	vector<pixel> pixels((unsigned int)(b.info.width * b.info.height));
	for (unsigned int i = 0; i < (unsigned int)(b.info.width * b.info.height); i++)
	{
		pixels[i].blue = b.pixels[i].blue;
		pixels[i].green = g.pixels[i].green;
		pixels[i].red = r.pixels[i].red;
	}
	this->info = info;
	this->pixels = pixels;
}

void image::write(ofstream& ofile) const
{
	ofile.write(&info.idlength, 1);
	ofile.write(&info.colormaptype, 1);
	ofile.write(&info.datatypecode, 1);
	ofile.write((char*)&info.colormaporigin, 2);
	ofile.write((char*)&info.colormaplength, 2);
	ofile.write(&info.colormapdepth, 1);
	ofile.write((char*)&info.xorigin, 2);
	ofile.write((char*)&info.yorigin, 2);
	ofile.write((char*)&info.width, 2);
	ofile.write((char*)&info.height, 2);
	ofile.write(&info.bitsperpixel, 1);
	ofile.write(&info.imagedescriptor, 1);
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		ofile.write((char*)&pixels[i].blue, 1);
		ofile.write((char*)&pixels[i].green, 1);
		ofile.write((char*)&pixels[i].red, 1);
	}
	ofile.close();
}


image image::operator*(image& rhs) const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		newpixels[i].blue = denormalize(normalize(pixels[i].blue) * normalize(rhs.pixels[i].blue));
		newpixels[i].green = denormalize(normalize(pixels[i].green) * normalize(rhs.pixels[i].green));
		newpixels[i].red = denormalize(normalize(pixels[i].red) * normalize(rhs.pixels[i].red));
	}
	image newimage(newinfo, newpixels);
	return newimage;

}

image image::operator-(image& rhs) const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		newpixels[i].blue = denormalize(clamp(normalize(pixels[i].blue) - normalize(rhs.pixels[i].blue)));
		newpixels[i].green = denormalize(clamp(normalize(pixels[i].green) - normalize(rhs.pixels[i].green)));
		newpixels[i].red = denormalize(clamp(normalize(pixels[i].red) - normalize(rhs.pixels[i].red)));
	}
	image newimage(newinfo, newpixels);
	return newimage;
}

image image::operator/(image& rhs) const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		newpixels[i].blue = denormalize(clamp(1.0f - clamp(1.0f - normalize(pixels[i].blue)) *clamp(1.0f-normalize(rhs.pixels[i].blue))));
		newpixels[i].green = denormalize(clamp(1.0f - clamp(1.0f - normalize(pixels[i].green)) * clamp(1.0f - normalize(rhs.pixels[i].green))));
		newpixels[i].red = denormalize(clamp(1.0f - clamp(1.0f - normalize(pixels[i].red)) * clamp(1.0f - normalize(rhs.pixels[i].red))));
	}
	image newimage(newinfo, newpixels);
	return newimage;
}

image image::operator%(image& rhs) const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		float gray = (normalize(pixels[i].blue) + normalize(pixels[i].green) + normalize(pixels[i].red)) / 3.0f;
		if (gray <= 0.5f)
		{
			newpixels[i].blue = denormalize(clamp(2.0f*normalize(pixels[i].blue) * normalize(rhs.pixels[i].blue)));
			newpixels[i].green = denormalize(clamp(2.0f*normalize(pixels[i].green) * normalize(rhs.pixels[i].green)));
			newpixels[i].red = denormalize(clamp(2.0f*normalize(pixels[i].red) * normalize(rhs.pixels[i].red)));
		}
		else
		{
			newpixels[i].blue = denormalize(clamp(1.0f - 2.0f*clamp(1.0f - normalize(pixels[i].blue)) * clamp(1.0f - normalize(rhs.pixels[i].blue))));
			newpixels[i].green = denormalize(clamp(1.0f - 2.0f*clamp(1.0f - normalize(pixels[i].green)) * clamp(1.0f - normalize(rhs.pixels[i].green))));
			newpixels[i].red = denormalize(clamp(1.0f - 2.0f*clamp(1.0f - normalize(pixels[i].red)) * clamp(1.0f - normalize(rhs.pixels[i].red))));
		}
	}
	image newimage(newinfo, newpixels);
	return newimage;
}

float image::normalize(const unsigned char& p) const
{
	return (float)p/255.0f;
}

float image::clamp(const float& p) const
{
	if (p < 0.0f)
		return 0.0f;
	else if (p > 1.0f)
		return 1.0f;
	else
		return p;
}

unsigned char image::denormalize(const float& p) const
{
	return (unsigned char)(p*255.0f + 0.5f);
}

bool image::operator==(image& rhs) const
{
	if (this->info.idlength != rhs.info.idlength)
		return false;
	if (this->info.colormaptype != rhs.info.colormaptype)
		return false;
	if (this->info.datatypecode != rhs.info.datatypecode)
		return false;
	if (this->info.colormaporigin != rhs.info.colormaporigin)
		return false;
	if (this->info.colormapdepth != rhs.info.colormapdepth)
		return false;
	if (this->info.xorigin != rhs.info.xorigin)
		return false;
	if (this->info.yorigin != rhs.info.yorigin)
		return false;
	if (this->info.width != rhs.info.width)
		return false;
	if (this->info.height != rhs.info.height)
		return false;
	if (this->info.bitsperpixel != rhs.info.bitsperpixel)
		return false;
	if (this->info.imagedescriptor != rhs.info.imagedescriptor)
		return false;
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		if (pixels[i].blue != rhs.pixels[i].blue)
			return false;
		if (pixels[i].green != rhs.pixels[i].green)
			return false;
		if (pixels[i].red != rhs.pixels[i].red)
			return false;
	}
	return true;
}

image image::add(unsigned int channel,unsigned char amount) const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		if (channel == 0)
		{
			newpixels[i].blue = denormalize(clamp(normalize(pixels[i].blue) + normalize(amount)));
			newpixels[i].green = pixels[i].green;
			newpixels[i].red = pixels[i].red;
		}
		else if(channel==1)
		{
			newpixels[i].blue = pixels[i].blue;
			newpixels[i].green = denormalize(clamp(normalize(pixels[i].green) + normalize(amount)));
			newpixels[i].red = pixels[i].red;
		}
		else if(channel==2)
		{
			newpixels[i].red = denormalize(clamp(normalize(pixels[i].red) + normalize(amount)));
			newpixels[i].blue = pixels[i].blue;
			newpixels[i].green = pixels[i].green;
		}
		else
		{
			newpixels[i].blue = denormalize(clamp(normalize(pixels[i].blue) + normalize(amount)));
			newpixels[i].green = denormalize(clamp(normalize(pixels[i].green) + normalize(amount)));
			newpixels[i].red = denormalize(clamp(normalize(pixels[i].red) + normalize(amount)));
		}
	}
	image newimage(newinfo, newpixels);
	return newimage;
}
image image::scale(unsigned int channel, float amount) const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		if (channel == 0)
		{
			newpixels[i].blue = denormalize(clamp(normalize(pixels[i].blue) * amount));
			newpixels[i].green = pixels[i].green;
			newpixels[i].red = pixels[i].red;
		}
		else if (channel == 1)
		{
			newpixels[i].blue = pixels[i].blue;
			newpixels[i].green = denormalize(clamp(normalize(pixels[i].green) * amount));
			newpixels[i].red = pixels[i].red;
		}
		else if (channel == 2)
		{
			newpixels[i].blue = pixels[i].blue;
			newpixels[i].green = pixels[i].green;
			newpixels[i].red = denormalize(clamp(normalize(pixels[i].red) * amount));
		}
		else
		{
			newpixels[i].blue = denormalize(clamp(normalize(pixels[i].blue) * amount));
			newpixels[i].green = denormalize(clamp(normalize(pixels[i].green) * amount));
			newpixels[i].red = denormalize(clamp(normalize(pixels[i].red) * amount));
		}
	}
	image newimage(newinfo, newpixels);
	return newimage;
}

image* image::split() const
{
	imageinfo newinfo = this->info;
	vector<pixel> newbpixels((unsigned int)(info.width * info.height));
	vector<pixel> newgpixels((unsigned int)(info.width * info.height));
	vector<pixel> newrpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		newbpixels[i].blue = pixels[i].blue;
		newbpixels[i].green = pixels[i].blue;
		newbpixels[i].red = pixels[i].blue;

		newgpixels[i].blue = pixels[i].green;
		newgpixels[i].green = pixels[i].green;
		newgpixels[i].red = pixels[i].green;

		newrpixels[i].blue = pixels[i].red;
		newrpixels[i].green = pixels[i].red;
		newrpixels[i].red = pixels[i].red;
	}
	image newbimage(newinfo, newbpixels);
	image newgimage(newinfo, newgpixels);
	image newrimage(newinfo, newrpixels);
	static image images[3];
	images[0] = newbimage;
	images[1] = newgimage;
	images[2] = newrimage;
	return images;
}

image image::rotate180() const
{
	imageinfo newinfo = this->info;
	vector<pixel> newpixels((unsigned int)(info.width * info.height));
	for (unsigned int i = 0; i < (unsigned int)(info.width * info.height); i++)
	{
		newpixels[i].blue = pixels[(unsigned int)(info.width * info.height)-i-1].blue;
		newpixels[i].green = pixels[(unsigned int)(info.width * info.height)-i-1].green;
		newpixels[i].red = pixels[(unsigned int)(info.width * info.height)-i-1].red;
	}
	image newimage(newinfo, newpixels);
	return newimage;
}




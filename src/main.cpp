#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "imageprocessing.h"
using namespace std;

int open(ifstream& file, string filename, bool example=false)
{
	string path;
	if (example)
		path = "./examples/";
	else
		path = "./input/";
	file.open(path+filename, ios::in | ios::binary);
	if (!file)
	{
		cout << "cannot open "<<filename<< endl;
		return 1;
	}
	return 0;
}
int open(ofstream& file, string filename)
{
	file.open("./output/" + filename, ios::out | ios::binary);
	if (!file)
	{
		cout << "cannot open " << filename << endl;
		return 1;
	}
	return 0;
}



int main()
{
	
	ifstream file;
	ifstream file2;
	ifstream efile;
	ofstream ofile;
	//TEST 1
	cout << "Testing Multiply Effect, Test 1" << endl;
	if (open(file, "layer1.tga") == 1) 
		return 1;
	if (open(file2, "pattern1.tga") == 1)
		return 1;
	if (open(efile, "EXAMPLE_part1.tga",true) == 1)
		return 1;
	if (open(ofile, "part1.tga") == 1)
		return 1;
	image layer1(file);
	image pattern1(file2);
	image example1(efile);
	image part1 = layer1 * pattern1;
	part1.write(ofile);
	if (part1 == example1)
		cout << "Test 1 Passed!"<<endl;
	else
		cout << "Test 1 Failed!!"<<endl;
	//TEST 2
	cout << "Testing Subtract Effect, Test 2" << endl;
	if (open(file, "layer2.tga") == 1)
		return 1;
	if (open(file2, "car.tga") == 1)
		return 1;
	if (open(efile, "EXAMPLE_part2.tga", true) == 1)
		return 1;
	if (open(ofile, "part2.tga") == 1)
		return 1;
	image layer2(file);
	image car(file2);
	image example2(efile);
	image part2 = car - layer2;
	part2.write(ofile);
	if (part2 == example2)
		cout << "Test 2 Passed!" << endl;
	else
		cout << "Test 2 Failed!!" << endl;
	//TEST 3
	ifstream file3;
	cout << "Testing Multiply and Screen Effects, Test 3" << endl;

	if (open(file2, "pattern2.tga") == 1)
		return 1;
	if (open(file3, "text.tga") == 1)
		return 1;
	if (open(efile, "EXAMPLE_part3.tga", true) == 1)
		return 1;
	if (open(ofile, "part3.tga") == 1)
		return 1;

	image pattern2(file2);
	image text(file3);
	image example3(efile);
	image part12 = layer1*pattern2;
	image part3 = part12 / text;
	part3.write(ofile);
	if (part3 == example3)
		cout << "Test 3 Passed!" << endl;
	else
		cout << "Test 3 Failed!!" << endl;
	//TEST 4
	cout << "Testing Multiply and Subtract Effects, Test 4" << endl;

	if (open(file2, "circles.tga") == 1)
		return 1;
	
	if (open(efile, "EXAMPLE_part4.tga", true) == 1)
		return 1;
	if (open(ofile, "part4.tga") == 1)
		return 1;
	image circles(file2);
	image example4(efile);
	image part23 = layer2 * circles;
	image part4 = part23 - pattern2;
	part4.write(ofile);
	if (part4 == example4)
		cout << "Test 4 Passed!" << endl;
	else
		cout << "Test 4 Failed!!" << endl;
	//TEST 5
	cout << "Testing Overlay Effects, Test 5" << endl;
	if (open(efile, "EXAMPLE_part5.tga", true) == 1)
		return 1;
	if (open(ofile, "part5.tga") == 1)
		return 1;
	image example5(efile);
	image part5 = pattern1 % layer1;
	part5.write(ofile);
	if (part5 == example5)
		cout << "Test 5 Passed!" << endl;
	else
		cout << "Test 5 Failed!!" << endl;
	//TEST 6
	cout << "Testing adding to channels, Test 6" << endl;
	if (open(efile, "EXAMPLE_part6.tga", true) == 1)
		return 1;
	if (open(ofile, "part6.tga") == 1)
		return 1;
	image example6(efile);
	image part6 = car.add(1,200);
	part6.write(ofile);
	if (part6 == example6)
		cout << "Test 6 Passed!" << endl;
	else
		cout << "Test 6 Failed!!" << endl;
	//TEST 7
	cout << "Testing scaling channels, Test 7" << endl;
	if (open(efile, "EXAMPLE_part7.tga", true) == 1)
		return 1;
	if (open(ofile, "part7.tga") == 1)
		return 1;
	image example7(efile);
	image part7 = car.scale(2, 4.0f).scale(0,0.0f);
	part7.write(ofile);
	if (part7 == example7)
		cout << "Test 7 Passed!" << endl;
	else
		cout << "Test 7 Failed!!" << endl;
	//TEST 8
	ifstream efile2;
	ifstream efile3;
	ofstream ofile2;
	ofstream ofile3;
	cout << "Testing splitting images into channels, Test 8" << endl;
	if (open(efile, "EXAMPLE_part8_b.tga", true) == 1)
		return 1;
	if (open(efile2, "EXAMPLE_part8_g.tga", true) == 1)
		return 1;
	if (open(efile3, "EXAMPLE_part8_r.tga", true) == 1)
		return 1;
	if (open(ofile, "part8_b.tga") == 1)
		return 1;
	if (open(ofile2, "part8_g.tga") == 1)
		return 1;
	if (open(ofile3, "part8_r.tga") == 1)
		return 1;
	image example8_b(efile);
	image example8_g(efile2);
	image example8_r(efile3);
	image* images = car.split();
	image part8_b = *images;
	image part8_g = *(images + 1);
	image part8_r = *(images + 2);
	part8_b.write(ofile);
	part8_g.write(ofile2);
	part8_r.write(ofile3);
	if(part8_b==example8_b && part8_g == example8_g && part8_r == example8_r)
		cout << "Test 8 Passed!" << endl;
	else
		cout << "Test 8 Failed!!" << endl;
	//TEST 9
	cout << "Testing combining channels into images, Test 9" << endl;
	if (open(file, "layer_blue.tga") == 1)
		return 1;
	if (open(file2, "layer_green.tga") == 1)
		return 1;
	if (open(file3, "layer_red.tga") == 1)
		return 1;
	if (open(efile, "EXAMPLE_part9.tga", true) == 1)
		return 1;
	if (open(ofile, "part9.tga") == 1)
		return 1;
	image example9(efile);
	image layer_blue(file);
	image layer_green(file2);
	image layer_red(file3);
	image part9(layer_blue, layer_green, layer_red);
	part9.write(ofile);
	if (part9 == example9)
		cout << "Test 9 Passed!" << endl;
	else
		cout << "Test 9 Failed!!" << endl;
	//TEST 10
	cout << "Testing rotating image 180 degrees" << endl;
	if (open(file, "text2.tga") == 1)
		return 1;
	if (open(efile, "EXAMPLE_part10.tga", true) == 1)
		return 1;
	if (open(ofile, "part10.tga") == 1)
		return 1;
	image text2(file);
	image example10(efile);
	image part10 = text2.rotate180();
	part10.write(ofile);
	if (part10 == example10)
		cout << "Test 10 Passed!" << endl;
	else
		cout << "Test 10 Failed!!" << endl;
};


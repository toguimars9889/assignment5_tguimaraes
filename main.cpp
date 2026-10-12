#include <stdio.h>
#include <vector> //STORES THE PIXELS VIA std::vector
#include "pixel.h"
#include <fstream> //PROVIDES FILES STREAMS FOR READING/WRITING FILES
#include <string> //PROVIDES STRINGS AND TEXT-TO-NUMBER CONVERSIONS
#include <algorithm> //ORDERS PIXELS BY THEIR COORDINATES VIA std::sort
#include <iomanip> //CONTROLS OUTPUT PRECISION VIA std::setprecision BY SPECIFYING NUMBER OF SIGNIFICANT DIGITS

void flip_vertically(std::vector<Pixel> &pixel_list) { //FLIP BY SUBTRACTING Y-VALUE FROM MAX, THEREFORE pixel 0 BECOMES 255 AND SO FORTH
    for (Pixel &pixel : pixel_list) {
        pixel.y = 255 - pixel.y;
    }
}

void average_colors(std::vector<Pixel> &pixel_list) { //FINDS SUM OF EACH CATEGORY AND THEN DIVIDES BY SIZE OF pixel_list IN THE PRINTF STATEMENT
    double r_total = 0.0;
    double g_total = 0.0;
    double b_total = 0.0;

    for (const Pixel &pixel : pixel_list) { //const PREVENTS THE LOOP FROM ACCIDENTALLY CHANGING THAT PIXEL
        r_total = r_total + pixel.r;
        g_total = g_total + pixel.g;
        b_total = b_total + pixel.b;
    }

    printf("Average red: %f\n", r_total / pixel_list.size());
    printf("Average green: %f\n", g_total / pixel_list.size());
    printf("Average blue: %f\n", b_total / pixel_list.size());
}

int main (int argc, char *argv[]) { //CHECKS COMMAND LINE ARGUMENTS

	if(argc != 2) { 				//REQUIRES PROGRAM NAME AND ONE INPUT FILENAME
		printf("\nUsage: %s [filename here]\nTry: './main pixels.dat'\n\n", argv[0]);
		return 1;
	}

	std::ifstream input_file(argv[1]); //OPEN FILE PROVIDED BY COMMAND LINE ARGUMENT

	if (!input_file.is_open()) {	   //STOPS WITH AN ERROR MESSAGE IF FILE CANNOT BE OPENED
		printf("Could not open file: %s\n", argv[1]);
    		return 1;
	}

	std::string line; //HOLDS ONE LINE OF TEXT READ FROM THE INPUT FILE

	std::vector<Pixel> pixel_list; //STORES ALL PIXEL OBJECTS PARSED FROM THE FILE

	while (std::getline(input_file, line)) { //READS ONE LINE AT A TIME UNTIL THE FILE ENDS OR READING FAILS
		size_t comma = line.find(','); //THIS CREATES A STOPPING POINT AT THE NEXT COMMA, INDICATING HOW FAR TO READ

		std::string x_text = line.substr(0, comma); //READS UNTIL THE FIRST COMMA

		Pixel pixel;
		pixel.x = std::stoi(x_text); //CONVERTS CHARACTERS FROM A STRING TO INTEGERS

		size_t start = comma + 1;
		comma = line.find(',', start);

		std::string y_text = line.substr(start, comma - start);
		pixel.y = std::stoi(y_text);

		start = comma + 1;
		comma = line.find(',', start);

		std::string r_text = line.substr(start, comma - start);
		pixel.r = std::stof(r_text); //CONVERTS CHARACTERS FROM A STRING TO A FLOAT

		start = comma + 1;
		comma = line.find(',', start);

		std::string g_text = line.substr(start, comma - start);
		pixel.g = std::stof(g_text);

		start = comma + 1; 
		comma = line.find(',',start);

		std::string b_text = line.substr(start);
		pixel.b = std::stof(b_text);

		pixel_list.push_back(pixel); //APPENDS THE COMPLETED PIXEL TO THE VECTOR

	}

	printf("Pixels read: %zu\n", pixel_list.size()); //DISPLAY THE NUMBER OF PIXELS STORED IN THE VECTOR
	average_colors(pixel_list); //CALCULATES AND DISPLAYS THE AVERAGE OF EACH COLOR
	flip_vertically(pixel_list); //REFLECTS EACH PIXEL'S Y-COORDINATE VERTICALLY


	//std:sort REARRANGES PIXELS FROM begin() TO end()
	//LAMBDA FUNCTION COMPARES TWO PIXELS; IF TRUE, a SHOULD COME BEFORE b
	//ORDER BY X-VALUES FIRST, THEN BY Y WHEN VALUES MATCH
	std::sort(pixel_list.begin(), pixel_list.end(), [](const Pixel &a, const Pixel &b) { //SORTS THE WHOLE VECTOR 
        	if (a.x != b.x) {
		return a.x < b.x; //CHECKS X VALUES FIRST
                }
        return a.y < b.y;
        });

	std::ofstream output_file("flipped.dat"); //WRITES THE FLIPPED VECTOR TO A SEPARATE FILE LOCATION 

	if (!output_file.is_open()) { //EXITS WITH AN ERROR IF FILE CANNOT BE OPENED
    	printf("Could not create flipped.dat\n");
    	return 1;
	}

	output_file << std::setprecision(9); //WRITES 9 SIGNIFICANT DIGITS FOR CONTROLLING OUTPUT PRECISION

	for (const Pixel &pixel : pixel_list) { //WRITES EACH PIXEL IN X,Y,R,G,B FORMAT WITH ONE PIXEL PER LINE
    	output_file << pixel.x << ','
                << pixel.y << ','
                << pixel.r << ','
                << pixel.g << ','
                << pixel.b << '\n';
	}

	output_file.close(); //FINISHES WRITING DATA AND CLOSES THE FILE
	printf("Saved flipped.dat\n");

	return 0; 
}

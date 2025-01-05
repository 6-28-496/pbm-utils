/* Utility to handle P1 - eventually P2 through P6 as well

Syntax for use:

./convert -h // displays a help message like this one
./convert -i image.pbm // inverts image's colours
./convert -mh image.pbm // mirrors image horizontally
./convert -mv image.pbm // mirrors image vertically
./convert -mhv image.pbm // mirrors image horizontally and vertically

Possible file formats (v0.1):

.pbm (mode P1)

For more info, see: https://en.wikipedia.org/wiki/Netpbm#File_formats

*/

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void display_help(void);
vector<vector<int> > input_image(char *filename);
void invert_image(vector<vector<int> > &image);
void output_image(char *filename, vector<vector<int> > &image);

int main(int argc, char *argv[]) {
    switch(argc) {
        case 1:
            cout << "No arguments supplied.\n";
            display_help();
            return 1;
            break;
        case 2:
            if (!strcmp(argv[1], "-h")) {
                display_help();
                return 0;
            } else {
                cout << "Parameter " << argv[1] << " not recognised.\n";
                display_help();
                return 1;
            }
            break;
        case 3:
            if (!strcmp(argv[1], "-i")) {
                // inversion
                vector<vector<int> > image;
                try {
                    image = input_image(argv[2]);
                } catch (runtime_error& e) {
                    cerr << "Runtime error: " << e.what() << '\n';
                    return 1;
                }
                invert_image(image);
                try {
                    output_image(argv[2], image);
                } catch (runtime_error& e) {
                    cerr << "Runtime error: " << e.what() << '\n';
                }

                cout << "File inverted\n";
            } else {
                cout << "Parameter " << argv[1] << " not recognised.\n";
                display_help();
                return 1;
            }
            break;
        default:
            cout << "Too many arguments supplied.\n";
            display_help();
            return 1;
            break;
    }
}

void display_help(void) {
    cout << "Typical syntax is: \n";
    cout << "\t./convert // displays a help message like this one\n";
    cout << "\t./convert -h // also displays a help message like this one\n";
    cout << "\t./convert -i image.pbm // inverts image's colours\n";
    cout << "\t./convert -mh image.pbm // mirrors image horizontally\n";
    cout << "\t./convert -mv image.pbm // mirrors image vertically\n";
    cout << "\t./convert -mhv image.pbm // mirrors image horizontally and vertically\n";
}

vector<vector<int> > input_image(char *filename) {
    ifstream file_reader;
    file_reader.open(filename);

    if (!file_reader) {
        throw runtime_error("error reading from file");
    }

    string image_format;
    getline(file_reader, image_format);
    
    if (image_format != "P1") {
        throw runtime_error("only P1 bitmap format currently supported");
    }

    string dimensions_line;

    // ignore comments
    do {
        getline(file_reader, dimensions_line);
    } while (dimensions_line[0] == '#');

    int width, height;
    try {
        int space_pos = dimensions_line.find(' ');
        width = stoi(string(dimensions_line, 0, space_pos));
        height = stoi(string(dimensions_line, space_pos+1, dimensions_line.length()));
    } catch(...) {
        throw runtime_error("image dimension data not valid");
    }

    if (width < 1 || height < 1) {
        throw runtime_error("image width and height must equal at least 1 pixel each");
    }

    char pixel; // treat pixels as chars to allow for whitespace
    int col_counter = 0, row_counter = 0;
    vector<int> current_row;
    vector<vector<int> > image;

    while (file_reader >> pixel && !file_reader.eof()) {
        if (pixel == '0' || pixel == '1') {
            if (row_counter >= height) {
                throw runtime_error("too many pixels in data section - aborting");
            }
            
            // convert ASCII char (48. 49) to int (0, 1)
            current_row.push_back(int(pixel) - int('0'));
            if (col_counter == (width-1)) {
                row_counter++;
                col_counter = 0;
                image.push_back(current_row);
                current_row.clear();
            } else {
                col_counter++;
            }
        }
    }

    file_reader.close();

    if(col_counter != 0 || row_counter != height) {
        throw runtime_error("wrong number of pixels in data section - aborting");
    }
    return image;
}

void invert_image(vector<vector<int> > &image) {
    for (int j = 0; j < image.size(); j++) {
        for (int i = 0; i < image[j].size(); i++) {
            if (image[j][i] == 0) {
                image[j][i] = 1;
            } else if (image[j][i] == 1) {
                image[j][i] = 0;
            }
        }
    }
}

void output_image(char *filename, vector<vector<int> > &image) {
    ofstream file_writer;
    file_writer.open(filename);
    if (!file_writer) {
        throw runtime_error("error writing to file");
    }

    file_writer << "P1\n";

    int width = image[0].size(), height = image.size();
    file_writer << width << ' ' << height << endl;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            file_writer << image[j][i] << ' ';
        }
        file_writer << '\n';
    }

    file_writer.close();
}
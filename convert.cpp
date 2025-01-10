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

#include "pbm_classes.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using namespace std;

void display_help(void);
Image input_image(string& filename);
void invert_image(vector<vector<int> >& image);
void mirror_image_horizontally(vector<vector<int> >& image);
void mirror_image_vertically(vector<vector<int> >& image);
void mirror_image_hv(vector<vector<int> >& image);
void output_image(string& filename, Image& image);

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
            if (strcmp(argv[1], "-i") && strcmp(argv[1], "-mh")
            && strcmp(argv[1], "-mv") && strcmp(argv[1], "-mhv")) {
                cout << "Parameter " << argv[1] << " not recognised.\n";
                display_help();
                return 1;
            } else {
                Image image;
                string filename = argv[2];
                try {
                    image = input_image(filename);
                } catch (runtime_error& e) {
                    cerr << "Runtime error: " << e.what() << '\n';
                    return 1;
                }

                if (!strcmp(argv[1], "-i")) {
                    image.invert();
                } else if (!strcmp(argv[1], "-mh")) {
                    image.mirror_image_horizontally();
                } else if (!strcmp(argv[1], "-mv")) {
                    image.mirror_image_vertically();
                } else if (!strcmp(argv[1], "-mhv")) {
                    image.mirror_image_hv();
                }

                try {
                    output_image(filename, image);
                } catch (runtime_error& e) {
                    cerr << "Runtime error: " << e.what() << '\n';
                }

                cout << "File transformed with operation " << argv[1] << '\n';
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
    cout << "Typical syntax is: \n"
         << "\t./convert // displays a help message like this one\n"
         << "\t./convert -h // also displays a help message like this one\n"
         << "\t./convert -i image.pbm // inverts image's colours\n"
         << "\t./convert -mh image.pbm // mirrors image horizontally\n"
         << "\t./convert -mv image.pbm // mirrors image vertically\n"
         << "\t./convert -mhv image.pbm // mirrors image horizontally and vertically\n";
}

Image input_image_header(ifstream& file_reader) {
    string image_format = "";
    int width = 0, height = 0;
    vector<string> header = {};
    string next_line;
    bool is_header_read;

    while (!is_header_read && !file_reader.eof()) {
        getline(file_reader, next_line);
        header.push_back(next_line);

        // ignore comment:
        int comment_pos = next_line.find('#');
        if (comment_pos == 0) {
             // whole line is a comment - no data
            continue;
        } else if (comment_pos > 0) {
            // part of the line is a comment
            next_line = string(next_line, 0, comment_pos);
        }
        
        if (image_format == "") {
            regex format_pattern {R"(P[1-6])"}; // TODO - support P2-P6
            smatch matches;
            if (regex_search(next_line, matches, format_pattern)) {
                // image format found
                image_format = matches[0];
                int format_pos = next_line.find(image_format);
                next_line = next_line.substr(format_pos+2);
            }
        }
        if (width == 0) {
            regex dimension_pattern {R"(\d{1,5})"};
            smatch matches;
            if (regex_search(next_line, matches, dimension_pattern)) {
                width = stoi(matches[0]);
                int width_pos = next_line.find(matches[0]);
                next_line = next_line.substr(width_pos+2);
            }
        }
        if (height == 0) {
            regex dimension_pattern {R"(\d{1,5})"};
            smatch matches;
            if (regex_search(next_line, matches, dimension_pattern)) {
                height = stoi(matches[0]);
            }
        }

        is_header_read = image_format.length() > 0 && (width > 0) && (height > 0);
    }

    if (!is_header_read) {
        throw runtime_error("file header invalid - aborting");
    }
    
    if (image_format != "P1") {
        throw runtime_error("only P1 bitmap format currently supported");
    }

    if (width < 1 || height < 1) {
        throw runtime_error("image width and height must equal at least 1 pixel each");
    }

    return Image(header, File_format::p1, width, height, 1);
}

void input_image_data(ifstream& file_reader, Image& image) {
    char pixel; // treat pixels as chars to allow for whitespace
    int col_counter = 0, row_counter = 0;
    vector<Pixel> current_row;
    
    while (file_reader >> pixel && !file_reader.eof()) {
        if (pixel == '0' || pixel == '1') {
            if (row_counter >= image.get_height()) {
                throw runtime_error("too many pixels in data section - aborting");
            }
            
            // convert ASCII char (48. 49) to int (0, 1)
            current_row.push_back(Pixel({int(pixel) - int('0')}));
            if (col_counter == (image.get_width()-1)) {
                row_counter++;
                col_counter = 0;
                image.add_pixel_row(current_row);
                current_row.clear();
            } else {
                col_counter++;
            }
        }
    }
}

Image input_image(string& filename) {
    string extension = string(filename, filename.length()-4);
    if (extension != ".pbm") {
        throw runtime_error("filename provided " + extension + " not currently supported");
    }
    
    ifstream file_reader;
    file_reader.open(filename);

    if (!file_reader) {
        throw runtime_error("error reading from file");
    }

    Image image;
    try {
        image = input_image_header(file_reader);
    } catch (runtime_error& e) {
        throw runtime_error("error reading file header: " + string(e.what())); 
    }

    try {
        input_image_data(file_reader, image);
    } catch (runtime_error& e) {
        throw runtime_error("error reading file data: " + string(e.what()));
    }

    return image;
}

void invert_image(vector<vector<int> >& image) {
    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[j].size(); j++) {
            if (image[i][j] == 0) {
                image[i][j] = 1;
            } else if (image[j][i] == 1) {
                image[i][j] = 0;
            }
        }
    }
}

void mirror_image_horizontally(vector<vector<int> >& image) {
    int width = image[0].size(), height = image.size();
    int temp;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < int(width/2); j++) {
            temp = image[i][j];
            image[i][j] = image[i][(width-j)-1];
            image[i][(width-j)-1] = temp;
        }
    }
}

void mirror_image_vertically(vector<vector<int> >& image) {
    int width = image[0].size(), height = image.size();
    int temp;

    for (int i = 0; i < int(height/2); i++) {
        for (int j = 0; j < width; j++) {
            temp = image[i][j];
            image[i][j] = image[(height-i)-1][j];
            image[(height-i)-1][j] = temp;
        }
    }
}

void mirror_image_hv(vector<vector<int> >& image) {
    mirror_image_horizontally(image);
    mirror_image_vertically(image);
}

void output_image(string& filename, Image& image) {
    ofstream file_writer;
    file_writer.open(filename);
    if (!file_writer) {
        throw runtime_error("error writing to file");
    }

    for (auto line : image.get_header()) {
        file_writer << line << '\n';
    }

    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            file_writer << image.get_pixels()[i][j].get_color_channels()[0] << ' ';
        }
        file_writer << '\n';
    }

    file_writer.close();
}
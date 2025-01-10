/*
    Header file to provide class definitions for convert.cpp
*/

#ifndef PBM_CLASSES_H
#define PBM_CLASSES_H

#include <vector>

using namespace std;

class Pixel {
public:
    Pixel();
    Pixel(const vector<int>& pixel_colors);

    vector<int> get_color_channels() const;
    void invert(int max_color);
private:
    vector<int> color_channels;
};

enum class Rgb {
    red, green, blue
};

enum class File_format {
    p1, p2, p3, p4, p5, p6
};

class Image {
public:
    Image();  
    Image(vector<string> &file_header, File_format file_format,
          int image_width, int image_height, int image_max_color);
    Image(vector<vector<Pixel> > &image_pixels, vector<string> &file_header,
          File_format file_format, int image_width, int image_height, int image_max_color);

    vector<vector<Pixel> > get_pixels() const;
    vector<string> get_header() const;
    int get_width() const;
    int get_height() const;
    int get_max_color() const;
    
    bool add_pixel_row(vector<Pixel> &pixel_row);
    void invert();
    void mirror_image_horizontally();
    void mirror_image_vertically();
    void mirror_image_hv();
private:
    vector<vector<Pixel> > pixels;
    vector<string> header;
    File_format format;
    int width, height, max_color;
};

#endif
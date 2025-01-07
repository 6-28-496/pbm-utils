#include "pbm_classes.h"
#include <vector>

using namespace std;

Pixel::Pixel() : color_channels{0} {}

Pixel::Pixel(const vector<int>& pixel_colors) : color_channels(pixel_colors) {}

vector<int> Pixel::get_color_channels() const {
    return color_channels;
}

void Pixel::invert(int max_color) {
    for (int i = 0; i < color_channels.size(); i++) {
        color_channels[i] = max_color - color_channels[i];
    }
}

Image::Image() : pixels{}, header{}, format(File_format::p1), width(1),
                 height(1), max_color(1) {}
    
Image::Image(vector<string> &file_header, File_format file_format,
             int image_width, int image_height, int image_max_color)
             : pixels{}, header(file_header), format(file_format),
             width(image_width), height(image_height), max_color(image_max_color) { }

Image::Image(vector<vector<Pixel> > &image_pixels, vector<string> &file_header,
          File_format file_format, int image_width, int image_height, int image_max_color)
          : pixels(image_pixels), header(file_header), format(file_format),
          width(image_width), height(image_height), max_color(image_max_color) { }

vector<vector<Pixel> > Image::get_pixels() const {
    return pixels;
}

bool Image::add_pixel_row(vector<Pixel> &pixel_row) {
    if(pixels.size() < height && pixel_row.size() < width) {
        pixels.push_back(pixel_row);
        return true;
    }

    return false;
}

void Image::invert() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            pixels[i][j].invert(max_color);
        }
    }
}

void Image::mirror_image_horizontally() {
    Pixel temp;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < int(width/2); j++) {
            temp = pixels[i][j];
            pixels[i][j] = pixels[i][(width-j)-1];
            pixels[i][(width-j)-1] = temp;
        }
    }
}

void Image::mirror_image_vertically() {
    Pixel temp;

    for (int i = 0; i < int(height/2); i++) {
        for (int j = 0; j < width; j++) {
            temp = pixels[i][j];
            pixels[i][j] = pixels[(height-i)-1][j];
            pixels[(height-i)-1][j] = temp;
        }
    }
}

void Image::mirror_image_hv() {
    mirror_image_horizontally();
    mirror_image_vertically();
}
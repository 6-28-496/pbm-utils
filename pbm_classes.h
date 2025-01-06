/*
    Header file to provide class definitions for convert.cpp
*/

class Pixel {
public:
    virtual void invert() =0;
};

class Black_white_pixel : public Pixel {
private:
    bool is_black;
public:
    Black_white_pixel() {
        is_black = false;
    }
    
    Black_white_pixel(bool is_pixel_black) {
        is_black = is_pixel_black;
    }

    bool is_pixel_black() {
        return is_black;
    }
    
    void invert() {
        is_black = !is_black;
    }
};

class Greyscale_pixel : public Pixel {
private:
    int intensity;
    static int max_intensity;
public:
    Greyscale_pixel() {
        intensity = 0;
    }

    Greyscale_pixel(int pixel_intensity) {
        intensity = pixel_intensity;
    }

    int get_intensity() {
        return intensity;
    }

    void invert() {
        intensity = max_intensity - intensity;
    }
};

class Color_pixel : public Pixel {
private:
    int red, green, blue;
    static int max_color;
public:
    Color_pixel() {
        red = max_color;
        green = max_color;
        blue = max_color;
    }

    Color_pixel(int red_color, int green_color, int blue_color) {
        red = red_color;
        green = green_color;
        blue = blue_color;
    }

    int get_red() {
        return red;
    }

    int get_green() {
        return green;
    }

    int get_blue() {
        return blue;
    }

    void invert() {
        red = max_color - red;
        green = max_color - green;
        blue = max_color - blue;
    }
};
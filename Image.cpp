
#include "Image.h"

#include "CImg.h"

void Image::SaveBMP(const std::string &filename, int w, int h,
                    const unsigned char *pixels) {
    // Creates an image with three channels and sets it to black
    cimg_library::CImg<float> image((2 * w), (2 * h), 1, 3, 0);
    for (int y = 0; y < (2 * h); ++y) {
        for (int x = 0; x < (2 * w); ++x) {
            image(x, y, 0) = *(pixels + 0 + 3 * (y * (2 * w) + x));
            image(x, y, 1) = *(pixels + 1 + 3 * (y * (2 * w) + x));
            image(x, y, 2) = *(pixels + 2 + 3 * (y * (2 * w) + x));
        }
    }

    // Save out the image in BMP format.Pixel values must be in the range
    // [0,255]
    image.save(filename.c_str());
}
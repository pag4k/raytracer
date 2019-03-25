
#include "Image.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "CImg.h"

const int FILEHEADERSIZE = 14;
const int INFOHEADERSIZE = 40;
const unsigned char BITSPERPIXEL = 24;
const int PPM = 2835;

void Image::GenerateBMP(const std::string &filename, int width, int height,
                        const unsigned char *pixels) {
    static_assert(sizeof(int) == 4);
    std::ofstream imageFile;
    imageFile.open(filename);

    const int pixelCount = width * height;
    const int pixelByteCount = 3 * pixelCount;
    const int OFFSET = FILEHEADERSIZE + INFOHEADERSIZE;
    const int fileSize = OFFSET + pixelByteCount;

    // Bitmap file header (14 bytes)
    // the header field used to identify the BMP and DIB file
    imageFile << 'B' << 'M';
    // the size of the BMP file in bytes
    // imageFile << swapEndianInt(fileSize);
    imageFile << (unsigned char)(fileSize >> 0)
              << (unsigned char)(fileSize >> 8)
              << (unsigned char)(fileSize >> 16)
              << (unsigned char)(fileSize >> 24);

    // reserved
    // imageFile << swapEndianInt(0);
    imageFile << (unsigned char)0 << (unsigned char)0 << (unsigned char)0
              << (unsigned char)0;
    // the offset
    // std::cout << sizeof(swapEndianInt(fileSize));
    // imageFile << swapEndianInt(OFFSET);
    imageFile << (unsigned char)(OFFSET >> 0) << (unsigned char)(OFFSET >> 8)
              << (unsigned char)(OFFSET >> 16) << (unsigned char)(OFFSET >> 24);

    // Bitmap info header (40 bytes)
    // the size of this header (40 bytes)
    // imageFile << swapEndianInt(INFOHEADERSIZE);
    imageFile << (unsigned char)(INFOHEADERSIZE >> 0)
              << (unsigned char)(INFOHEADERSIZE >> 8)
              << (unsigned char)(INFOHEADERSIZE >> 16)
              << (unsigned char)(INFOHEADERSIZE >> 24);
    // the bitmap width in pixels (signed integer)
    //    imageFile << swapEndianInt(width);
    imageFile << (unsigned char)(width >> 0) << (unsigned char)(width >> 8)
              << (unsigned char)(width >> 16) << (unsigned char)(width >> 24);
    // the bitmap height in pixels (signed integer)
    //    imageFile << swapEndianInt(height);
    imageFile << (unsigned char)(height >> 0) << (unsigned char)(height >> 8)
              << (unsigned char)(height >> 16) << (unsigned char)(height >> 24);
    // the number of color planes (must be 1), the number of bits per pixel
    imageFile << (unsigned char)1 << (unsigned char)0
              << (unsigned char)BITSPERPIXEL << (unsigned char)0;
    // the compression method being used
    imageFile << (unsigned char)0 << (unsigned char)0 << (unsigned char)0
              << (unsigned char)0;
    // the image size
    //    imageFile << swapEndianInt(pixelByteCount);
    imageFile << (unsigned char)(pixelByteCount >> 0)
              << (unsigned char)(pixelByteCount >> 8)
              << (unsigned char)(pixelByteCount >> 16)
              << (unsigned char)(pixelByteCount >> 24);
    // the horizontal resolution of the image
    //    imageFile << swapEndianInt(PPM);
    imageFile << (unsigned char)(PPM >> 0) << (unsigned char)(PPM >> 8)
              << (unsigned char)(PPM >> 16) << (unsigned char)(PPM >> 24);
    // the vertical resolution of the image
    //    imageFile << swapEndianInt(PPM);
    imageFile << (unsigned char)(PPM >> 0) << (unsigned char)(PPM >> 8)
              << (unsigned char)(PPM >> 16) << (unsigned char)(PPM >> 24);
    // the number of colors in the color palette
    imageFile << (unsigned char)0 << (unsigned char)0 << (unsigned char)0
              << (unsigned char)0;
    // the number of important colors used
    imageFile << (unsigned char)0 << (unsigned char)0 << (unsigned char)0
              << (unsigned char)0;
    // Print pixels
    imageFile.write((char *)pixels, pixelByteCount);
    //    fwrite(pixels, 1, )

    imageFile.close();
}

int Image::swapEndianInt(int sourceInt) {
    union {
        int u;
        unsigned char u8[sizeof(int)];
    } source, destination;
    source.u = sourceInt;
    destination.u8[0] = source.u8[3];
    destination.u8[1] = source.u8[2];
    destination.u8[2] = source.u8[1];
    destination.u8[3] = source.u8[0];

    return destination.u;
}

void Image::ViewBMP(const std::string &filename, int w, int h,
                    const unsigned char *pixels) {
    // Creates an image with three channels and sets it to black
    cimg_library::CImg<float> image((2 * w + 1), (2 * h + 1), 1, 3, 0);
    for (int y = 0; y <= (2 * h + 1); ++y) {
        for (int x = 0; x <= (2 * w + 1); ++x) {
            image(x, y, 0) = (pixels)[0 + 3 * (y * (2 * w + 1) + x)];
            image(x, y, 1) = (pixels)[1 + 3 * (y * (2 * w + 1) + x)];
            image(x, y, 2) = (pixels)[2 + 3 * (y * (2 * w + 1) + x)];
        }
    }

    // Save out the image in BMP format.Pixel values must be in the range
    // [0,255] delete pixels;
    image.save(filename.c_str());
    // Display the rendered image on screen
    cimg_library::CImgDisplay main_disp(image, "Render");
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
}
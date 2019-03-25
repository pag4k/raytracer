

#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image {
  public:
    static void GenerateBMP(const std::string &filename, int width, int height,
                            const unsigned char *pixels);
    static void ViewBMP(const std::string &filename, int w, int h,
                        const unsigned char *pixels);

  private:
    static int swapEndianInt(int sourceInt);
};

#endif // IMAGE_H

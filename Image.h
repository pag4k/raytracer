

#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image {
  public:
    static void SaveBMP(const std::string &filename, int w, int h,
                        const unsigned char *pixels);
};

#endif // IMAGE_H

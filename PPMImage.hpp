/**
  * \class PPMImage
  *
  * \brief A class to work with Raw PPM images
  *
  * This is a general class to read and write images in the PPM
  * format, please refer to the following link for more information
  * about the format: http://netpbm.sourceforge.net/doc/index.html
  *
  * \author $Author: Abhishek Tiwary $
  *
  * \version $Revision: 0.1 $
  *
  * \date $Date: 2021/09/26 $
  *
  * Contact: ab.tiwary@gmail.com
  *
  * Created on: Sun September 2021
  *
  * $Id: PPMImage.hpp,v 0.1 2021/09/26 Abhishek Tiwary $
  *
  */

#ifndef _PPM_IMAGE_H_
#define _PPM_IMAGE_H_

#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <strstream>

namespace PPMImage {

using MagicNumber = std::tuple<uint8_t, uint8_t>;
using Dimensions = std::tuple<size_t, size_t>;

class PPMImage {
public:
    PPMImage(std::string file_path) :
        m_file_path(file_path)
    {}

    ~PPMImage() {
        m_image_data.clear();
        if(m_infile.is_open()) {
            m_infile.close();
        }
    }

    bool ReadImageFile();
    bool WriteImageFile(std::string path_to_outfile);

    const std::vector<uint8_t>& GetImageData();

    size_t GetImageWidth();
    size_t GetImageHeight();

private:
    std::string m_file_path;
    std::ifstream m_infile;

    bool m_is_valid;
    bool m_image_was_read;

    size_t m_file_size;
    size_t m_width;
    size_t m_height;

    std::vector<std::string> m_comment_lines;
    std::vector<std::string> m_header_lines;
    std::vector<uint8_t> m_image_data;

    std::vector<uint32_t> m_rgba_data;

    Dimensions getWidthAndHeightFromLine(std::string &line);
};


inline size_t
PPMImage::GetImageWidth() {
    return m_width;
}

inline size_t
PPMImage::GetImageHeight() {
    return m_height;
}

inline const std::vector<uint8_t>&
PPMImage::GetImageData() {
    return m_image_data;
}

inline Dimensions
PPMImage::getWidthAndHeightFromLine(std::string &line) {
    std::vector<size_t> temp;
    std::stringstream ss{line};
    while(ss.good()) {
        std::string substr;
        std::getline(ss, substr, ' ');

        temp.push_back(std::strtod(substr.c_str(), nullptr));
    }

    Dimensions ret{ temp[0], temp[1] };
    return ret;
}

}
#endif // _PPM_IMAGE_H_

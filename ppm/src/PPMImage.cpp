/**
  * \brief The implementation of a class to work with Raw PPM images
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

#include "PPMImage.hpp"

bool
PPMImage::PPMImage::ReadImageFile() {
    auto retval = false;

    if(m_file_path.empty()) {
        return retval;
    }

    try {
        m_infile.open(m_file_path.c_str(), std::ios::in | std::ios::binary);
        m_infile.seekg(0, std::ios::end);
        m_file_size = m_infile.tellg();
        std::cout << "File size of " << m_file_path << " is: " << m_file_size << '\n';
        m_infile.seekg(0, std::ios::beg);

        std::string line;
        auto linecount = 0;
        bool done_first_line = false;

        while (m_infile.good()) {
            std::getline(m_infile, line);

            // process the first line to work out the magic number
            if (!done_first_line) {
                if(line[0] == 'P' && line[1]  == '6') {
                    m_is_valid = true;
                } else {
                    std::cout << "Invalid file type!\n";
                    break;
                }
                done_first_line = true;
                m_header_lines.emplace_back(line);
                linecount++;
                continue;
            }

            // append comment lines to the vector, else increment the line count
            if (line[0] == '#') {
                // comment line
                m_comment_lines.emplace_back(line);
                continue;
            } else {
                linecount++;
            }

            // get the width and the height from the second line
            if(linecount == 2) {
                // get the width and height
                auto [ width, height ] = getWidthAndHeightFromLine(line);
                m_width = width;
                m_height = height;
                m_header_lines.emplace_back(line);
            }

            // the following line contains the maximum color value - read that line first
            if(linecount == 3) {
                m_header_lines.emplace_back(line);
                m_image_data.resize(3 * m_width * m_height);
                m_infile.read(reinterpret_cast<char*>(&m_image_data[0]), 3 * m_width * m_height);
                retval = true;
                m_image_was_read = true;
                break;
            }
        }
    } catch(const std::runtime_error& e) {
        std::cerr << "A runtime error occurred: " << e.what() << "\n";
    } catch(const std::exception& ex) {
        std::cerr << "A exception occurred: " << ex.what() << "\n";
    } catch (...) {
        std::cerr << "An unknown error occurred: " << "\n";
    }

    return retval;
}

bool
PPMImage::PPMImage::WriteImageFile(std::string path_to_outfile) {
    if (!m_is_valid || !m_image_was_read) {
        return false;
    }
    auto write_successful = true;
    std::ofstream outfile;
    outfile.open(path_to_outfile, std::ios::out | std::ios::binary | std::ios::trunc);
    try {
        // header lines
        for (const auto& line : m_header_lines) {
            std::strstream ss;
            ss << line << "\n";
            std::string temp_str = ss.str();
            outfile.write(temp_str.c_str(), temp_str.size());
        }
        outfile.write(reinterpret_cast<char*>(m_image_data.data()), m_image_data.size());
    } catch (...) {
        write_successful = false;
    }
    outfile.close();
    return write_successful;
}

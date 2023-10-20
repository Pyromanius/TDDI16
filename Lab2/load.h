#pragma once
#include "image.h"
#include <string>
#include <vector>
#include <exception>

// Find all files with the type 'type' in the specified directory.
std::vector<std::string> list_files(const std::string &dir, const std::string &type = ".jpg");

// Load an image from a file.
Image load_image(const std::string &path);


/**
 * Exception thrown on load failure.
 */
class Image_Load_Error : public std::exception {
public:
    Image_Load_Error(const char *message)
        : message(message) {}

    virtual const char *what() const noexcept {
        return message;
    }

private:
    const char *message;
};

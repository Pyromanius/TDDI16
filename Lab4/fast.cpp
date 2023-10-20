#include "image.h"
#include "load.h"
#include "window.h"
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

/**
 * Class that stores a summary of an image.
 *
 * This summary is intended to contain a high-level representation of the
 * important parts of an image. I.e. it shall contain what a human eye would
 * find relevant, while ignoring things that the human eye would find
 * irrelevant.
 *
 * To approximate human perception, we store a series of booleans that indicate
 * if the brightness of the image has increased or not. We do this for all
 * horizontal lines and vertical lines in a downsampled version of the image.
 *
 * See the lab instructions for more details.
 *
 * Note: You will need to use this data structure as the key in a hash table. As
 * such, you will need to implement equality checks and a hash function for this
 * data structure.
 */
class Image_Summary {
  public:
    // Horizontal increases in brightness.
    vector<bool> horizontal;

    // Vertical increases in brightness.
    vector<bool> vertical;
};

inline bool operator==(const Image_Summary &lhs, const Image_Summary &rhs) {
    return lhs.horizontal == rhs.horizontal && lhs.vertical == rhs.vertical;
}

template <> struct std::hash<Image_Summary> {
    std::size_t operator()(Image_Summary const &s) const noexcept {
        // From boost library.
        size_t hash1 = std::hash<vector<bool>>{}(s.horizontal);
        size_t hash2 = std::hash<vector<bool>>{}(s.vertical);
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash2 >> 2));
    }
};

const size_t summary_size = 7; // Hittar en falsk dublett vid 5 (d1/d2), och
                               // missar b2 vid 7, 8 finner d1/d2 men ej e1/e2

// Compute an Image_Summary from an image. This is described in detail in the
// lab instructions.
Image_Summary compute_summary(const Image &image) {
    Image_Summary result;

    result.horizontal.reserve(image.height() * (image.width() - 1));
    result.vertical.reserve(image.width() * (image.height() - 1));

    for (size_t x = 0; x < image.width(); x++) {
        for (size_t y = 0; y < image.height(); y++) {
            const auto &current = image.pixel(x, y);

            if (x < image.width() - 1) {
                const Pixel &horizontal_next = image.pixel(x + 1, y);
                result.horizontal.push_back(current.brightness() <
                                            horizontal_next.brightness());
            }

            if (y < image.height() - 1) {
                const Pixel &vertical_next = image.pixel(x, y + 1);
                result.vertical.push_back(current.brightness() <
                                          vertical_next.brightness());
            }
        }
    }

    return result;
}

int main(int argc, const char *argv[]) {
    WindowPtr window = Window::create(argc, argv);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [--nopause] [--nowindow] <directory>"
             << endl;
        cerr << "Missing directory containing files!" << endl;
        return 1;
    }

    vector<string> files = list_files(argv[1], ".jpg");
    cout << "Found " << files.size() << " image files." << endl;

    if (files.size() <= 0) {
        cerr << "No files found! Make sure you entered a proper path!" << endl;
        return 1;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    window->show_single("Loading images...", load_image(files[0]), false);

    vector<std::pair<std::string, Image>> images;
    for (const auto &file : files)
        images.push_back(std::make_pair(
            file, load_image(file).shrink(summary_size, summary_size)));

    auto load_time = std::chrono::high_resolution_clock::now();
    cout << "Loading images took: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(load_time -
                                                                  begin)
                .count()
         << " milliseconds." << endl;

    window->show_single("Comparing images...", load_image(files[1]), false);

    unordered_map<Image_Summary, vector<std::string>> hashmap;
    for (auto &[filename, image] : images) {
        auto summary = compute_summary(image);
        if (hashmap.find(summary) == hashmap.end()) {
            std::vector<std::string> matches{std::move(filename)};
            hashmap.insert(std::make_pair(summary, std::move(matches)));
        } else {
            hashmap.at(summary).push_back(std::move(filename));
        }
    }

    for (auto const &pair : hashmap) {
        if (pair.second.size() > 1) {
            window->report_match(pair.second);
            // } else {
            //     std::cout << "No match for " << pair.second.front() <<
            //     std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    cout << "Total time: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
                .count()
         << " milliseconds." << endl;

    return 0;
}

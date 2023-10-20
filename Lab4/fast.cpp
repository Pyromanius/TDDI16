/*
 * fast < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "point.h"
#include "window.h"

using namespace std;

ssize_t find_next_consecutive(vector<Point> const& points, Point origin, size_t start, size_t &count) {
    if (start >= points.size()) return -1;

    size_t constart = start;
    count = 0;
    while (constart < points.size()) {
        auto target = abs(origin.slopeTo(points.at(constart)));
        count = 1;

         // std::cout << constart << " " << target << std::endl;

        for (size_t curr = constart + 1; 
            curr < points.size() && abs(origin.slopeTo(points.at(curr))) == target; 
            curr++) 
        {
            // std::cout << " " << curr << std::endl;
            count += 1;
        }


        if (count > 2) {
            break;
        }

        constart += count;
    }

    if (count > 2) return constart;
    return -1;
}

int main(int argc, const char* argv[]) {
    WindowPtr window = create_window(argc, argv);

    // The array of points
    vector<Point> points;

    // Read points from cin
    int N{};
    cin >> N;

    for (int i{0}; i < N; ++i) {
        unsigned int x, y;
        cin >> x >> y;
        points.push_back(Point{x, y});
    }

    // draw points to screen all at once
    window->draw(points);

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());

    auto begin = chrono::high_resolution_clock::now();

    int line_count = 0;
    for (size_t i = 0; i < points.size(); i++) {
        sort(points.begin(), points.end(), PolarSorter(points.at(i)));

        size_t con = -1;
        size_t count = 0;
        bool printed = false;
        while ((con = find_next_consecutive(points, points.at(i), con + 1, count)) != -1) {
            line_count += 1;
            Point from = points.at(con);
            Point to = points.at(con + count - 1);
            std::cout << from.x << "," << from.y << " -> ";
            std::cout << to.x << "," << to.y << std::endl;
            window->draw(from, to);

            // std::cout << con << ":" << count << " "; printed = true;
            con += count;
        }
        if (printed) std::cout << std::endl;
        
    }

    std::cout << "lines: " << line_count << std::endl;

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}

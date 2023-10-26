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

vector<vector<Point>> find_consecutives(vector<Point>& points, Point origin) {
    vector<vector<Point>> result{};

    sort(points.begin(), points.end(), PolarSorter(origin));
    for (size_t i = 0; i < points.size(); i++) {
        vector<Point> series{};
        auto target = abs(origin.slopeTo(points.at(i)));
        for (size_t j = i; 
            j <points.size() && abs(origin.slopeTo(points.at(j))) == target; 
            j++) 
        {
            series.push_back(points.at(j));
        }

        if (series.size() > 2) {
            result.push_back(std::move(series));
            i += series.size();
        }
    }

    return result;
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

    for (Point point : points) {
        auto result = find_consecutives(points, point);
        for (auto const& series : result) {
            for (Point series_point : series) {
                window->draw(point, series_point);
            }
        }
    }

    // std::cout << "lines: " << line_count << std::endl;

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}
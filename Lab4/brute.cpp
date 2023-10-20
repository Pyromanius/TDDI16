/*
 * brute < input.txt
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

    // iterate through all combinations of 4 points
    for (int i{0} ; i < N-3 ; ++i) {
        for (int j{i+1} ; j < N-2 ; ++j) {
            for (int k{j+1} ; k < N-1 ; ++k) {
                //only consider fourth point if first three are colinear
                if (points[i].slopeTo(points[j]) == points[i].slopeTo(points[k])) {
                    for (int m{k+1} ; m < N ; ++m) {
                        if (points.at(i).slopeTo(points[j]) == points[i].slopeTo(points[m])) {
                            window->draw(points[i], points[m]);
                        }
                    }
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}

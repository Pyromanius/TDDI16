#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

auto word_neighbors(string const &word, std::unordered_set<string> const &dict)
    -> vector<string> {
    vector<string> neighbors{};
    for (size_t ci = 0; ci < word.size(); ci++) {
        for (char c = 'a'; c < 'z'; c++) {
            string copy = word;
            copy[ci] = c;

            if (copy == word || dict.find(copy) == dict.end())
                continue;

            neighbors.push_back(copy);
        }
    }

    return neighbors;
}

auto bfs(std::unordered_set<string> const &dict, string const &from,
         std::optional<string> to) -> std::vector<string> {
    if (to.has_value() &&
        (from == to.value() || dict.find(to.value()) == dict.end()))
        return std::vector<string>{};

    if (dict.find(from) == dict.end())
        return std::vector<string>{};

    std::unordered_set<string> visisted{};
    std::unordered_map<string, string> prev{};
    visisted.reserve(dict.size());
    prev.reserve(dict.size());

    std::queue<std::unordered_set<string>::const_iterator> q;
    q.push(dict.find(from));
    visisted.insert(from);

    string furthestWord = from;

    while (!q.empty()) {
        string const &curr = *q.front();
        q.pop();

        auto neighbors = word_neighbors(curr, dict);
        for (string const &neighbor : neighbors) {
            if (visisted.find(neighbor) == visisted.end()) {
                visisted.insert(neighbor);
                prev.insert(std::make_pair(neighbor, curr));

                furthestWord = neighbor;

                q.push(dict.find(neighbor));
            }

            if (to.has_value() && neighbor == to)
                goto end;
        }
    }
end:

    vector<string> path{};
    path.push_back(furthestWord);

    string curr = furthestWord;
    if (to.has_value())
        curr = to.value();

    for (;;) {
        auto const &prev_it = prev.find(curr);
        if (prev_it == prev.end())
            break;
        path.push_back(prev_it->second);
        curr = prev_it->second;
    }

    if (path.size() == 1)
        return std::vector<string>{};

    std::reverse(path.begin(), path.end());

    return path;
}

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som
 * finns i 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska
 * vara 'from' och sista 'to'. Om ingen ordkedja hittas kan en tom vector
 * returneras.
 */
vector<string> find_shortest(std::unordered_set<string> const &dict,
                             const string &from, const string &to) {
    return bfs(dict, from, to);
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'.
 * Returvärdet är den ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(std::unordered_set<string> const &dict,
                            const string &word) {
    return bfs(dict, word, std::nullopt);
}

/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser
 * även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
std::unordered_set<string> read_dictionary() {
    string line;
    std::unordered_set<string> result;
    while (std::getline(std::cin, line)) {
        if (line == "#")
            break;

        result.insert(line);
    }

    return result;
}

/**
 * Skriv ut en ordkedja på en rad.
 */
void print_chain(const vector<string> &chain) {
    if (chain.empty())
        return;

    vector<string>::const_iterator i = chain.begin();
    cout << *i;

    for (++i; i != chain.end(); ++i)
        cout << " -> " << *i;

    cout << endl;
}

/**
 * Skriv ut ": X ord" och sedan en ordkedja om det behövs. Om ordkedjan är tom,
 * skriv "ingen lösning".
 */
void print_answer(const vector<string> &chain) {
    if (chain.empty()) {
        cout << "ingen lösning" << endl;
    } else {
        cout << chain.size() << " ord" << endl;
        print_chain(chain);
    }
}

/**
 * Läs in alla frågor. Anropar funktionerna "find_shortest" eller "find_longest"
 * ovan när en fråga hittas.
 */
void read_questions(std::unordered_set<string> const &g) {
    string line;
    while (std::getline(std::cin, line)) {
        size_t space = line.find(' ');
        if (space != string::npos) {
            string first = line.substr(0, space);
            string second = line.substr(space + 1);
            vector<string> chain = find_shortest(g, first, second);

            cout << first << " " << second << ": ";
            print_answer(chain);
        } else {
            vector<string> chain = find_longest(g, line);

            cout << line << ": ";
            print_answer(chain);
        }
    }
}

int main() {
    auto dict = read_dictionary();
    read_questions(dict);

    return 0;
}

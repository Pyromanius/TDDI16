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
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

template <typename T> class Graph {
  private:
    struct Entry {
        size_t node;
    };

    std::vector<std::list<Entry>> entryList{};
    vector<T> nodes;
    uint32_t num_edges;

  public:
    Graph(size_t n) : nodes{n} {
        for (int i = 0; i < n; i++) {
            entryList.emplace_back();
        }
        num_edges = 0;
    }

    auto node_count() const -> size_t { return nodes.size(); }
    auto edge_count() const -> size_t { return num_edges; }

    auto find_entry(size_t v, size_t w) -> std::optional<Entry *> {
        auto &row = entryList[v];
        for (int i = 0; i < row.size(); i++) {
            if (row[i].node == w)
                return &row[i];
        }
    }

    auto find_entry(size_t v, size_t w) const -> std::optional<Entry const *> {
        auto const &row = entryList[v];
        auto it = row.begin();
        for (int i = 0; it != row.end(); i++, it++) {
            if (it->node == w)
                return &*it;
        }
        return std::nullopt;
    }

    auto weight(size_t v, size_t w) const -> int32_t {
        auto opt = find_entry(v, w);
        return opt.has_value() ? 1 : 0;
    }
    auto has_edge(size_t v, size_t w) const -> bool {
        return weight(v, w) != 0;
    }
    auto at(size_t v) const -> T const & { return nodes[v]; }
    auto at(size_t v) -> T & { return nodes[v]; }

    auto find(T const &v) const -> std::optional<size_t> {
        for (int i = 0; i < nodes.size(); i++) {
            if (at(i) == v)
                return i;
        }
        return std::nullopt;
    }

    void set(size_t v, T &&o) { nodes[v] = o; }
    void set(size_t v, T const &o) { nodes[v] = o; }

    void add_edge(size_t v, size_t w) {
        std::optional<Entry *> entry{};
        auto &row = entryList[v];
        for (auto &rowEntry : row) {
            if (rowEntry.node == w) {
                entry = &rowEntry;
                break;
            }
        }
        if (!entry.has_value()) {
            row.push_back(Entry{w});
        }
        num_edges += 1;
    }

    auto neighbors(size_t v) const -> vector<size_t> {
        vector<size_t> neighbors{};
        auto const &row = entryList[v];
        for (auto const &entry : row) {
            neighbors.push_back(entry.node);
        }
        return neighbors;
    }

    auto djikstra_pq(size_t s, std::optional<string> const &target) const
        -> std::pair<vector<int>, vector<std::optional<size_t>>> {
        vector<int> d{};
        vector<std::optional<size_t>> prev{};
        vector<bool> visited{};
        for (size_t i = 0; i < node_count(); i++) {
            d.push_back(__INT_MAX__);
            prev.push_back(std::nullopt);
            visited.push_back(false);
        }
        d[s] = 0; // Start node will the lowest unmarked node

        auto customLess = [](const std::pair<int, int> l,
                             const std::pair<int, int> r) {
            return l.first > r.first;
        };
        std::priority_queue<std::pair<int, int>,
                            std::vector<std::pair<int, int>>,
                            decltype(customLess)>
            pq(customLess);

        pq.push(std::make_pair(0, s));

        for (int i = 0; i < node_count(); i++) {
            size_t v = 0;
            do {
                if (pq.empty())
                    goto done;
                std::pair<int, int> temp = pq.top();
                pq.pop();
                v = temp.second;
            } while (visited[v]);
            visited[v] = true;
            if (d[v] == __INT_MAX__)
                goto done;

            auto ns = neighbors(v);
            for (size_t w : ns) {
                int dist = d[v] + weight(v, w);
                if (d[w] > dist) {
                    d[w] = dist;
                    prev[w] = v;
                    pq.push(std::make_pair(d[w], w));
                }

                if (target.has_value() && at(w) == target.value()) {
                    goto done;
                }
            }
        }

    done:

        return std::make_pair(std::move(d), std::move(prev));
    }

  private:
    auto min_vertex(vector<int> const &d, vector<bool> const &visited) const
        -> size_t {
        size_t v = 0;
        for (size_t i = 0; i < node_count(); i++) {
            if (!visited.at(i)) {
                v = i;
                break;
            }
        }

        for (size_t i = 0; i < node_count(); i++) {
            if (!visited.at(i) && (d[i] < d[v]))
                v = i;
        }

        return v;
    }
};

int char_diff(std::string const &a, std::string const &b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            diff += 1;
        }
    }
    return diff;
}

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som
 * finns i 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska
 * vara 'from' och sista 'to'. Om ingen ordkedja hittas kan en tom vector
 * returneras.
 */
vector<string> find_shortest(Graph<string> const &g, const string &from,
                             const string &to) {
    vector<string> result;

    size_t from_idx = g.find(from).value();
    size_t to_idx = g.find(to).value();

    auto [lengths, prevs] = g.djikstra_pq(from_idx, to);

    std::optional<size_t> current = to_idx;
    while (current != std::nullopt) {
        result.push_back(g.at(current.value()));
        current = prevs[current.value()];
    }

    std::reverse(result.begin(), result.end());

    if (result.front() != from) {
        vector<string> empty{};
        return empty;
    }

    return result;
}

template <typename T> void dump(vector<T> v, string file) {
    std::ofstream of(file);
    for (auto const &e : v) {
        of << e << std::endl;
    }
    of.close();
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'.
 * Returvärdet är den ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(Graph<string> const &g, const string &word) {
    vector<string> result{};

    size_t word_idx = g.find(word).value();

    auto [lengths, prevs] = g.djikstra_pq(word_idx, std::nullopt);
    dump(lengths, "out/" + word);
    auto longest = word_idx;
    for (size_t i = 0; i < lengths.size(); i++) {
        if (lengths[i] != __INT_MAX__ && lengths[i] > lengths[longest]) {
            longest = i;
        }
    }

    std::optional<size_t> current = longest;
    while (current != std::nullopt) {
        result.push_back(g.at(current.value()));
        current = prevs[current.value()];
    }

    return result;
}

/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser
 * även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
vector<string> read_dictionary() {
    string line;
    vector<string> result;
    while (std::getline(std::cin, line)) {
        if (line == "#")
            break;

        result.push_back(line);
    }

    return vector<string>(result.begin(), result.end());
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
void read_questions(Graph<string> const &g) {
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

std::string human_readable_bytes(double len) {
    std::vector<std::string> sizes{"B", "KiB", "MiB", "GiB", "TiB"};
    size_t order = 0;
    while (len >= 1000 && order < sizes.size() - 1) {
        order++;
        len = len / 1000;
    }

    std::stringstream ss;
    ss << len << " " << sizes[order];
    return ss.str();
}

int main() {
    // -- TEST CASES --
    Graph<std::string> graph(5);
    graph.set(0, "A");
    graph.set(1, "B");
    graph.set(2, "C");
    graph.set(3, "D");
    graph.set(4, "E");

    graph.add_edge(4, 2);
    graph.add_edge(0, 1);
    graph.add_edge(0, 4);
    graph.add_edge(1, 3);
    graph.add_edge(3, 4);

    graph.add_edge(2, 4);
    graph.add_edge(1, 0);
    graph.add_edge(4, 0);
    graph.add_edge(3, 1);
    graph.add_edge(4, 3);

    auto [lengths, prevs] = graph.djikstra_pq(1, std::nullopt);

    if (graph.djikstra_pq(1, std::nullopt).first.at(2) != 3)
        throw std::exception();
    // -- END OF TEST CASES --

    auto dict = read_dictionary();

    std::cout << "----- Building Graph -----" << std::endl;

    std::unordered_map<string, size_t> word_idx_map{};
    word_idx_map.reserve(dict.size());

    Graph<string> g(dict.size());

    for (size_t i = 0; i < dict.size(); i++) {
        string &w = dict.at(i);
        g.set(i, w);
        word_idx_map.insert(std::make_pair(w, i));
    }

    for (size_t i = 0; i < dict.size(); i++) {
        auto const &curr = dict.at(i);
        g.set(i, curr);

        if (i % (1 + dict.size() / 6) == 1)
            std::cout << static_cast<int>(static_cast<float>(i) / dict.size() *
                                          100)
                      << "%" << std::endl;

        for (size_t ci = 0; ci < curr.size(); ci++) {
            for (char c = 'a'; c < 'z'; c++) {
                string copy = curr;
                copy[ci] = c;

                auto it = word_idx_map.find(copy);
                if (copy != curr && it != word_idx_map.end()) {
                    g.add_edge(i, it->second);
                }
            }
        }
    }
    std::cout << "100%" << std::endl;
    std::cout << "---- Graph Statistics ----" << std::endl;
    std::cout << "V: " << g.node_count() << std::endl;
    std::cout << "E: " << g.edge_count() << std::endl;
    std::cout << std::endl;
    std::cout << "Adjency Matrix: "
              << human_readable_bytes(
                     static_cast<double>(g.node_count()) *
                         static_cast<double>(g.node_count()) * sizeof(size_t) +
                     static_cast<double>(g.node_count()) * sizeof(uint32_t))
              << std::endl;
    std::cout << "Adjency List:   "
              << human_readable_bytes(
                     static_cast<double>(g.node_count()) * sizeof(size_t) +
                     static_cast<double>(g.edge_count()) * (3 * sizeof(size_t)))
              << std::endl;
    std::cout << std::endl;
    std::cout << "Djikstra:   "
              << (static_cast<double>(g.node_count()) *
                  static_cast<double>(g.node_count()))
              << std::endl;
    std::cout << "DjikstraPQ: "
              << ((static_cast<double>(g.node_count()) +
                   static_cast<double>(g.edge_count())) *
                  std::log2(static_cast<double>(g.edge_count())))
              << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << std::endl;
    read_questions(g);

    return 0;
}

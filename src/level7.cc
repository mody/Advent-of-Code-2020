#include <cassert>
#include <iostream>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <string>

static const std::regex TOP_BAG_RX("^([a-z ]+) bags contain (.*)$");
static const std::regex INNER_BAG_RX("([0-9]+) ([a-z ]+) bag");

struct Bag {
    std::string name;
    unsigned count = 0;
};

unsigned examine_bag(
    std::map<std::string, std::vector<Bag>> const& tree2, std::string const& examine_name)
{
    auto it = tree2.find(examine_name);
    if (it == tree2.end()) {
        std::cout << examine_name << " not found = 0\n";
        return 0;
    }

    unsigned sum = 0;
    for (const auto& sub : it->second) {
        sum += sub.count + (sub.count * examine_bag(tree2, sub.name));
    }
    return sum;
}

int main(int argc, char* argv[]) {
    std::map<std::string, std::set<std::string>> tree;
    std::map<std::string, std::vector<Bag>> tree2;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        std::smatch m;
        if (!std::regex_match(line, m, TOP_BAG_RX)) {
            std::cout << "FAILED: " << line << "\n";
            exit(1);
        }
        assert(m.size() == 3);

        const std::string top_bag{m.str(1)};
        (void)tree2[top_bag];

        const std::string second{m.str(2)};
        if (second == "no other bags.") { continue; }

        std::sregex_iterator it{second.begin(), second.end(), INNER_BAG_RX}, end;
        for (; it != end; ++it) {
            assert(it->size() == 3);
            const unsigned cnt = std::stoi(it->str(1));
            const std::string name = it->str(2);
            tree[name].insert(top_bag);
            tree2[top_bag].push_back({name, cnt});
        }
    }

    std::set<std::string> possibilities;

    std::queue<std::string> q;
    q.push("shiny gold");

    for (; !q.empty();) {
        const auto bag = q.front();
        q.pop();
        auto it = tree.find(bag);
        if (it == tree.end()) {
            continue;
        }
        for (const auto& sub : it->second) {
            if (possibilities.insert(sub).second) {
                q.push(sub);
            }
        }
    }
    std::cout << "Possibilities: " << possibilities.size() << "\n";

    unsigned bag_sum = examine_bag(tree2, "shiny gold");
    std::cout << "Inner bags count: " << bag_sum << "\n";


    return 0;
}

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <map>
#include <set>
#include <string>

using Lines = std::vector<std::string>;

int main(int argc, char* argv[])
{
    std::map<std::string, std::set<std::string>> data;
    std::map<std::string, unsigned> all_ingredients;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) { break; }

        std::string _ingredients, _alergens;

        auto pos = line.find("(");
        if (pos != std::string::npos) {
            _ingredients = line.substr(0, pos);
            pos = line.find(" ", pos);
            _alergens = line.substr(pos + 1, line.size() - pos - 2);  // minus the last ')'
        } else {
            _ingredients = line;
        }

        Lines ingredients, alergens;
        boost::algorithm::split(ingredients, _ingredients, boost::algorithm::is_any_of(" "));
        boost::algorithm::split(alergens, _alergens, boost::algorithm::is_any_of(", "));


        std::map<std::string, std::set<std::string>> d2;

        for (auto const& i : ingredients) {
            if (i.empty()) continue;
            all_ingredients.insert({i, 0}).first->second++;
            for (auto const& a : alergens) {
                if (a.empty()) continue;
                d2[a].insert(i);
            }
        }

        for (auto& [k, s] : d2) {
            auto it = data.find(k);
            if (it == data.end()) {
                data[k] = s;
                continue;
            }
            std::set<std::string> intersection;
            std::set_intersection(
                it->second.begin(), it->second.end(), s.begin(), s.end(),
                std::inserter(intersection, intersection.begin()));
            std::swap(it->second, intersection);
        }
    }

    unsigned count = 0;
    for (auto const& [i, cnt] : all_ingredients) {
        count += cnt;
    }

    std::cout << "1 Result=" << count << "\n\n";


    std::map<std::string, std::string> res2;

    for (auto it = data.begin(); it != data.end();) {
        if (it->second.size() == 1) {
            auto i = *(it->second.begin());
            res2.insert({it->first, i});
            for (auto i2 = data.begin(); i2 != data.end(); ++i2) {
                i2->second.erase(i);
            }
            data.erase(it);
            it = data.begin();
        } else {
            ++it;
        }
    }

    std::cout << "2 Result=";
    for (auto const& [l, i] : res2) {
        std::cout << i << ",";
    }
    std::cout << "\n";

    return 0;
}

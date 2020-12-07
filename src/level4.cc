#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

const std::regex FIELD_RX{"(...):([^ ]+)"};
const std::regex HAIR_RX{"^#[0-9a-f]{6}"};
const std::regex PID_RX{"^[0-9]{9}"};


bool validate_number(const std::string data, unsigned min, unsigned max) {
    unsigned value = std::stoi(data);
    return min <= value && value <= max;
}

bool validate_height(const std::string& data) {
    if (boost::algorithm::ends_with(data, "cm")) {
        return validate_number(data, 150, 193);
    } else if (boost::algorithm::ends_with(data, "in")) {
        return validate_number(data, 59, 76);
    }
    return false;
}

bool validate_hair(const std::string& data) {
    std::smatch m;
    return std::regex_match(data, m, HAIR_RX);
}

bool validate_eye(const std::string& data) {
    static const std::set<std::string> VALUES = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    return VALUES.count(data) == 1;
}

bool validate_pid(const std::string& data) {
    std::smatch m;
    return std::regex_match(data, m, PID_RX);
}

const std::map<std::string, std::function<bool(const std::string&)>> FIELDS = {
    {"byr", [](const std::string& data) -> bool { return validate_number(data, 1920, 2002); }},
    {"iyr", [](const std::string& data) -> bool { return validate_number(data, 2010, 2020); }},
    {"eyr", [](const std::string& data) -> bool { return validate_number(data, 2020, 2030); }},
    {"hgt", [](const std::string& data) -> bool { return validate_height(data); }},
    {"hcl", [](const std::string& data) -> bool { return validate_hair(data); }},
    {"ecl", [](const std::string& data) -> bool { return validate_eye(data); }},
    {"pid", [](const std::string& data) -> bool { return validate_pid(data); }}};


bool validate(const std::vector<std::string>& data) {
    const std::string raw = boost::algorithm::join(data, " ");
    std::set<std::string> fields;

    std::sregex_iterator it{raw.begin(), raw.end(), FIELD_RX}, end;
    for (; it != end; ++it) {
        const auto& f = it->str(1);
        if (FIELDS.count(f) == 1 && FIELDS.at(f)(it->str(2))) {
            fields.insert(f);
        }
    }

    return FIELDS.size() == fields.size();
}

int main(int argc, char* argv[]) {
    std::vector<std::string> data;

    unsigned valid = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            if (validate(data)) {
                ++valid;
            }
            data.clear();
        }
        data.push_back(line);
    }

    if (validate(data)) {
        ++valid;
    }

    std::cout << "Valid passports: " << valid << "\n";
    return 0;
}

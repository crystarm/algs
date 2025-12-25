#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <cctype>

class JsonParser {
public:
    explicit JsonParser(const std::string& filename) : cursor(0) {
        std::ifstream file(filename, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            source.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(&source[0], source.size());
        }
    }

    bool is_valid() const {
        return !source.empty();
    }

    std::map<std::string, int> process() {
        std::map<std::string, int> max_counts;

        skip_whitespace();
        if (at_end() || source[cursor] != '[') return max_counts;
        cursor++;

        while (!at_end()) {
            skip_whitespace();
            if (peek() == ']') break;

            if (peek() == '{') {
                parse_object(max_counts);
            } else {
                cursor++;
            }

            skip_whitespace();
            if (peek() == ',') cursor++;
        }
        return max_counts;
    }

private:
    std::string source;
    size_t cursor;

    bool at_end() const {
        return cursor >= source.size();
    }

    char peek() const {
        if (at_end()) return '\0';
        return source[cursor];
    }

    void skip_whitespace() {
        while (!at_end() && std::isspace(static_cast<unsigned char>(source[cursor]))) {
            cursor++;
        }
    }

    std::string parse_string() {
        std::string result;
        if (peek() != '"') return result;
        cursor++;

        while (!at_end()) {
            char c = source[cursor];
            if (c == '"') {
                cursor++;
                return result;
            }
            if (c == '\\') {
                cursor++;
                if (!at_end()) {
                    result += source[cursor];
                    cursor++;
                }
            } else {
                result += c;
                cursor++;
            }
        }
        return result;
    }

    int parse_int() {
        skip_whitespace();
        size_t start = cursor;
        if (!at_end() && source[cursor] == '-') cursor++;
        while (!at_end() && std::isdigit(static_cast<unsigned char>(source[cursor]))) {
            cursor++;
        }

        if (start == cursor) return 0;
        try {
            return std::stoi(source.substr(start, cursor - start));
        } catch (...) {
            return 0;
        }
    }

    void ignore_value() {
        skip_whitespace();
        char c = peek();
        if (c == '"') {
            parse_string();
        } else if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') {
            parse_int();
        } else if (c == '{') {
            int balance = 1;
            cursor++;
            while (!at_end() && balance > 0) {
                if (source[cursor] == '{') balance++;
                else if (source[cursor] == '}') balance--;
                cursor++;
            }
        } else {
            while (!at_end() && source[cursor] != ',' && source[cursor] != '}') {
                cursor++;
            }
        }
    }

    void parse_object(std::map<std::string, int>& max_counts) {
        cursor++;
        std::string date;
        int count = -1;
        bool has_date = false;
        bool has_count = false;

        while (!at_end()) {
            skip_whitespace();
            if (peek() == '}') {
                cursor++;
                break;
            }

            std::string key = parse_string();
            skip_whitespace();
            if (peek() == ':') cursor++;

            if (key == "date") {
                date = parse_string();
                has_date = true;
            } else if (key == "count") {
                count = parse_int();
                has_count = true;
            } else {
                ignore_value();
            }

            skip_whitespace();
            if (peek() == ',') cursor++;
        }

        if (has_date && has_count) {
            if (max_counts.find(date) == max_counts.end()) {
                max_counts[date] = count;
            } else {
                max_counts[date] = std::max(max_counts[date], count);
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    JsonParser parser("data.json");
    if (!parser.is_valid()) return 1;

    auto counts = parser.process();

    if (counts.empty()) return 0;

    int min_peak = std::numeric_limits<int>::max();
    for (const auto& [date, val] : counts) {
        if (val < min_peak) {
            min_peak = val;
        }
    }

    for (const auto& [date, val] : counts) {
        if (val == min_peak) {
            std::cout << date << "\n";
        }
    }

    return 0;
}

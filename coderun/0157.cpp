// https://coderun.yandex.ru/problem/qx-d
// FSM + string parsing + data aggregation

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <cctype>

std::string json_content;
size_t pos = 0;

void skip_whitespace()
{ while (pos < json_content.size() && std::isspace(static_cast<unsigned char>(json_content[pos]))) pos++; }

std::string parse_string()
{
    std::string result;
    if (pos >= json_content.size() || json_content[pos] != '"') return "";
    pos++;

    while (pos < json_content.size())
    {
        char c = json_content[pos];
        if (c == '"') { pos++; return result; }
        if (c == '\\')
        {
            pos++;
            if (pos < json_content.size())
            { result += json_content[pos]; pos++; }
        } else {result += c; pos++; }
    }
    return result;
}

int parse_int()
{
    skip_whitespace();
    size_t start = pos;
    if (pos < json_content.size() && json_content[pos] == '-') pos++;

    while (pos < json_content.size() && std::isdigit(static_cast<unsigned char>(json_content[pos]))) pos++;

    if (start == pos) return 0;

    try
    { return std::stoi(json_content.substr(start, pos - start)); }
    catch (...) { return 0; }
}

void skip_value()
{
    skip_whitespace();
    if (pos >= json_content.size()) return;

    char c = json_content[pos];
    if (c == '"') parse_string();
    else if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') parse_int();
    else if (c == '{')
    {
        int balance = 1;
        pos++;
        while (pos < json_content.size() && balance > 0)
        {
            if (json_content[pos] == '{') balance++;
            if (json_content[pos] == '}') balance--;
            pos++;
        }
    }
    else while (pos < json_content.size() && json_content[pos] != ',' && json_content[pos] != '}') pos++;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::ifstream file("data.json");
    if (!file.is_open()) return 1;

    file.seekg(0, std::ios::end);
    json_content.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    json_content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::map<std::string, int> date_max_counts;

    pos = 0;
    skip_whitespace();

    if (pos < json_content.size() && json_content[pos] == '[') pos++;

    while (pos < json_content.size())
    {
        skip_whitespace();
        if (json_content[pos] == ']') break;

        if (json_content[pos] == '{')
        {
            pos++;

            std::string current_date;
            int current_count = -1;
            bool has_date = false;
            bool has_count = false;

            while (pos < json_content.size())
            {
                skip_whitespace();
                if (json_content[pos] == '}') { pos++; break; }

                std::string key = parse_string();

                skip_whitespace();
                if (json_content[pos] == ':') pos++;

                skip_whitespace();

                if (key == "date") { current_date = parse_string(); has_date = true; }
                else if (key == "count") { current_count = parse_int(); has_count = true; }
                else skip_value();


                skip_whitespace();
                if (json_content[pos] == ',') pos++;
            }

            if (has_date && has_count)
            {
                if (date_max_counts.find(current_date) == date_max_counts.end())
                { date_max_counts[current_date] = current_count; }
                else date_max_counts[current_date] = std::max(date_max_counts[current_date], current_count);
            }
        } else pos++;

        skip_whitespace();
        if (json_content[pos] == ',') pos++;
    }

    if (date_max_counts.empty()) return 0;

    int min_of_maxes = std::numeric_limits<int>::max();

    for (const auto& [date, max_val] : date_max_counts) if (max_val < min_of_maxes) min_of_maxes = max_val;
    for (const auto& [date, max_val] : date_max_counts) if (max_val == min_of_maxes) std::cout << date << "\n";


    return 0;
}

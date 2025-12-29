// https://acmp.ru/index.asp?main=task&id_task=937
// scopes + recurcive parsing + interpreting

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

using str = string;
using ctx_t = map<str, str>;

str parse_id(const str& buf, int& ptr)
{
    str id;
    while (ptr < buf.size() && islower(buf[ptr])) id += buf[ptr++];
    if (ptr < buf.size() && buf[ptr] == ' ') ptr++;
    return id;
}

int parse_int(const str& buf, int& ptr)
{
    str num_s;
    while (ptr < buf.size() && isdigit(buf[ptr])) num_s += buf[ptr++];
    if (ptr < buf.size() && buf[ptr] == ' ') ptr++;
    return num_s.empty() ? 0 : stoi(num_s);
}

str parse_blk(const str& buf, int& ptr)
{
    int bal = 1;
    int start = ++ptr;
    while (ptr < buf.size() && bal)
    {
        if (buf[ptr] == '{') bal++;
        else if (buf[ptr] == '}') bal--;
        if (bal) ptr++;
    }
    return buf.substr(start, ptr++ - start);
}

void exec(const str& buf, ctx_t ctx)
{
    int ptr = 0;
    int len = buf.size();

    while (ptr < len)
    {
        if (buf[ptr] == '#')
        {
            if (ptr + 1 < len && buf[ptr + 1] == '#')
            {
                ptr += 2;
                str id = parse_id(buf, ptr);
                if (ctx.count(id)) exec(ctx[id], ctx);
            }
            else
            {
                ptr++;
                str id = parse_id(buf, ptr);
                if (id == "rep")
                {
                    int n = parse_int(buf, ptr);
                    str blk = parse_blk(buf, ptr);
                    while (n--) exec(blk, ctx);
                } else ctx[id] = parse_blk(buf, ptr);
            }
        } else cout << buf[ptr++];
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n_lines;
    if (cin >> n_lines)
    {
        str tmp, full_buf;
        getline(cin, tmp);

        for (int i = 0; i < n_lines; ++i)
        {
            getline(cin, tmp);
            full_buf += tmp;
            if (i < n_lines - 1) full_buf += '\n';
        }
        exec(full_buf, {});
    }
    return 0;
}

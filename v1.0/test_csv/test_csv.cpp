#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

vector<string> split(const  string& s, const string& delim)
{
    vector<std::string> elems;
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0) return elems;
    while (pos < len)
    {
        int find_pos = s.find(delim, pos);
        if (find_pos < 0)
        {
            elems.push_back(s.substr(pos, len - pos));
            break;
        }
        elems.push_back(s.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
    return elems;
}

int main() {
    fstream outfile("test.csv", ios::out | ios::in);
    if (!outfile) {
        cout << "File test.csv open failed!" << endl;
        abort();
    }
    string line;
    vector<string> v;
    outfile.seekp(0);
    getline(outfile, line);
    streamoff cur_tmp = outfile.tellg();
    while (!outfile.eof())
    {
        getline(outfile, line);
        v = split(line, ",");
        if (strcmp("qirui", v[0].c_str()) == 0) {
            outfile.seekp(cur_tmp );
            outfile.seekp(-3, ios::cur );
            outfile << "uuu" << "," << "bbb" << "," << "ccc" << "," << "3" << "," << "5";
        }
        cur_tmp = outfile.tellg();
    }
    outfile.close();

    // ofstream outfile;
    // outfile.open("newfile.csv", ios::out | ios::app);
    // outfile << "tt" << "," << "ss" << "," << "hh" << endl;
    // outfile.close();

    return 0;
}


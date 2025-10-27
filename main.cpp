/*
    how to compile: g++ main.cpp
    how to run: ./a.out
    Michael Hofelich
    Sources: asked chat gpt “How do I reject a file if it has letters, commas, or blank lines instead of numbers?”
    w3schools:
    Files – https://www.w3schools.com/cpp/cpp_files.asp
    Vectors – https://www.w3schools.com/cpp/cpp_vectors.asp
    Maps – https://www.w3schools.com/cpp/cpp_maps.asp
    Math – https://www.w3schools.com/cpp/cpp_math.asp
    Input/Output – https://www.w3schools.com/cpp/cpp_files.asp
    Manipulators – https://www.w3schools.com/cpp/cpp_files.asp#iomanip
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <cerrno>
using namespace std;

static string ltrim(const string& s){
    size_t i = 0;
    while (i < s.size() && (s[i]==' '||s[i]=='\t'||s[i]=='\r'||s[i]=='\n')) i++;
    return s.substr(i);
}
static string rtrim(const string& s){
    if (s.empty()) return s;
    size_t j = s.size();
    while (j > 0 && (s[j-1]==' '||s[j-1]=='\t'||s[j-1]=='\r'||s[j-1]=='\n')) j--;
    return s.substr(0, j);
}
static string trim(const string& s){ return rtrim(ltrim(s)); }


static bool parseDoubleLine(const string& raw, double& out){
    string t = trim(raw);
    if (t.size() == 0) return false;

    errno = 0;
    const char* p = t.c_str();
    char* endp = NULL;
    double v = strtod(p, &endp);

    if (endp == p) return false;         
    if (errno == ERANGE) return false;   
    while (*endp==' '||*endp=='\t'||*endp=='\r'||*endp=='\n') endp++;
    if (*endp != '\0') return false;     // extra junk

    out = v;
    return true;
}

int main(){
    

    // banner
    cout << "*** Welcome to Michael's Data Analyzer ***\n\n";

    // read N (try until good)
    cout << "Enter the number of files to read: ";
    int N;
    while (true){
        if (!(cin >> N)){
            cin.clear();
            string junk; getline(cin, junk);
            cout << "Illegal input. Please enter a positive integer: ";
            continue;
        }
        if (N <= 0){
            cout << "Illegal input. Please enter a positive integer: ";
            continue;
        }
        string junk; getline(cin, junk); // flush rest of line
        break;
    }

    vector<double> all;                 // all numbers from all files

    // loop over files
    for (int fileIndex = 1; fileIndex <= N; fileIndex++){
        while (true){
            cout << "\n\nEnter the filename for file " << fileIndex << ": ";
            string fname;
            getline(cin, fname);

            // try open
            ifstream fin(fname.c_str());
            if (!fin.is_open()){
                cout << "\nFile not found. Please try again.\n";
                continue; // ask again
            }

            // read lines and validate
            vector<double> tmp; // values just from this file
            string line;
            bool bad = false;
            int linesRead = 0;

            while (std::getline(fin, line)){
                double x;
                if (!parseDoubleLine(line, x)){
                    bad = true;
                    break;
                }
                tmp.push_back(x);
                linesRead++;
            }
            fin.close();

            if (linesRead == 0){ // blank file
                bad = true;
            }

            if (bad){
                cout << "\nNot an input file. Illegal content/structure detected. Please try again.\n";
                // try again (don’t keep anything)
            } else {
                // echo per-file values
                size_t k = tmp.size();
                cout << "\nThe list of " << k << " values in file " << fname << " is:\n";
                cout.setf(ios::fmtflags(0), ios::floatfield);
                for (size_t i = 0; i < tmp.size(); i++){
                    cout << tmp[i] << "\n";
                }
                // append to global list
                for (size_t i = 0; i < tmp.size(); i++){
                    all.push_back(tmp[i]);
                }
                break; // go to next file
            }
        }
    }

    // summary
    cout << "\n\n*** Summarized Statistics ***\n\n";

    // sorted list
    vector<double> sorted = all;
    sort(sorted.begin(), sorted.end());

    cout << "The orderly sorted list of " << sorted.size() << " values is:\n";
    for (size_t i = 0; i < sorted.size(); i++){
        if (i) cout << " ";
        cout << sorted[i];
        if ((i + 1) % 12 == 0) cout << "\n"; 
    }
    if (!sorted.empty()) cout << "\n\n";

    // mean
    long double sum = 0.0L;
    for (size_t i = 0; i < all.size(); i++){
        sum += all[i];
    }
    double mean = 0.0;
    if (!all.empty()){
        mean = (double)(sum / all.size());
    }

    // median
    double median = 0.0;
    if (!sorted.empty()){
        size_t n = sorted.size();
        if ((n % 2) == 1){
            median = sorted[n/2];
        } else {
            double a = sorted[n/2 - 1];
            double b = sorted[n/2];
            median = (a + b) / 2.0;
        }
    }

    // mode
    map<double,int> freq;
    for (size_t i = 0; i < all.size(); i++){
        freq[all[i]] = freq[all[i]] + 1;
    }
    int best = 0;
    map<double,int>::iterator it;
    for (it = freq.begin(); it != freq.end(); ++it){
        if (it->second > best) best = it->second;
    }
    vector<double> modes;
    for (it = freq.begin(); it != freq.end(); ++it){
        if (it->second == best) modes.push_back(it->first);
    }
    long double msum = 0.0L;
    for (size_t i = 0; i < modes.size(); i++){
        msum += modes[i];
    }
    double modeAvg = 0.0;
    if (!modes.empty()){
        modeAvg = (double)(msum / modes.size());
    }

    cout.setf(ios::fixed);
    cout << setprecision(4);
    cout << "The mean is "   << mean    << "\n";
    cout << "The median is " << median  << "\n";
    cout << "The mode is "   << modeAvg << "\n\n";

    cout << "*** Goodbye. ***\n";
    return 0;
}

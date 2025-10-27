/*
    COMP 2710 – Project 4
    File: main_p4_beginner.cpp
    How to compile: g++ -std=c++17 main_p4_beginner.cpp -o analyzer
    How to run:     ./analyzer

    Notes (kept simple on purpose):
    - Prompts FIRST for an output filename and checks that it can be created.
      If not, it asks again.
    - Asks how many input files to read (must be a positive integer).
    - Each input file must contain ONE number per line. Anything else is illegal.
    - After reading all files, the program:
        * sorts all values,
        * prints mean, median, and the average of all modes,
        * writes a CSV file in the required format.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
    // 1) Ask for output filename and make sure it is writable.
    string outPath;
    while (true) {
        cout << "Enter the output filename to save: ";
        if (!getline(cin, outPath)) {
            return 0;
        }
        ofstream test(outPath.c_str(), ios::trunc);
        if (test.is_open()) {
            test.close();
            // Print confirmation line right away (like example).
            cout << "*** File " << outPath << " has been written to disk ***\n\n";
            break;
        } else {
            cout << "Invalid output path. Please try again.\n";
        }
    }

    // 2) Welcome message
    cout << "*** Welcome to Michael's Data Analyzer ***\n\n";

    // 3) Number of files (must be positive)
    int numFiles;
    cout << "Enter the number of files to read: ";
    while (true) {
        if (!(cin >> numFiles)) {
            cin.clear();
            string throwAway;
            getline(cin, throwAway);
            cout << "Illegal input. Please enter a positive integer: ";
            continue;
        }
        if (numFiles <= 0) {
            cout << "Illegal input. Please enter a positive integer: ";
            continue;
        }
        string throwAway;
        getline(cin, throwAway); // consume the end of line
        break;
    }

    vector<double> allNumbers;

    // 4) Read each file
    for (int i = 1; i <= numFiles; i++) {
        while (true) {
            cout << "\n\nEnter the filename for file " << i << ": ";
            string fname;
            getline(cin, fname);

            ifstream fin(fname.c_str());
            if (!fin.is_open()) {
                cout << "\nFile not found. Please try again.\n";
                continue;
            }

            vector<double> fileNumbers;
            string line;
            bool bad = false;
            int linesRead = 0;

            // Read one line at a time. Each line should be ONE number only.
            while (getline(fin, line)) {
                // Skip lines that are only spaces/tabs/newlines
                bool onlySpaces = true;
                for (size_t k = 0; k < line.size(); k++) {
                    if (!(line[k] == ' ' || line[k] == '\t' || line[k] == '\r' || line[k] == '\n')) {
                        onlySpaces = false;
                        break;
                    }
                }
                if (onlySpaces) { bad = true; break; }

                stringstream ss(line);
                double x;
                if (!(ss >> x)) { bad = true; break; }

                // Make sure there is no extra junk on the same line
                string extra;
                if (ss >> extra) { bad = true; break; }

                fileNumbers.push_back(x);
                linesRead++;
            }
            fin.close();

            if (linesRead == 0) bad = true;

            if (bad) {
                cout << "\nNot an input file. Illegal content/structure detected. Please try again.\n";
            } else {
                cout << "\nThe list of " << fileNumbers.size() << " values in file " << fname << " is:\n";
                // Print values exactly as read (default formatting)
                for (size_t j = 0; j < fileNumbers.size(); j++) {
                    cout << fileNumbers[j] << "\n";
                }
                // Add to the total list
                for (size_t j = 0; j < fileNumbers.size(); j++) {
                    allNumbers.push_back(fileNumbers[j]);
                }
                break;
            }
        }
    }

    // 5) Summary
    cout << "\n\n*** Summarized Statistics ***\n\n";

    // Sort a copy for display and stats
    vector<double> sorted = allNumbers;
    sort(sorted.begin(), sorted.end());

    cout << "The orderly sorted list of " << sorted.size() << " values is:\n";
    for (size_t i = 0; i < sorted.size(); i++) {
        if (i > 0) cout << " ";
        cout << sorted[i];
        // Put a newline every 12 numbers to avoid long lines
        if ((i + 1) % 12 == 0) cout << "\n";
    }
    if (!sorted.empty()) cout << "\n\n";

    // Mean
    double mean = 0.0;
    if (!allNumbers.empty()) {
        double sum = 0.0;
        for (size_t i = 0; i < allNumbers.size(); i++) {
            sum += allNumbers[i];
        }
        mean = sum / (double)allNumbers.size();
    }

    // Median
    double median = 0.0;
    if (!sorted.empty()) {
        size_t n = sorted.size();
        if (n % 2 == 1) {
            median = sorted[n / 2];
        } else {
            median = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
        }
    }

    // Mode average (beginner approach: since it's sorted, count runs)
    double modeAvg = 0.0;
    if (!sorted.empty()) {
        int bestCount = 0;
        vector<double> modes;

        int currentCount = 1;
        for (size_t i = 1; i <= sorted.size(); i++) {
            if (i < sorted.size() && sorted[i] == sorted[i - 1]) {
                currentCount++;
            } else {
                // End of a run for value sorted[i-1]
                double value = sorted[i - 1];
                if (currentCount > bestCount) {
                    bestCount = currentCount;
                    modes.clear();
                    modes.push_back(value);
                } else if (currentCount == bestCount) {
                    modes.push_back(value);
                }
                currentCount = 1;
            }
        }
        // Average of all modes
        double sumM = 0.0;
        for (size_t i = 0; i < modes.size(); i++) {
            sumM += modes[i];
        }
        if (!modes.empty()) modeAvg = sumM / (double)modes.size();
    }

    cout.setf(ios::fixed);
    cout << setprecision(4);
    cout << "The mean is "   << mean    << "\n";
    cout << "The median is " << median  << "\n";
    cout << "The mode is "   << modeAvg << "\n\n";
    cout << "*** Goodbye. ***\n";

    // 6) Write the CSV file (retry if it somehow fails here).
    while (true) {
        ofstream fout(outPath.c_str(), ios::trunc);
        if (fout.is_open()) {
            // First line (label) + newline
            fout << "The orderly sorted list of " << sorted.size() << " values is:\n";
            // Second line: values comma-separated
            for (size_t i = 0; i < sorted.size(); i++) {
                if (i > 0) fout << ",";
                fout << sorted[i]; // default formatting for values
            }
            fout << "\n";
            // Stats: fixed with 4 decimals
            fout.setf(ios::fixed);
            fout << setprecision(4);
            fout << "mean,"   << mean    << "\n";
            fout << "median," << median  << "\n";
            fout << "mode,"   << modeAvg << "\n";
            fout.close();
            cout << "\n*** File " << outPath << " has been written to disk ***\n\n";
            break;
        } else {
            // Shouldn't happen because we already checked, but handle anyway.
            cout << "\nInvalid output path. Please enter another output path: ";
            if (!getline(cin, outPath)) return 0;
        }
    }

    return 0;
}

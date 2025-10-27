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
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
   
    string outPath;
    while (true) {
        cout << "Enter the output filename to save: ";
        if (!getline(cin, outPath)) {
            return 0;
        }
        ofstream test(outPath.c_str(), ios::trunc);
        if (test.is_open()) {
            test.close();
            
            cout << "*** File " << outPath << " has been written to disk ***\n\n";
            break;
        } else {
            cout << "Invalid output path. Please try again.\n";
        }
    }

    
    cout << "*** Welcome to Michael's Data Analyzer ***\n\n";

    
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
        getline(cin, throwAway);
        break;
    }

    vector<double> allNumbers;

    
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

            
            while (getline(fin, line)) {
                
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
                
                for (size_t j = 0; j < fileNumbers.size(); j++) {
                    cout << fileNumbers[j] << "\n";
                }
                
                for (size_t j = 0; j < fileNumbers.size(); j++) {
                    allNumbers.push_back(fileNumbers[j]);
                }
                break;
            }
        }
    }

   
    cout << "\n\n*** Summarized Statistics ***\n\n";

   
    vector<double> sorted = allNumbers;
    sort(sorted.begin(), sorted.end());

    cout << "The orderly sorted list of " << sorted.size() << " values is:\n";
    for (size_t i = 0; i < sorted.size(); i++) {
        if (i > 0) cout << " ";
        cout << sorted[i];
        
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

    // Mode average
    double modeAvg = 0.0;
    if (!sorted.empty()) {
        int bestCount = 0;
        vector<double> modes;

        int currentCount = 1;
        for (size_t i = 1; i <= sorted.size(); i++) {
            if (i < sorted.size() && sorted[i] == sorted[i - 1]) {
                currentCount++;
            } else {
                
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

    // Write the CSV file
    while (true) {
        ofstream fout(outPath.c_str(), ios::trunc);
        if (fout.is_open()) {
     
            fout << "The orderly sorted list of " << sorted.size() << " values is:\n";
           
            for (size_t i = 0; i < sorted.size(); i++) {
                if (i > 0) fout << ",";
                fout << sorted[i]; 
            }
            fout << "\n";
    
            fout.setf(ios::fixed);
            fout << setprecision(4);
            fout << "mean,"   << mean    << "\n";
            fout << "median," << median  << "\n";
            fout << "mode,"   << modeAvg << "\n";
            fout.close();
            cout << "\n*** File " << outPath << " has been written to disk ***\n\n";
            break;
        } else {
            
            cout << "\nInvalid output path. Please enter another output path: ";
            if (!getline(cin, outPath)) return 0;
        }
    }

    return 0;
}

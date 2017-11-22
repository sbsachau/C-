#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>

using namespace std;

struct Temparature
{
    int day,month,year;
    float value;
};

void getValues(string s, Temparature* ptr)
{

    ptr->day = stoi(s.substr(0,2));
    ptr->month = stoi(s.substr(3,2));
    ptr->year = stoi(s.substr(6,4));
    ptr->value = stof(s.substr(11));
}


/**
    Input: .csv file name, a 2-D array
    Output: none

    read from a .csv file the yearly temparature values and
    stores them in a 2-D array.
**/
int &readFile(string fileName, float yearlyValues[12][31])
{
    ///char s[20];
    string s;
    Temparature *ptr = new Temparature;
    ifstream in;
    in.open(fileName);
    int day = 1, month = 1;
    int y;
    int &year = y;


    if( !in.is_open()) {
        year = -1;
        cerr<< endl <<"Error opening File "<< '"' << fileName << '"'<<endl;

        return year;
    }
    else {
        while( !in.eof()) {
            in >> s;
            getValues(s,ptr);

            /// We fill up 2-D array with real values from file
            yearlyValues[ptr->month - 1][ptr->day - 1] = ptr->value;

            /// If not the end of month, increase day
            if ( (ptr->day) == day && (ptr->month) == month ) {
                day++;
            }
            else {
                ///If it end of the month, but 31 values are not filled up
                if(day  <= 31) {
                    for(int i = day; i <= 31; i++) {
                        yearlyValues[month-1][i-1] = NAN;
                    }
                }
                /// Increase day and month, so that it could match up
                /// with real month and date on next real date
                day = 2;
                month ++;

            }
        }

        /// If Data for all months is not available, fill up with "NAN"
       while(month < 13)
       {
           for(day = 0; day < 31; day++)
           {
               yearlyValues[month - 1][day] = NAN;
           }
           month++;
       }
        y = ptr->year;
        in.close();
        delete ptr;
    }
    return year;
}


/**
    Input: @fileName name of the file to be written
           @yearlyValues array filled up with data
           @year year of the data
    Output: @none
**/

void writeFile(string fileName, float yearlyValues[12][31], int year)
{
    ofstream out;
    out.open(fileName);
    float tValue, mini, maxi,tot;
    mini = 1000;
    maxi = -1000;
    tot = 0;
    int days = 0;
    Temparature minTmp,maxTmp;


    if( !out.is_open()) {
        cerr<<"Error opening File"<<fileName<<endl;
    }
    else {
        out << "<!Doctype html>" << endl << "<html>" << endl << "<head>" << endl;
        out << "<title >Mean Temperatures" << year << "</title>" << endl;
        out << "<style>" << endl << "table {" << endl << "\twidth: 100%;" << endl << "}" << endl;
        out << "th, td {" << endl << "\t" << "border: 1px solid black;" << endl;
        out << "\t" << "text-align: center;" << endl << "}" <<endl;
        out << "tr:nth -child(even) {" << endl << "\t" << "background-color: #f2f2f2;" << endl << "}" << endl;
        out << "</style>" << endl << "</head>" << endl << "<body>" << endl;
        out << "<h1>" << "Mean Temparatures" << year<< "(Unit Degree Celsius)" << "</h1>";
        out << "<table>" << endl << "<tr><th> </th><th>JAN</th><th>FEB</th><th>MAR</th><th>APR</th><th>MAY</th>"<<
               "<th>JUN</th><th>JUL</th><th>AUG</th><th>SEPT</th><th>OCT</th><th>NOV</th><th>DEC</th></tr>" <<endl;

        ///Loop through the file to find data day by day
        for (int i = 0; i < 31; i++) {
            out << "<tr><th>" << i + 1 << "</th>";
            for (int j = 0; j < 12; j++) {

                tValue = yearlyValues[j][i];
                if ( !isnan(tValue)) {
                    out << "<td>" << tValue << "</td>";
                    days++;
                    tot = tot + tValue;
                    mini > tValue ? mini = tValue: mini  = mini;
                    maxi < tValue ? maxi = tValue: maxi = maxi;
                }
                ///ignore array index is not a real day in calendar
                else {
                    out << "<td>" << "</td>";
                }
            }
            out << "</tr>" << endl;
        }

        ///Find the  date of min and max values

        for (int i = 0; i < 13; i++) {
                for (int j = 0; j < 31; j++) {
                    if (yearlyValues[i][j] == mini) {
                        minTmp.day = j +1;
                        minTmp.month = i +1;
                    }
                    else if (yearlyValues[i][j] == maxi) {
                        maxTmp.day = j +1;
                        maxTmp.month = i +1;
                    }


                }
            }

        out << "</table>" << endl;

        out << "<p>" <<"Minimum Temperature: " << setprecision(3) << mini << " (" << minTmp.day <<"." << minTmp.month<< ".)" <<"</p>" << endl;
        out << "<p>Maximum Temperature: " << setprecision(3) << maxi << " (" << maxTmp.day <<"." << maxTmp.month << ".)" << "</p>" << endl;
        out << "<p>Average Temperature: " << setprecision(3) << tot / days << "</p>" << endl << "</body>" <<endl;

    }
    out.close();
}



int main()
{   float field[12][31];
    string csvFile, htmlFile;
    int year;
    htmlFile = ".html";


    cout << "input temparature file name:";
    cin >> csvFile;
    int position = csvFile.find('.');
    htmlFile = csvFile.substr(0,position) + htmlFile;

    year = readFile(csvFile,field);

    ///if the .csv file could have open
    if ( year != -1)
    {
        cout << "start reading temparature file " << '"' << csvFile << '"';
        cout <<  " ... done" << endl;
        cout << "start writing HTML file " << '"' << htmlFile << '"' << " ...done";
        writeFile(htmlFile, field,year);

    }


    return 0;
}

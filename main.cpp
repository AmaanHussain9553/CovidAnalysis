/*main.cpp*/

//
// Amaan Hussain
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <experimental/filesystem>
#include <locale>
#include <iomanip>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>

using namespace std;
namespace fs = std::experimental::filesystem;

template<typename KeyT, typename ValueT>
class Covid19{
  public:

    map <KeyT, ValueT> data;

    //
    // returns the size of the map
    //
    int Size(){
        return data.size();
    }
    
    
    //
    // returns the no. of deaths for a country
    //
    string getDeath(string key){
        return data.at(key).Deaths;
    }


    //
    // returns the date for a country
    //
    string getDateFile(string key){
        return data.at(key).DateFile;
    }


    //
    // returns the no. of recovered cases 
    // for a country
    //
    string getRecovered(string key){
        return data.at(key).Recovered;
    }


    //
    // returns the no. of confirmed cases 
    // for a country
    //
    string getConfirmed(string key){
        return data.at(key).Confirmed;
    }


    //
    // returns the lifeExpectancy for a country
    //
    string getLifeExpectancy(string key){
        return data.at(key).LifeExpectancy;
    }


    //
    // returns the Population for a country
    //
    string getPopulation(string key){
        return data.at(key).Population;
    }


    //
    // inserts the Population into a struct 
    // for a key (country)
    //
    void insertPopulation(KeyT key, ValueT value){
         data[key].Population = value;
    }


    //
    // inserts the LifeExpectancy into a struct 
    // for a key (country)
    //
    void insertLifeExpectancy(KeyT key, ValueT value){
         data[key].LifeExpectancy = value;
    }


    //
    // inserts an entire struct for a key (country)
    //
    void insert(KeyT key, ValueT value){
         data[key] = value;
    }


    //
    // clears the map of all content
    //
    void clear(){
      data.clear();
    }


    //
    // checks to see if a certain key
    // exists in a map
    //
    int find(KeyT key){
      if(data.count(key) == 0){
          return 0;
      }else{
          return 1;
      }

    }
   
};


//
// typedef to iterate through map when needed
// 
typedef pair<string, int> pair;
  

//
// struct defined to hold the number of cases of
// confirmed, recovered and death along with date.
// Being used as value for the country which is key
//
  typedef struct Statistics{
    string DateFile;
    string Recovered;
    string Deaths;
    string Confirmed;  
}Stats;


//
// struct defined to hold the population 
// and lifeExpectancy in a different map
// that takes this struct as value
//
typedef struct WorldFacts{
    string Population;
    string LifeExpectancy;  
}Facts;



//
// This function sorts through a vector of string or int to 
// have them in descending order. This function is used in 
// the top10 command where the top ten confirmed cases
// are displyed
//
void sortNumeric(vector <int> &cases){
    for (unsigned int j = 0; j < cases.size() - 1; j++) {
        for (unsigned int i = 0; i < cases.size() - 1; i++) {
            if (cases[i] < cases[i + 1]) {
                //for numeric order: 1 < 2
                int swap;
                //creating a temp variable to swap later
                swap = cases[i + 1];
                cases[i + 1] = cases[i];
                cases[i] = swap;
            }
        }
    }
}


//
// This function takes the filenames and isolates
// the date and returns it to be stored as a value
// within the struct
//
string getDate(string date){
    string temp = date;
    //finds the first digit of the month
    char a = '0';
    size_t found1 = temp.find(a);
    if(found1 != string::npos){
        size_t pos = temp.find(a);
        temp.erase(0, pos);
          
    }
    //stores only the date into temp 
    temp = temp.substr(0,10);
    return temp;
}


//
// getFilesWithinFolder
//
// Given the path to a folder, e.g. "./daily_reports/", returns 
// a vector containing the full pathnames of all regular files
// in this folder.  If the folder is empty, the vector is empty.
//
vector<string> getFilesWithinFolder(string folderPath)
{
  vector<string> files;

  for (const auto& entry : fs::directory_iterator(folderPath))
  {
    files.push_back(entry.path().string());
  }
  // let's make sure the files are in alphabetical order, so we
  // process them in the correct order by date:
  std::sort(files.begin(), files.end());
  return files;
}


//
// This functions reads in the population into a map taking one of the files
// in the wordFacts folder
//
void readWorldPopulation(string filename, Covid19<string, Facts> &world){
    
    ifstream infile(filename);

    //Error message if the file is unable to open
    if (!infile.is_open()){
        cout << "**Error, unable to open '" <<filename<<"'"<< endl;
        exit(0);
    }

    string line;
    getline(infile, line); // skip first line (header row):
    while (getline(infile, line))
    {
        // retrieving the country and population from each line
         stringstream s(line);
         string pos,country, population;
         getline(s, pos, ',');
         getline(s, country, ',');
         getline(s, population, ' ');
        
        // inserting the value into the struct that holds Population
        world.data[country].Population = population;
     
    }    
    
    infile.close();
}//end of function


//
// This functions reads in the lifeExpectancy into a map taking one of the files
// in the wordFacts folder
//
void readWorldLifeExpectancy(string filename, Covid19<string, Facts> &world){
    
    ifstream infile(filename);

    //Error message if the file is unable to open
    if (!infile.is_open()){
        cout << "**Error, unable to open '" <<filename<<"'"<< endl;
        exit(0);
    }

    string line;
    getline(infile, line); // skip first line (header row):
    while (getline(infile, line))
    {
        // retrieving the country and lifeExpectancy from each line
         stringstream s(line);
         string pos,country, lifeExpectancy;
         getline(s, pos, ',');
         getline(s, country, ',');
         getline(s, lifeExpectancy, ' ');
        
        // inserting the value into the struct that holds LifeExpectancy
        world.data[country].LifeExpectancy = lifeExpectancy;        
     
    }    
    
    infile.close();
}//end of function


//
// This functions reads in the cases into a map taking one of the files
// in the daily reports folder
//
void readCoronaFiles(string filename, Covid19<string, Statistics> &cases ){
    
    ifstream infile(filename);

    //Error message if the file is unable to open
    if (!infile.is_open()){
        cout << "**Error, unable to open '" <<filename<<"'"<< endl;
        exit(0);
    }

    string line;
    getline(infile, line); // skip first line (header row):
    while (getline(infile, line))
    {
        if (line[0] == '"'){ // => province is "city, state"
         
         //
         // we want to delete the " on either end of the value, and
         // delete the ',' embedded within => will get city state:
         //
         line.erase(0, 1); // delete the leading "
         size_t pos = line.find(','); // find embedded ','
         line.erase(pos, 1); // delete ','
         pos = line.find('"'); // find closing "
         line.erase(pos, 1); // delete closing "     
       }    
            
         // retrieving the country, deaths, recovered, confirmed
         // from each line
         stringstream s(line);
         string province, country, last_update;
         string confirmed, deaths, recovered;
         getline(s, province, ',');
         getline(s, country, ',');
         getline(s, last_update, ',');
         getline(s, confirmed, ',');
         getline(s, deaths, ',');
         getline(s, recovered, ',');
            
         //setting the cases to 0, if they are not present        
         if (confirmed == "")
         confirmed = "0";
         if (deaths == "")
         deaths = "0";
         if (recovered == "")
         recovered = "0";
            
         if (country == "Mainland China") // map to name in the earlier reports:
         country = "China"; 
        
        // isolating the date from the filename to store in struct
        last_update = getDate(filename);
        
        //if the key(country) doesnt exist in the map
         if(cases.find(country) == 0){
             
             Statistics newStruct;
             newStruct.DateFile = last_update;
             newStruct.Confirmed = confirmed;
             newStruct.Deaths = deaths;
             newStruct.Recovered = recovered;
             
             //inserting the struct as a value for a country in the map
             cases.insert(country, newStruct);
        
        // if the country already exists in the map 
         }else if(cases.find(country) == 1){
             
             int currentDeath = 0, currentRecovered = 0, currentConfirmed = 0;
             
             currentDeath = stoi(cases.getDeath(country));
             currentRecovered = stoi(cases.getRecovered(country));
             currentConfirmed = stoi(cases.getConfirmed(country));
             
             int newDeath = 0, newRecovered = 0, newConfirmed = 0;
             
             //adding up the additional cases along with the existing records
             newDeath = currentDeath + stoi(deaths);
             newRecovered = currentRecovered + stoi(recovered);
             newConfirmed = currentConfirmed + stoi(confirmed);
             
             Statistics addStruct;
             addStruct.DateFile = last_update;
             addStruct.Confirmed = to_string(newConfirmed);
             addStruct.Recovered = to_string(newRecovered);
             addStruct.Deaths = to_string(newDeath);
             
             //inserting the struct as a value for a country in the map
             cases.insert(country, addStruct);
             
         }
   }//end of while (getline(infile, line))
    infile.close();
}//end of function


//
// This function stores all the values of confirmed cases in a vector
// and then sorts through them for the ten biggest values. It then 
// for each values find the corresponding key in the map and displays them
//
void topTenConfirmed(vector<string> facts){
    
    Covid19<string,Statistics> cases;
    vector<int> topTenValues;
    vector<string> topTenKeys;
    int counter = 1;
    
    // reading in the latest daily report
    readCoronaFiles(facts.at(facts.size()-1), cases);
    
    // storing all values into a vector
    for (auto pair: cases.data){
          topTenValues.push_back(stoi(pair.second.Confirmed));
      }
    
    //sorts all values for the ten biggest values
    sortNumeric(topTenValues);
    
    // for each of the top ten values finding the corresponding key (country)
    for(unsigned int i = 0; i < 10; i++){
        for (auto pair: cases.data){
           if(stoi(pair.second.Confirmed) == topTenValues[i]){
               topTenKeys.push_back(pair.first);
           }
      }
    }
        
    // displaying the top ten confirmed cases        
    for(unsigned int i = 0; i < 10; i++){
        cout << counter << ". " << topTenKeys[i] << ": " << topTenValues[i] << endl;
        counter++;
    }
    cout << endl;
}


//
// This functions displays all countries along with their confirmed,
// recovered and death cases. 
//
void countriesCommand(vector <string> files){
    Covid19<string,Statistics> cases;
    
    //Reading in all countries as keys in the map
    for(unsigned int i = 0; i < files.size(); i++){
        readCoronaFiles(files.at(i), cases);
    }
    
    //Setting all values to zero because all values are to be set now
    // so far just making sure all countries are present in the map
    for (auto& pair: cases.data){
         pair.second.Confirmed = "0";
         pair.second.Deaths = "0";
         pair.second.Recovered = "0";
      }
    
    // reading the latest daily report to gather all the cases
    readCoronaFiles(files.at(files.size()-1), cases);
    
    //displaying all the data
    for (auto pair: cases.data){
         cout << pair.first << ": " << pair.second.Confirmed << ", " 
              << pair.second.Deaths << ", " << pair.second.Recovered << endl;
      }
}


//
// This functions displays the overall number of confirmed, death and 
// revocered cases around the world-wide
//
void totalsCommand(vector <string> files){
    
    Covid19<string,Statistics> cases;
    
    //reading in the latest daily report
    readCoronaFiles(files.at(files.size()-1), cases);
    
    int confirmed = 0;
    int deaths = 0;
    int recovered = 0;
    
    //adding up all the cases by iterating through the map
    for (auto pair: cases.data){
        confirmed += stoi(pair.second.Confirmed);
        deaths += stoi(pair.second.Deaths);
        recovered += stoi(pair.second.Recovered);
      }
    
    //setting the percentages using typecast into doubles
    double deathPercentage = (((double)deaths/(double)confirmed) * 100.0);
    double recoveredPercentage = (((double)recovered/(double)confirmed) * 100.0);
    
    //displaying the world-wide totals
    cout << "As of 03-27-2020, the world-wide totals are:" << endl
         << " confirmed: " << confirmed << endl
         << " deaths: " << deaths << " (" << deathPercentage << "%)" << endl
         << " recovered: " << recovered << " (" << recoveredPercentage << "%)" << endl << endl;
}


//
// This function finds the first confirmed cases which is required when
// displaying the statisitcs of a country. Goes through each daily report,
// searches for confirmed cases, then clears map and keeps doing so until
// first confirmed case is found
//
void findFirstConfirmedCase(vector<string> files, string country){
    
    Covid19<string,Statistics> cases;
    
    for(unsigned int i = 0; i < files.size(); i++){
        
        //reads in file into map to check through map for first confirmed
        readCoronaFiles(files.at(i), cases);
        if(cases.find(country) == 1 && cases.getConfirmed(country) != "0"){
            cout << "First confirmed case: " << cases.getDateFile(country) << endl;
            return;
        }
        //clears map to check next day report
        cases.clear();
    }
    cout << "First confirmed case: none" << endl;
}


//
// This function finds the first death cases which is required when
// displaying the statisitcs of a country. Goes through each daily report,
// searches for death cases, then clears map and keeps doing so until
// first death case is found
//
void findFirstDeathCase(vector<string> files, string country){
    Covid19<string,Statistics> cases;
    
    for(unsigned int i = 0; i < files.size(); i++){
        
        //reads in file into map to check through map for first confirmed
        readCoronaFiles(files.at(i), cases);
        if(cases.find(country) == 1 && cases.getDeath(country) != "0" ){
            cout << "First recorded death: " << cases.getDateFile(country) << endl;
            return;
        }
        //clears map to check next day report
        cases.clear();
    }
    cout << "First recorded death: none" << endl;
}


//
// This function checks number of confirmed cases and displays the whole 
// of the first seven and last seven. This function finds the first
// confirmed case and then reads in file and displays the confirmed
// cases accordingly
//
void timelineConfirmHelper(int end, int start, Covid19<string,Statistics> cases,
string country, vector<string> files){
    
    // checks for number of cases
    if((end - start) > 14){
            int dayCounter = start + 1;
            //displays the first seven cases
            for(int h = start; h < (start + 7); h++){
                
                // reads in one file at a time for confirmed cases
                readCoronaFiles(files.at(h), cases);

                    //checks value as 0 or not
                    if(cases.getConfirmed(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " 
                       << dayCounter << "): " << cases.getConfirmed(country) 
                       << endl;
                       dayCounter++;
                    }else{
                        cout << cases.getDateFile(country) << " (day " 
                        << dayCounter << "): 0" << endl;
                        dayCounter++;
                    }
                // clears map to read next day's report
                cases.clear();
            }
            // to match syntax of solution
            cout  << " ." << endl;
            cout  << " ." << endl;
            cout  << " ." << endl;
            
            // displays the last seven cases
             for(int g = end-6; g < end + 1; g++){
                readCoronaFiles(files.at(g), cases);
                    if(cases.getConfirmed(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " << (g + 1) 
                       << "): " << cases.getConfirmed(country) << endl;
                    }else{
                        cout << cases.getDateFile(country) << " (day " 
                        << (g + 1) << "): 0" << endl;
                    }
                // clears map to read next day's report
                cases.clear();
            }
            
    }else{ //if the no. of cases are less than or equal to 14 then displays all
        int count = 1;
        //displays all cases
        for(int g = start; g <= end; g++){
                readCoronaFiles(files.at(g), cases);
                    //checks if values is 0 or not
                    if(cases.getConfirmed(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " << count 
                       << "): " << cases.getConfirmed(country) << endl;
                       count++;
                    }else{
                        cout << cases.getDateFile(country) << " (day " << count 
                        << "): 0" << endl;
                        count++;
                    }
                //clears map to read in next day reports
                cases.clear();
        }
    }
}


//
// This function calculates the days and based on such calls its helper
// function to display timeline for confirmed cases
//
void nameOfCountryConfirmed(vector<string> files, string country){
    
    Covid19<string,Statistics> cases;
    cout << "Confirmed:" << endl;
    int start = 0;
    int end = 0;
    
    //claculates the first confirmed cases as starting point
    for(unsigned int i = 0; i < files.size(); i++){
        readCoronaFiles(files.at(i), cases);
        if(cases.find(country) == 1){
            if(cases.getConfirmed(country) != "0"){
                start = i;
                cases.clear();
                break;
            }
        }
    }
    
    //calculates the last confirmed case
    for(unsigned int j = files.size()-1; j > 0; j--){
        readCoronaFiles(files.at(j), cases);
        if(cases.find(country) == 1){
            if(cases.getConfirmed(country) != "0"){
                end = j;
                cases.clear();
                break;
            }
        }     
    }
    
    //calls helper function that displays based on first and last
    // confirmed case
    timelineConfirmHelper(end, start, cases, country, files);
    
    cout << endl;
}


//
// This function checks number of recovered cases and displays the whole 
// of the first seven and last seven. This function finds the first
// recovered case and then reads in file and displays the recovered
// cases accordingly
//
void timelineRecoveredHelper(int recoveredEnd, int recoveredStart, 
                        int confirmedStart, Covid19<string,Statistics> cases, 
                             string country, vector<string> files){
    //// checks for number of cases
    if((recoveredEnd - recoveredStart) > 14){
            int counter = recoveredStart - confirmedStart + 1;
            
            //prints out the first seven cases
            for(int h = recoveredStart; h <= (recoveredStart + 6); h++){
                readCoronaFiles(files.at(h), cases);
                    if(cases.getRecovered(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " << counter 
                       << "): " << cases.getRecovered(country) << endl;
                        counter++;
                    }else if(cases.getRecovered(country) == "0"){
                       cout << cases.getDateFile(country) << " (day " 
                       << counter << "): 0" << endl; 
                        counter++;
                    }
                //clears map to read in next days report
                cases.clear();
            }
            
            //calculates the day counter for last seven cases
            int dayCounter = 0;
            for(unsigned int i = confirmedStart; i < files.size(); i++){
                dayCounter++;
            }
            // to match syntax of solution
            cout  << " ." << endl;
            cout  << " ." << endl;
            cout  << " ." << endl;
            // displays the last seven cases
             for(int g = (recoveredEnd-6); g <= recoveredEnd ; g++){
                readCoronaFiles(files.at(g), cases);
                    if(cases.getRecovered(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " 
                       << (dayCounter - 6) << "): " 
                       << cases.getRecovered(country) << endl;
                       dayCounter++;
                    }else{
                       cout << cases.getDateFile(country) << " (day " 
                       << (dayCounter - 6) << "): 0" << endl; 
                       dayCounter++;
                    }  
                cases.clear();
            }
    }else{
        //else if less than 14 cases then prints out all the cases
        for(int i = recoveredStart; i <= recoveredEnd; i++){
            readCoronaFiles(files.at(i), cases);
                    if(cases.getRecovered(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " << (i + 1) 
                       << "): " << cases.getRecovered(country) << endl;
                    }else{
                       cout << cases.getDateFile(country) << " (day " << (i + 1) 
                       << "): 0" << endl; 
                    }  
                //clears map to read next day's file
                cases.clear();
            }  
        }
}


//
// This function calculates the days and based on such calls its helper
// function to display timeline for recovered cases
//
void nameOfCountryRecovered(vector<string> files, string country){
    
    Covid19<string,Statistics> cases;
    cout << "Recovered:" << endl;
    int confirmedStart = 0;
    int recoveredStart = 0;
    int recoveredEnd = 0;
    
    //claculates the first confirmed case to set the right day counter
    for(unsigned int i = 0; i < files.size(); i++){
        readCoronaFiles(files.at(i), cases);
        if(cases.find(country) == 1){
            if(cases.getConfirmed(country) != "0"){
                confirmedStart = i;
                cases.clear();
                break;
            }
        }  
        //clears map to read next days reports
        cases.clear();
    }
    
    //calculates the first recovered case starting from the day the 
    // first confirmed case takes place
    for(unsigned int k = confirmedStart; k < files.size(); k++){
        readCoronaFiles(files.at(k), cases);
        if(cases.find(country) == 1){
            if(cases.getRecovered(country) != "0"){
                recoveredStart = k;
                cases.clear();
                break;
            }
        }   
        //clears map to analyze next day
        cases.clear();
    }
    
    //calculates the last day of a recovered case
    for(unsigned int j = files.size()-1; j > 0; j--){
        readCoronaFiles(files.at(j), cases);
        if(cases.find(country) == 1){
            if(cases.getRecovered(country) != "0"){
                recoveredEnd = j;
                cases.clear();
                break;
            }
        } 
        // clears map to analyze next day
        cases.clear();
    }
    //calls helper function that displays based on first and last
    // recovered cases
    timelineRecoveredHelper(recoveredEnd, recoveredStart, confirmedStart, cases, country, files);
    cout << endl;
}


//
// This function checks number of death cases and displays the whole 
// of the first seven and last seven. This function finds the first
// recovered case and then reads in file and displays the death 
// cases accordingly
//
void timelineDeathHelper(int deathEnd, int deathStart, int confirmedStart, 
                        Covid19<string,Statistics> cases, string country, 
                        vector<string> files ){
    // checks the number of cases
    if((deathEnd - deathStart) > 14){
            int counter = deathStart - confirmedStart + 1;
            
            //displays the first seven cases
            for(int h = deathStart; h <= (deathStart + 6); h++){
                readCoronaFiles(files.at(h), cases);
                    if(cases.getDeath(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " << counter 
                       << "): " << cases.getDeath(country) << endl;
                        counter++;
                    }else if(cases.getDeath(country) == "0"){
                       cout << cases.getDateFile(country) << " (day " << counter 
                       << "): 0" << endl; 
                        counter++;
                    }
                cases.clear();
            }
            
            //sets day counter for the last seven cases
            int dayCounter = 0;
            for(unsigned int i = confirmedStart; i < files.size(); i++){
                dayCounter++;
            }
            // sytax to match solution
            cout  << " ." << endl;
            cout  << " ." << endl;
            cout  << " ." << endl;
            
            //prints the last seven cases
             for(int g = (deathEnd-6); g <= deathEnd ; g++){
                readCoronaFiles(files.at(g), cases);
                    if(cases.getDeath(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " 
                       << (dayCounter - 6) << "): " << cases.getDeath(country) 
                       << endl;
                       dayCounter++;
                    }else{
                       cout << cases.getDateFile(country) << " (day " 
                       << (dayCounter - 6) << "): 0" << endl; 
                       dayCounter++;
                    }
                //clears map to read in next day's data
                cases.clear();
            }
    }
    
    else{
        //else the no. of cases are less than or equal to 14 and thus 
        // prints out all cases
        for(int i = deathStart; i <= deathEnd; i++){
            readCoronaFiles(files.at(i), cases);
                    if(cases.getDeath(country) != "0"){
                       cout << cases.getDateFile(country) << " (day " << i 
                       << "): " << cases.getDeath(country) << endl;
                       //dayCounter++;
                    }else{
                       cout << cases.getDateFile(country) << " (day " << i 
                       << "): 0" << endl; 
                       //dayCounter++;
                    }  
                //clears map to analyze next day's data
                cases.clear();
            }  
        }
}


//
// This function calculates the days and based on such calls its helper
// function to display timeline for death cases
//
void nameOfCountryDeaths(vector<string> files, string country){
    
    Covid19<string,Statistics> cases;
    cout << "Deaths:" << endl;
    bool isThere = false;
    // first checks if there were any deaths at all
    for(unsigned int i = 0; i < files.size(); i++){
        
        readCoronaFiles(files.at(i), cases);
        if(cases.find(country) == 1 && cases.getDeath(country) != "0" ){
            isThere = true;
            break;
        }
        cases.clear();
    }
    
    //if a country has no deaths then leaves this function
    if(isThere == false){
        return;
    }
   
    int confirmedStart = 0;
    int deathStart = 0;
    int deathEnd = 0;
    
    //This calculates the first confirmed case
    for(unsigned int i = 0; i < files.size(); i++){
        readCoronaFiles(files.at(i), cases);
        if(cases.find(country) == 1){
            if(cases.getConfirmed(country) != "0"){
                confirmedStart = i;
                cases.clear();
                break;
            }
        }
        //clears map to analyze next day's data
        cases.clear();
    }
    
    //This calculates the first death with the first confirmed cases
    // and end of file as the bounds
    for(unsigned int k = confirmedStart; k < files.size(); k++){
        readCoronaFiles(files.at(k), cases);
        if(cases.find(country) == 1){
            if(cases.getDeath(country) != "0"){
                deathStart = k;
                cases.clear();
                break;
            }
        }
        //clears map to analyze next day's data
        cases.clear();
    }
    
    //This calculates the last death case
    for(unsigned int j = files.size()-1; j > 0; j--){
        readCoronaFiles(files.at(j), cases);
        if(cases.find(country) == 1){
            if(cases.getDeath(country) != "0"){
                deathEnd = j;
                cases.clear();
                break;
            }
        }  
        //clears map to analyze next day's data
        cases.clear();
    }
    
    //helper function that prints out death cases timeline
    timelineDeathHelper(deathEnd, deathStart, confirmedStart, cases, country, files);
    
    cout << endl;
}


//
// This functions outputs the basic facts of a country before the timeline
// question. It displays population, LifeExpectancy, first confirmed case,
// first death recorded and latest data in terms of cases
//
void nameOfCountry(Covid19<string, WorldFacts> population, vector<string> files, string country){
    
    if(population.find(country) == 1){
    cout << "Population: " << population.getPopulation(country) << endl
         << "Life Expectancy: " << population.getLifeExpectancy(country) << " years" << endl
         << "Latest data: " << endl;
    }else{
        cout << "Population: 0" << endl
         << "LifeExpectancy: 0" << endl
         << "Latest data: " << endl;
    }
    
    Covid19<string,Statistics> cases;
    readCoronaFiles(files.at(files.size()-1), cases);
    
    cout << " confirmed: " << cases.getConfirmed(country) << endl
         << " deaths: " << cases.getDeath(country) << endl
         << " recovered: " << cases.getRecovered(country) << endl;
    
    //displays first confirmed and first death recorded
    findFirstConfirmedCase(files, country);
    findFirstDeathCase(files, country);
    
}

//
// THis function is part of the option that I implemented which displays
// the recovery rate of a country
//
void recoveryRate(vector<string> files, string country){

    Covid19<string,Statistics> cases;
    
    //reads in the latest daily report
    readCoronaFiles(files.at(files.size()-1), cases);
    
    // typecasting into double to display percantages with decimals
    double confirmed = stod(cases.getConfirmed(country));
    double recovered = stod(cases.getRecovered(country));
    
    double percentageRecovered = ((recovered/confirmed) * 100);
    
    cout << country << " has a recovery rate of " << percentageRecovered 
         << "%" << endl;

}

//
// THis function is part of the option that I implemented which displays
// the deathRate rate of a country
//
void deathRate(vector<string> files, string country){

    Covid19<string,Statistics> cases;
    
    //reads in the latest daily report 
    readCoronaFiles(files.at(files.size()-1), cases);
    
     // typecasting into double to display percantages with decimals
    double confirmed = stod(cases.getConfirmed(country));
    double death = stod(cases.getDeath(country));
    
    double percentageDead = ((death/confirmed) * 100);
    
    cout << country << " has a death rate of " << percentageDead << "%" << endl;
}


//
// This function handles menu option of my custom
// option built
//
void menuHandlingCustom(vector<string> coronaFacts){
    string countryName; 
    string rateInput;
    cout << "Enter name of country ";
    cin >> countryName;
    cout << "Enter r for recovery rate, d for death rate ";
    cin >> rateInput; 
    if(rateInput == "r"){
        recoveryRate(coronaFacts, countryName);
    }else{
        deathRate(coronaFacts, countryName);
    }
}

// 
// This function handles all the options for the user and calls the necessary
// functions for the execution of the userInput
//
void menuHandling(vector<string> coronaFacts, vector<string> worldFacts, 
            Covid19<string, Facts> world, Covid19<string, Statistics> cases){
    //declaring the variables the store various inputs
    string userInput, timeLineOption;

    cout << "Enter command (help for list, # to quit)> ";
    getline(cin, userInput);
while(userInput != "#"){
    //handles displaying of commands
      if(userInput == "help"){
        cout << "Available commands: " << endl;
        cout << "<name>: enter a country such as US or China" << endl;
        cout << "countries: list all countries and most recent reports" << endl;
        cout << "top10: list of top 10 countries based on most recent # of confirmed cases" << endl;
        cout << "totals: world-wide totals of confirmed, deaths, recovered" << endl;
        cout << "rates: displays recovery rate and death rate" << endl;
        cout << "Enter command> " << endl;
        getline(cin, userInput);

      }
      //handles displaying top10 countries with confirmed cases
      else if(userInput == "top10"){
          topTenConfirmed(coronaFacts);
          cout << "Enter command> ";
          getline(cin, userInput);
      }
      // handles displaying all countries along with their cases
      else if(userInput == "countries"){
          countriesCommand(coronaFacts);
          cout << "Enter command> ";
          getline(cin, userInput);
      }
      // handles displaying world-wide totals
      else if(userInput == "totals"){
          totalsCommand(coronaFacts);
          cout << "Enter command> ";
          getline(cin, userInput);
      }
      // handles all userInput that is a country and displays accordingly
      else if((world.find(userInput) == 1)  || 
                (world.find(userInput) == 0 && cases.find(userInput) == 1)){
          nameOfCountry(world, coronaFacts, userInput);
          
          //handles the timeLine options and input
          cout << "Do you want to see a timeline? Enter c/d/r/n> ";
          getline(cin, timeLineOption);   
     
              if(timeLineOption == "c"){
                  nameOfCountryConfirmed(coronaFacts, userInput);
                  cout << "Enter command> ";
                  getline(cin, userInput);
              }
              else if(timeLineOption == "d"){
                  nameOfCountryDeaths(coronaFacts, userInput);
                  cout << "Enter command> ";
                  getline(cin, userInput);
              }
              else if(timeLineOption == "r"){
                  nameOfCountryRecovered(coronaFacts, userInput);
                  cout << "Enter command> ";
                  getline(cin, userInput);
              }
              else if(timeLineOption == "n"){
                  
                  cout << endl << "Enter command> ";
                  getline(cin, userInput);
              }
      }
      //this is the custom option that I created
      else if(userInput == "rates"){
          menuHandlingCustom(coronaFacts);
          cout << "Enter command> " << endl;
          getline(cin, userInput);
      }
      // this handles exiting the program
      else if(userInput == "#"){
          cout << endl;
          exit(0);
      }
      // error message is country or command is invalid
      else{
          cout << "country or command not found... " << endl;
          cout << "Enter command> ";
          getline(cin, userInput);
      }
}
}


//
// main:
//
int main()
{
  cout << "** COVID-19 Data Analysis **" << endl;
  cout << endl;
  cout << "Based on data made available by John Hopkins University" << endl;
  cout << "https://github.com/CSSEGISandData/COVID-19" << endl;
  cout << endl;

  //
  // setup cout to use thousands separator, and 2 decimal places:
  //
  cout.imbue(std::locale(""));
  cout << std::fixed;
  cout << std::setprecision(2);

  //
  // get a vector of the daily reports, one filename for each:
  //
  vector<string> coronaFacts = getFilesWithinFolder("./daily_reports/");

  vector<string> worldFacts = getFilesWithinFolder("./worldfacts/");

    
  // declaring map that stores the cases of all countries
  Covid19<string, Statistics> cases;

  // declaring map that stores all populations and LifeExpectancy
  Covid19<string, Facts> world;

  //Printing bunch of stuff to make sure everything is being read well
  cout<<">> Processed "<< coronaFacts.size() <<" daily reports"<<endl;
  cout<<">> Processed "<< worldFacts.size() <<" files of world facts"<<endl;
  
    for(unsigned int i = 0; i < coronaFacts.size(); i++){

        readCoronaFiles(coronaFacts.at(i), cases);
    }
    
    // To check processing functionality of readCoronaFiles function
    cout<<">> Current data on "<< cases.Size() <<" countries" << endl << endl;
    
    //storing all populations
    readWorldPopulation(worldFacts.at(1), world);
   
    //storing all LifeExpectancy
    readWorldLifeExpectancy(worldFacts.at(0), world);
    
    //calling for userInput and handling of commands
    menuHandling(coronaFacts, worldFacts, world, cases);

    
  return 0;
  }
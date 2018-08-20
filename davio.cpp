//Restaurant Meal Ticketing System, Davio Restaurant in Philadelphia
//Grant Thorburn, BTE320

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

string const ifileName = "foodcsv.txt";

string trim(string str);
 
vector<string> split(const string &text, char sep);

void printHeader(){
     //Header print out of restaurant name. Could be in void print, but for simplicity it
     //is within its own function (I likely need to modify print function)
      cout << setw(4) << ' ' << "DAVIOS ITALIAN RESTAURANT\n" << endl;
      cout << setw(4) << ' ' << "Lounge & white-tablecloth steakhouse in a restored bank" << endl,
      cout << setw(4) << ' ' << "for seafood, meats & creative Italian meals.\n" << endl;
 }

struct MenuItem{
  int mid;
  string category;
  string name;
  double price;
  
  
  void print(){
      //if the number is 10 or greater, due to formating of mid and it's affect on setw, separate cout
      //for n of 10 to ensure that prices line up and are right set, from a left set menu item. 
      //first setw(2) is right set, to protect the following setw's
      //second setw(89) and setw(90) differ because mid is one greater for n>=10 than n<=9. Left formatted 
      //likewise, for setw(5) and setw(4), same difference from mid in setw formatting. 
      //could use showpoint for the price, but for simplicity it is not included. I believe number.00 looks weird on a menu,
      //unless in the ex. of Carrot Cake it is 7.99, other than number.00
      
      if(mid<=9){
          cout << setw(2) << left << mid << ' ' << setw(89) << name << right << setw(5) << price << endl;
      }
      else{
          cout << setw(2) << left << mid << ' ' << setw(90) << name << right << setw(4) << price << endl;
      }
      
  }//end void print, of struct
    
};//end menuItem struct

//need separate function to print the catagory. Probably better way to do this, but 
//unsure how to extract the actual catagory names from a fin of a different menu.
//In the case of, say, unique category names, unsure how to address. I used rough knowledge
//of when the categories start, due to 1)uncomplexity of given datafile, easy to manipulate 2)unique categories, unsure how to address.
//pushback relational operator?

void printCategory(int i){
    if(i==0){
         cout << setw(4) << ' ' << "Antipasta\n" << endl;
    }
    if(i==4){
        cout << '\n' << setw(4) << ' ' << "Insalat\n" << endl;
    }
    if(i==8){
        cout << '\n' << setw(4) << ' ' << "Entrata\n" << endl;
    }
    if(i==15){
        cout << '\n' << setw(4) << ' ' << "Bevanda\n" << endl;
    }
    if(i==20){
        cout << "\n" << setw(4) << ' ' << "Dolce\n" << endl;
    }
}// end void printCategory. 

struct Ticket{
    int tid;
    vector<MenuItem> ticketItems;
};

void readMenu(ifstream &fin, vector<MenuItem> &mv){
    
    int i=1;
    while(!fin.eof()){
        MenuItem m;
        string line; 
        getline(fin,line);
        vector<string>  lineComponents = split(line,';');
        m.category  =   lineComponents[0];
        m.name      =   lineComponents[1];
        m.price     =   std::stod(lineComponents[2]);
        m.mid = i;
        mv.push_back(m);
        i++;
    }
}//end readMenu function


string trim(string str){
    int i = 0;
    for (char c : str)
    {
        if (!isspace(c))
            break;
        i++;
    }

    string trimmed = str.substr(i, (str.length()-i));

    i = 0;
    for (char c : str)
    {
        if (isspace(c))
            break;
        i++;
    }

    trimmed = trimmed.substr(0, i);
    return trimmed;
}//end of trim function


vector<string> split(const string &text, char sep) {
  vector<string> tokens;
  int start = 0, end = 0;
  while ((end = text.find(sep, start)) != string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}//end vector split

void divider(){
    cout << endl; //space between menu and divider
        for(int i=0; i<=96; i++){
            cout << '=';
        }
    cout << endl;
}

void printFooter(int menuSize){
    divider(); //create divider
    cout << "\n"; //two spaces between divider and userChoice
    cout << "Please enter the digit below for desired menu item from [1-" << menuSize << "]." << endl;
    cout << "Once you have completed your ticket, type 0 to recieve final bill." << endl;
    cout << "Type digit, then press enter: ";
    
}//end printFooter

void countDigit(int total, int subTotal, int tax, int &totalC, int &subC, int &taxC){
    while (total!= 0){
        totalC++;
        total /= 10;
    }
    while (subTotal!= 0){
        subC++;
        subTotal /= 10;
    }
     while (tax!= 0){
        taxC++; 
        tax /= 10; 
    }
}//end countDigit function

void printEndTotal(double total, double sum, double tax){
    //initialized count for following
    int totalC = 0;
    int sumC = 0;
    int taxC = 0;
    double gratuityPrct;
    
    cout << "\nWhat percentage graduity would you like to add? %";
    cin >> gratuityPrct;
    //total = total + total*(gratuityPrct/100);
    double gratuity = (gratuityPrct/100)*sum;
    double finalTotal = total + tax + (total*(gratuityPrct/100));
    //cout << finalTotal;
    total=finalTotal;
    //rerun count to update
    countDigit(total, sum, tax, totalC, sumC, taxC);

    //final pricing output
        cout << fixed << setprecision(2);
        cout << right << setw(84-sumC) << ' ' << left << "Subtotal=$" << sum << endl;
        cout << right << setw(89-taxC) << ' ' << left << "Tax=$" << tax << endl;
        cout << right << setw(85-sumC) << ' ' << left << "Gratuity=$" << gratuity << endl;
        cout << right << setw(87-totalC) << ' ' << left << "Total=$" << finalTotal << endl;
}//end of printEndTotal function. 


int main(){
    
    
    Ticket myTicket;
    vector<MenuItem> menuV;
    ifstream fin;
    fin.open(ifileName.c_str());
    if (!fin) { cout << "Error opening file.." << endl; exit(1);}
    
    //i is an arbitary number to run the bill up to a maximum of 100.
    //A do...while is out of scope for the while statement, for doesnt work either,
    //as you do not have a defined number of reps. 
    //once the user enters 0, program breaks and total bill is printed.
    
    //just an arbitrary number, limiting menu items to 1000. Real end to while loop within, entering 0. 
    int i=0;
    while(i<1000){
        
        //Processing fin Section
        
        readMenu(fin,menuV);
    
        int menuSize = menuV.size(); //Makes it easier to use.

        printHeader(); //prints header for restaurant menu
    
        //print menu in correct format
        for(int i=0;i<menuSize;i++){
            printCategory(i); //If i is equal to the critical numbers, print cat before item line
            menuV[i].print(); //print each menu item line, within each category. 
        }
        
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
        //ticket section
        
        //NOTE: I didn't include printing category names on items within the ticket
        //because I wanted the style to be continuous like a restaurant bill would be. 
        
        //divider between menu and ticket
        divider();
        
        //print the menu first so the customer//employee can see the running input stream, and subtotal increases.    
        cout << "Ticket" << endl;    
        double sum=0;
        for(int i=0;i<myTicket.ticketItems.size();i++){
            myTicket.ticketItems[i].print();
            sum = sum + myTicket.ticketItems[i].price;}
        
        //insert the subtotal, tax and total part. 
        
        //get the count of total, sum and tax to adjust formatting accordingly
        double tax=0;
        double taxPercent=.07; //usually 7% sales tax
        double total=0;
        int totalC = 0; //count of total digits, initialized
        int sumC = 0; //count of sum digits, initialized
        int taxC = 0; //count of tax digits initialized
        tax = (sum*taxPercent);
        total=sum+tax;
        countDigit(total, sum, tax, totalC, sumC, taxC);
        
        //Show subtotal within ticket, before main ticket. 
        //Based on graduity being calculated from subtotal, not total bill. 
        cout << fixed << setprecision(2);
        cout << left << ' ' << setw(83-sumC) << ' ' << right << setw(5) << "Subtotal=$" << sum;
        cout <<setprecision(0);
        
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //User choice section
        int userChoice=0; //initialize
        
        //includes divider
        printFooter(menuSize);
        //find userChoice, given menu digits of [1-menuSize]
        cin >> userChoice; 
        
        //take user input, and pushback item i. Then reload program accordingly.
        for(int i=0;i<menuV.size();i++){
            if (userChoice == menuV[i].mid){
                myTicket.ticketItems.push_back(menuV[i]);
            }//end if
        }//end for
    
        //divider after user has given choice !=0
        divider();
    
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //If user ends program
        //finish with function printEndTotal and break.  
        
        if(userChoice==0){
            //end function, takes three parameters to print. 
            printEndTotal(total, sum, tax);
            
            //end while loop with a break, as user selected 0.
            break; //break the loop, no more input
        }//end if of userchoice==0
    }//end while
    return 0;
}//end int main

//*************************************************
// CODE FILENAME: SerpeAssn6
// DESCRIPTION: House listing database using linked list. Can load/save files,
//              add/remove listings, and adjust prices based on changes text data file.
// CLASS/TERM: CS 362 Fall 8 W 1
// DESIGNER: Steve Serpe
// FUNCTIONS:
/*
int load_data(house_data*&);                    //loads data from listing file
void user_menu(char&, house_data*&);            //user options
void display_all(house_data*);                 //displays all listings
void add_listing(house_data*&);                 //adds a listing
int verify_mls();                              //checks mls input format
double verify_price();                                //checks price input format
int verify_status();                                  //checks status input format
string verify_zip();                                  //checks zip input format
string verify_realtor();                              //checks realty input format
void remove_listing(house_data*&);              //removes a listing
void exit_save(house_data*);                    //saves new listings file
int price_changes(house_data*);                 //adjusts prices based on changes data file
*/
//*************************************************

#include <iostream>                                       //enables I/O
#include <iomanip>                                         //enables I/O formatting
#include <string>                                         //enables strings
#include <fstream>                                        //file I/O
#include <cctype>

using namespace std;

//constants
const string INTRO = "Realtor's Association Listing Information";
const int REALTY_STRING_MAX = 20;                   //max length of realty company name
const int MLS_SIZE = 6;                             //size of MLS number
const int ZIP_SIZE = 5;                             //size of first part of zip
const int ADD_ON_ZIP_START = 7;                     //starting place of last 4 digit of zip
const int FULL_ZIP_SIZE = 10;                       //size of full zip code
const int ZIP_DASH_SPOT = 5;                        //spot in zip where dash goes
const int LINE_MAX = 8;                             //max number of mls printed per line
const string CHANGES_FILE = "changes.txt";

enum listing_status {AVAILABLE, CONTRACT, SOLD};    //enum holding listing status

               
struct house_data {int mls_number;                  //struct of all listing info
                   string zip;
                   double price;
                   listing_status status; 
                   string name;
                   house_data* next;
                   };


//prototypes
int load_data(house_data*&);
void user_menu(char&, house_data*&);
void display_all(house_data*);
void add_listing(house_data*&);
int verify_mls();
double verify_price();
int verify_status();
string verify_zip();
string verify_realtor();
void remove_listing(house_data*&);
void exit_save(house_data*);
int price_changes(house_data*);


//*************************************************
// FUNCTION: main
// DESCRIPTION: base of all other functions
// INPUT:
// Parameters:
// File:
// OUTPUT:
// Return Val: returns 0 if sucessful
// Parameters:
// File:
// CALLS TO: load_data, user_menu
//*************************************************
int main()
{

    char read_existing_prompt;                      //holds user input of if they wish to use
                                                    //existing data
    char converted_prompt;                          //holds user input of if they want to load
                                                    //data from the file
    char converted_continue_prompt;                 //uppercase of continue after file load error char
    char converted_selection;                       //uppercause of user menu selection
    int return_check;                               //holds return value of load data function
                                                    //used to exit if user doesnt want to continue 
                                                    //after load error
    house_data *list_top = NULL;                    //creates a data pointer to the top of 
                                                    //a house data struct list


    cout << INTRO << endl << endl;
    do {
          cout << "Load existing data from file (y/n)?";
          cin >> read_existing_prompt;
          converted_prompt = toupper(read_existing_prompt);
          if (converted_prompt != 'Y' && converted_prompt != 'N')
          {
               cout << "Invalid Selection." << endl;
          }
    } while (converted_prompt != 'Y' && converted_prompt != 'N');
    
    
    if (converted_prompt == 'Y')
         return_check = load_data(list_top);
    else if (converted_prompt == 'N')
         user_menu(converted_selection, list_top);
    
    if (return_check == 1)
        return 0;
    else if (return_check == 88)
        cout << "Data load sucessful!" << endl;
         
    while (converted_selection != 'E')
    {
        user_menu(converted_selection, list_top);
    }
    
    system("Pause");
    return 0;
}

//*************************************************
// FUNCTION:load_data
// DESCRIPTION: loads data from listings text file
// INPUT:
// Parameters: house_data* &list_top - top of listing data linked list
// File: user-entered file name containing house listing data
// OUTPUT:
// Return Val: returns 1 if error loading data and user chooses to exit
//             returns 88 if data loaded successfully
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// CALLS TO:
//*************************************************
int load_data(house_data* &list_top)
{
     ifstream load_listings;                             //input stream for listings
     
     bool report_error_once = false;                     //bool used so error is only reported once
     int convert_status;                                 //int value of status input
     char continue_prompt;                               //user input for if they want to continue
                                                         //after file load error
     char converted_cont_prompt;                         //converted continue prompt
     bool file_name_check = false;                       //used to check if file was found
     string file_input;                                  //user input of file they wish to load
     house_data* new_entry;                              //creates a pointer to a new house data data set
     
     int temp_mls;                                       //holds a mls number
     double temp_price;                                  //holds a house price
     string temp_zip;                                    //holds a zip
     string temp_name;                                   //holds a realty name
     
     do {
          cout << "Please enter the name of the data file " << endl << "you wish to load, or 'menu' to go to the main menu: ";
          cin >> file_input;
          
          load_listings.clear();
          load_listings.open(file_input.c_str());
          if (file_input == "menu")
             return 3;
          else if (!load_listings)
          {
             cout << "Error opening data file. Please try again" << endl;
          }
          else 
               file_name_check = true;
          
     } while (!file_name_check);
     
     if (load_listings)
     {   
        load_listings >> temp_mls;
        while (load_listings)
        {
              new_entry = new house_data;

             if (new_entry == NULL) {
                 cout << "Heap error - could not allocate memory" << endl;
                 system("Pause");
             }
             else {
                 load_listings >> temp_price >> convert_status >> temp_zip;
                 load_listings.ignore(1);
                 getline(load_listings, temp_name);
                   

                 new_entry->mls_number = temp_mls;
                 new_entry->price = temp_price;
                 new_entry->status = static_cast<listing_status>(convert_status);
                 new_entry->zip = temp_zip;
                 new_entry->name = temp_name;
             
                 new_entry->next = list_top; 
                 list_top = new_entry;
                     
                 load_listings >> temp_mls;
             }
         }
        
    }
   	load_listings.clear();
    load_listings.close();
    return 88;
}
//*************************************************
// FUNCTION: user_menu
// DESCRIPTION: displays user menu options. calls function of user selection
// INPUT:
// Parameters: 
//             char converted_selection - uppercase user input for menu selection
//             house_data* &list_top - top of listing data linked list
// File:
// OUTPUT:
// Return Val:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// CALLS TO: display_all, add_listing, remove_listing, exit_save
//*************************************************
void user_menu(char& converted_selection, house_data* &list_top)
{
     char user_selection;                               //users menu selection
     char save_prompt;                                  //user input for if they want to save
     char converted_save_prompt;                        //uppercase conversion of save prompt

     
     cout << endl;
     cout << "Please choose from the list below" << endl;
     cout << "Enter D to display all listings" << endl;
     cout << "Enter A to add a listing" << endl;
     cout << "Enter R to remove a listing" << endl;
     cout << "Enter C to load in price changes" << endl;
     cout << "Enter E to exit the program" << endl;
     cout << "Your choice: ";
     cin >> user_selection;
     converted_selection = toupper(user_selection);
     
     switch (converted_selection){
          case 'D':
               {
                   display_all(list_top);
                   system("pause");
               }
               break;
          case 'A':
               {

                   cout << endl << "Add new listing" << endl;
                   add_listing(list_top);
               }
               break;
          case 'R':
                   remove_listing(list_top);
               break;
          case 'C':
                   price_changes(list_top);
                   break;
          case 'E':
               do {
                   cout << "Do you save the data to the listings file (y/n)?";
                   cin >> save_prompt;
                   converted_save_prompt = toupper(save_prompt);
                   if (converted_save_prompt == 'Y')
                      exit_save(list_top);
                   else if (converted_save_prompt != 'N')
                        cout << "Invalid selection" << endl;
               } while (converted_save_prompt != 'N' && converted_save_prompt != 'Y');
               break;
          default:
                  cout << "Invalid selection" << endl;
     }
}
//*************************************************
// FUNCTION: display_all
// DESCRIPTION: displays all listings
// INPUT:
// Parameters: house_data* list_top - top of listing data linked list
// File:
// OUTPUT:
// Return Val:
// Parameters: 
// File:
// CALLS TO:
//*************************************************
void display_all (house_data* list_top)
{
     house_data* current = list_top;                       //creates a pointer to the current spot
                                                          //in house data list. initialized to list top
     string converted_string;                             //holds string form of house status enum
     

    
     if (current == NULL)
         cout << "No listings stored." << endl;
     else
     {
      
         cout << right << setw(15) << "Asking" << setw(14) << "Listing" << endl;
         cout << left << setw(9) << "MLS#" << setw(13) << "Price" << setw(12) << "Status"
              << setw(15) << "Zip Code" << "Realtor" << endl;
         cout << setw(9) << "------" << setw(13) << "-----------" << setw(12) << "========="
              << setw(15) << "----------" << "------------" << endl;
          
         while (current != NULL)
         {
            switch (current->status) {
                   case 0:
                        converted_string = "Available";
                        break;
                   case 1:
                        converted_string = "Contract";
                        break;
                   case 2:
                        converted_string = "Sold";
                        break;
            }
                        

             cout << setw(9) << fixed << setprecision(0) << current->mls_number 
                  << setw(13) << setprecision(2) << current->price 
                  << setw(12) << converted_string << setw(15) << setprecision(0)
                  << current->zip
                  << current->name << endl;
                  
             current = current->next;
          }
     }
}
//*************************************************
// FUNCTION: add_listing
// DESCRIPTION: adds a listing, calls to various functions to verify input
// INPUT:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// OUTPUT:
// Return Val:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// CALLS TO: verify_mls, verify_price, verify_status, verify_zip, verify_realtor
//*************************************************
void add_listing (house_data* &list_top)
{
    char user_selection;                     //used to check if user wants to add new 
                                             //listing or return to menu
    char converted_selection;                //uppercase conversion of user selection
    int new_mls;                             //holds new mls number to be added
    double new_price;                        //holds new price to be added
    int new_status;                          //holds new status to be added
    string new_zip;                          //holds new zip to be added
    string new_realty;                       //holds new realty name to be added
    
    house_data* add_entry;                   //creates a pointer to a new house data set
     
    do {
       
       new_mls = verify_mls();
       new_price = verify_price();
       new_status = verify_status();
       new_zip = verify_zip();
       new_realty = verify_realtor();
       
       add_entry = new house_data;
       
       
       if (add_entry == NULL) {
           cout << "Heap error - could not allocate memory" << endl;
           system("pause");
           }
       else {  
           add_entry->mls_number = new_mls;
           add_entry->price = new_price;
           add_entry->status = static_cast<listing_status>(new_status);
           add_entry->zip = new_zip;
           add_entry->name = new_realty;
           
           add_entry->next = list_top; 
           list_top = add_entry;          
       
       }
       

          cout << "Enter any single key to add another listing or M to return to menu):";
          cin >> user_selection;
          cin.ignore(100, '\n');
          converted_selection = toupper(user_selection);
       
       
    } while (converted_selection != 'M');
     
     
}
//*************************************************
// FUNCTION: verify_mls
// DESCRIPTION: verifies user input of mls number
// INPUT:
// Parameters: 
// File:
// OUTPUT:
// Return Val: int verified_mls - verified mls number
// Parameters:
// File:
// CALLS TO:
//*************************************************
int verify_mls()
{
    string user_input;                        //user input of listing mls number
    int length;                               //length of user input
    bool input_check;                         //bool to flag when input is correctly input
    bool report_error_once;                   //bool used so only one error message is sent
    int verified_mls;                         //verified mls input
    
    do {
        report_error_once = false;
        input_check = true;
        
        cout << "Please enter MLS #: ";
        cin >> user_input;
    
        length = user_input.length();
    
    
            for (int current = 0; current < length; current++)
            {
                if (isalpha(user_input[current]))
                {
                    if (!report_error_once)
                    {
                         cout << "Error. MLS must contain only numbers. Please re-enter." << endl;
                         input_check = false; 
                         report_error_once = true;
                    }
                }
            }
            if (user_input[0] == '0')
            {
               cout << "Error. First number of MLS cannot be 0. Please re-enter." << endl;
               input_check = false;
            }

            if (length > MLS_SIZE)
            {
               cout << "Error. Your number is greater than the required length of 6. Please re-enter." << endl;
               input_check = false;
            }
            else if (length < MLS_SIZE)
            {
               cout << "Error. Your number is less than the required length of 6. Please re-enter." << endl;
               input_check = false;
            }
            verified_mls = atoi(user_input.c_str());
            
            
    } while (!input_check);
    
    return verified_mls;
    
}
//*************************************************
// FUNCTION: verify_price
// DESCRIPTION: verifies user input of listing price
// INPUT:
// Parameters:
// File:
// OUTPUT:
// Return Val:double verified_price - verified price of listing
// Parameters:
// File:
// CALLS TO:
//*************************************************
double verify_price()
{
    string user_input;                             //user input of listing price
    bool input_check;                              //bool to flag when input is correctly input
    bool report_error_once;                        //bool used to only report an error message once
    double verified_price;                            //verified price input
    int length;                                    //length of user input
    
    
    do {
        report_error_once = false;
        input_check = true;
        
        cout << "Please enter listing price: ";
        cin >> user_input;
    
        length = user_input.length();
    
    
            for (int current = 0; current < length; current++)
            {
                if (isalpha(user_input[current]))
                {
                    if (!report_error_once)
                    {
                         cout << "Error. Price must contain only numbers. Please re-enter." << endl;
                         input_check = false; 
                         report_error_once = true;
                    }
                }
                if (user_input[current] == '-')
                {
                    cout << "Error. Price must contain only numbers. Please re-enter." << endl;
                    input_check = false; 
                }
            }
        verified_price = atof(user_input.c_str());
        if (verified_price < 1)
        {
            cout << "Error. Price must be greater than 0." << endl;
            input_check = false;
        }
    } while (!input_check);
    
    return verified_price;
}
//*************************************************
// FUNCTION: verify_status
// DESCRIPTION: verifies listing status input
// INPUT:
// Parameters:
// File:
// OUTPUT:
// Return Val: int user_input - verified user status input
// Parameters:
// File:
// CALLS TO:
//*************************************************
int verify_status()
{
     int user_input;                               //user input of listing status
     bool input_check;                             //bool to flag when input is correctly input
     
     
     do {
        input_check = true;
        cout << "Please enter listing status." << endl << "Enter 0 for available, 1 for under contract, or 2 for sold: ";
        cin >> user_input;
        
        if (isalpha(user_input))
        {
            cout << "Error. Listing status must only contain numbers. Please re-enter." << endl;
            input_check = false;
        }
        if (user_input < AVAILABLE || user_input > SOLD)
        {
            cout << "Error. Listing status must be a number between 0 and 2. Please re-enter." << endl;
            input_check = false;
        }

     } while (!input_check);
     
     return user_input;
     
}
//*************************************************
// FUNCTION: verify_zip
// DESCRIPTION: verifies zip of user input of listing
// INPUT:
// Parameters:
// File:
// OUTPUT:
// Return Val: string user_input - verified zip of listing
// Parameters:
// File:
// CALLS TO:
//*************************************************
string verify_zip()
{
    string user_input;                             //user input of listing zip
    int length;                                    //length of user input
    bool input_check;                              //bool to flag when input is correctly input
    bool report_error_once;                        //bool used so only one error message is sent
    
    do {
        report_error_once = false;
        input_check = true;
        
        cout << "Please enter zip code in format xxxxx-xxxx: ";
        cin >> user_input;
    
        length = user_input.length();
    
    
            for (int current = 0; current < ZIP_SIZE; current++)
            {
                if (isalpha(user_input[current]))
                {
                    if (!report_error_once)
                    {
                         cout << "Error. Zip must contain only numbers. Please re-enter." << endl;
                         input_check = false; 
                         report_error_once = true;
                    }
                }
                if (user_input[current] == '-')
                {
                    cout << "Error. Zip must contain only numbers. Please re-enter." << endl;
                    input_check = false; 
                }
            }
            for (int current = ADD_ON_ZIP_START; current < length; current++)
            {
                if (isalpha(user_input[current]))
                {
                    if (!report_error_once)
                    {
                         cout << "Error. Zip must contain only numbers. Please re-enter." << endl;
                         input_check = false; 
                         report_error_once = true;
                    }
                }
                if (user_input[current] == '-')
                {
                    cout << "Error. Zip must contain only numbers. Please re-enter." << endl;
                    input_check = false; 
                }
            }
            if (user_input[ZIP_DASH_SPOT] != '-')
            {
               cout << "Error. Must have a dash between 5 digit zip code and 4 digit add on code. Please re-enter." << endl;
               input_check = false;
            }

            if (length > FULL_ZIP_SIZE)
            {
               cout << "Error. Entered zip code is too long. Please re-enter." << endl;
               input_check = false;
            }
            if (length < FULL_ZIP_SIZE)
            {
               cout << "Error. Entered zip code is too short. Please re-enter." << endl;
               input_check = false;
            }
    } while (!input_check);

    return user_input;       
}
//*************************************************
// FUNCTION: verify_realtor
// DESCRIPTION: verifies input of realtor name input
// INPUT:
// Parameters:
// File:
// OUTPUT:
// Return Val: string user_input - verified realtor name input
// Parameters:
// File:
// CALLS TO:
//*************************************************
string verify_realtor()
{
    string user_input;                             //used to store user input for realtor name
    int length;                                    //length of user input
    bool input_check;                              //bool to flag when input is correctly input
    bool report_error_once;                        //bool used so only one error message is sent
    
    do {
        report_error_once = false;
        input_check = true;
        
        cout << "Please enter realty company name: ";
        cin.ignore(1,'\n');
        getline(cin, user_input);
    
        length = user_input.length();
    
            for (int current = 0; current < length; current++)
            {
                if (isdigit(user_input[current]))
                {
                    if (!report_error_once)
                    {
                         cout << "Error. Name must contain only letters. Please re-enter." << endl;
                         input_check = false; 
                         report_error_once = true;
                    }
                }
                if (user_input[current - 1] == ' ')
                   user_input[current] = toupper(user_input[current]);
                else 
                   user_input[current] = tolower(user_input[current]);
            }
            
            user_input[0] = toupper(user_input[0]);
            
            if (length > REALTY_STRING_MAX)
            {
               cout << "Error. Name must be less than 20 characters long. Please re-enter." << endl;
               input_check = false;
            }
    } while (!input_check);

    return user_input;  
       
}
//*************************************************
// FUNCTION: remove_listing
// DESCRIPTION: removes a listing from the listings array
// INPUT:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// OUTPUT:
// Return Val:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// CALLS TO: display_all, verify_mls
//*************************************************
void remove_listing(house_data* &list_top)
{
     bool target_found = false;                           //bool to check if target mls number was found
     int mls_target;                                      //return of verify mls, input 
                                                          //by user of mls they wish to remove
     bool remove_check = true;                            //used to check if verified mls
                                                          //is for adding or removing #
     int line_check = 0;                                  //checks so only so many mls
                                                          //numbers are printed on one line
                                                          
     house_data* current = list_top;                      //creates a pointer to the current spot
                                                          //in house data list
     house_data* prev;                                    //used to hold previous spot in house 
                                                          //data list
     
     if (current == NULL)
         cout << "No listings stored." << endl;
     else
     {
     
        cout << endl << "All current MLS numbers on record:" << endl;
        while (current != NULL)
        {
            if (line_check == LINE_MAX)
           {
                 cout << endl;
                 line_check = 0;                         
           }
           cout << current->mls_number << ' ';
           line_check++;
           current = current->next;
        }
        cout << endl << "To remove listing ";
        mls_target = verify_mls();
        
        
        current = list_top;
        if (list_top->mls_number == mls_target)
        {
            current = list_top;
            list_top = list_top->next;
            delete current;     
            cout << "MLS entry " << mls_target << " has been successfully deleted!" << endl;                    
        }
        else {
             while (current != NULL && (current->mls_number != mls_target))
             {
                  prev = current;
                   current = current->next;
             }
             if (current != NULL)
             {
                 prev->next = current->next;
                 delete current;
                 cout << "MLS entry " << mls_target << " has been successfully deleted!" << endl;
             }
             else 
                 cout << "MLS not found." << endl;
        
        }
        
     }
     system("pause");
}
//*************************************************
// FUNCTION: exit_save
// DESCRIPTION: saves listing file to new path or overwrites old file
// INPUT:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// OUTPUT:
// Return Val:
// Parameters:
// File: user-entered file name - holds all house listing data
// CALLS TO:
//*************************************************
void exit_save (house_data* list_top)
{
     ofstream listing_output;                             //out stream for listings
     ifstream file_check;                                 //in stream to check file existence
     string user_input;                                   //user input file destination
     bool existing_file;                                  //used to flag if file already exists
     char overwrite_selection;                            //answer to if user wishes to overwrite file
     char converted_selection;                            //uppercase conversion of overwrite_selection
     
     house_data* current = list_top;                      //creates a pointer to the current spot
                                                          //in house data list
     
     do {
          existing_file = false;
          cout << "Please enter the name you wish to save the file as: ";
          cin.clear();
          cin >> user_input;
     
          file_check.clear();
          file_check.open(user_input.c_str());
          if (file_check)
          {
             do {
                  cout << "File already exists. Please enter 'Y' to overwrite" << endl << "or 'N' to enter a new file name: ";
                  cin >> overwrite_selection;
                  converted_selection = toupper(overwrite_selection);
                  
                  if (converted_selection == 'Y')
                      existing_file = true;
                  else if (converted_selection == 'N')
                  {
                      cout << "You will need to enter a new file name." << endl;
                      existing_file = false;
                  }
                  else
                      cout << "Invalid choice." << endl;
             } while (converted_selection != 'Y' && converted_selection != 'N');
          }
          else 
             existing_file = true;
          
     } while (!existing_file);
     
        listing_output.open(user_input.c_str());
        while (current != NULL)
        {
              listing_output << current->mls_number << ' ' << current->price << ' '
                             << current->status << ' ' << current->zip << ' '
                             << current->name << endl;
              current = current->next;
              
        }
     
    
     

     file_check.close();
     listing_output.close();
     cout << "Data sucessfully saved!" << endl;
}

//*************************************************
// FUNCTION: price_changes
// DESCRIPTION: changes house listing data according to changes text data file
// INPUT:
// Parameters: house_data* &list_top - top of listing data linked list
// File:
// OUTPUT: CHANGES_FILE (changes.txt) - holds price changes for certain mls numbers
// Return Val: 8 - error loading changes file
// Parameters:
// File: CHANGES.TXT
// CALLS TO:
//*************************************************
int price_changes(house_data* list_top)
{
     
     ifstream change_data;                                //in stream for price changes data file
     change_data.open(CHANGES_FILE.c_str());
     
     house_data* current;                                 //creates a pointer to the current spot
                                                          //in house data list
     
     int temp_mls;                                        //holds mls number
     double temp_price;                                   //holds house price
     bool changes_made = false;                           //used to flag if any changes were made
     
     
     
     if (list_top == NULL)
     {
         cout << "Error. No listings stored to match to changes" << endl;
         system("Pause");
     }
     else 
     {
          if (!change_data)
          {
              cout << "Error. Changes file not found" << endl;
              return 8;
          }
          else
          {
               change_data >> temp_mls;
               while (change_data)
               {
                     change_data >> temp_price;
                     change_data.ignore(200, '\n');
                     current = list_top;
                     while (current != NULL)
                     {
                          if (current->mls_number == temp_mls)
                          {
                              current->price = temp_price;
                              cout << "MLS number " << current->mls_number << " successfully changed." << endl;
                              changes_made = true;
                          }
                          current = current->next;
                     }
                     
                         
                     change_data >> temp_mls;
                     
               }
               if (!changes_made)
               {
                   cout << "No matches were found, therefore no changes were made." << endl;
                   system("Pause");
               }
          }
     }
     system("Pause");
     change_data.close();
}

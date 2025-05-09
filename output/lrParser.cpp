#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <iomanip>  


using namespace std;

// STUDENT NUMBER : B231202072
// STUDENT NAME : BEYZA KOMISH
// LR PARSER HOMEWORK-1


//!!!!!!!!!!!!!!!!!!!!!

//HONOR CODE :  
//ALL THE LOGIC HAS BEEN LEARNED AND IMPLEMENTED BY BEYZA KOMISH FOR CLARIFICATION GOT HELP FROM RECOURCES WHEN LOGGING , READING THE FILES 
// AND ADJUSTED CODE READABILITY 

//!!!!!!!!!!!!!!!!!!!!!!

// please be located in the output folder and run in the terminal these commands below:
//   g++ -o lrParser.exe lrParser.cpp       --for compilation
//  ./lrParser.exe                          --for runnung the executable

//!!!!!!!!!!!!!

//PLEASE CHANGE THE INPUT FİLE STRING AS YOUR DESIRED STRING FOR TESTING 


struct parseTree
{
    string value;
    vector<parseTree*> children;

    parseTree(const string& val) : value(val) {}
};



// a vector string for the input string 
vector<string> input;

// a void for reading the input from the file and filling in the corresponding data strucutre which is declared just before the function 
void readInput(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        exit(1);
    }
    string line;
    if(getline(file,line))
    {
        istringstream iss(line);
        string token;
        while (iss >> token)
        {
            input.push_back(token);
        }
    }
}


// a strucutre decalred for the left hand side and the right handside of the grammar rule between there is an arrow
struct grammarRule {
    string lhs;
    vector<string> rhs;
};

unordered_map<int, grammarRule> grammarRules;

// a void for reading the grammar from the file and filling in the corresponding data strucutre which is declared just before the function 
void readGrammar(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        exit(1);
    }
    string line;
    while(getline(file,line))
    {
        istringstream iss(line);
        int id;
        string lhs,arrow,token;
        iss >> id >> lhs >> arrow;
        grammarRule rule;
        rule.lhs = lhs;
        while (iss >> token)
        {
            rule.rhs.push_back(token);
        }
        grammarRules[id] = rule;
    }
}

unordered_map<int, unordered_map<string, int>> gotoTable;
vector<string> headersGotoTable;//declaring the header globally which is used for reading and logging which is the 1 ever row of the GotoTable

// a void for reading the GotoTable from the file and filling in the corresponding data strucutre which is declared just before the function 
void readGotoTable(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        exit(1);
    }

    string line;

    getline(file, line); 
    istringstream headerStream(line);
    string temp , header;

    headerStream >> temp;
    while(headerStream >> header)
    {
        headersGotoTable.push_back(header);
    }

    while(getline(file,line))
    {
        istringstream rowStream(line);
        int state;
        rowStream >> state;

        for(const auto&col : headersGotoTable)// for every header we are filling in a coloumn 
        {
            string value;
            rowStream >> value;
            if(value != "-")//if the value isnt - fill in the action table with the value you read from the rows but convert it to integer first 
            {
                gotoTable[state][col] = stoi(value);
            }
        }
    }
}

unordered_map<int, unordered_map<string, string>> actionTable;
vector<string> headersActionTable;//declaring the header globally which is used for reading and logging which is the 1 ever row of the Action table
// a void for reading the action table from the file and filling in the corresponding data strucutre which is declared just before the function 
void readActionTable(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        exit(1);
    }

    string line;

    getline(file, line);
    istringstream headerStream(line);
    string temp , header;
   

    headerStream >> temp;
    while(headerStream >> header)
    {
        headersActionTable.push_back(header);
    }

    while(getline(file,line))
    {
        istringstream rowStream(line);
        int state;
        rowStream >> state;

        for(const auto&col : headersActionTable)// for every header we are filling in a coloumn 
        {
            string value;
            rowStream >> value;
            if(value != "-")//if the value isnt - fill in the action table with the value you read from the rows
            {
                actionTable[state][col] = value;
            }
        }

    }

    
}

string stackToString(stack<string> s) {// as tring function to convert the stack to string for code readability and maintain original stack
    vector<string> elements;
    while (!s.empty()) {
        elements.push_back(s.top());
        s.pop();
    }
    reverse(elements.begin(), elements.end());

    string result;
    for (const string& item : elements) {
        result += item + " ";
    }
    return result;
}

string inputToString(vector<string>& input, int index) {//a string funciton to convert input to string for logging
    string result;
    for (int i = index; i < input.size(); i++) {
        result += input[i] + " ";
    }
    return result;
}


void printParseTree(parseTree* node, string pathSoFar, ofstream& logfile) {
    if (!node) return;
    
    pathSoFar += "/" + node->value;
    logfile << pathSoFar << endl;
    
    for (auto child : node->children) {
        printParseTree(child, pathSoFar, logfile);
    }
}



stack<string> Stack;


int main()
{


    ofstream outputFile("output.txt");
    ofstream logfile("Log.txt");

    if(!outputFile.is_open())
    {
        cerr << "Error: Could not open output.txt" << endl;
        return 1;
    }
    if(!logfile.is_open())
    {
        cerr << "Error: Could not open Log.txt" << endl;
        return 1;
    }

        outputFile << "Step\t\t\t\t\tStack\t\t\t\t\t\t\t\tInput Left\t\t\t\tAction\n";
        outputFile << "-------------------------------------------------------------------------------------------------------------\n";
    

    readInput("input.txt");
    readGrammar("Grammar.txt");
    readGotoTable("GotoTable.txt");
    readActionTable("ActionTable.txt");



//*************************************************************************************************************************************************** */    
//LOGGING ALL THE TABLES AND RULES TO THE LOG.TXT IN A STRUCTURED FORMATTED WAY WITH A ORDERED LOOPS
    logfile << "Action table loaded:" << endl;
    logfile << "------------------------------------------------------------" << endl;

    logfile << "State\t";
    for (const auto& header : headersActionTable) {
        logfile << header << "\t";
    }
    logfile << endl;
    logfile << "------------------------------------------------------------" << endl;


    vector<int> states;
    for (const auto& entry : actionTable) {
        states.push_back(entry.first);
    }
    sort(states.begin(), states.end());
    
    for (int state : states) {
        logfile << state << "\t";
        for (const auto& header : headersActionTable) {
            auto it = actionTable[state].find(header);
            if (it != actionTable[state].end()) {
                logfile << it->second << "\t";
            } else {
                logfile << "-\t";
            }
        }
        logfile << endl;
    }
    
    logfile << "------------------------------------------------------------" << endl;

    logfile << "Goto table loaded:" << endl;
    logfile << "------------------------------------------------------------" << endl;

    logfile << left << setw(10) << "State";
    for (const auto& header : headersGotoTable) {
        logfile << left << setw(10) << header;
    }
    logfile << endl;
    
    vector<int> statesGoto = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    
    for (int state : statesGoto) {
        logfile << left << setw(10) << state;
        
        for (const auto& header : headersGotoTable) {
            if (gotoTable[state].count(header)) {
                logfile << left << setw(10) << gotoTable[state][header];
            } else {
                logfile << left << setw(10) << "-"; 
            }
        }
        logfile << endl;
    }

    logfile << "------------------------------------------------------------" << endl;


    logfile << "Grammar loaded:" << endl;
    logfile << "------------------------------------------------------------" << endl;

    logfile << left << setw(5) << "ID" << setw(5) << "LHS    \t" << "RHS" << endl;
    logfile << "------------------------------------------------------------" << endl;

    vector<pair<int, grammarRule>> sortedGrammarRules;
    
    for (const auto& entry : grammarRules) {
        sortedGrammarRules.push_back(entry);
    }

    sort(sortedGrammarRules.begin(), sortedGrammarRules.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    for (const auto& entry : sortedGrammarRules) {
        int id = entry.first;
        const grammarRule& rule = entry.second;

        logfile << left << setw(5) << id 
                << setw(5) << rule.lhs << "->  ";
        
        for (size_t i = 0; i < rule.rhs.size(); ++i) {
            logfile << rule.rhs[i];
            if (i < rule.rhs.size() - 1) {
                logfile << " "; 
            }
        }
        logfile << endl;
    }

    logfile << "------------------------------------------------------------" << endl;
    logfile << "Parsing for the input  : ";
    for (const auto& elem : input) {
        logfile << elem << " ";
    }
    logfile << endl;
    logfile <<"\n";
    logfile << "------------------------------------------------------------" << endl;
    
//*************************************************************************************************************************************************** */    
//LOGGING ALL THE TABLES AND RULES TO THE LOG.TXT 

    // Initialize the stack , input position and the counter with the initial state
    // The initial state is usually 0 in LR parsers
    Stack.push("0");
    int inputIndex = 0;
    string action;
    int Counter = 0;
    stack<parseTree*> parseStack; 

    while(true)
    {  
        // the statenumber will the top of the stack every time
        int stateNumber = stoi(Stack.top());
        string currentInput = input[inputIndex];// the current input
        action = actionTable[stateNumber][currentInput];// this is used for code readability the exact cell of the action table

     
        if( currentInput == "-")
        {
            logfile<<"\n";
            logfile<<"   UNKOWN TOKEN  :   " << currentInput<<endl;
            outputFile<<"\n";
            outputFile<<"   UNKOWN TOKEN  :   " << currentInput<<endl;
            cout<<"\n";
            cout<<"   UNKOWN TOKEN  :   " << currentInput<<endl;
            
            break;
        }

       

        if(action == "accept")//if the action cell says accept than print the stack the input and the accept message in a structured way
        {outputFile << Counter 
            << setw(30) << stackToString(Stack) 
            << setw(40) << inputToString(input, inputIndex)
            << setw(20) << "ACCEPT\n";  

            //generate parse tree
            logfile<<"ACCEPTED"<<endl;
            logfile<<"\n";

            
            logfile<<"Parse tree generated"<<endl;//****************************** */

            parseTree*root = parseStack.top(); // Get the root of the parse tree
            printParseTree(root, "", logfile); // Print the parse tree to the log file

            outputFile<<"\n";
            outputFile<<"Parse tree generated"<<endl;//****************************** */
            outputFile<<"\n";

            printParseTree(root, "", outputFile); // Print the parse tree to the output file

          
    

            break;
        }
        else if ( action[0] == 's' )//if the cell starts with s which means the action 'SHIFT'
            {
              
               
    //LOG THE EXPRESSIONS WHICH IS PUSHING THE INPUT TO STACK AND THE THE NUMBER NEXT TO 'S' WHİCH İS SHOWN LIKE THİS BELOW :'action.substr(1)'
    // WHICH BASICALLY MEANS SELECT THE SECOND ELEMENT AFTER THE S İN THE CELL
                logfile << "Current stack content: " << stackToString(Stack) << endl;
                logfile << "Current state (stack-top) is " << stoi(Stack.top()) << ", next token is '" << currentInput << "' (action table column " << stateNumber << ")" << endl;
                logfile << "Fetched action from the table: " << action << endl;
                logfile<<"\n";

                logfile << "Shift and goto state " << action.substr(1) << ": push the token \"" << currentInput << "\" and state " << action.substr(1) << " onto the stack" << endl;
                logfile << "Parse tree: token \"" << currentInput << "\" becomes a node (leaf)" << endl;
                logfile << "Move to next token" << endl;
                logfile<<"\n";
               
                // Shift
                cout<<"Shift "<<action.substr(1)<<endl;              
                
                Stack.push(currentInput);
                Stack.push(action.substr(1));

                // OUTPUT THE STACK , İNOUT AND THE SHIFT COMMAND RULE
                outputFile << Counter 
                << setw(30) << stackToString(Stack) 
                << setw(40) << inputToString(input, inputIndex)
                << setw(20) << "Shift " << action.substr(1) << "\n";

                parseTree* newNode = new parseTree(currentInput); // Create a new node for the parse tree
                parseStack.push(newNode);


                // THIS IF STATEMENT IS TO CHECK IF THERE IS AN OPERATOR ONE AFTER THE OTHER WHICH IS A SYNTAX ERROR,PRINT ANY SHIFT COMMANDS
                if(currentInput == "+" || currentInput == "-" || currentInput == "*" || currentInput == "/")
                {
                    inputIndex++;
                    string currentInput2 = input[inputIndex];
                    
                    if(currentInput2 == "+" || currentInput2== "-" || currentInput2 == "*" || currentInput2 == "/")
                    {
                        logfile<<"\n";
                        logfile<<"   SYNTAX ERROR AT :   " << currentInput2<<endl;
                        outputFile<<"\n";
                        outputFile<<"   SYNTAX ERROR AT :   " << currentInput2<<endl;
                        cout<<"\n";
                        cout<<"   SYNTAX ERROR AT :   " << currentInput2<<endl;
                        break;
        
                    }
                    inputIndex--;
        
                }
                
  

               inputIndex++;// INCREMENT THE INDEX FOR THE NEXT INPUT VALUE
              currentInput= input[inputIndex];//UPDATE THE CURRENT INDEX 


           
   
          
            }
           
            else if (action[0] == 'r' )//if the cell starts with an r than that means reduce
            {


                vector<parseTree*> children;

    //LOG THE EXPRESSIONS WHICH reduces the GRAMMAR AND POPS THE STACK ELEMENTS WİTH THE RHS GRAMMARS RULE SIZE WITH 2
    // AND PUSHES THE LEFT HAND SIDE OF THE GRAMMER TO THE STACK AND THE GOTO TABLE VALUE TO THE STACK
    // THE LOGFILE COMMANDS ARE IN BETWEEN EVERY CHANGE TO LOG CORRECT ACTIONS
                logfile<<"\n";
                logfile << "Current stack content: " << stackToString(Stack) << endl;
                logfile << "Current state (stack-top) is " << stoi(Stack.top()) << ", next token is '" << currentInput << "' (action table column " << Counter << ")" << endl;
                logfile << "Fetched action from the table: " << action << endl;
                logfile<<"\n";
                
                //the GRAMMAR RULE NUMBER İS NEXT TO THE r LETTER 
                int grammarRuleNum = stoi(action.substr(1));
                grammarRule rule = grammarRules[grammarRuleNum];

                logfile<<"\n";
                logfile << "Reduce using grammar rule " << grammarRuleNum << ": " << rule.lhs << " -> ";
                for (const auto& sym : rule.rhs) logfile << sym << " ";
                logfile << endl;
                logfile << "Stack content before reduce: " << stackToString(Stack) << endl;
                logfile << "Pop tokens and associated states from the stack considering the tokens at RHS of grammar rule (" << grammarRuleNum << ": " << rule.lhs << " -> ";
                for (const auto& sym : rule.rhs) logfile << sym << " ";
                logfile << ")" << endl;
                logfile<<"\n";

                cout << "Reduce by " << rule.lhs << " -> ";

                for (const auto& sym : rule.rhs) cout << sym << " ";
                cout << endl;
                
                int length = rule.rhs.size()*2;
                for (int i = 0; i < length; i++)
                {
                    Stack.pop();
                }
                logfile<<"\n";
                logfile << "State at stack is " << Stack.top() << " (current stack content: " << stackToString(Stack) << ")" << endl;
                logfile << "Push LHS (" << rule.lhs << ") of the grammar rule (" << grammarRuleNum << ": " << rule.lhs << " -> ";
                for (const auto& sym : rule.rhs) logfile << sym << " ";
                logfile << ") onto stack" << endl;
                logfile<<"\n";

                int stackTop = stoi(Stack.top());

                int newState =  gotoTable[stackTop][rule.lhs];

                Stack.push(rule.lhs);
                Stack.push(to_string(newState));


                for (int i = 0; i < rule.rhs.size(); i++)
                {
                   if(!parseStack.empty()){
                        children.push_back(parseStack.top());
                        parseStack.pop();
                   }
                }
                reverse(children.begin(), children.end());
                parseTree* parent = new parseTree(rule.lhs); // Create a new node for the parse tree
                parent->children = children; // Assign the children to the parent node

                parseStack.push(parent); // Push the parent node onto the parse stack
                

                    // OUTPUT THE STACK THE INOUT AND THE GRAMMAR RULE AS WELL AS THE GOTO COMMAND IN THE OUTPUT.TXT
                    outputFile << Counter 
                    << setw(30) << stackToString(Stack) 
                    << setw(40) << inputToString(input, inputIndex)
                    << setw(20) <<"  Reduce " << grammarRuleNum <<"  " <<rule.lhs << " -> ";

                for (const auto& sym : rule.rhs) outputFile << sym << " ";
                outputFile << " (GOTO[" << stackTop << ", " << rule.lhs << "])" << endl;

        
            

                
                logfile<<"\n";
                logfile << "Push new state comes from goto table onto stack: " << newState << " (GOTO[" << stackTop << ", " << rule.lhs << "])" << endl;
                logfile << "Parse tree: grammar rule's LHS (" << rule.lhs << ") becomes a node (parent) and tokens at the RHS (";
                for (const auto& sym : rule.rhs) logfile << sym << " ";
                logfile << ") becomes its children" << endl;
                logfile << "Stack content after reduce: " << stackToString(Stack) << endl;
                logfile<<"\n";
//eND OF LOGGONG DETAILED ACTIONS***************************************************************************************************
            }
            else
            {
                for (const auto& action : headersActionTable) {
                    if (action == "-" || action == "/" ) {
                        cout << "Error: Invalid action!" << endl;
                        outputFile << "Error: Invalid action!" << action << endl;
                        logfile << "Error: Invalid action!" << action << endl;
                        logfile << "\n";
                        break;
                    }
                }
                

            }
        Counter++;
    }

    outputFile.close();
    logfile.close();
 return 0;
}
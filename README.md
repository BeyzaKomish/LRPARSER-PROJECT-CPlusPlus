#Please check the report for more information :


# LR Parser 

## 📜 Honor Code

All the logic in this project has been personally learned and implemented by **Beyza Komish**. External resources were only used for understanding logging and file reading techniques. Code readability and functionality were adjusted accordingly.

---

## 📌 Introduction

This project implements a **Shift-Reduce LR Parser** in C++. It reads an input sequence and determines whether it conforms to a predefined grammar by using **action** and **goto tables**, ultimately generating a parse tree if the input is valid.

---

## ⚙️ How to Compile and Run

Please navigate to the `output` folder in your terminal and execute the following commands:

```bash
g++ -o lrParser.exe lrParser.cpp      # Compile the code
./lrParser.exe                        # Run the parser
````

---

## 🧠 Parser Logic

### Main Parsing Loop

The parser continuously processes input tokens in a loop until it either:

* **Accepts** the input (valid according to grammar),
* Encounters a **Syntax Error** or **Unknown Token**, or
* Executes a **Shift** or **Reduce** operation.

---

### 🔁 Shift Operation

When an action table entry starts with `'s'`, the parser:

* Pushes the current token and the next state onto the stack.
* Logs the shift operation and updates the parse tree.

```cpp
if (action[0] == 's') {
    Stack.push(action.substr(1));
    Stack.push(currentInput);
}
```

Error checking is also included to detect cases such as two consecutive operators.

---

### 🔽 Reduce Operation

When the action table indicates a reduce (`'r'`), the parser:

* Pops the right-hand side of the rule from the stack.
* Pushes the left-hand side non-terminal and updates the parse tree.
* Moves to a new state using the **goto table**.

```cpp
int length = rule.rhs.size() * 2;
for (int i = 0; i < length; i++) {
    Stack.pop();
}
Stack.push(rule.lhs);
Stack.push(to_string(newState));
```

---

## 🪵 Logging & Debugging

Each parsing step is logged for transparency and debugging. Logs include:

* Stack content
* Current token and action
* Grammar rule applied
* Parse tree updates
* Errors (e.g., syntax or unknown tokens)

Example log:

```text
Shift and goto state 3: push the token "+" and state 3 onto the stack
Parse tree: token "+" becomes a node (leaf)
Move to next token
```

---

## ❗ Error Handling

The parser checks for:

1. **Unknown Tokens:** Tokens not found in the action table.
2. **Syntax Errors:** For example, detecting two operators in a row.

Example:

```cpp
if(currentInput == "+" || currentInput == "-" || currentInput == "*" || currentInput == "/") {
    inputIndex++;
    if(input[inputIndex] == "+" || ... ) {
        cout << "SYNTAX ERROR AT: " << currentInput2 << endl;
        break;
    }
    inputIndex--;
}
```

---

## 📂 Data Reading Functions

These `void` functions populate the parser’s data structures:

```cpp
readInput("input.txt");
readGrammar("Grammar.txt");
readGotoTable("GotoTable.txt");
readActionTable("ActionTable.txt");
```

---

## 📊 Data Structures

* **Action Table:** `unordered_map` for efficient lookup of actions.
* **Goto Table:** `unordered_map` for tracking transitions on non-terminals.
* **Input Sequence:** `vector` for fast, indexed access during parsing.
* **Stack:** Simulates the state transitions during parsing.

These were chosen for **efficiency** in time and space complexity.

---

## 🌳 Parse Tree

Once input is accepted:

* A parse tree is generated and logged.
* Represents how the input maps to the grammar.

```cpp
logfile << "Parse tree generated" << endl;
printParseTree(root, "", logfile);
```

---

## ✅ Conclusion

This LR parser:

* Implements shift-reduce parsing with detailed logging.
* Supports robust error handling.
* Efficiently uses data structures for state transitions.
* Produces a parse tree for valid input.

The project demonstrates a clear understanding of compiler parsing techniques and stack-based automata using C++.



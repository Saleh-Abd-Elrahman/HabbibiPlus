# Table of Contents

# Project Planning

## I.I Aim
The project's core objective is to create a unique programming language, deeply rooted in the Arabic language. This language is designed to facilitate native Arabic speakers in learning and mastering programming concepts in a more intuitive way. It aims to eliminate language barriers in coding between Arabic speakers and the programming world, enabling a broader audience to engage in software development. 

## I.II Main Features

1. **Arabic-centric Syntax**  
   - The language features a syntax that is inherently Arabic, making it familiar to native speakers. 
   - It employs Arabic script, idiomatic expressions, and grammatical constructs, offering a seamless transition from natural language to programming.

2. **Custom Lexer and Parser Development**  
   - Tailored specifically for parsing Arabic syntax. 
   - Handles unique aspects of the language, such as various verb forms, gendered nouns, and script nuances.

3. **Integration with Traditional Programming Concepts**  
   - While focusing on Arabic, the language supports various data types and standard programming paradigms. 
   - Ensures it meets the expectations of modern programming practices.

4. **Interactive Development Environment (IDE) Support**  
   - Develop or integrate with an existing IDE to provide a user-friendly interface. 
   - Equipped with syntax highlighting, code completion, and error diagnostics, tailored for the Arabic script.

5. **Comprehensive Documentation and Tutorials**  
   - Comprehensive documentation and tutorials in Arabic. 
   - Aims to lower the learning curve and provide extensive support for new programming learners.

## I.III Target Audience

1. **Native Arabic Speakers**  
   - Especially those who may find English-oriented programming languages as a barrier to learning coding.

2. **Educational Institutions in Arabic-speaking Regions**  
   - A valuable tool in educational settings, making it easier for students to learn programming concepts.

3. **Linguistic Researchers and Enthusiasts**  
   - Those interested in the intersection of language and technology, particularly in how programming paradigms can adapt to different linguistic frameworks.

---

# Language Design

## II.I Syntax

Our custom-designed programming language contains a unique mix of standard C programming language features with support for Arabic script.

### Data Types

1. **Integer (‘TOKEN_INT’)**  
   - Represents whole numbers without any fractional or decimal components. 
   - Mainly used for mathematical operations including counting and indexing. 
   - The code handles integer values by using `int` data type in C. 
   - Supports arithmetic operations (addition, subtraction, multiplication, and division), logical operations, incrementation, decrementation, and modulo.

2. **Double (‘TOKEN_DOUBLE’)**  
   - Represents floating-point numbers which include numbers with decimal points. 
   - Essential for operations that require fractions or precise calculations. 
   - Uses the `double` data type in C to handle floating-point numbers. 
   - Handles type conversion in expressions involving both integers and doubles, ensuring accurate calculations.

3. **Character Strings (‘TOKEN_CHAR’)**  
   - Used for representing text in the code, which can be single characters, words, or sentences. 
   - **Wide Characters (`wchar_t`):** The use of wide characters allows Habibi++ to support a wide range of characters (including Arabic), enabling multilingual text handling. 
   - Supports string operations such as assignment and printing.

### Arithmetic and Logical Operators
- Addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`)
- Modulus operation (`%`)
- Increment (`TOKEN_INCREMENT_BY`, `+=`)
- Decrement (`TOKEN_DECREASE_BY`, `-=`)
- Multiply By (`TOKEN_MULTIPLY_BY`, `*=`)
- Modulus By (`TOKEN_MOD_BY`, `%=`) 
- Divide By (`TOKEN_DIVIDE_BY`, `/=`)

### Variable Declaration and Assignment
- **Variable Naming:** Supports variable names with Arabic letters only.
- **Assignment Operator (`=`):** For assigning values to variables.

### Printing and Output
- **Print Function (`TOKEN_PRINT`):** For outputting values, supports both string literals and variables.

### Error Handling
- **`TOKEN_ERROR`:** Used for raising errors when unexpected or invalid tokens are used in the code, e.g. using the wrong syntax or adding an integer variable to a string variable. 
  - Example error message for an invalid increment by a string: `"Type error: %ls is not an integer\n"`.
  - Division by zero error also raises a similar informative message.
- **`void expect()`:** Function to ensure the next token in code is the correct/expected token, otherwise an error is raised via `parseError()` with a message: `"Unexpected token"`.
- **`void parseError()`:** Universal function for handling errors. It prints out the specific error that occurs and the related tokens, using `printToken(currentToken)`.

## II.II Semantics of the Language

1. **Expressions and Operations**  
   - Defines precise behaviors for arithmetic operations, ensuring standard mathematical operations in Arabic. 
   - Follows a deterministic evaluation strategy for consistent results.

2. **Variable Assignment and Scoping Rules**  
   - Variables are lexically scoped with block-level scoping. 
   - Assignments mutate the program state in a logical and traceable manner.

3. **Error Detection and Reporting**  
   - Semantic analysis ensures errors are detected and reported in an informative way. 
   - Provides meaningful feedback that resonates with the developer's linguistic context.

### Language Usage

- **Mathematical Calculations and Operations**:  
  Allows coders to perform mathematical calculations in Arabic using arithmetic and logical operations, as well as variable assignments.

- **User Interactions**:  
  Facilitated through print statements and variable declarations, enhancing utility in various scenarios.

- **Educational & Experimental Use**:  
  Well-suited for educational settings and for beginner programmers who are not fluent in English, focusing on fundamental concepts with simple syntax.

---

# III. Running Programs in Habibi++
![image](https://github.com/user-attachments/assets/1c895aee-5709-461e-8af1-f275029e950a)
- This phase demonstrates writing in the Habibi++ programming language.
- Showcases variable assignments and interactive activities (printing to terminal), highlighting ease and intuitiveness in coding with a native language.

![image](https://github.com/user-attachments/assets/6b4aecbd-9a36-46ed-b722-8c0e33a65e4e)
- Demonstrates mathematical operations in Habibi++, enabling the use of variable assignments, arithmetic, and retrieval of results.

![image](https://github.com/user-attachments/assets/7ed0fe33-0c18-44e2-a113-30148c6d2e15)
> _Example: Demonstrates the use of logical operators for mathematical manipulation and calculations in Arabic._

![image](https://github.com/user-attachments/assets/346bbc23-826e-4abc-a4fb-7babb7bdaf74)
> _Example: Shows how programmers can assign strings to variables, print string variables, or print strings alongside mathematical operations._

---

# IV. Design Choices & Design Concepts

## Building the Lexer in Habibi++
1. **Character Recognition**  
   - Scans the source code character by character, identifying and categorizing each based on predefined Arabic Unicode ranges.

2. **Token Generation**  
   - Classifies sequences of characters into tokens (e.g., `TOKEN_INT`, `TOKEN_DOUBLE`, `TOKEN_CHAR`). 
   - Recognizes Arabic keywords (e.g., _إذا_ for "if", _بينما_ for "while") and converts them into corresponding tokens.

3. **Error Handling**  
   - Generates `TOKEN_ERROR` for unrecognized input, providing informative messages.

4. **Unicode and Locale Support**  
   - By including `<wchar.h>` and `<locale.h>`, the lexer is made Unicode-aware, essential for processing Arabic text.

## Building the Parsers
1. **Parser Design**  
   - Ensures the code adheres to the syntactical rules of Arabic scripting. 
   - Handles operator precedence and associativity, manages control flow with loops and conditionals (not currently implemented).

2. **Semantic Actions**  
   - Builds symbol tables and checks for type consistency during parsing, ensuring semantic validity.

3. **Integration with Lexer**  
   - Consumes tokens from the lexer and requests the next token as needed. 
   - Collaborates closely with the lexer to accurately translate user intentions into executable actions.

# XML Parser Project

## Overview

This project is a C++ XML parser designed to parse XML files into a tree structure. It supports parsing of tags, attributes, and text nodes, and includes features for error handling, debugging output, and callback functionality based on tag paths and conditions. The project also includes an example of a callback system that can trigger specific actions when certain conditions are met, such as when an `order/amount` tag exceeds a specified value.

## Features

- **Parsing XML Structure**: Reads and parses XML files into a tree structure.
- **Tag Handling**: Handles opening and closing tags, including nested tags.
- **Attribute Extraction**: Extracts and prints attributes of tags.
- **Text Node Parsing**: Processes and decodes text nodes.
- **Callback System**: Provides callback functionality based on tag paths and conditions.
- **Error Handling**: Identifies and reports errors such as mismatched or unclosed tags.
- **Debugging Output**: Option to print the parsed XML structure for debugging purposes.

## Files

- `src/`: Contains the source code for the project.
  - `xmlParser.cpp`: Contains the core XML parser logic.
  - `xmlUtils.cpp`: Contains utility functions such as decoding entities and trimming strings.
  - `main.cpp`: The main entry point for the application.
- `tests/`: Contains the test cases for the parser.
  - `test_parser.cpp`: Unit tests for validating the XML parser's functionality.
- `Makefile`: The build script for compiling the project.

## Setup Instructions

### Prerequisites

Ensure you have the following installed:
- A C++17 compatible compiler (e.g., GCC, Clang).
- `make` build tool.

### Cloning the Repository

Clone the project to your local machine:

```bash```
git clone https://github.com/yourusername/xmlparse.git
cd xmlparse


## Usage
The XML parser can be used to parse any XML file. You can configure the behavior using the callback system to act upon specific tags and conditions.

# Example:

```
std::string pathTofollow = "*/order/amount";
xmlParser.addCallBack(pathTofollow,[](const xmlParser::xmlNode& node){
  if(xmlUtils::pathEndsIn(node.path, "order/amount") && xmlUtils::parseIfInteger(node.text)>100){
    auto it = node.parent->attributes.find("id");
    if(it != node.parent->attributes.end()){
      std::cout<< "Order "<<it->first<<":" << it->second << " has amount "<<node.text<<" which is greater than 100.\n";
            }
        }
    });
```
This callback listens for an '<amount>' tag under an '<order>' tag and checks if its value exceeds 100. If so, it prints the id attribute of the parent order tag.

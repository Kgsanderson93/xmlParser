#include <iostream>
#include <fstream>

#include "xmlDemoRunner.h"

bool debug = false;
bool testMode = false;


int main(int argc, char *argv[])
{
    bool testMode = argc > 1 && std::string(argv[1]) == "--test";
    std::ofstream outFile;
    std::streambuf *coutBuf = std::cout.rdbuf();//save original buffer
    if (testMode) {
        outFile.open("test_output.txt");
        std::cout.rdbuf(outFile.rdbuf());  // Redirect cout to file
    }

    std::string command;
    if(!testMode){
    std::cout << "Select the demo to run\n- 1 default order greater than 100 demo\n- 2 custom demo\n";
    getline(std::cin, command);
    }
    xmlDemoRunner runner;
    runner.testMode = testMode;
    runner.debug=debug;
    if (command == "1" || testMode)
    {
        runner.runDefault();
        return 0;
    }
    else if (command == "2")
    {
        runner.run();
        return 0;
    }

    if (testMode) {
        std::cout.rdbuf(coutBuf);  // Restore original buffer
        outFile.close();
    }

    return 0;
}
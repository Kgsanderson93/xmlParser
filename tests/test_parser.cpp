#include <iostream>
#include <string>
#include "xmlParser.h"
#include <chrono>



int main()
{

    std::cout << "Running basic XML parser test...\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::string xml = R"(
        <root>
            <order id="123">
                <amount>200</amount>
            </order>
            <order id="456">
                <amount>50</amount>
            </order>
        </root>
    )";

    Xml_Parser::xmlParser parser;
    parser.setPrintMode(false);
    parser.parse(xml);
    // Record end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Output duration
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;



    return 0;
}

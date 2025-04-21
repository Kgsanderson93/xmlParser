#include <iostream>
#include <string>
#include "xmlParser.h"

int main() {
    std::cout << "Running basic XML parser test...\n";

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

    xmlParser::xmlParser parser;
    parser.setPrintMode(true);
    parser.parse(xml);

    return 0;
}

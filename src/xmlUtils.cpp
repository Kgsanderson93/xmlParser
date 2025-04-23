#include "xmlUtils.h"
#include <vector>
#include <climits> // For INT_MIN
#include <iostream>
#include <fstream>
#include <sstream>


namespace xmlUtils
{

    /**
     * Reads in a file to a string;
     */
    std::string readFileToString(std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "File failed to open\n";
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    /**
     * Returns the last piece of a path.
     */
    std::string getLastPathComponent(const std::string &path)
    {
        size_t pos = path.rfind('/');
        if (pos == std::string::npos)
        {
            return path; // No slash found, return the whole string
        }
        return path.substr(pos + 1);
    }
    /**
     * Normalizes input paths so that they never start or end in / and removes duplicate /
     */
    std::string normalizePath(const std::string &rawPath)
    {
        std::string result;
        bool lastWasSlash = false;

        for (char c : rawPath)
        {
            if (c == '/')
            {
                if (!lastWasSlash)
                {
                    result += c;
                    lastWasSlash = true;
                }
            }
            else
            {
                result += c;
                lastWasSlash = false;
            }
        }

        // Remove leading slash
        if (!result.empty() && result[0] == '/')
        {
            result = result.substr(1);
        }

        // Remove trailing slash
        if (!result.empty() && result.back() == '/')
        {
            result.pop_back();
        }

        return result;
    }

    /**
     * This function takes in a pattern from the callback and the actual path of the current node and checks if either the path is exactly correct or if it contains a wildcard and the ending matches.
     */
    bool matchPath(const std::string &pattern, const std::string &path)
    { // currently this only handles front wildcards. If a case was found where end wildcards would be useful this would need to be updated... Haven't seen one yet.
        // std::cout<<"\n From match path: "<<path<<" pattern: "<< pattern<<"\n";
        if (pattern == path)
        {
            return true; // this was an exact match
        }
        else if (!pattern.empty() && pattern[0] == '*' && pattern[1] == '/')
        {
            // easiest wildcard situation. First char is the wildcard

            std::string suffix = pattern.substr(1); // just remove the *
            if (path.size() >= suffix.size() && pathEndsIn(path, suffix))
            {
                return true;
            }
        }

        return false;
    }
    /**
     * This function attempts to parse a string to an int and returns the value if the entire string was successfully converted and returns int_min if not.
     */
    double parseIfDouble(const std::string &str)
    {
        if (str.empty())
            return INT_MIN;
        size_t idx = 0;
        try
        {
            double val = std::stod(str, &idx); // idx tells us how many chars were converted
            /// so theoretically if they all converted it really was an int (we're currently ignoring numbers too large to be an int.)
            if (idx == str.size())
            {
                return val; // conversion successful
            }
        }
        catch (...)
        {
        } // fall through returning false anyway no need to handle error.
        return INT_MIN;
    }
    /**
     * This checks if the path supplied by the node ends in the target path
     * basically allows '../' to act as a wildcard.
     */
    bool pathEndsIn(std::string nodePath, std::string endsin)
    {
        if (nodePath.length() >= endsin.length())
        {
            return (nodePath.compare(nodePath.length() - endsin.length(), endsin.length(), endsin) == 0);
        }
        else
        {
            return false;
        }
    }
    /**
     * Checks if a string is functionally empty by including only whitespace
     */
    bool isWhiteSpaceOnly(const std::string &str)
    {
        for (char ch : str)
        {
            if (!std::isspace(ch))
            {
                return false;
            }
        }
        return true;
    }
    /**
     * Trims a string of any extreneous whitespace
     */
    std::string trim(const std::string &str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos)
        {
            return ""; // String only contains whitespace
        }
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
    /**
     * This function recieves a stack of tags as seen in the xml parser so far.
     * Currently it is assumed that this stack only contains tags that are currently open.
     * We make a copy of the stack as it is, then append the bits to a vector to correct the order.
     * We then concat all the pathBits together with '/' markers to build a proper path
     * This function then returns the path.
     */
    std::string buildPath(std::stack<std::string> &tagstack)
    {

        std::vector<std::string> pathBits;
        std::stack<std::string> stackCpy = tagstack; // I don't believe I want the stack disrupted before the other elements have been found and closed so I will make a copy of the current stack
        // this may need to be modified to ignore any closed tags we see if we end up keeping those similar to how Rosie functions. Currently however we
        // pop off tags as they are closed.

        while (!stackCpy.empty())
        {
            pathBits.push_back(stackCpy.top());
            stackCpy.pop();
        }
        std::string path = "/";
        for (auto it = pathBits.rbegin(); it != pathBits.rend(); ++it)
        {
            path += *it + "/";
        }
        if (path.back() == '/')
            path.pop_back();
        return path;
    }

    /**
     * This function will recieve a string which may potentially have xml escaped entities
     * This function returns the string recieved with the escape entities decoded
     */
    std::string decodeEnt(const std::string &input)
    {
        std::string output;
        size_t i = 0;
        // This function uses an array to make changes to the list of escaped entities easier and to be rid of any "magic numbers"
        std::string entities[MAXENTITIES] = {"&quot;", "&amp;", "&apos;", "&lt;", "&gt;"};
        char result[MAXENTITIES] = {'\"', '&', '\\', '<', '>'};

        // I may have also been able to make this a struct but since this is so far only used in this function it seemed easier/more efficient to inline it
        while (i < input.size()) // loops through the provided string stopping at any possible escape entities and replacing them.
        {
            if (input[i] == '&')
            {
                int index = 0;
                bool found = false;
                while (index < MAXENTITIES && found != true)
                {
                    if (input.compare(i, entities[index].length(), entities[index]) == 0)
                    {
                        output += result[index];
                        i += entities[index].length();
                        found = true;
                    }
                    index++;
                }
                if (found == false) // if input[i] was an & char and did not match any of the provided escape entities it may just be an & char. Please proceed.
                {
                    output += '&';
                    i++;
                }
            }
            else // skip over all not & chars as all escape entities begin with &
            {
                output += input[i++];
            }
        }
        return output;
    }
}
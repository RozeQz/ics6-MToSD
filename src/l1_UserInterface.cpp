/* Программа-заготовка для домашнего задания
 */

#include <iostream>
#include <string>
#include <cassert>

#include <sstream>

#include "hw/l2_ApplicationLayer.h"

using namespace std;

class TerminalOutput : public IOutput
{
public:
    virtual void Output(string s) const override final
    {
        cout << s << endl;
    }
};

int main(int, char **)
{
    TerminalOutput out;
    Application app(out);

    for (string line; getline(cin, line);)
    {
        if (line.empty())
            break;

        istringstream iss(line);
        vector<string> args;

        for (string str; iss.good();)
        {
            iss >> str;
            args.emplace_back(str);
        }

        if (!app.performCommand(args))
            return 1;
    }

    cout << "Execution completed successfully" << endl;
    return 0;
}

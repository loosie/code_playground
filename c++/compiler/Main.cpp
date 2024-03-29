#include "Main.h"

auto main() -> int
{
    string sourceCode = R""""(
        function main() {
            p 'Hello, World!';
            pl 1+2*3;

            for i=0, i<3, i=i+1 {
                pl i;
            }

            if true {
                p 'if';
            } elif false {
                p 'elif';
            } else {
                p 'else';
            }
        }
    )"""";

    cout << sourceCode << endl;

    auto tokenList = scan(sourceCode);
    auto syntaxTree = parse(tokenList);
    auto objectCode = generate(syntaxTree);
    execute(objectCode);
    // printObjectCode(objectCode);

    return 1;
}

// auto printObjectCode(tuple<vector<Code>, map<string, size_t>> objectCode) -> void
// {
//     auto codeList = get<0>(objectCode);
//     auto functionTable = get<1>(objectCode);
//     cout << setw(11) << left << "FUNCTION"
//          << "ADDRESS" << endl;
//     cout << string(18, '-') << endl;
//     for (auto &[key, value] : functionTable)
//         cout << setw(11) << key << value << endl;
//     cout << endl;
//     cout << "ADDR"
//          << " " << setw(15) << "INSTRUCTION"
//          << "OPERAND" << endl;
//     cout << string(36, '-') << endl;
//     for (size_t i = 0; i < codeList.size(); i++)
//         cout << setw(4) << right << i << " " << codeList[i] << endl;
// }
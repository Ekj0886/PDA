#include <bits/stdc++.h>
#include "Legalizer.h"

#define F first 
#define S second

using namespace std;
// using time = std::chrono::time_point<std::chrono::high_resolution_clock>;

std::string GetStringAfterSlash(const std::string& input) {
    size_t pos = input.find_last_of('/');
    if (pos != std::string::npos) return input.substr(pos + 1);
    return input;
}

int main(int argc, char *argv[]) {

    string input_lg  = argv[1];
    string input_opt = argv[2];
    string output_lg = argv[3];

    string in_image  = GetStringAfterSlash(input_lg);
    string out_image = GetStringAfterSlash(output_lg);

    TIME clock;

    clock.StartTimer();

    LEGALIZER LGR;
    LGR.parse(input_lg, output_lg);
    LGR.PlaceCell();

    LGR.DumpLayout("Layout/img_" + in_image);

    LGR.RunOpt(input_opt);

    clock.EndTimer();
    cout << "Runtime: " << clock.Duration() << endl;

    LGR.DumpLayout("Layout/img_" + out_image);

    return 0;

}
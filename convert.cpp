/* Utility to handle P1 and P2 - eventually P3 through P6 as well 

Syntax for use:

./convert -h // displays a help message like this one
./convert -i image.pbm // inverts image's colours
./convert -mh image.pbm // mirrors image horizontally
./convert -mv image.pbm // mirrors image vertically
./convert -mhv image.pbm // mirrors image horizontally and vertically

Possible file formats (v0.1):

.pbm (modes P1 and P2)

For more info, see: https://en.wikipedia.org/wiki/Netpbm#File_formats

*/

#include <iostream>

using namespace std;

void display_help(void);

int main(int argc, char *argv[]) {
    switch(argc) {
        case 1:
            cout << "No arguments supplied.\n";
            display_help();
            return 1;
            break;
        case 2:
            if (!strcmp(argv[1], "-h")) {
                display_help();
                return 0;
            } else {
                cout << "Parameter " << argv[1] << " not recognised.\n";
                display_help();
                return 1;
            }
            break;
        case 3:
            return 0; // stubbed out for now
            break;
        default:
            cout << "Too many arguments supplied.\n";
            display_help();
            return 1;
            break;
    }
}

void display_help(void) {
    cout << "Typical syntax is: \n";
    cout << "\t./convert // displays a help message like this one\n";
    cout << "\t./convert -h // also displays a help message like this one\n";
    cout << "\t./convert -i image.pbm // inverts image's colours\n";
    cout << "\t./convert -mh image.pbm // mirrors image horizontally\n";
    cout << "\t./convert -mv image.pbm // mirrors image vertically\n";
    cout << "\t./convert -mhv image.pbm // mirrors image horizontally and vertically\n";
}
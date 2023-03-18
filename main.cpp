#include <iostream>
#include <string>
#include <optional>
#include <getopt.h>
#include <chrono>
#include "image_adjustment.h"
#include "bitmap.h"

using namespace std;
using namespace std::chrono;

void usage(const char* appname);
bool isValid(const char* name, float value, float min, float max);

const int HUE_MIN = 0;
const int HUE_MAX = 360;

const int SAT_MIN = -1;
const int SAT_MAX = 1;

const int BRI_MIN = -1;
const int BRI_MAX = 1;

const int CON_MIN = -1;
const int CON_MAX = 1;

int main(int argc, char* argv[]) {
    // Default values for image adjustments
    optional<float> hue {};
    optional<float> saturation {};
    optional<float> brightness {};
    optional<float> contrast {};

    // Define the command line options
    static struct option long_options[] = {
        {"hue", required_argument, 0, 'h'},
        {"saturation", required_argument, 0, 's'},
        {"brightness", required_argument, 0, 'b'},
        {"contrast", required_argument, 0, 'c'},
        {"help", required_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;
    string input_file;
    string output_file;

    // Parse the command line options
    while ((c = getopt_long(argc, argv, "h:s:b:c:?", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                hue = stof(optarg);
                break;
            case 's':
                saturation = stof(optarg);
                break;
            case 'b':
                brightness = stof(optarg);
                break;
            case 'c':
                contrast = stof(optarg);
                break;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    // Verify that the user provided the input and output file paths
    if (optind + 1 >= argc) {
        usage(argv[0]);
        return 1;
    }

    input_file = argv[optind];
    output_file = argv[optind+1];

    // Load the input bitmap file
    Bitmap* bitmap = new Bitmap();
    bool res = bitmap->Load(input_file);

    if (!res) {
        cout << "Failed to load the input bitmap." << endl;
        return 1;
    }

    cout << "read bitmap: " << bitmap->Width() << "x" << dec << bitmap->Height() 
        << ", bitcount=" << bitmap->BitCount() 
        << ", size:" << bitmap->SizeImage() 
        << endl;

    // Extract bitmap data
    unsigned char* data = bitmap->Data();
    int width = bitmap->Width();
    int height = bitmap->Height();

    auto start = high_resolution_clock::now();
    // Adjust the image attributes
    
    if(hue)
    {
        if(!isValid("hue", hue.value(), HUE_MIN, HUE_MAX)) return 1;
        cout << "adjusting hue to " << hue.value() << endl;
        adjust_hue(data, width, height, hue.value());
    }
    if(saturation)
    {
        if(!isValid("saturation", saturation.value(), SAT_MIN, SAT_MAX)) return 1;
        cout << "adjusting saturation" << endl;
        adjust_saturation(data, width, height, saturation.value());
    }
    if(brightness)
    {
        if(!isValid("brightness", brightness.value(), BRI_MIN, BRI_MAX)) return 1;
        cout << "adjusting brightness" << endl;
        adjust_brightness(data, width, height, brightness.value());
    }
    if(contrast)
    {
        if(!isValid("contrast", contrast.value(), CON_MIN, CON_MAX)) return 1;
        cout << "adjusting contrast" << endl;
        adjust_contrast(data, width, height, contrast.value());
    }

    auto stop = high_resolution_clock::now();  // Stop measuring execution time
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Execution time: " << duration.count() << " ms" << endl;

    cout << "Saving to file " << output_file << endl;
    // Save the adjusted bitmap to the output file
    bitmap->Save(output_file);

    // Free the bitmap memory
    delete bitmap;

    return 0;
}

void usage(const char* appname)
{
    cout << "Usage: " << appname << " -h [hue] -s [saturation] -b [brightness] -c [contrast] [input file] [output file]" << endl << endl;
    cout << "-h, --hue: [0, 360]" << endl;
    cout << "-s, --saturation: [-1, 1]" << endl;
    cout << "-b, --brightness: [-1, 1]" << endl;
    cout << "-c, --contrast: [-1, 1]" << endl;
    cout << endl << "-?, --help: This message" << endl;

}

bool isValid(const char* name, float value, float min, float max)
{
    bool res = value >= min && value <= max;

    if(!res) 
    {
        cerr << "Invalid " << name  << "(" << value << ")"
        << ", min = " << min 
        << ", max = " << max
        << endl;
    }

    return res;
}
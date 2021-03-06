#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstring>

#include "polynomial.hpp"
#include "generator.hpp"
#include "utilsMethod.hpp"
#include "SafeInput.hpp"
#include "pSettings.hpp"
#include "menu.hpp"
#include "interactive.hpp"
#include "performance.hpp"

void printMainMenu();
void interactiveMode();
void generateMode();
bool argumentMode(int64_t, char *[]);

int main(int argc, char *argv[])
{
    std::srand(std::time(0));
    int choice;

    if (argc > 1)
    {
        return argumentMode(argc, argv);
    }

    while (true)
    {
        menu::mainMenu();
        choice = tsi::getInt("Choice: ");
        switch (choice)
        {
        case 1:
            interactiveMode();
            break;
        case 2:
            generateMode();
            break;
        case 3:
            pSettings::page();
            break;
        case 0:
            std::cout << "GOODBYE JOJO!!!" << std::endl;
            return 0;
        default:
            std::cout << "Unknown Choice!" << std::endl;
        }
    }

    return 0;
}

void interactiveMode()
{
    menu::interactive();

    int choice = tsi::getInt("Your Choice: ");
    switch (choice)
    {
    case 1:
        testMode();
        break;
    case 2:
        endlessMode();
        break;
    case 3:
        timelimitMode();
        break;
    case 0:
        return;
    default:
        std::cout << "Unknown Choice!" << std::endl;
    }
}

void generateMode()
{
    std::string ofile_location = "./generated/";
    std::string keyf_location = ofile_location;
    std::string ofile_name = tsi::getString("Enter Output File Name: ");
    ofile_location += ofile_name;
    ofile_location += ".txt";

    std::string keyf_name = tsi::getString("Enter Key File Name: ");
    keyf_location += keyf_name;
    keyf_location += ".txt";

    if (ofile_name == keyf_name)
    {
        std::cout << "ERROR: Two Files can't be the same!" << std::endl;
        return;
    }

    if (!(ofile_name.size() && keyf_name.size()))
    {
        std::cout << "ERROR: File name can't be blank!" << std::endl;
        return;
    }

    int64_t difficulity = tsi::getInt("Degree of Polynomial: ");
    if (difficulity < 2)
    {
        std::cout << "WARNING: Polynomial degree must be at least 2!" << std::endl;
        return;
    }

    int64_t T = tsi::getInt("Number of polynomial: ");

    std::ofstream OutFile(ofile_location);
    std::ofstream KeyFile(keyf_location);
    for (int64_t i = 1; i <= T; i++)
    {
        Polynomial res = PolyGenerator::random(difficulity);
        OutFile << i << ") Solve " << res.printPoly(true) << " = 0" << std::endl;
        KeyFile << "Key of #" << i << " : " << rootsToStr(PolyGenerator::getCurrRoots()) << std::endl;
    }

    std::cout << "Successfully generated both files" << std::endl;
    OutFile.close();
    KeyFile.close();
}

bool argumentMode(int64_t argc, char *argv[])
{
    // * args : ./main [NUM_RANGE] [DENOM_RANGE] [DEGREE] [COUNT] [FNAMEPROB] [FNAMEKEY/__JSON_MODE__]
    if (argc != 7)
    {
        std::cout << "Syntax: ./main [NUM_RANGE] [DENOM_RANGE] [DEGREE] "
                  << "[COUNT] [FNAMEPROB] [FNAMEKEY/__JSON_MODE__]" << std::endl;
        return EXIT_FAILURE;
    }

    std::stringstream nr(argv[1]);
    std::stringstream dr(argv[2]);
    std::stringstream degree(argv[3]);
    std::stringstream count(argv[4]);

    int64_t nr_int = 0;
    nr >> nr_int;
    pSettings::setNumRange(nr_int);

    int64_t dr_int = 0;
    dr >> dr_int;
    pSettings::setDenomRange(dr_int);

    int64_t degree_int = 0;
    degree >> degree_int;

    int64_t count_int = 0;
    count >> count_int;

    if (strcmp(argv[6], "__JSON_MODE__"))
    {
        std::string outfileloc = "./generated/";
        outfileloc += argv[5];
        outfileloc += ".txt";

        std::string keyfileloc = "./generated/";
        keyfileloc += argv[6];
        keyfileloc += ".txt";

        std::ofstream outfile(outfileloc), keyfile(keyfileloc);

        for (int64_t i = 1; i <= count_int; i++)
        {
            Polynomial res = PolyGenerator::random(degree_int);
            outfile << i << ") Solve " << res.printPoly(true) << " = 0" << std::endl;
            keyfile << rootsToStr(PolyGenerator::getCurrRoots()) << std::endl;
        }

        std::cout << "Successfully generated both files" << std::endl;
        outfile.close();
        keyfile.close();
    }
    else
    {
        std::cout << "{\n";
        std::cout << "\"difficulty\": " << pp::diffFactor(degree_int)
                  << "," << std::endl;
        std::cout << "\"questions\": {" << std::endl;

        for (int64_t i = 1; i <= count_int; i++)
        {
            Polynomial res = PolyGenerator::random(degree_int);
            std::cout << "\"" << res.printPoly(true) << "\": " << std::endl;
            std::cout << "[" << rootsToStr(PolyGenerator::getCurrRoots(), true)
                      << ((i == count_int)
                              ? "]"
                              : "],")
                      << std::endl;
        }

        std::cout << "}\n}" << std::endl;
    }

    return EXIT_SUCCESS;
}
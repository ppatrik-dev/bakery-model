/**
 * @file model.cpp
 * @author Patrik Procházka (xprochp00)
 * @brief Model source file
 */

#include "model.hpp"
#include "parameters.hpp"
#include "resources.hpp"
#include "statistics.hpp"

Parameters *Param = nullptr;
Resources *Res = nullptr;
Statistics *Stats = nullptr;

int main(int argc, char **argv) {
    // Konfiguracny subor
    const char *filename = nullptr;

    if (argc == 2) filename = argv[1];

    // Parametre experimentu
    Param = new Parameters(filename);
    Param->print();

    // Zdroje a statistiky
    Res = new Resources(Param);
    Stats = new Statistics;

    // Simulacia
    Init(0, Param->simulation_duration());

    DoughGenerator generator;
    generator.Activate();

    Run();

    // Vypis vysledkov
    Res->print();
    Stats->print();

    // Uvolnenie zdrojov
    delete Param;
    delete Res;
    delete Stats;

    return 0;
}
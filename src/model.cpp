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

    DoughGenerator *dough_generator = new DoughGenerator;
    dough_generator->Activate();

    MixerFailure *mixer_failure = nullptr;
    if (Param->mixer_failure()) {        
        mixer_failure = new MixerFailure;
        double t = Uniform(
            Param->new_dough_period()/3,
            2*Param->new_dough_period()/3
        );
        mixer_failure->Activate(Time + t);
    }

    LaminatorFailure *laminator_failure = nullptr;
    if (Param->laminator_failure()) {
        laminator_failure = new LaminatorFailure;
        double t = Uniform(
            Param->simulation_duration()/4,
            3*Param->simulation_duration()/4
        );
        laminator_failure->Activate(Time + t);
    }

    ShaperFailure *shaper_failure = nullptr;
    if (Param->shaper_failure()) {
        shaper_failure = new ShaperFailure;
        double t = Uniform(
            Param->simulation_duration()/3,
            2*Param->simulation_duration()/3
        );
        shaper_failure->Activate(Time + t);
    }

    Run();

    // Vypis vysledkov
    Res->print();
    Stats->print();

    // Uvolnenie zdrojov
    if (dough_generator) delete dough_generator;
    if (mixer_failure) delete mixer_failure;
    if (laminator_failure) delete laminator_failure;
    if (shaper_failure) delete shaper_failure;
    
    if (Param) delete Param;
    if (Res) delete Res;
    if (Stats) delete Stats;

    return 0;
}
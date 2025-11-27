/**
 * @file model.cpp
 * @author Patrik Procházka (xprochp00)
 * @brief Model source file
 */

#include "model.hpp"

Facility miesacka("miesacka");
Facility pekar1("pekar1");
Facility pekar2("pekar2");
Facility pekar3("pekar3");
Facility pekar4("pekar4");
Store chladnicka("chladnicka", 8);
Facility laminator("laminator");
Facility tvarovac("tvarovac");
Store plechy("plechy", 20);
Store kysiaren("kysiaren", 60);
Store pec("pec", 5);

Histogram doba_vyroby_hist("Doba vyroby croisantov", 200, 50, 10);
Stat pocet_kusov_stat("Pocet kusov na plechu");

int pocet_ocakavanych_kusov = 0;
int pocet_pripravenych_davok = 0;

int main(int agrc, char **argv) {

    Init(0, CAS_SIMULACIE);
    (new GeneratorDavok)->Activate();
    Run();

    pekar1.Output();
    miesacka.Output();

    pekar2.Output();
    laminator.Output();

    tvarovac.Output();
    
    chladnicka.Output();

    pekar3.Output();
    plechy.Output();
    kysiaren.Output();
    pec.Output();

    doba_vyroby_hist.Output();
    pocet_kusov_stat.Output();
    
    std::cout << "\nPripravenych davok: " << pocet_pripravenych_davok << std::endl;
    std::cout << "\nOcakavanych croaissantov: " << pocet_ocakavanych_kusov << std::endl;
    std::cout << "\nDokoncenych croaissantov: " << pocet_kusov_stat.Sum() << std::endl;
    
    return 0;
}
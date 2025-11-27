/**
 * @file model.hpp
 * @author Patrik Procházka (xprochp00)
 * @brief Model header file
 */

#include <simlib.h>
#include <iostream>

#define CAS_SIMULACIE       (8*60) // min
#define POCET_CROISSANTOV   300    // pcs
#define KAPACITA_PLECHU     50     // pcs

extern Facility miesacka;
extern Facility pekar1;
extern Facility pekar2;
extern Facility pekar3;
extern Store chladnicka;
extern Facility laminator;
extern Facility tvarovac;
extern Store plechy;
extern Store kysiaren;
extern Store pec;

extern Histogram doba_vyroby_hist;
extern Stat pocet_kusov_stat;

extern int pocet_ocakavanych_kusov;
extern int pocet_pripravenych_davok;

class Plech : public Process {
private:
    double zaciatok_vyroby;
    int pocet_kusov;

public:
    Plech(double cas, int ks): 
        zaciatok_vyroby(cas), pocet_kusov(ks) {}

private:
    void Behavior() {
        pocet_ocakavanych_kusov += pocet_kusov;

        // Zabratie plechu
        Enter(plechy, 1);

        // Presun plechu do kysiarne
        Enter(kysiaren, 1);
        Seize(pekar3, 0);
        Wait(Uniform(1.5, 3.0));
        Release(pekar3);

        // Kysnutie
        Wait(Triag(90, 60, 120));

        // Vybratie plechu z kysiarne
        Seize(pekar3, 2);
        Wait(Uniform(2.0, 4.0));
        Leave(kysiaren, 1);
        Release(pekar3);

        // Presun plechu do pece
        Enter(pec, 1);
        Seize(pekar3, 1);
        Wait(Uniform(4, 7));
        Release(pekar3);

        // Pecenie
        Wait(Triag(15, 12, 18));

        // Vybratie z pece
        Seize(pekar3, 3);
        Wait(Uniform(3, 6));
        Leave(pec, 1);
        Release(pekar3);

        // Chladenie
        Wait(Triag(15, 10, 20));

        // Vratenie plechu
        Leave(plechy, 1);

        // Pripravene na predaj

        doba_vyroby_hist(Time - zaciatok_vyroby);
        pocet_kusov_stat(pocet_kusov);
    }
};

class Cesto : public Process {
private:
    int pocet_kusov;

public:
    Cesto(): pocet_kusov(Triag(300, 295, 305)) {}

    void Behavior() {
        double zaciatok = Time;

        // Nova davka na miesanie
        Seize(miesacka);
        Seize(pekar1, 0);

        // Priprava na miesanie
        Wait(Triag(7, 5, 10));
        Release(pekar1);

        // Miesanie
        Wait(Triag(10, 8, 12));

        // Domiesane cesto
        Seize(pekar1, 1);

        // Vybratie a zabalenie cesta
        Wait(Triag(5, 3, 7));
        Release(pekar1);
        Release(miesacka);

        // Presun do chladnicky
        Enter(chladnicka, 1);
        Seize(pekar1, 3);
        Wait(Uniform(1, 2));
        Release(pekar1);

        // 1. oddych cesta
        Wait(Triag(30, 20, 40));

        // Vybratie z chladnicky
        Seize(pekar1, 2);
        Wait(Uniform(1, 2));
        Leave(chladnicka, 1);
        Release(pekar1);

        // Pripravene na laminaciu
        Seize(laminator);
        Seize(pekar2, 3);

        // Laminovanie
        Wait(Triag(20, 15, 25));
        Release(laminator);
        Release(pekar2);

        // Presun do chladnicky
        Enter(chladnicka, 1);
        Seize(pekar2, 1);
        Wait(Uniform(1, 2));
        Release(pekar2);

        // 2. oddych cesta
        Wait(Triag(30, 20, 40));

        // Vybratie z chladnicky
        Seize(pekar2, 0);
        Wait(Uniform(1, 2));
        Leave(chladnicka, 1);
        Release(pekar2);

        // Cesto pripravene na tvarovanie
        Seize(tvarovac);
        Seize(pekar2, 2);

        // Tvarovanie
        Wait(Triag(25, 20, 30));

        Release(tvarovac);
        Release(pekar2);
        int plech = 0;
        
        int pocet_plnych = pocet_kusov / KAPACITA_PLECHU;   // počet úplne plných plechov
        int zostavajuci = pocet_kusov % KAPACITA_PLECHU;    // počet kusov na posledný plech

        // ak existuje neúplný plech, rozdelíme zostávajúce kusy na všetky plné plechy
        if (zostavajuci > 0) {
            // počet kusov, ktoré treba pridať na posledný plech
            int celkoveNaNovy = zostavajuci + pocet_plnych; // pridáme 1 kus z každého plného plechu
            int odoberNaPlne = 1;  // koľko kusov odoberieme z každého plného plechu (môže sa meniť)

            // generovanie plných plechov znížených o odober
            for (int i = 0; i < pocet_plnych; ++i) {
                (new Plech(zaciatok, KAPACITA_PLECHU - odoberNaPlne))->Activate();
                plech++;
            }

            // posledný plech
            (new Plech(zaciatok, celkoveNaNovy))->Activate();
            plech++;
        } else {
            // všetky plechy sú úplné
            for (int i = 0; i < pocet_plnych; ++i) {
                (new Plech(zaciatok, KAPACITA_PLECHU))->Activate();
                plech++;
            }
        }
    }
};

class GeneratorDavok : public Event {
    void Behavior() {
        if (Time < CAS_SIMULACIE - 6*60) {
            (new Cesto)->Activate();
            pocet_pripravenych_davok++;
            Activate(Time + Normal(30, 5));
        }
    }
};
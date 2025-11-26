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
extern Facility pekar4;
extern Store chladnicka;
extern Facility laminator;
extern Facility tvarovac;
extern Store plechy;
extern Store kysiaren;
extern Store pec;

extern Histogram doba_vyroby;

class Plech : public Process {
    double zaciatok_vyroby;

public:
    Plech(double cas): zaciatok_vyroby(cas) {}

private:
    void Behavior() {
        // Zabratie plechu
        Enter(plechy, 1);

        // Presun plechu do kysiarne
        Enter(kysiaren, 1);
        Seize(pekar4);
        Wait(Uniform(1.5, 3.0));
        Release(pekar4);

        // Kysnutie
        Wait(Triag(90, 60, 120));

        // Vybratie plechu z kysiarne
        Seize(pekar4);
        Wait(Uniform(2.0, 4.0));
        Leave(kysiaren, 1);
        Release(pekar4);

        // Presun plechu do pece
        Enter(pec, 1);
        Seize(pekar4);
        Wait(Uniform(4, 7));
        Release(pekar4);

        // Pecenie
        Wait(Triag(15, 12, 18));

        // Vybratie z pece
        Seize(pekar4);
        Wait(Uniform(3, 6));
        Leave(pec, 1);
        Release(pekar4);

        // Chladenie
        Wait(Triag(15, 10, 20));

        // Vratenie plechu
        Leave(plechy, 1);

        // Pripravene na predaj
        doba_vyroby(Time - zaciatok_vyroby);
    }
};

class Cesto : public Process {
    void Behavior() {
        double zaciatok = Time;

        // Nova davka na miesanie
        Seize(miesacka);
        Seize(pekar1, 1);

        // Priprava na miesanie
        Wait(Triag(7, 5, 10));
        Release(pekar1);

        // Miesanie
        Wait(Triag(10, 8, 12));

        // Domiesane cesto
        Seize(pekar1, 2);

        // Vybratie a zabalenie cesta
        Wait(Triag(5, 3, 7));
        Release(pekar1);
        Release(miesacka);

        // Cesto pripravene do chladnicky
        Enter(chladnicka, 1);

        // 1. oddych cesta
        Wait(Triag(30, 20, 40));
        Leave(chladnicka, 1);

        // Pripravene na laminaciu
        Seize(laminator);
        Seize(pekar2);

        // Laminovanie
        Wait(Triag(20, 15, 25));
        Release(pekar2);
        Release(laminator);

        // Cesto pripravene do chladnicky
        Enter(chladnicka, 1);

        // 2. oddych cesta
        Wait(Triag(30, 20, 40));
        Leave(chladnicka, 1);

        // Cesto pripravene na tvarovanie
        Seize(tvarovac);
        Seize(pekar3);

        // Tvarovanie
        Wait(Triag(12, 10, 15));

        Release(pekar3);
        Release(tvarovac);

        // Naplnenie plechov
        int pocet_plechov = POCET_CROISSANTOV / KAPACITA_PLECHU;

        for (int i = 0; i < pocet_plechov; ++i)
            (new Plech(zaciatok))->Activate();
    }
};

class GeneratorDavok : public Event {
    void Behavior() {
        if (Time < CAS_SIMULACIE - 6*60) {
            (new Cesto)->Activate();
            Activate(Time + Normal(30, 5));
        }
    }
};
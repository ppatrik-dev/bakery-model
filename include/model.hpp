/**
 * @file model.hpp
 * @author Patrik Procházka (xprochp00)
 * @brief Model header file
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include <simlib.h>
#include <iostream>
#include "resources.hpp"
#include "parameters.hpp"
#include "statistics.hpp"

extern Resources *Res;
extern Parameters *Param;
extern Statistics *Stats;

class Tray : public Process {
private:
    double m_start_time;
    int m_pieces_count;

public:
    Tray(double time, int pieces): 
        m_start_time(time), m_pieces_count(pieces) {}

private:
    void Behavior() {
        Stats->m_trays_total_count += 1;
        Stats->m_pieces_total_count += m_pieces_count;

        Stats->m_tray_pieces_count_stat(m_pieces_count);

        // Zabratie plechu
        auto &tray = Res->tray();
        Enter(tray, 1);

        // Presun plechu do kysiarne
        auto &proofer = Res->proofer();
        Enter(proofer, 1);

        auto &proofer_baker1 = Res->proofer_oven_baker();
        Seize(proofer_baker1, 0);

        Wait(Uniform(1.5, 3.0));
        Release(proofer_baker1);

        // Kysnutie
        Wait(Triag(90, 60, 120));

        // Vybratie plechu z kysiarne
        auto &proofer_baker2 = Res->proofer_oven_baker();
        Seize(proofer_baker2, 2);

        Wait(Uniform(1.5, 3.0));
        Leave(proofer, 1);
        Release(proofer_baker2);

        // Presun plechu do pece
        auto &oven = Res->oven();
        Enter(oven, 1);

        auto &oven_baker1 = Res->proofer_oven_baker();
        Seize(oven_baker1, 1);

        Wait(Uniform(1.0, 2.0));
        Release(oven_baker1);

        // Pecenie
        Wait(Triag(15, 12, 18));

        // Vybratie z pece
        auto &oven_baker2 = Res->proofer_oven_baker();

        Seize(oven_baker2, 3);
        Wait(Uniform(1.0, 2.0));
        Leave(oven, 1);
        Release(oven_baker2);

        // Chladenie
        Wait(Triag(15, 10, 20));

        // Vratenie plechu
        Leave(tray, 1);

        // Pripravene na predaj
        
        Stats->m_trays_finished_count += 1;
        Stats->m_pieces_finished_count += m_pieces_count;

        Stats->m_production_duration_hist(Time - m_start_time);
    }
};

class Dough : public Process {
private:
    double m_start_time;
    int m_pieces_count;

public:
    bool m_interrupted;

public:
    Dough(): m_interrupted(false), 
        m_pieces_count(Normal(Param->one_dough_capacity(), 2)) {}

    void Behavior() {
        Stats->m_doughs_total_count += 1;
        Res->m_dough_mixing_active_count += 1;

        m_start_time = Time;

        // Nova davka na miesanie
        auto &mixer = Res->mixer();
        Seize(mixer, 0);

        auto &mixer_baker1 = Res->mixer_baker();
        Seize(mixer_baker1, 0);

        // Priprava na miesanie
        Wait(Triag(7, 5, 10));
        Release(mixer_baker1);

        if (m_interrupted) {
            Release(mixer);
            Res->m_dough_mixing_active_count -= 1;
            Terminate();
        }

        // Miesanie
        Wait(Triag(12.5, 10, 15));
        
        if (m_interrupted) {
            Release(mixer);
            Res->m_dough_mixing_active_count -= 1;
            Terminate();
        }

        // Domiesane cesto
        auto &mixer_baker2 = Res->mixer_baker();
        Seize(mixer_baker2, 1);

        // Vybratie a zabalenie cesta
        Wait(Triag(5, 3, 7));
        Release(mixer_baker2);
        Release(mixer);

        // Presun do chladnicky
        auto &fridge = Res->fridge();
        Enter(fridge, 1);

        auto &mixer_baker3 = Res->mixer_baker();
        Seize(mixer_baker3, 2);

        Wait(Uniform(1, 2));
        Release(mixer_baker3);

        Res->m_dough_mixing_active_count -= 1;

        // 1. oddych cesta
        Wait(Triag(30, 20, 40));

        // Pripravene na laminaciu
        // (kym nie je volny laminovaci stroj zostava v chladnicke)
        auto &laminator = Res->laminator(); 
        Seize(laminator);

        auto &laminator_baker1 = Res->laminator_shaper_baker();
        Seize(laminator_baker1, 1);

        // Presun z chladnicky
        Wait(Uniform(1, 2));
        Leave(fridge, 1);
        
        // Laminovanie
        Wait(Triag(20, 15, 25));
        Release(laminator);
        Release(laminator_baker1);

        if (m_interrupted) Terminate();

        // Presun do chladnicky
        Enter(fridge, 1);

        auto &laminator_baker2 = Res->laminator_shaper_baker();
        Seize(laminator_baker2, 2);

        Wait(Uniform(1, 2));
        Release(laminator_baker2);

        // 2. oddych cesta
        Wait(Triag(30, 20, 40));

        // Cesto pripravene na tvarovanie
        // (kym nie je volny tvarovaci stroj zostava v chladnicke)
        auto &shaper = Res->shaper(); 
        Seize(shaper);

        auto &shaper_baker1 = Res->laminator_shaper_baker();
        Seize(shaper_baker1, 0);
        
        // Presun z chladnicky
        Wait(Uniform(1, 2));
        Leave(fridge, 1);
        
        // Tvarovanie
        Wait(Triag(25, 20, 30));
        Release(shaper);
        Release(shaper_baker1);

        if (m_interrupted) Terminate();

        Stats->m_doughs_finished_count += 1;

        create_trays_from_dough_effective();
    }

private:
    void create_trays_from_dough_effective() const {
        int tray_capacity = Param->tray_capacity();
        int full_tray_cnt = m_pieces_count / tray_capacity;
        int remaining_pcs = m_pieces_count % tray_capacity;

        // Ziadny zvysok kusov
        if (remaining_pcs == 0) {
            for (int i = 0; i < full_tray_cnt; ++i) {
                (new Tray(m_start_time, tray_capacity))->Activate();
            }
            return;
        }

        //  Ak pocet zvysnych kusov je mensi ako 7 rozdelime ich medzi plne plechy
        if (remaining_pcs > 0 && remaining_pcs < 7 && full_tray_cnt > 0) {

            // Kazdy plech dostane o jeden kus viac, kym mame zvysne kusy
            for (int i = 0; i < full_tray_cnt; ++i) {
                int tray_load = tray_capacity;

                if (remaining_pcs > 0) {
                    tray_load += 1;
                    remaining_pcs--;
                }

                (new Tray(m_start_time, tray_load))->Activate();
            }
        }

        // Pocet zvysnych kusov viac ako 7
        else {
            for (int i = 0; i < full_tray_cnt; ++i) {
                (new Tray(m_start_time, tray_capacity))->Activate();
            }

            // posledný plech so zvysnymi kusmi
            (new Tray(m_start_time, remaining_pcs))->Activate();
        }
    }
};

class DoughGenerator : public Event {
    void Behavior() {
        if (Time < Param->new_dough_period()) {
            (new Dough)->Activate();

            Activate(Time + Normal(Param->new_dough_interval(), 2));
        }
        else {
            // Kontrola ci este prebieha miesanie
            if (!Res->dough_mixing_all_finished()) {
                Activate(Time + 10);
            }
            // presun pekarov na miesanie, na laminaciu a tvarovanie
            else {
                Res->move_mixer_bakers_to_tray_bakers();
            }
        }
    }
};

class MixerRepair : public Process {
private:
    int m_index;

public:
    MixerRepair(int index): m_index(index) {}

    void Behavior() {
        auto &mixer = Res->mixer_by_index(m_index);

        if (mixer.Busy()) {
            Dough *dough = reinterpret_cast<Dough*>(mixer.In());

            dough->m_interrupted = true;
        }

        Seize(mixer, 255);
        Wait(Normal(60, 12));
        Release(mixer);
    }
};

class MixerFailure : public Event {
public:
    void Behavior() {
        int index = (int)(Random() * Param->mixers_count());
        (new MixerRepair(index))->Activate();
    }
};

class LaminatorRepair : public Process {
private:
    int m_index;

public:
    LaminatorRepair(int index): m_index(index) {}

    void Behavior() {
        auto &laminator = Res->laminator_by_index(m_index);

        if (laminator.Busy()) {
            Dough *dough = reinterpret_cast<Dough*>(laminator.In());

            dough->m_interrupted = true;
        }

        Seize(laminator, 255);
        Wait(Normal(45, 9));
        Release(laminator);
    }
};

class LaminatorFailure : public Event {
public:
    void Behavior() {
        int index = (int)(Random() * Param->laminators_count());
        (new LaminatorRepair(index))->Activate();
    }
};

class ShaperRepair : public Process {
private:
    int m_index;

public:
    ShaperRepair(int index): m_index(index) {}

    void Behavior() {
        auto &shaper = Res->shaper_by_index(m_index);

        if (shaper.Busy()) {
            Dough *dough = reinterpret_cast<Dough*>(shaper.In());

            dough->m_interrupted = true;
        }

        Seize(shaper, 255);
        Wait(Normal(25, 5));
        Release(shaper);
    }
};

class ShaperFailure : public Event {
public:
    void Behavior() {
        int index = (int)(Random() * Param->shapers_count());
        (new ShaperRepair(index))->Activate();
    }
};

#endif
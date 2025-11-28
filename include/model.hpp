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

        auto &tray_baker1 = Res->tray_baker();
        Seize(tray_baker1, 0);

        Wait(Uniform(1.5, 3.0));
        Release(tray_baker1);

        // Kysnutie
        Wait(Triag(90, 60, 120));

        // Vybratie plechu z kysiarne
        auto &tray_baker2 = Res->tray_baker();
        Seize(tray_baker2, 2);

        Wait(Uniform(1.5, 3.0));
        Leave(proofer, 1);
        Release(tray_baker2);

        // Presun plechu do pece
        auto &oven = Res->oven();
        Enter(oven, 1);

        auto &tray_baker3 = Res->tray_baker();
        Seize(tray_baker3, 1);

        Wait(Uniform(1.0, 2.0));
        Release(tray_baker3);

        // Pecenie
        Wait(Triag(15, 12, 18));

        // Vybratie z pece
        auto &tray_baker4 = Res->tray_baker();

        Seize(tray_baker4, 3);
        Wait(Uniform(1.0, 2.0));
        Leave(oven, 1);
        Release(tray_baker4);

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
    int m_pieces_count;

public:
    Dough(): m_pieces_count(Triag(300, 295, 305)) {}

    void Behavior() {
        double m_start_time = Time;

        // Nova davka na miesanie
        auto &mixer = Res->mixer();
        Seize(mixer);

        auto &mixer_baker1 = Res->mixer_baker();
        Seize(mixer_baker1, 0);

        // Priprava na miesanie
        Wait(Triag(7, 5, 10));
        Release(mixer_baker1);

        // Miesanie
        Wait(Triag(10, 8, 12));

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
        Seize(mixer_baker3, 3);

        Wait(Uniform(1, 2));
        Release(mixer_baker3);

        // 1. oddych cesta
        Wait(Triag(30, 20, 40));

        // Vybratie z chladnicky
        auto &mixer_baker4 = Res->mixer_baker();
        Seize(mixer_baker4, 2);

        Wait(Uniform(1, 2));
        Leave(fridge, 1);
        Release(mixer_baker4);

        // Laminovanie
        auto &laminator = Res->laminator();
        Seize(laminator);

        auto &laminator_baker1 = Res->laminator_shaper_baker();
        Seize(laminator_baker1, 3);

        Wait(Triag(20, 15, 25));
        Release(laminator);
        Release(laminator_baker1);

        // Presun do chladnicky
        Enter(fridge, 1);

        auto &laminator_baker2 = Res->laminator_shaper_baker();
        Seize(laminator_baker2, 1);

        Wait(Uniform(1, 2));
        Release(laminator_baker2);

        // 2. oddych cesta
        Wait(Triag(30, 20, 40));

        // Vybratie z chladnicky
        auto &shaper_baker1 = Res->laminator_shaper_baker();

        Seize(shaper_baker1, 0);
        Wait(Uniform(1, 2));
        Leave(fridge, 1);
        Release(shaper_baker1);

        // Cesto pripravene na tvarovanie
        auto &shaper = Res->shaper();
        Seize(shaper);

        auto &shaper_baker2 = Res->laminator_shaper_baker();
        Seize(shaper_baker2, 2);

        // Tvarovanie
        Wait(Triag(25, 20, 30));

        Release(shaper);
        Release(shaper_baker2);
        
        int tray_capacity = Param->tray_capacity();
        int full_tray_cnt = m_pieces_count / tray_capacity;
        int remaining_pcs = m_pieces_count % tray_capacity;

        if (remaining_pcs > 0) {
            int last_tray_pieces_count = remaining_pcs + full_tray_cnt;
            int take_from_full_tray_cnt = 1;

            for (int i = 0; i < full_tray_cnt; ++i) {
                (new Tray(m_start_time, tray_capacity - take_from_full_tray_cnt))->Activate();
            }

            (new Tray(m_start_time, last_tray_pieces_count))->Activate();
        } else {
            for (int i = 0; i < full_tray_cnt; ++i) {
                (new Tray(m_start_time, tray_capacity))->Activate();
            }
        }
    }
};

class DoughGenerator : public Event {
    void Behavior() {
        if (Time < Param->simulation_duration() - 7*60) {
            (new Dough)->Activate();
            Stats->m_generated_doughs_count++;

            Activate(Time + Normal(Param->new_dough_interval(), 5));
        }
    }
};

#endif
/**
 * @file resources.hpp
 * @author Patrik Procházka (xprochp00)
 * @brief Resources header file
 */

#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include "simlib.h"
#include "parameters.hpp"

class Resources {
private:
    std::vector<std::unique_ptr<Facility>> m_mixer_bakers;              // pekari na miesanie
    std::vector<std::unique_ptr<Facility>> m_mixers;                    // miesacie stroje

    std::vector<std::unique_ptr<Facility>> m_laminator_shaper_bakers;   // pekari na miesanie
    std::vector<std::unique_ptr<Facility>> m_laminators;                // limanatovacie stroje
    std::vector<std::unique_ptr<Facility>> m_shapers;                   // tvarovacie stroje

    std::vector<std::unique_ptr<Facility>> m_proofer_oven_bakers;       // pekari na kysnutie, pecenie
    std::unique_ptr<Store> m_tray;                                      // plechy
    std::unique_ptr<Store> m_proofer;                                   // kysiaren
    std::unique_ptr<Store> m_oven;                                      // pec

    std::unique_ptr<Store> m_fridge;                                    // chladnicka

public:
    unsigned long m_dough_mixing_active_count;

public:
    Resources(const Parameters *param);

public:
    bool dough_mixing_all_finished() const;
    void move_mixer_bakers_to_tray_bakers();

    Facility& get_facility(const std::vector<std::unique_ptr<Facility>> &facilities) const {
        auto ptr = facilities.front().get();

        for (auto &fac : facilities) {
            if (!fac->Busy())
                return *(fac.get());
            
            if (fac->QueueLen() < ptr->QueueLen())
                ptr = fac.get();
        }

        return *(ptr);
    }

    Facility& mixer_baker() const {
        return get_facility(m_mixer_bakers);
    }

    Facility& laminator_shaper_baker() const {
        return get_facility(m_laminator_shaper_bakers);
    }

    Facility& proofer_oven_baker() const {
        return get_facility(m_proofer_oven_bakers);
    }

    Facility& mixer() const {
        return get_facility(m_mixers);
    }

    Facility& mixer_by_index(int index) const {
        return *(m_mixers.at(index));
    }

    Facility& laminator() const {
        return get_facility(m_laminators);
    }

    Facility& laminator_by_index(int index) const {
        return *(m_mixers.at(index));
    }

    Facility& shaper() const {
        return get_facility(m_shapers);
    }

    Facility& shaper_by_index(int index) const {
        return *(m_shapers.at(index));
    }

    Store& fridge() const {
        return *(m_fridge.get());
    }

    Store& tray() const {
        return *(m_tray.get());
    }

    Store& proofer() const {
        return *(m_proofer.get());
    }

    Store& oven() const {
        return *(m_oven.get());
    }

    void print() const;
};

#endif
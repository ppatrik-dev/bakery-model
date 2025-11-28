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

    std::vector<std::unique_ptr<Facility>> m_tray_bakers;               // pekari na kysnutie, pecenie
    std::unique_ptr<Store> m_tray;                                      // plechy
    std::unique_ptr<Store> m_proofer;                                   // kysiaren
    std::unique_ptr<Store> m_oven;                                      // pec

    std::unique_ptr<Store> m_fridge;                                    // chladnicka

public:
    Resources(const Parameters *param);

public:
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

    Facility& tray_baker() const {
        return get_facility(m_tray_bakers);
    }

    Facility& mixer() const {
        return get_facility(m_mixers);
    }

    Facility& laminator() const {
        return get_facility(m_laminators);
    }

    Facility& shaper() const {
        return get_facility(m_shapers);
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
/**
 * @file resources.cpp
 * @author Patrik Procházka (xprochp00)
 * @brief Resources source file
 */

#include "resources.hpp"
#include <iostream>

Resources::Resources(const Parameters *param): 
        m_dough_mixing_active_count(0) {

    // Pekari na miesanie
    int mixer_bakers_cnt = param->mixer_bakers_count();
    m_mixer_bakers.reserve(mixer_bakers_cnt);

    for (int i = 0; i < mixer_bakers_cnt; ++i) {
        m_mixer_bakers.emplace_back(std::make_unique<Facility>(
            ("pekar_na_miesanie_a_kysnutie_pecenie" + std::to_string(i+1)).c_str()
        ));
    }

    // Miesacie stroje
    int mixers_cnt = param->mixers_count();
    m_mixers.reserve(mixers_cnt);

    for (int i = 0; i < mixers_cnt; ++i) {
        m_mixers.emplace_back(std::make_unique<Facility>(
            ("miesaci_stroj_" + std::to_string(i+1)).c_str()
        ));
    }

    // Pekari na laminovanie a tvarovanie
    int laminator_shaper_bakers_count = param->laminator_shaper_bakers_count();
    m_laminator_shaper_bakers.reserve(laminator_shaper_bakers_count);

    for (int i = 0; i < laminator_shaper_bakers_count; ++i) {
        m_laminator_shaper_bakers.emplace_back(std::make_unique<Facility>(
            ("pekar_na_laminovanie_a_tvarovanie_" + std::to_string(i+1)).c_str()
        ));
    }

    // Laminovacie stroje
    int laminators_cnt = param->laminators_count();
    m_laminators.reserve(laminators_cnt);

    for (int i = 0; i < laminators_cnt; ++i) {
        m_laminators.emplace_back(std::make_unique<Facility>(
            ("laminovaci_stroj_" + std::to_string(i+1)).c_str()
        ));
    }

    // Tvarovacie stroje
    int shapers_cnt = param->shapers_count();
    m_shapers.reserve(shapers_cnt);

    for (int i = 0; i < shapers_cnt; ++i) {
        m_shapers.emplace_back(std::make_unique<Facility>(
            ("tvarovaci_stroj_" + std::to_string(i+1)).c_str()
        ));
    }

    // Ppekari na kysnutie, pecenie
    int tray_bakers_count = param->tray_bakers_count();
    m_proofer_oven_bakers.reserve(tray_bakers_count);

    for (int i = 0; i < tray_bakers_count; ++i) {
        m_proofer_oven_bakers.emplace_back(std::make_unique<Facility>(
            ("pekar_na_kysnutie_a_pecenie_" + std::to_string(i+1)).c_str()
        ));
    }
    
    // Plechy
    m_tray = std::make_unique<Store>("plechy", param->tray_count());
    
    // Kysiaren
    m_proofer = std::make_unique<Store>("kysiaren", param->proofer_capacity());
    
    // Pec
    m_oven = std::make_unique<Store>("pec", param->oven_capacity());

    // Chladnicka
    m_fridge = std::make_unique<Store>("chladnicka", param->fridge_capacity());
}

bool Resources::dough_mixing_all_finished() const {
    return (m_dough_mixing_active_count == 0);
}

void Resources::move_mixer_bakers_to_tray_bakers() {
    if (!dough_mixing_all_finished() || m_mixer_bakers.empty()) 
        return;

    m_proofer_oven_bakers.reserve(
        m_proofer_oven_bakers.size() + m_mixer_bakers.size()
    );

    for (auto &baker : m_mixer_bakers) {
        m_proofer_oven_bakers.push_back(std::move(baker));
    }

    m_mixer_bakers.clear();
}

void Resources::print() const {
    for (auto &baker : m_mixer_bakers)
        baker->Output();

    for (auto &mixer : m_mixers)
        mixer->Output();

    for (auto &baker : m_laminator_shaper_bakers)
        baker->Output();
    
    for (auto &laminator : m_laminators)
        laminator->Output();

    for (auto &shaper : m_shapers)
        shaper->Output();
    
    for (auto &baker : m_proofer_oven_bakers)
        baker->Output();

    m_tray->Output();
    m_proofer->Output();
    m_oven->Output();

    m_fridge->Output();
}
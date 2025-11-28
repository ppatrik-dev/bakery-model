/**
 * @file parameters.hpp
 * @author Patrik Procházka (xprochp00)
 * @brief Parameters header file
 */

#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <unordered_map>
#include <sstream>

#define SIMULATION_DURATION 480
#define NEW_DOUGH_INTERVAL  30

#define MIXER_BAKERS_COUNT              1
#define LAMINATOR_SHAPER_BAKERS_COUNT   1
#define TRAY_BAKERS_COUNT               1

#define MIXERS_COUNT        1
#define LAMINATORS_COUNT    1
#define SHAPERS_COUNT       1
#define FRIDGE_CAPACITY     8
#define TRAYS_COUNT        20
#define TRAY_CAPACITY      50
#define PROOFER_CAPACITY   60
#define OVEN_CAPACITY       5

class Parameters {
private:
    std::string m_filename;
    std::unordered_map<std::string, double> m_parameters{
        {"TRVANIE_SIMULACIE", SIMULATION_DURATION},
        {"INTERVAL_NOVEHO_CESTA", NEW_DOUGH_INTERVAL},
        {"POCET_PEKAROV_NA_MIESANIE", MIXER_BAKERS_COUNT},
        {"POCET_PEKAROV_NA_LAMINACIU_A_TVAROVANIE", LAMINATOR_SHAPER_BAKERS_COUNT},
        {"POCET_PEKAROV_NA_KYSNUTIE_A_PECENIE", TRAY_BAKERS_COUNT},
        {"POCET_MIESACICH_STROJOV", MIXERS_COUNT},
        {"POCET_LAMINOVACICH_STROJOV", LAMINATORS_COUNT},
        {"POCET_TVAROVACICH_STROJOV", SHAPERS_COUNT},
        {"KAPACITA_CHLADNICKY", FRIDGE_CAPACITY},
        {"POCET_PLECHOV", TRAYS_COUNT},
        {"KAPACITA_PLECHU", TRAY_CAPACITY},
        {"KAPACITA_KYSIARNE", PROOFER_CAPACITY},
        {"KAPACITA_PECE", OVEN_CAPACITY}
    };

public:
    Parameters(const char *filename);

public:
    double simulation_duration() const {
        return m_parameters.at("TRVANIE_SIMULACIE");
    }

    double new_dough_interval() const {
        return m_parameters.at("INTERVAL_NOVEHO_CESTA");
    }

    int mixer_bakers_count() const {
        return static_cast<int>(
            m_parameters.at("POCET_PEKAROV_NA_MIESANIE")
        );
    }

    int laminator_shaper_bakers_count() const {
        return static_cast<int>(
			m_parameters.at("POCET_PEKAROV_NA_LAMINACIU_A_TVAROVANIE")
        );
    }

    int tray_bakers_count() const {
        return static_cast<int>(
			m_parameters.at("POCET_PEKAROV_NA_KYSNUTIE_A_PECENIE")
        );
    }

    int mixers_count() const {
        return static_cast<int>(
			m_parameters.at("POCET_MIESACICH_STROJOV")
        ); 
    }

    int laminators_count() const {
        return static_cast<int>(
			m_parameters.at("POCET_LAMINOVACICH_STROJOV")
        );
    }

    int shapers_count() const {
        return static_cast<int>(
			m_parameters.at("POCET_TVAROVACICH_STROJOV")
        );
    }
    

    int fridge_capacity() const {
        return static_cast<int>(
			m_parameters.at("KAPACITA_CHLADNICKY")
        );
    }

    int tray_count() const {
        return static_cast<int>(
			m_parameters.at("POCET_PLECHOV")
        );
    }

    int tray_capacity() const {
        return static_cast<int>(
			m_parameters.at("KAPACITA_PLECHU")
        );
    }

    int proofer_capacity() const {
        return static_cast<int>(
			m_parameters.at("KAPACITA_KYSIARNE")
        );
    }
    
    int oven_capacity() const {
        return static_cast<int>(
			m_parameters.at("KAPACITA_PECE")
        );
    }

    void print();
};

#endif
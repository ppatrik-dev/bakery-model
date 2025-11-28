/**
 * @file parameters.cpp
 * @author Patrik Procházka (xprochp00)
 * @brief Parameters source file
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include "parameters.hpp"

Parameters::Parameters(const char *filename) {
    if (filename) {
        m_filename = filename;

        std::ifstream file(m_filename);
        if (!file.is_open()) {
            std::cerr << "Chyba: nedá sa otvoriť súbor " << m_filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string key, value;

            if (std::getline(ss, key, ';') && std::getline(ss, value)) {
                m_parameters[key] = std::stod(value);
            }
        }
    }
}

void Parameters::print() {
    std::ostringstream headline;
    headline << "PARAMETERS " << (
        !m_filename.empty() ? m_filename : "implicitne hodnoty"
    );

    std::cout << "\n";
    std::cout << "+----------------------------------------------------------+\n";
    std::cout << "| " << std::left << std::setw(57) << headline.str() << "|\n";
    std::cout << "+----------------------------------------------------------+\n";

    for (const auto& [key, value] : m_parameters) {
        std::ostringstream line;
        line << key << " = " << value;

        std::cout << "|  "
                  << std::left << std::setw(56) << line.str()
                  << "|\n";
    }

    std::cout << "+----------------------------------------------------------+\n\n";
}
/**
 * @file statistics.cpp
 * @author Patrik Procházka (xprochp00)
 * @brief Statistics source file
 */

#include "statistics.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

void Statistics::print() const {
    m_production_duration_hist.Output();
    m_tray_pieces_count_stat.Output();

    std::ostringstream headline;
    headline << "STATISTICS " << "vysledne pocty";

    std::cout << "\n";
    std::cout << "+----------------------------------------------------------+\n";
    std::cout << "| " << std::left << std::setw(57) << headline.str() << "|\n";
    std::cout << "+----------------------------------------------------------+\n";

    std::cout
        << "|  " << std::left << std::setw(56) 
        << ("Vygenerovanych davok = " + std::to_string(m_generated_doughs_count))
        << "|\n";

    std::cout << "|" << std::left << std::setw(58) << " " << "|\n";

    std::cout
        << "|  " << std::left << std::setw(56) 
        << ("Celkovy pocet plechov = " + std::to_string(m_trays_total_count))
        << "|\n";

    std::cout
        << "|  " << std::left << std::setw(56) 
        << ("Dokoncenych plechov = " + std::to_string(m_trays_finished_count))
        << "|\n";

    std::cout << "|" << std::left << std::setw(58) << " " << "|\n";

    std::cout
        << "|  " << std::left << std::setw(56) 
        << ("Celkovy pocet kusov = " + std::to_string(m_pieces_total_count))
        << "|\n";

    std::cout
        << "|  " << std::left << std::setw(56) 
        << ("Dokoncenych kusov = " + std::to_string(m_pieces_finished_count))
        << "|\n";

    std::cout << "+----------------------------------------------------------+\n\n";
}
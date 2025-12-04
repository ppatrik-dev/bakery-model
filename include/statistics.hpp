/**
 * @file statistics.hpp
 * @author Patrik Procházka (xprochp00)
 * @brief Statistics header file
 */

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "simlib.h"

class Statistics {
public:
    Histogram m_production_duration_hist;
    Stat m_tray_pieces_count_stat;

    unsigned long m_doughs_total_count;
    unsigned long m_doughs_finished_count;

    unsigned long m_trays_total_count;
    unsigned long m_trays_finished_count;

    unsigned long m_pieces_total_count;
    unsigned long m_pieces_finished_count;

public:
    Statistics():
        m_production_duration_hist("Doba vyroby croisantov", 200, 25, 10),
        m_tray_pieces_count_stat("Pocet kusov na plechu"),
        m_doughs_total_count(0), m_doughs_finished_count(0),
        m_trays_total_count(0), m_trays_finished_count(0),
        m_pieces_total_count(0), m_pieces_finished_count(0) {}

public:
    void print() const;
};

#endif
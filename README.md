# Croissant Bakery Production Simulation

A discrete-event simulation model of a small croissant bakery that models the complete croissant production process during a single 8-hour shift, including dough preparation, mixing, lamination, shaping, proofing, baking, resource allocation, and machine failures. The simulation was implemented in **C++20** using the **SIMLIB** simulation library.

---

## Project Overview

The goal of this project is to analyze the production workflow of a small bakery and identify bottlenecks, resource limitations, and opportunities for increasing production efficiency.

The simulation models:

- Dough batch generation
- Mixing process
- Dough resting periods
- Lamination
- Shaping
- Proofing
- Baking
- Baker workload distribution
- Machine utilization
- Machine failures and repairs

The model was designed to closely resemble a real-world bakery production process based on consultations with an experienced baker and publicly available production data.

---

## Production Process

The production workflow consists of the following stages:

```text
Dough Preparation
        ↓
      Mixing
        ↓
 Dough Resting #1
        ↓
    Lamination
        ↓
 Dough Resting #2
        ↓
     Shaping
        ↓
     Proofing
        ↓
      Baking
        ↓
     Cooling
        ↓
 Finished Croissants
```

The bakery operates with:

- 3 bakers
- 1 mixer
- 1 laminating machine
- 1 shaping machine
- Proofing chamber
- Oven
- Refrigeration storage

---

## Simulation Architecture

The simulator consists of:

- Dough batch generator
- Dough process entities
- Tray process entities
- Bakers modeled as resources
- Machines modeled as facilities
- Storage capacities
- Machine failure generators
- Repair processes

Each dough batch moves through the production chain until it is converted into trays containing shaped croissants.

---

## Technologies

- `C++20`
- `SIMLIB`
- `GNU Make`
- `g++`

---

## Project Structure

```text
.
├── src/                    Source code implementation files
├── include/                Header files for the simulation components
├── docs/                   LaTeX documentation, diagrams and technical report
├── sim/                    Experiment parameter configuration and run script
├── lib/                    External dependencies (SIMLIB)
├── Makefile                Build configuration and compilation rules
└── README.md               Project documentation
```

---

## Building the Project

Compile the simulation:

```bash
make
```

Run the simulation with default parameters:

```bash
make run
```

Or execute manually:

```bash
./simulation
```

---

## Custom Configuration

Simulation parameters can be loaded from a CSV file:

```bash
./simulation parameters.csv
```

CSV format:

```csv
PARAMETER_NAME;VALUE
```

Example:

```csv
TRVANIE_SIMULACIE;480
DOBA_GENEROVANIA_NOVYCH_CIEST;75
INTERVAL_NOVEHO_CESTA;15
POCET_KUSOV_Z_JEDNEHO_CESTA;300
```

When a configuration file is supplied, its values override the defaults defined in `parameters.hpp`.

---

## Experiments

Several experiments were performed to validate the model and evaluate production efficiency.

### Experiment 1 — Production Time Analysis

Measured the production time of a single dough batch.

Results:

- Average production time: ~275 minutes
- Minimum: ~249 minutes
- Maximum: ~292 minutes

---

### Experiment 2 — Bottleneck Identification

Objective:

- Identify the weakest point in the production line.

Result:

The shaping machine was identified as the primary bottleneck.

- Average waiting time: ~70 minutes
- Maximum waiting time: ~76 minutes

---

### Experiment 3 — Resource Reduction

Objective:

- Determine whether certain resources can be reduced without affecting output.

Resources successfully reduced:

- Refrigerator capacity
- Proofing chamber capacity
- Oven capacity
- Number of trays

Result:

Production remained unchanged.

---

### Experiment 4 — Production Scaling

Objective:

- Double production output.

Changes:

- Double number of bakers
- Double machine count
- Increase oven capacity
- Increase tray count

Result:

Production doubled from approximately 1500 to 3000 croissants per shift.

---

### Experiment 5 — Mixer Failure

Simulated:

- Random mixer breakdown
- Repair duration ≈ 60 minutes

Result:

Production decreased due to discarded dough batches.

---

### Experiment 6 — Laminator Failure

Simulated:

- Random laminator breakdown
- Repair duration ≈ 45 minutes

Result:

No significant impact on final production.

---

### Experiment 7 — Shaping Machine Failure

Simulated:

- Random shaping machine breakdown
- Repair duration ≈ 25 minutes

Result:

Largest production loss among all tested failures.

- Significant reduction in completed trays
- Approximately 400 fewer croissants produced

---

## Key Findings

- The shaping machine is the primary production bottleneck.
- Production of approximately **1500 croissants per shift** is achievable with the baseline configuration.
- Several resources can be reduced without affecting output.
- Production can be doubled through proportional scaling of workers and equipment.
- Failures of the shaping machine have the most severe impact on production.

---

## License

This project is provided for educational and academic purposes.
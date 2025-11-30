#!/bin/bash

# Step 1: go up one level and build
cd .. || exit
echo "Running make..."
make || exit

# Step 2: return to sim/
cd sim || exit

# Step 3: iterate through all subdirectories
for d in */ ; do
    if [ -d "$d" ]; then
        echo "Running model in: $d"
        (
            cd "$d" || exit
            ../../model experiment.csv > experiment.out
        )
    fi
done

echo "All simulations finished."

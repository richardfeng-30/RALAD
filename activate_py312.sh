#!/bin/bash
# Script to activate the new Python 3.12 environment

# Source conda
source $HOME/miniconda/etc/profile.d/conda.sh

# Activate the new Python 3.12 environment
conda activate ralad_py312

echo "Activated Python 3.12 environment:"
python --version

echo ""
echo "Available packages:"
pip list | head -20

echo ""
echo "... (use 'pip list' to see all packages)"
echo ""
echo "To run your Python scripts, use: python script_name.py"
echo "To deactivate this environment, use: conda deactivate"

#!/usr/bin/env python3

import os
import subprocess
import shutil

# Configuration 
VITIS_HLS_CMD = "/tools/Xilinx/Vitis_HLS/2023.1/bin/vitis_hls"
TARGET_PART = "xcu250-figd2104-2L-e"
CLOCK_PERIOD = "5.0ns"

def create_test_project():
    """Create a test project to manually inspect synthesis results"""
    project_dir = "/home/richardqfeng/RALAD/git/RALAD/manual_test_project"
    
    # Clean up existing project
    if os.path.exists(project_dir):
        shutil.rmtree(project_dir)
    os.makedirs(project_dir)
    
    # Create TCL script
    tcl_content = f"""
# Create project
open_project test_project
set_top kernel_2mm

# Add source file
add_files /home/richardqfeng/RALAD/git/RALAD/cfile-small/kernel_2mm.c

# Create solution
open_solution "solution1" -flow_target vivado

# Set target part and clock
set_part {{{TARGET_PART}}}
create_clock -period {CLOCK_PERIOD}

# Run synthesis
csynth_design

# Exit
exit
"""
    
    tcl_file = os.path.join(project_dir, "test.tcl")
    with open(tcl_file, 'w') as f:
        f.write(tcl_content)
    
    return project_dir, tcl_file

def run_test():
    project_dir, tcl_file = create_test_project()
    
    print(f"Created test project in: {project_dir}")
    print(f"TCL file: {tcl_file}")
    
    # Change to project directory and run synthesis
    old_cwd = os.getcwd()
    try:
        os.chdir(project_dir)
        print(f"Running synthesis...")
        
        cmd = [VITIS_HLS_CMD, tcl_file]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=300)
        
        print(f"Return code: {result.returncode}")
        if result.stdout:
            print("STDOUT:")
            print(result.stdout[-1000:])  # Last 1000 chars
        if result.stderr:
            print("STDERR:")
            print(result.stderr[-1000:])
        
        # List all files in project directory
        print("\nProject directory contents:")
        for root, dirs, files in os.walk(project_dir):
            level = root.replace(project_dir, '').count(os.sep)
            indent = ' ' * 2 * level
            print(f"{indent}{os.path.basename(root)}/")
            subindent = ' ' * 2 * (level + 1)
            for file in files:
                print(f"{subindent}{file}")
    
    finally:
        os.chdir(old_cwd)
    
    print(f"\nProject kept at: {project_dir}")
    print("Please inspect the directory structure to find where synthesis reports are generated.")

if __name__ == "__main__":
    run_test()

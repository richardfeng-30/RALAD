import os
import sys
import subprocess
import csv
import re
import time
import shutil
from pathlib import Path

method = "graphrag"

# config
VITIS_HLS_CMD = "/tools/Xilinx/Vitis_HLS/2023.1/bin/vitis_hls"
TARGET_PART = "xcu250-figd2104-2L-e"
CLOCK_PERIOD = "5.0ns"
TIMEOUT = 1800

# Directories
WORKSPACE_ROOT = "/home/richardqfeng/RALAD/git/RALAD"
FILES_DIR = os.path.join(WORKSPACE_ROOT, "output", "llama", method, "code")
RESULTS_DIR = os.path.join(WORKSPACE_ROOT, "synthesis_results")
TEMP_DIR = os.path.join(RESULTS_DIR, "temp")

CSV_COLUMNS = [
    "filename",
    "can_compile",
    "latency_cycles", 
    "latency_ns",
    "BRAM",
    "DSP", 
    "FF",
    "LUT",
    "URAM"
]

def create_tcl_script(c_file_path, project_name, temp_dir):
    filename = os.path.basename(c_file_path).replace('.c', '')
    kernel_name = filename.replace('optimized_', '') if filename.startswith('optimized_') else filename
    kernel_function_name = kernel_name.replace('-', '_')
    
    tcl_content = f"""
# Open project
open_project -reset {project_name}
set_top {kernel_function_name}
add_files {c_file_path}
open_solution -reset "solution1"
set_part {{{TARGET_PART}}}
create_clock -period {CLOCK_PERIOD}
csynth_design
exit
"""
    
    tcl_file_path = os.path.join(temp_dir, f"{kernel_function_name}.tcl")
    with open(tcl_file_path, 'w') as f:
        f.write(tcl_content)
    
    return tcl_file_path

def run_synthesis(tcl_file_path, project_dir):
    try:
        old_cwd = os.getcwd()
        os.chdir(project_dir)
        
        cmd = [VITIS_HLS_CMD, tcl_file_path]
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        try:
            stdout, stderr = process.communicate(timeout=TIMEOUT)
            success = process.returncode == 0
            
            if not success:
                print(f"Synthesis failed for {tcl_file_path}")
                print(f"Return code: {process.returncode}")
                if stderr.strip():
                    print(f"STDERR: {stderr[:500]}")
                if stdout.strip():
                    print(f"STDOUT: {stdout[:500]}")
            
            return success, stdout, stderr
            
        except subprocess.TimeoutExpired:
            process.kill()
            print(f"Synthesis timed out for {tcl_file_path}")
            return False, "", "Timeout"
            
    except Exception as e:
        print(f"Error running synthesis: {e}")
        return False, "", str(e)
    finally:
        os.chdir(old_cwd)

def extract_synthesis_results(project_dir, kernel_name):
    results = {
        "can_compile": False,
        "latency_cycles": "N/A",
        "latency_ns": "N/A", 
        "BRAM": "N/A",
        "DSP": "N/A",
        "FF": "N/A",
        "LUT": "N/A",
        "URAM": "N/A"
    }
    
    report_paths = [
        os.path.join(project_dir, kernel_name, "solution1", "syn", "report", f"{kernel_name}_csynth.rpt"),
        os.path.join(project_dir, kernel_name, "solution1", "syn", "report", "csynth.rpt"),

        os.path.join(project_dir, f"{kernel_name}_project", "solution1", "syn", "report", f"{kernel_name}_csynth.rpt"),
        os.path.join(project_dir, f"{kernel_name}_project", "solution1", "syn", "report", "csynth.rpt"),
    ]
    
    report_file = None
    for path in report_paths:
        if os.path.exists(path):
            report_file = path
            break
    
    if not report_file:
        for root, dirs, files in os.walk(project_dir):
            if "report" in root and files:
                for file in files:
                    if file.endswith('.rpt'):
                        report_file = os.path.join(root, file)
                        break
                if report_file:
                    break
    
    if not report_file:
        print(f"No synthesis report found for {kernel_name}")
        return results
    
    results["can_compile"] = True
    
    try:
        with open(report_file, 'r') as f:
            content = f.read()
        
        latency_match = re.search(r'Latency.*?min.*?(\d+).*?cycles', content, re.IGNORECASE | re.DOTALL)
        if latency_match:
            results["latency_cycles"] = latency_match.group(1)
            try:
                cycles = int(latency_match.group(1))
                clock_period_ns = float(CLOCK_PERIOD.replace('ns', ''))
                results["latency_ns"] = f"{cycles * clock_period_ns:.2f}"
            except:
                results["latency_ns"] = "N/A"
        
        total_match = re.search(r'\|\s*Total\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|\s*(\d+)', content)
        if total_match:
            results["BRAM"] = total_match.group(1)
            results["DSP"] = total_match.group(2)
            results["FF"] = total_match.group(3)
            results["LUT"] = total_match.group(4)
            results["URAM"] = total_match.group(5)
        else:
            resource_patterns = {
                "BRAM": [r'BRAM_18K.*?(\d+)', r'Block RAM.*?(\d+)'],
                "DSP": [r'DSP.*?(\d+)', r'DSP48E.*?(\d+)'],
                "FF": [r'FF.*?(\d+)', r'Flip Flop.*?(\d+)', r'Register.*?(\d+)'],
                "LUT": [r'LUT.*?(\d+)', r'Look-Up Table.*?(\d+)'],
                "URAM": [r'URAM.*?(\d+)', r'UltraRAM.*?(\d+)']
            }
            
            for resource, patterns in resource_patterns.items():
                for pattern in patterns:
                    match = re.search(pattern, content, re.IGNORECASE)
                    if match:
                        results[resource] = match.group(1)
                        break
    
    except Exception as e:
        print(f"Error parsing synthesis report for {kernel_name}: {e}")
    
    return results

def write_csv_header(csv_file):
    if not os.path.exists(csv_file):
        with open(csv_file, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=CSV_COLUMNS)
            writer.writeheader()

def append_result_to_csv(csv_file, result):
    with open(csv_file, 'a', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=CSV_COLUMNS)
        writer.writerow(result)

def get_c_files(directory, pattern="*.c"):
    path = Path(directory)
    return list(path.glob(pattern))

def synthesize_files_realtime(files_list, output_csv, file_type="original"):
    results = []
    
    print(f"Starting synthesis for {len(files_list)} {file_type} files...")
    print(f"Results will be saved to: {output_csv}")
    
    write_csv_header(output_csv)
    
    for i, c_file_path in enumerate(files_list):
        filename = os.path.basename(c_file_path)
        base_name = filename.replace('.c', '')
        kernel_name = base_name.replace('optimized_', '') if base_name.startswith('optimized_') else base_name
        
        print(f"[{i+1}/{len(files_list)}] Processing {filename}...")
        
        project_name = f"{base_name}_project"
        project_dir = os.path.join(TEMP_DIR, project_name)
        
        try:
            if os.path.exists(project_dir):
                shutil.rmtree(project_dir)
            os.makedirs(project_dir, exist_ok=True)
            
            tcl_file = create_tcl_script(str(c_file_path), project_name, project_dir)
            success, stdout, stderr = run_synthesis(tcl_file, project_dir)
            
            if success:
                synthesis_results = extract_synthesis_results(project_dir, base_name)
                synthesis_results["filename"] = filename
                
                print(f"✓ Synthesis successful for {filename}")
                print(f"  Latency: {synthesis_results['latency_cycles']} cycles, "
                      f"Resources: FF={synthesis_results['FF']}, LUT={synthesis_results['LUT']}, "
                      f"DSP={synthesis_results['DSP']}, BRAM={synthesis_results['BRAM']}")
            else:
                synthesis_results = {col: "N/A" for col in CSV_COLUMNS}
                synthesis_results["filename"] = filename
                synthesis_results["can_compile"] = False
                
                print(f"✗ Synthesis failed for {filename}")
            
            append_result_to_csv(output_csv, synthesis_results)
            results.append(synthesis_results)
            print(f"  → Result written to {output_csv}")
            
            try:
                shutil.rmtree(project_dir)
            except:
                pass
                
        except Exception as e:
            print(f"Error processing {filename}: {e}")
            synthesis_results = {col: "N/A" for col in CSV_COLUMNS}
            synthesis_results["filename"] = filename
            synthesis_results["can_compile"] = False
            append_result_to_csv(output_csv, synthesis_results)
            results.append(synthesis_results)
    
    print(f"Completed {file_type} files. Results saved to {output_csv}")
    return results

def main():
    print("Vitis HLS Synthesis Automatic Eval")
    print("=====================================================")
    
    os.makedirs(RESULTS_DIR, exist_ok=True)
    os.makedirs(TEMP_DIR, exist_ok=True)
    
    try:
        subprocess.run([VITIS_HLS_CMD, "-version"], capture_output=True, check=True)
        print("✓ Vitis HLS found")
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("✗ Vitis HLS not found. Please ensure Vitis HLS is installed and in PATH.")
        sys.exit(1)
    
    files = get_c_files(FILES_DIR, "*.c")
    
    print(f"Found {len(files)} C files")
    
    if not files:
        print("No C files found to process!")
        sys.exit(1)
    
    start_time = time.time()
    
    if files:
        csv = os.path.join(RESULTS_DIR, f"{method}.csv")
        results = synthesize_files_realtime(files, csv, method)

        successful = sum(1 for r in results if r["can_compile"])
        print(f"\n{successful}/{len(files)} compiled successfully")
    
    try:
        shutil.rmtree(TEMP_DIR)
    except:
        pass
    
    total_time = time.time() - start_time
    print(f"\nTotal execution time: {total_time:.2f} seconds")
    print(f"Results saved in: {RESULTS_DIR}")

if __name__ == "__main__":
    main()

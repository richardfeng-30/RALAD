import os
import re
import shutil
from pathlib import Path


def extract_c_code(content):
    # pattern 1: ```c ... ```
    pattern1 = re.search(r'```c\s*\n(.*?)\n```', content, re.DOTALL)
    if pattern1:
        code = pattern1.group(1).strip()
        if is_valid_c_code(code):
            return code
    
    # pattern 2: ``` ... ```
    pattern2 = re.search(r'```\s*\n(.*?)\n```', content, re.DOTALL)
    if pattern2:
        code = pattern2.group(1).strip()
        if is_valid_c_code(code):
            return code
    
    return None


def is_valid_c_code(code):
    c_indicators = [
        'void ',           # function definitions
        'int ',            # variable declarations
        'float ',          # variable declarations
        '#pragma',         # HLS pragmas
        '{',              # code blocks
        ';',              # statement endings
        'for (',          # for loops
        'if (',           # if statements
    ]
    
    # code should contain at least a few C-like elements
    matches = sum(1 for indicator in c_indicators if indicator in code)
    
    # check that it's not just explanatory text
    lines = code.strip().split('\n')
    code_lines = [line for line in lines if line.strip() and not line.strip().startswith('*') and not line.strip().startswith('//')]
    
    return matches >= 3 and len(code_lines) > 5


def process_file(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        extracted_code = extract_c_code(content)
        
        if extracted_code:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(extracted_code)
            print(f"✓ {file_path.name}")
            return True
        else:
            print(f"✗ {file_path.name}: No C code found")
            return False
            
    except Exception as e:
        print(f"✗ {file_path.name}: Error - {e}")
        return False


def main():
    script_dir = Path(__file__).parent
    input_dir = script_dir / "output" / "graphrag2" / "code"

    if not input_dir.exists():
        print(f"Error: Directory {input_dir} not found!")
        return
    
    c_files = list(input_dir.glob("*.c"))
    if not c_files:
        print("No .c files found!")
        return
    
    print(f"Processing {len(c_files)} files...")
    
    successful = 0
    for c_file in sorted(c_files):
        if process_file(c_file):
            successful += 1
    
    print(f"\nDone: {successful}/{len(c_files)} files processed")


if __name__ == "__main__":
    main()

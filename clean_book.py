import re

def clean_book(input_file, output_file):
    # Pattern to match the header format
    # Matches: UG1399 (v2022.2) October 19, 2022\nVitis HLS User Guide\n\nSend Feedback\n\nwww.xilinx.com\nPAGE_NUMBER (with or without brackets)
    pattern = r'UG1399 \(v2022\.2\) October 19, 2022\s*\n\s*Vitis HLS User Guide\s*\n\s*Send Feedback\s*\n\s*www\.xilinx\.com\s*\n\s*\[?\d+\]?'
    
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Remove the header instances
        cleaned_content = re.sub(pattern, '', content, flags=re.MULTILINE)
        
        # Also remove partial headers that might appear separately
        partial_pattern = r'UG1399 \(v2022\.2\) October 19, 2022\s*\n\s*Vitis HLS User Guide\s*\n?'
        cleaned_content = re.sub(partial_pattern, '', cleaned_content, flags=re.MULTILINE)
        
        # Remove footer fragments: Send Feedback\n\nwww.xilinx.com\nPAGE_NUMBER
        footer_pattern = r'Send Feedback\s*\n\s*www\.xilinx\.com\s*\n\s*\d+'
        cleaned_content = re.sub(footer_pattern, '', cleaned_content, flags=re.MULTILINE)
        
        # Clean up any excessive whitespace left behind
        cleaned_content = re.sub(r'\n\s*\n\s*\n', '\n\n', cleaned_content)
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(cleaned_content)
        
        print(f"Successfully cleaned the book. Output saved to: {output_file}")
        
    except FileNotFoundError:
        print(f"Error: File '{input_file}' not found.")
    except Exception as e:
        print(f"Error processing file: {e}")

if __name__ == "__main__":
    input_file = "documents/ug1399-vitis-hls.txt"
    output_file = "documents/ug1399-vitis-hls-cleaned.txt"

    clean_book(input_file, output_file)
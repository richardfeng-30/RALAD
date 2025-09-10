# RAG+Llama for codgen

from typing import List, Optional
import fire
from llama import Dialog, Llama
import os
import json
from pathlib import Path

def process_single_kernel(generator, kernel_file: str, output_dir: str, temperature: float, top_p: float, max_gen_len: Optional[int]) -> dict:
    kernel_name = Path(kernel_file).stem
    print(f"\n=== Processing {kernel_name} ===")
    
    try:
        with open(kernel_file, 'r', encoding='utf-8') as f:
            kernel_content = f.read()
        
        print(f"Read kernel file: {kernel_file}")
        print(f"Content length: {len(kernel_content)} characters")

        query_file = f"retrieve_query/self_rag/{kernel_name}.txt"
        with open(query_file, 'r', encoding='utf-8') as f:
            query = f.read()

        # create dialog for llama
        dialogs: List[Dialog] = [
            [
                {"role": "system", "content": "Always answer with optimized C code including HLS pragmas. Add appropriate #pragma HLS directives for interfaces, loops, and memory optimization."},
                {"role": "user", "content": query},
            ],
        ]
        
        print("Generating optimized code with LLaMA...")
        print(f"Query length: {len(query)} characters")
        
        results = generator.chat_completion(
            dialogs,
            max_gen_len=max_gen_len,
            temperature=temperature,
            top_p=top_p,
        )
        
        generated_content = results[0]['generation']['content']
        
        print(f"Generated content length: {len(generated_content)} characters")
        
        Path(f"{output_dir}/code").mkdir(parents=True, exist_ok=True)
        Path(f"{output_dir}/dialog").mkdir(parents=True, exist_ok=True)
        
        output_file = f"{output_dir}/code/{kernel_name}.c"
        with open(output_file, 'w') as f:
            f.write(generated_content)
        
        dialog_file = f"{output_dir}/dialog/{kernel_name}_dialog.txt"
        with open(dialog_file, 'w') as f:
            f.write("=== SYSTEM ===\n")
            f.write(dialogs[0][0]['content'])
            f.write("\n\n=== USER QUERY ===\n")
            f.write(dialogs[0][1]['content'])
            f.write("\n\n=== LLAMA RESPONSE ===\n")
            f.write(generated_content)
        
        print(f"✓ Generated code saved to: {output_file}")
        
        return {
            "status": "success",
            "input_file": str(kernel_file),
            "output_file": output_file,
            "query_file": query_file,
            "dialog_file": dialog_file,
        }
        
    except Exception as e:
        print(f"Error processing {kernel_name}: {e}")
        import traceback
        traceback.print_exc()
        return {
            "status": "error",
            "error": str(e),
            "input_file": str(kernel_file)
        }

def main(
    ckpt_dir: str,
    tokenizer_path: str,
    kernel_file: str = None,
    output_dir: str = "output/llama/self_rag",
    temperature: float = 0.6,
    top_p: float = 0.9,
    max_seq_len: int = 8192,
    max_batch_size: int = 1,
    max_gen_len: Optional[int] = 2048,
):   
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    Path(f"{output_dir}/code").mkdir(parents=True, exist_ok=True)
    Path(f"{output_dir}/dialog").mkdir(parents=True, exist_ok=True)
    
    # Determine which kernels to process
    if kernel_file:
        if not os.path.exists(kernel_file):
            print(f"Error: Kernel file not found: {kernel_file}")
            return
        kernel_files = [kernel_file]
        print(f"Processing single kernel: {kernel_file}")
    else:
        # Process all kernels in cfile-small
        kernels_dir = Path("cfile-small")
        if not kernels_dir.exists():
            print("Error: cfile-small directory not found")
            return
        kernel_files = list(kernels_dir.glob("kernel_*.c"))
        print(f"Processing {len(kernel_files)} kernels from cfile-small/")
    
    if not kernel_files:
        print("No kernel files found to process")
        return
    
    # Build LLaMA generator
    print("\n=== Loading LLaMA Model ===")
    try:
        generator = Llama.build(
            ckpt_dir=ckpt_dir,
            tokenizer_path=tokenizer_path,
            max_seq_len=max_seq_len,
            max_batch_size=max_batch_size,
        )
        print("LLaMA model loaded successfully")
    except Exception as e:
        print(f"Error loading LLaMA model: {e}")
        return
    
    # Process each kernel
    results = {}
    
    for i, kf in enumerate(kernel_files, 1):
        print(f"\n--- Processing {i}/{len(kernel_files)} ---")
        
        result = process_single_kernel(
            generator=generator,
            kernel_file=str(kf),
            output_dir=output_dir,
            temperature=temperature,
            top_p=top_p,
            max_gen_len=max_gen_len
        )
        
        kernel_name = Path(kf).stem
        results[kernel_name] = result
    
    print(f"\n=== Generation Complete ===")
    successful = sum(1 for r in results.values() if r['status'] == 'success')
    total = len(results)
    print(f"Successfully processed: {successful}/{total} kernels")
    

if __name__ == "__main__":
    fire.Fire(main)

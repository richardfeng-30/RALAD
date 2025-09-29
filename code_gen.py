import os
import asyncio
from pathlib import Path
import openai
from dotenv import load_dotenv

load_dotenv(dotenv_path="graphrag/.env")

api_key = os.environ["GRAPHRAG_API_KEY"]
llm_model = os.environ["GRAPHRAG_QUERY_LLM_MODEL"]

client = openai.OpenAI(api_key=api_key)

async def generate_code_with_gpt4(kernel_content, kernel_name):    
    prompt = """
      You are an expert in High-Level Synthesis (HLS) optimization. Given the following C code, your objective is to optimize the performance while keeping the resource utilization under 80% and
      the compilation time under 1800s. Here are the specific resource constraints for the target FPGA device (Xilinx Alveo U250):

      LUTs: 1,341K (i.e. 1,341,000)
      Registers (Flip-Flops / FFs): 2,749K (i.e. 2,749,000)
      DSP slices: 11,508
      Block RAM (36 Kb): 2,000 (i.e. 2,000 blocks of 36 Kb)
      UltraRAM (288 Kb): 1,280 (blocks)
      
      Insert the appropriate pragmas directly into the C code. Only return the final code with pragmas. Do not include explanations or comments.
    """
    
    full_prompt = prompt + "\n\n" + kernel_content
    
    try:
        response = client.chat.completions.create(
            model=llm_model,
            messages=[
                {"role": "system", "content": "You are an expert in High-Level Synthesis (HLS) optimization for FPGAs."},
                {"role": "user", "content": full_prompt}
            ],
            max_tokens=4000,
            temperature=0.0
        )
        
        return response.choices[0].message.content
        
    except Exception as e:
        print(f"Error generating code for {kernel_name}: {str(e)}")
        return f"// Error occurred during code generation for {kernel_name}\n{kernel_content}"

async def main():
    output_dir = Path("output/gpt4") 
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    Path(f"{output_dir}/code").mkdir(parents=True, exist_ok=True)

    # Process all kernels in cfile-small
    kernels_dir = Path("cfile-small")
    if not kernels_dir.exists():
        print("Error: cfile-small directory not found")
        return
    
    kernel_files = list(kernels_dir.glob("kernel_*.c"))
    print(f"Processing {len(kernel_files)} kernels from cfile-small/")

    for i, kf in enumerate(kernel_files, 1):
        print(f"\n--- Processing {i}/{len(kernel_files)} ---")

        kernel_name = Path(kf).stem
        with open(kf, 'r', encoding='utf-8') as f:
            kernel_content = f.read()

        print(f"Read kernel file: {kf}")
        print(f"Content length: {len(kernel_content)} characters")

        # Generate optimized code using GPT-4.1 directly
        result = await generate_code_with_gpt4(kernel_content, kernel_name)

        # Save the generated code
        output_file = f"{output_dir}/code/{kernel_name}.c"
        with open(output_file, 'w') as f:
            f.write(result)
        print(f"Generated code file: {output_file}")

if __name__ == "__main__":
    asyncio.run(main())
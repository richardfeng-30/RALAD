# retrieve documents for all input c files

import sentence_embedding as st
from typing import List, Optional
import fire
from llama import Dialog, Llama
import os
 
kernel_descriptions = {
    "2mm": "2 Matrix Multiplications",
    "3mm": "3 Matrix Multiplications",
    "adi": "Alternating Direction Implicit solver",
    "aes": "Advanced Encryption Standard",
    "atax": "Matrix Transpose and Vector Multiplication",
    "atax-medium": "Matrix Transpose and Vector Multiplication",
    "bicg": "BiCG Sub Kernel of BiCGStab Linear Solver",
    "bicg-large": "BiCG Sub Kernel of BiCGStab Linear Solver",
    "bicg-medium": "BiCG Sub Kernel of BiCGStab Linear Solver",
    "correlation": "Correlation Computation",
    "covariance": "Covariance Computation",
    "doitgen": "Multiresolution Analysis",
    "doitgen-r": "Multiresolution Analysis",
    "fdtd-2d": "2-D Finite Different Time Domain Kernel",
    "fdtd-2d-l": "2-D Finite Different Time Domain Kernel",
    "gemm-b": "Blocked Version of Matrix Multiplication",
    "gemm-n": "Matrix Transpose and Vector Multiplication",
    "gemm-p": "Matrix Multiplication",
    "gemm-p-l": "Matrix Transpose and Vector Multiplication",
    "gemver": "Vector Multiplication and Matrix Addition",
    "gemver-m": "Vector Multiplication and Matrix Addition",
    "gesummv": "Scalar, Vector and Matrix Multiplication",
    "gesummv-m": "Scalar, Vector and Matrix Multiplication",
    "heat-3d": "Heat Equation over 3D Data Domain",
    "jacobi-1d": "1-D Jacobi Stencil Computation",
    "jacobi-2d": "2-D Jacobi Stencil Computation",
    "md": "n-body Molecular Dynamics",
    "mvt": "Matrix-Vector Product and Transpose",
    "mvt-m": "Matrix-Vector Product and Transpose",
    "nw": "Dynamic Programming for Sequence Alignment",
    "seidel-2d": "2D Seidel Computation",
    "spmv-crs": "Sparse Mat-Vec Mult. w/ Variable-Len. Neighbor",
    "spmv-ellpack": "Sparse Mat-Vec Mult. w/ Fixed-size Neighbor",
    "stencil": "A Two-Dimensional Stencil Computation",
    "stencil-3d": "A Three-Dimensional Stencil Computation",
    "symm": "Symmetric Matrix Multiplication",
    "symm-opt": "Symmetric Matrix Multiplication",
    "symm-opt-m": "Symmetric Matrix Multiplication",
    "syr2k": "Symmetric Rank-2k Operations",
    "syrk": "Symmetric Rank-k Operations",
    "trmm": "Triangular Matrix Multiplication",
    "trmm-opt": "Triangular Matrix Multiplication",
}

def read_c_files(directory):
    c_files_content = {}
    
    for filename in os.listdir(directory):
        if filename.endswith('.c'):
            filepath = os.path.join(directory, filename)
            with open(filepath, 'r', encoding='utf-8') as file:
                c_files_content[filename] = file.read()
    
    return c_files_content

def main(
    ckpt_dir: str = "/home/haochx5/llama3/Meta-Llama-3-8B-Instruct",
    tokenizer_path: str = "/home/haochx5/llama3/Meta-Llama-3-8B-Instruct/tokenizer.model",
    max_seq_len: int = 8192,
    max_batch_size: int = 1,
):
    """
    Main function to retrieve documents for all C files using Llama model.
    
    Args:
        ckpt_dir (str): Path to the directory containing checkpoint files.
        tokenizer_path (str): Path to the tokenizer file.
        max_seq_len (int): Maximum sequence length for input text.
        max_batch_size (int): Maximum batch size for inference.
    """
    
    # 指定cfile-small文件夹路径
    directory_path = 'cfile-small'

    # 读取所有.c文件内容
    c_files_content = read_c_files(directory_path)

    # Assuming you already have the c_files_content dictionary and the select_sentenceembedding function
    for filename, content in c_files_content.items():
        print(f"\n=== Processing {filename} ===")
        
        kernel_name = filename.replace('.c', '').replace('kernel_', '')
        if kernel_name in kernel_descriptions:
            content = "//" + kernel_descriptions[kernel_name] + "\n\n" + content
            
        helper = st.select_sentenceembedding(query=content, ckpt_dir=ckpt_dir, tokenizer_path=tokenizer_path)
        helper = '\n\n'.join([f'Example {I + 1}: \n{c}' for I, c in enumerate(helper)])
        helper = f'You are given some HLS pragma insertion examples.\n\n{helper}\n\nOptimize the following code by inserting pragma HLS lines.\n{content}'
        
        # 创建一个新的文件夹，文件夹名称为"output_files"
        output_dir = 'retrieve_query/self_rag'
        os.makedirs(output_dir, exist_ok=True)

        # remove .c from filename
        filename = filename.replace('.c', '')
        
        # 为每个文件创建一个新的文件，文件名为filename.txt，同时文件内容为helper
        output_file = os.path.join(output_dir, f'{filename}.txt')
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(helper)
        
        print(f"Saved output to: {output_file}")

if __name__ == "__main__":
    fire.Fire(main)

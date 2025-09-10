import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import seaborn as sns
from matplotlib.patches import Rectangle

plt.rcParams['figure.facecolor'] = 'white'
plt.rcParams['axes.facecolor'] = 'white'
plt.rcParams['axes.edgecolor'] = 'black'
plt.rcParams['axes.linewidth'] = 1.0
plt.rcParams['xtick.color'] = 'black'
plt.rcParams['ytick.color'] = 'black'
plt.rcParams['axes.labelcolor'] = 'black'
plt.rcParams['text.color'] = 'black'
plt.rcParams['axes.grid'] = False
plt.rcParams['axes.spines.left'] = True
plt.rcParams['axes.spines.bottom'] = True
plt.rcParams['axes.spines.top'] = False
plt.rcParams['axes.spines.right'] = False
sns.set_palette("husl")

COLORS = ['#fee391', '#fec44f', '#fe9929', '#d95f0e', '#993404']

METHODS = {
    'Original': {'file': 'synthesis_results/original.csv'},
    'Zero-shot': {'file': 'synthesis_results/zero_shot.csv'},
    'Zero-shot CoT': {'file': 'synthesis_results/cot.csv'},
    'RAG': {'file': 'synthesis_results/rag.csv'},
    'Self-RAG': {'file': 'synthesis_results/self_rag.csv'},
    'graphRAG': {'file': 'synthesis_results/graphrag.csv'},
}

method_dataframes = {}
for method_name, config in METHODS.items():
    df = pd.read_csv(config['file'])
    df['source'] = method_name
    method_dataframes[method_name] = df

combined_df = pd.concat(method_dataframes.values(), ignore_index=True)

# Convert N/A strings to NaN for proper handling
numeric_columns = ['latency_cycles', 'latency_ns', 'BRAM', 'DSP', 'FF', 'LUT', 'URAM']
for col in numeric_columns:
    combined_df[col] = pd.to_numeric(combined_df[col], errors='coerce')


def create_chart1(selected_methods=None):
    if selected_methods is None:
        selected_methods = list(METHODS.keys())
    
    # Validate selected methods
    selected_methods = [m for m in selected_methods if m in METHODS]
    if not selected_methods:
        raise ValueError("No valid methods selected")
    
    print(f"Creating chart with methods: {selected_methods}")
    
    fig, axes = plt.subplots(1, 3, figsize=(12, 6))
    fig.patch.set_facecolor('white')
    
    colors = COLORS[:len(selected_methods)]

    # Find compilable files for each method
    compilable_sets = {}
    for method in selected_methods:
        df = method_dataframes[method]
        compilable_sets[method] = set(df[df['can_compile'] == True]['filename'])
    
    # Find intersection of all compilable files
    all_compilable = set.intersection(*compilable_sets.values()) if compilable_sets else set()
    all_compilable = list(all_compilable)
    
    legend_handles = []
    legend_labels = selected_methods
    
    if all_compilable:
        # Prepare data subsets for each method
        method_subsets = {}
        for method in selected_methods:
            df = method_dataframes[method]
            method_subsets[method] = df[df['filename'].isin(all_compilable)]

        kernel_names = [name.replace('kernel_', '').replace('.c', '') for name in all_compilable]
        x = np.arange(len(kernel_names))
        width = 0.8 / len(selected_methods)  # Dynamic width based on number of methods

        # Latency chart
        ax1 = axes[0]
        bars1 = []
        for i, method in enumerate(selected_methods):
            subset = method_subsets[method]
            latencies = subset.set_index('filename').loc[all_compilable, 'latency_ns'].values
            bar = ax1.bar(x + i*width - (len(selected_methods)-1)*width/2, latencies, width, 
                         color=colors[i], alpha=0.8, edgecolor='black', linewidth=1, label=method)
            bars1.append(bar)
        
        legend_handles = bars1
        
        ax1.set_ylabel('Latency (ns)')
        ax1.set_title('Latency', fontweight='bold')
        ax1.set_xticks(x)
        ax1.set_xticklabels(kernel_names, rotation=45, ha='right')
        ax1.set_yscale('log')
        ax1.yaxis.set_minor_locator(ticker.NullLocator())
        ax1.spines['top'].set_visible(False)
        ax1.spines['right'].set_visible(False)
        
        # LUT chart
        ax2 = axes[1]
        bars2 = []
        for i, method in enumerate(selected_methods):
            subset = method_subsets[method]
            luts = subset.set_index('filename').loc[all_compilable, 'LUT'].values
            bar = ax2.bar(x + i*width - (len(selected_methods)-1)*width/2, luts, width,
                         color=colors[i], alpha=0.8, edgecolor='black', linewidth=1)
            bars2.append(bar)

        ax2.set_ylabel('LUT Count')
        ax2.set_title('LUT Utilization', fontweight='bold')
        ax2.set_xticks(x)
        ax2.set_xticklabels(kernel_names, rotation=45, ha='right')
        ax2.spines['top'].set_visible(False)
        ax2.spines['right'].set_visible(False)
    
        # Resource efficiency chart
        ax3 = axes[2]
        efficiency_data = []
        for kernel in all_compilable:
            eff_row = {'kernel': kernel.replace('kernel_', '').replace('.c', '')}
            for method in selected_methods:
                subset = method_subsets[method]
                row = subset[subset['filename'] == kernel].iloc[0]
                if pd.notna(row['latency_ns']) and pd.notna(row['LUT']) and row['LUT'] > 0:
                    eff = 1 / (row['latency_ns'] * row['LUT'])
                else:
                    eff = 0
                eff_row[method] = eff
            efficiency_data.append(eff_row)
            
        if efficiency_data:
            eff_df = pd.DataFrame(efficiency_data)
            x = np.arange(len(eff_df))
            bars3 = []
            for i, method in enumerate(selected_methods):
                bar = ax3.bar(x + i*width - (len(selected_methods)-1)*width/2, eff_df[method], width,
                             color=colors[i], alpha=0.8, edgecolor='black', linewidth=1)
                bars3.append(bar)
            ax3.set_ylabel('Efficiency (1/latency*LUT)')
            ax3.set_title('Resource Efficiency', fontweight='bold')
            ax3.set_xticks(x)
            ax3.set_xticklabels(eff_df['kernel'], rotation=45, ha='right')
            ax3.set_yscale('log')
            ax3.yaxis.set_minor_locator(ticker.NullLocator())
            ax3.spines['top'].set_visible(False)
            ax3.spines['right'].set_visible(False)

    if legend_handles:
        from matplotlib.patches import Rectangle
        custom_handles = []
        
        for color in colors:
            handle = Rectangle((0, 0), 1, 1, facecolor=color, edgecolor='black', linewidth=1)
            custom_handles.append(handle)
        
        fig.legend(custom_handles, legend_labels, loc='upper center', 
                  bbox_to_anchor=(0.5, 0.95), ncol=len(selected_methods), frameon=False,
                  handlelength=1, handletextpad=0.5, handleheight=1.2)

    plt.tight_layout(rect=[0, 0, 1, 0.88])
    plt.savefig('charts/chart1.png', dpi=300, bbox_inches='tight', facecolor='white')
    plt.show()


def create_compile_table(selected_methods=None):
    if selected_methods is None:
        selected_methods = list(METHODS.keys())
    selected_methods = [m for m in selected_methods if m in METHODS]
    if not selected_methods:
        raise ValueError("No valid methods selected")

    # Get all unique filenames across selected methods
    all_filenames = set()
    for method in selected_methods:
        df = method_dataframes[method]
        all_filenames.update(df['filename'].unique())
    all_filenames = sorted(all_filenames)

    # Build a DataFrame: rows are filenames, columns are methods, values are True/False for can_compile
    table_data = []
    for filename in all_filenames:
        row = {'Kernel': filename.replace('kernel_', '').replace('.c', '')}
        for method in selected_methods:
            df = method_dataframes[method]
            match = df[df['filename'] == filename]
            if not match.empty:
                can_compile = bool(match.iloc[0]['can_compile'])
            else:
                can_compile = False
            row[method] = can_compile
        table_data.append(row)
    table_df = pd.DataFrame(table_data)

    # Make the table wider and remove vertical space above and below
    fig_width = 2.5 + len(selected_methods) * 2.2  # wider columns
    fig_height = max(0.32 * len(all_filenames), 2.2)  # minimal vertical space
    fig, ax = plt.subplots(figsize=(fig_width, fig_height))
    ax.axis('off')

    # Prepare cell colors: green for True, red for False
    cell_colours = []
    for i, row in table_df.iterrows():
        row_colours = ['white']  # Kernel name column
        for method in selected_methods:
            row_colours.append('#a1d99b' if row[method] else '#fc9272')
        cell_colours.append(row_colours)

    # Table column labels
    col_labels = ['Kernel'] + selected_methods
    # Add a small padding around the table (e.g., 5% on each side)
    table = ax.table(cellText=table_df.values,
                     colLabels=col_labels,
                     cellColours=cell_colours,
                     loc='center',
                     cellLoc='center',
                     colLoc='center',
                     bbox=[0.05, 0.10, 0.9, 0.85])

    # Add a title above the table
    fig.suptitle('Compile Success (pass@1)', fontsize=18, fontweight='bold', y=0.98)
    table.auto_set_font_size(False)
    table.set_fontsize(13)
    table.scale(1.4, 1.05)  # less vertical padding

    # Set header color
    for j in range(len(col_labels)):
        table[0, j].set_facecolor('#fee391')
        table[0, j].set_fontsize(14)
        table[0, j].set_text_props(weight='bold')

    # Remove all extra vertical space
    # Use tight layout and a small padding to keep a white border
    plt.savefig('charts/compile_table.png', dpi=300, bbox_inches='tight', pad_inches=0.1, facecolor='white', transparent=False)
    plt.close(fig)





if __name__ == "__main__":
    print("Creating chart 1...")
    create_chart1(['Original', 'Zero-shot', 'RAG', 'graphRAG'])

    print("\nCharts completed!")

    print("\nCreating compile table...")
    create_compile_table(['Original', 'Zero-shot', 'Zero-shot CoT', 'RAG', 'Self-RAG', 'graphRAG'])
    print("Compile table saved as charts/compile_table.png")
import os
import asyncio
from pathlib import Path

import pandas as pd
import tiktoken

from graphrag.query.context_builder.entity_extraction import EntityVectorStoreKey
from graphrag.query.indexer_adapters import (
    read_indexer_covariates,
    read_indexer_entities,
    read_indexer_relationships,
    read_indexer_reports,
    read_indexer_text_units,
)
from graphrag.query.question_gen.local_gen import LocalQuestionGen
from graphrag.query.structured_search.local_search.mixed_context import (
    LocalSearchMixedContext,
)
from graphrag.query.structured_search.local_search.search import LocalSearch
from graphrag.vector_stores.lancedb import LanceDBVectorStore
from graphrag.config.models.vector_store_schema_config import VectorStoreSchemaConfig

INPUT_DIR = "graphrag/output"
LANCEDB_URI = f"{INPUT_DIR}/lancedb"

COMMUNITY_REPORT_TABLE = "community_reports"
ENTITY_TABLE = "entities"
COMMUNITY_TABLE = "communities"
RELATIONSHIP_TABLE = "relationships"
COVARIATE_TABLE = "covariates"
TEXT_UNIT_TABLE = "text_units"
COMMUNITY_LEVEL = 1 # was 2

# read nodes table to get community and degree data
entity_df = pd.read_parquet(f"{INPUT_DIR}/{ENTITY_TABLE}.parquet")
community_df = pd.read_parquet(f"{INPUT_DIR}/{COMMUNITY_TABLE}.parquet")

entities = read_indexer_entities(entity_df, community_df, COMMUNITY_LEVEL)

# load description embeddings to an in-memory lancedb vectorstore
# to connect to a remote db, specify url and port values.
vector_store_config = VectorStoreSchemaConfig(
    id_field="id",
    vector_field="vector", 
    text_field="text",
    attributes_field="attributes",
    vector_size=1536,
    index_name="default-entity-description",
)
description_embedding_store = LanceDBVectorStore(
    vector_store_schema_config=vector_store_config,
)
description_embedding_store.connect(
    collection_name="default-entity-description",
    db_uri=LANCEDB_URI,
)

print(f"Entity count: {len(entity_df)}")
entity_df.head()

relationship_df = pd.read_parquet(f"{INPUT_DIR}/{RELATIONSHIP_TABLE}.parquet")
relationships = read_indexer_relationships(relationship_df)

print(f"Relationship count: {len(relationship_df)}")
relationship_df.head()

# covariate_df = pd.read_parquet(f"{INPUT_DIR}/{COVARIATE_TABLE}.parquet")

# claims = read_indexer_covariates(covariate_df)

# print(f"Claim records: {len(claims)}")
# covariates = {"claims": claims}

report_df = pd.read_parquet(f"{INPUT_DIR}/{COMMUNITY_REPORT_TABLE}.parquet")
reports = read_indexer_reports(report_df, community_df, COMMUNITY_LEVEL)

print(f"Report records: {len(report_df)}")
report_df.head()

text_unit_df = pd.read_parquet(f"{INPUT_DIR}/{TEXT_UNIT_TABLE}.parquet")
text_units = read_indexer_text_units(text_unit_df)

print(f"Text unit records: {len(text_unit_df)}")
text_unit_df.head()


from graphrag.config.enums import ModelType
from graphrag.config.models.language_model_config import LanguageModelConfig
from graphrag.language_model.manager import ModelManager
from graphrag.tokenizer.tiktoken_tokenizer import TiktokenTokenizer

from dotenv import load_dotenv

load_dotenv(dotenv_path="graphrag/.env")

api_key = os.environ["GRAPHRAG_API_KEY"]
llm_model = os.environ["GRAPHRAG_QUERY_LLM_MODEL"]
embedding_model = os.environ["GRAPHRAG_EMBEDDING_MODEL"]

chat_config = LanguageModelConfig(
    api_key=api_key,
    type=ModelType.OpenAIChat,
    model=llm_model,
    max_retries=20,
)
chat_model = ModelManager().get_or_create_chat_model(
    name="local_search",
    model_type=ModelType.OpenAIChat,
    config=chat_config,
)

token_encoder = tiktoken.encoding_for_model(llm_model)
tokenizer = TiktokenTokenizer(encoding_name="cl100k_base")

embedding_config = LanguageModelConfig(
    api_key=api_key,
    type=ModelType.OpenAIEmbedding,
    model=embedding_model,
    max_retries=20,
)

text_embedder = ModelManager().get_or_create_embedding_model(
    name="local_search_embedding",
    model_type=ModelType.OpenAIEmbedding,
    config=embedding_config,
)

context_builder = LocalSearchMixedContext(
    community_reports=reports,
    text_units=text_units,
    entities=entities,
    relationships=relationships,
    # if you did not run covariates during indexing, set this to None
    covariates=None,
    entity_text_embeddings=description_embedding_store,
    embedding_vectorstore_key=EntityVectorStoreKey.ID,  # if the vectorstore uses entity title as ids, set this to EntityVectorStoreKey.TITLE
    text_embedder=text_embedder,
)


local_context_params = {
    "text_unit_prop": 0.5,
    "community_prop": 0.1,
    "conversation_history_max_turns": 5,
    "conversation_history_user_turns_only": True,
    "top_k_mapped_entities": 10,
    "top_k_relationships": 10,
    "include_entity_rank": True,
    "include_relationship_weight": True,
    "include_community_rank": True,
    "return_candidate_context": True,
    "embedding_vectorstore_key": EntityVectorStoreKey.ID,  # set this to EntityVectorStoreKey.TITLE if the vectorstore uses entity title as ids
    "max_tokens": 20_000,  # change this based on the token limit you have on your model (if you are using a model with 8k limit, a good setting could be 5000)
}

model_params = {
    "max_tokens": 2_000,  # change this based on the token limit you have on your model (if you are using a model with 8k limit, a good setting could be 1000=1500)
    "temperature": 0.0,
}

search_engine = LocalSearch(
    model=chat_model,
    context_builder=context_builder,
    tokenizer=tokenizer,
    model_params=model_params,
    context_builder_params=local_context_params,
    response_type="multiple paragraphs",  # free form text describing the response type and format, can be anything, e.g. prioritized list, single paragraph, multiple paragraphs, multiple-page report
)

async def main():
    output_dir = Path("output/graphrag2") #change this
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    Path(f"{output_dir}/code").mkdir(parents=True, exist_ok=True)
    Path(f"{output_dir}/dialog").mkdir(parents=True, exist_ok=True)

    # Process all kernels in cfile-small
    kernels_dir = Path("cfile-small")
    if not kernels_dir.exists():
        print("Error: cfile-small directory not found")
        return
    


    kernel_files = list(kernels_dir.glob("kernel_*.c"))
    print(f"Processing {len(kernel_files)} kernels from cfile-small/")

    prompt = """
      You are an expert in High-Level Synthesis (HLS) optimization. Given the following C code, your objective is to optimize the performance while keeping the resource utilization under 80% and
      the compilation time under 1800s. Here are the specific resource constraints for the target FPGA device (Xilinx Alveo U250):

      LUTs: 1,341K (i.e. 1,341,000)
      Registers (Flip-Flops / FFs): 2,749K (i.e. 2,749,000)
      DSP slices: 11,508
      Block RAM (36 Kb): 2,000 (i.e. 2,000 blocks of 36 Kb)
      UltraRAM (288 Kb): 1,280 (blocks)
      
      First, retrieve relevant pragma usage examples and summarize optimization strategies from the knowledge base. Then, insert the appropriate pragmas directly 
      into the C code. Return the final code with pragmas.
    """

    for i, kf in enumerate(kernel_files, 1):
        print(f"\n--- Processing {i}/{len(kernel_files)} ---")

        kernel_name = Path(kf).stem
        with open(kf, 'r', encoding='utf-8') as f:
            kernel_content = f.read()

        print(f"Read kernel file: {kf}")
        print(f"Content length: {len(kernel_content)} characters")

        result = await search_engine.search(prompt + "\n\n" + kernel_content)
        # print(result.response)
        retrieved_context = result.context_data

        # Ensure context directory exists
        context_dir = f"{output_dir}/context"
        Path(context_dir).mkdir(parents=True, exist_ok=True)

        # Check text units (likely contains raw code/pragmas/examples from docs)
        text_units_df = retrieved_context.get("text_units")
        if text_units_df is not None and not text_units_df.empty:
            print(f"Retrieved text units for {kernel_name}:")
            print(text_units_df[['id', 'text', 'entity_ids', 'relationship_ids']].head())  # Focus on key columns
            text_units_df.to_csv(f"{context_dir}/{kernel_name}_text_units.csv", index=False)
        else:
            print("No text units retrieved.")

        # Check entities (may include pragma descriptions/examples)
        entities_df = retrieved_context.get("entities")
        if entities_df is not None and not entities_df.empty:
            print(f"Retrieved entities for {kernel_name}:")
            print("Available columns:", entities_df.columns.tolist())
            print(entities_df.head())  # Print the full head to avoid column-specific errors
            entities_df.to_csv(f"{context_dir}/{kernel_name}_entities.csv", index=False)
            # Save pragma-specific descriptions to a text file
            pragma_examples = entities_df[entities_df['description'].str.contains('PRAGMA', case=False, na=False)]['description'].tolist()
            with open(f"{context_dir}/{kernel_name}_pragma_examples.txt", 'w', encoding='utf-8') as f:
                f.write('\n\n'.join(pragma_examples))
        else:
            print("No entities retrieved.")

        # Check relationships (connections between entities, possibly linking to examples)
        relationships_df = retrieved_context.get("relationships")
        if relationships_df is not None and not relationships_df.empty:
            print(f"Retrieved relationships for {kernel_name}:")
            print(relationships_df[['source', 'target', 'description', 'weight']].head())
            relationships_df.to_csv(f"{context_dir}/{kernel_name}_relationships.csv", index=False)
        else:
            print("No relationships retrieved.")

        # Reports (already checking, but for completeness)
        reports_df = retrieved_context.get("reports")
        if reports_df is not None and not reports_df.empty:
            print(f"Retrieved reports for {kernel_name}:")
            print(reports_df[['community_id', 'summary', 'full_content']].head())
            reports_df.to_csv(f"{context_dir}/{kernel_name}_reports.csv", index=False)
        else:
            print("No reports retrieved.")


        output_file = f"{output_dir}/code/{kernel_name}.c"
        with open(output_file, 'w') as f:
            f.write(result.response)
        print(f"Generated code file: {output_file}")

        dialog_file = f"{output_dir}/dialog/{kernel_name}_dialog.txt"
        with open(dialog_file, 'w') as f:
            f.write(result.response)
        print(f"Generated dialog file: {dialog_file}")


if __name__ == "__main__":
    asyncio.run(main())
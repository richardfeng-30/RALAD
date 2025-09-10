# split text into chunks, convert text to numerical embeddings, return top 3 similar docs

from langchain.callbacks.manager import CallbackManager
from langchain.callbacks.streaming_stdout import StreamingStdOutCallbackHandler
from langchain.chains import LLMChain
from langchain.prompts import PromptTemplate

from langchain_community.document_loaders.generic import GenericLoader
from langchain_community.document_loaders.parsers import LanguageParser
from langchain_text_splitters import Language
import numpy as np

from typing import List, Optional
from llama import Dialog, Llama


# repo_path = "/home/haotian/Splitted_code"

# loader = GenericLoader.from_filesystem(
#     repo_path,
#     glob="**/*",
#     suffixes=[".cpp"],
#     # exclude=["**/non-utf8-encoding.py"],
#     parser=LanguageParser(language=Language.CPP, parser_threshold=500),
# )
# documents = loader.load()
repo_path_2 = "documents"

loader2 = GenericLoader.from_filesystem(
    repo_path_2,
    glob="**/*",
    suffixes=[".txt"],
    # exclude=["**/non-utf8-encoding.py"],
    parser=LanguageParser(language=Language.CPP, parser_threshold=500),
)
documents2 = loader2.load()

documents = documents2
print(len(documents))
from langchain_text_splitters import RecursiveCharacterTextSplitter

python_splitter = RecursiveCharacterTextSplitter.from_language(
    language=Language.CPP, chunk_size=1000, chunk_overlap=200
)
texts = python_splitter.split_documents(documents)
len(texts)
from sentence_transformers import SentenceTransformer, util
model = SentenceTransformer("flax-sentence-embeddings/st-codesearch-distilroberta-base")
import numpy as np
np.array(model.encode(texts[0].page_content).shape)
txt = []
for text in texts:
    txt.append(np.array(model.encode(text.page_content)))

# encode query into embedding and return top 3 similar documents
def select_sentenceembedding(query, ckpt_dir=None, tokenizer_path=None):
    k = 5
    query_embed = model.encode(query)
    score = []
    for t in txt:
        score.append(query_embed.T @ t )
        # score.append(np.linalg.norm(query_embed - t))
    indexed_list = sorted(enumerate(score), key=lambda x: x[1], reverse= True)
    # indexed_list = sorted(enumerate(score), key=lambda x: x[1], reverse= False)
    sorted_indices = [index for index, value in indexed_list]
    top_k_index = sorted_indices[:k]
    top_k_doc = []
    for i in top_k_index:
        top_k_doc.append(texts[i])
    r_doc = []
    for t in top_k_doc:
        relevance_result = self_rag(t.page_content, query, ckpt_dir, tokenizer_path)
        if "yes" in relevance_result.lower():
            r_doc.append(t.page_content)
    r_doc = r_doc[:3]
    print(len(r_doc))
    return r_doc

# Global model instance
_llama_generator = None

def initialize_llama_model(ckpt_dir, tokenizer_path):
    """Initialize the Llama model once and store it globally."""
    global _llama_generator
    if _llama_generator is None:
        _llama_generator = Llama.build(
            ckpt_dir=ckpt_dir,
            tokenizer_path=tokenizer_path,
            max_seq_len=8192,
            max_batch_size=1,
        )
    return _llama_generator

def self_rag(document, question, ckpt_dir=None, tokenizer_path=None):
    if ckpt_dir is None:
        ckpt_dir = "/home/haochx5/llama3/Meta-Llama-3-8B-Instruct"
    if tokenizer_path is None:
        tokenizer_path = "/home/haochx5/llama3/Meta-Llama-3-8B-Instruct/tokenizer.model"
    
    system = """You are a grader assessing relevance of a retrieved document to a user question. \n 
    If the document contains keyword(s) or semantic meaning related to the question, grade it as relevant. \n
    Give a binary score 'yes' or 'no' score to indicate whether the document is relevant to the question. ONLY OUTPUT ONE WORD."""


    # create dialog for llama
    dialogs: List[Dialog] = [
        [
            {"role": "system", "content": system},
            {"role": "user", "content": f"Retrieved document: \n\n {document} \n\n User question: Optimize the following code by inserting pragma HLS lines. \n {question}"},
        ],
    ]

    # Use the global generator instance
    generator = initialize_llama_model(ckpt_dir, tokenizer_path)
    
    results = generator.chat_completion(
        dialogs,
        max_gen_len=16,
        temperature=0.6,
        top_p=0.9,
    )

    print(f"Generated response: {results[0]['generation']['content']}")
    
    return results[0]['generation']['content']

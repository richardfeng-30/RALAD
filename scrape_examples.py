import os
import re
from pathlib import Path

ROOT = Path(__file__).parent
EXAMPLES_ROOT = ROOT / "Vitis-HLS-Introductory-Examples-master"
OUTPUT_FILE = ROOT / "documents" / "hls_intro_examples.txt"

README_CANDIDATES = {"README", "README.txt", "README.md"}
# Only source file extensions (exclude headers intentionally)
SRC_EXTS = {".c", ".cc", ".cpp", ".cxx"}

# Filenames (case-insensitive) to exclude if substring appears
EXCLUDE_NAME_SUBSTR = (
	"test",  # unit or host test harness
	"host",  # host code
	"tb",    # testbench
	"main",  # generic main wrappers
)

def _is_candidate_source(name: str) -> bool:
	low = name.lower()
	if Path(name).suffix.lower() not in SRC_EXTS:
		return False
	if any(sub in low for sub in EXCLUDE_NAME_SUBSTR):
		return False
	return True

def _score_kernel_file(path: Path) -> int:
	"""Heuristic score to pick the most relevant HLS kernel source file."""
	score = 0
	name_low = path.name.lower()
	try:
		text = path.read_text(encoding="utf-8", errors="ignore")
	except Exception:
		text = ""
	if "#pragma HLS" in text:
		score += 10  # contains HLS pragmas
	if "kernel" in name_low:
		score += 3
	if "top" in name_low:
		score += 2
	# Favor cpp over c for richer constructs (arbitrary)
	if path.suffix == ".cpp":
		score += 1
	# Slight preference for shorter filenames (less likely to be test harness)
	score -= len(name_low) // 20
	return score


def find_examples(base: Path):
	"""Yield directories that contain a README and one selected HLS kernel source file.

	For each qualifying directory we choose a single 'kernel' file using heuristics:
	  - must be a source file (no headers)
	  - exclude names containing test/host/tb/main
	  - prefer files containing '#pragma HLS'
	  - prefer names containing 'kernel' or 'top'
	  - prefer .cpp over .c if otherwise tied
	"""
	for dirpath, dirnames, filenames in os.walk(base):
		dir_path = Path(dirpath)
		# Skip hidden directories
		if any(part.startswith('.') for part in dir_path.relative_to(base).parts):
			continue
		name_set = set(filenames)
		readme_file = next((dir_path / cand for cand in README_CANDIDATES if cand in name_set), None)
		if not readme_file:
			continue
		candidates = [dir_path / f for f in filenames if _is_candidate_source(f)]
		if not candidates:
			continue
		# Score candidates
		best = max(candidates, key=_score_kernel_file)
		yield dir_path, readme_file, best


def collect():
	if not EXAMPLES_ROOT.exists():
		raise SystemExit(f"Examples root not found: {EXAMPLES_ROOT}")

	examples = list(find_examples(EXAMPLES_ROOT))
	# Sort deterministically by relative path
	examples.sort(key=lambda tup: str(tup[0].relative_to(EXAMPLES_ROOT)))

	lines = []
	lines.append("")
	for dir_path, readme, kernel_file in examples:
		rel = dir_path.relative_to(EXAMPLES_ROOT)
		lines.append("=" * 80)
		lines.append(f"Example: {rel}")
		lines.append("=" * 80)
		# README
		lines.append("--- README ---")
		try:
			readme_txt = readme.read_text(encoding="utf-8", errors="replace")
			readme_txt = readme_txt.rstrip()
		except Exception as e:
			readme_txt = f"[Could not read README: {e}]"
		# Sanitize README to remove license/copyright fragments
		readme_txt = readme_txt.replace('\r','')
		readme_txt = re.sub(r"/\*.*?\*/", lambda m: "" if re.search(r"(?i)(copyright|apache|xilinx|advanced micro devices|all rights reserved)", m.group(0)) else m.group(0), readme_txt, flags=re.DOTALL)
		license_line_re = re.compile(r"(?i)(copyright|apache license|xilinx|advanced micro devices|all rights reserved|confidential|warranties|disclaimer|liability|critical applications)")
		lines_raw = readme_txt.split('\n')
		# Remove 'Files Included in this Package' sections
		out_lines = []
		i = 0
		while i < len(lines_raw):
			ln = lines_raw[i]
			if re.match(r"(?i)^\s*(files\s+)?included\s+in\s+this\s+package", ln.strip()):
				# Skip this line and subsequent non-blank lines (list) until blank line or heading like 'Running'
				i += 1
				while i < len(lines_raw):
					ln2 = lines_raw[i]
					if ln2.strip() == '':
						i += 1
						break
					if re.match(r"(?i)^\s*(running|description|use )", ln2.strip()):
						break
					# Continue skipping file list lines
					i += 1
				continue
			# Keep other lines (will filter licenses later)
			out_lines.append(ln)
			i += 1
		# Now filter license lines
		filtered_lines = [ln for ln in out_lines if not license_line_re.search(ln)]
		# Collapse multiple blank lines
		collapsed = []
		for ln in filtered_lines:
			if ln.strip() == '' and (not collapsed or collapsed[-1].strip() == ''):
				continue
			collapsed.append(ln)
		readme_txt = '\n'.join(collapsed).strip()
		lines.append(readme_txt)
		# Kernel source file
		lines.append("")
		lines.append(f"--- KERNEL FILE: {kernel_file.name} ---")
		try:
			raw = kernel_file.read_text(encoding="utf-8", errors="replace")
		except Exception as e:
			raw = f"[Could not read file: {e}]"
		# Strip leading multi-line or single-line license header if present
		def strip_license(text: str) -> str:
			# Remove leading whitespace
			idx = 0
			length = len(text)
			# Helper to skip a leading C-style block comment if it seems like a license
			def skip_block(i):
				if not text.startswith("/*", i):
					return i
				end = text.find("*/", i + 2)
				if end == -1:
					return i
				block = text[i:end+2]
				lic_keywords = ("copyright", "apache", "all rights reserved")
				if any(k in block.lower() for k in lic_keywords):
					# Skip following newlines
					j = end + 2
					while j < length and text[j] in "\r\n" :
						j += 1
					return j
				return i
			# Helper to skip consecutive // lines forming a header
			def skip_slash_lines(i):
				start = i
				lines = []
				while True:
					j = i
					while j < length and text[j] in ' \t':
						j += 1
					if not text.startswith("//", j):
						break
					line_end = text.find("\n", j)
					if line_end == -1:
						line_end = length
					lines.append(text[j:line_end])
					i = line_end + 1
				# Decide if it looks like a license
				joined = "\n".join(lines).lower()
				lic_keywords = ("copyright", "apache", "all rights reserved")
				if lines and any(k in joined for k in lic_keywords):
					while i < length and text[i] in "\r\n":
						i += 1
					return i
				return start
			# Skip initial whitespace
			while idx < length and text[idx] in "\r\n\t \f":
				idx += 1
			# Attempt block comment removal
			ni = skip_block(idx)
			if ni != idx:
				idx = ni
			else:
				ni = skip_slash_lines(idx)
				if ni != idx:
					idx = ni
			return text[idx:]
		content = strip_license(raw)
		# Remove any remaining license style block comments anywhere in body
		content = re.sub(r"/\*.*?\*/", lambda m: "" if re.search(r"(?i)(copyright|apache|xilinx|advanced micro devices|all rights reserved)", m.group(0)) else m.group(0), content, flags=re.DOTALL)
		# Remove line comments with license keywords
		license_line_re = re.compile(r"(?i)(copyright|apache license|xilinx|advanced micro devices|all rights reserved|confidential|warranties|disclaimer|liability|critical applications)")
		filtered = []
		for ln in content.splitlines():
			if license_line_re.search(ln):
				continue
			filtered.append(ln)
		# Collapse excessive blank lines
		clean = []
		for ln in filtered:
			if ln.strip() == '' and (not clean or clean[-1].strip() == ''):
				continue
			clean.append(ln)
		content = '\n'.join(clean).rstrip()
		lines.append(content)
		lines.append("")
		lines.append("")

	OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
	OUTPUT_FILE.write_text("\n".join(lines) + "\n", encoding="utf-8")
	print(f"Wrote {len(examples)} examples (one kernel file each) to {OUTPUT_FILE}")


if __name__ == "__main__":
	collect()


import argparse
import re
from pathlib import Path
from typing import Tuple
from PIL import Image



def sanitize_name(name: str) -> str:
	name = re.sub(r"[^0-9a-zA-Z_]", "_", name)
	if re.match(r"^[0-9]", name):
		name = "img_" + name
	return name


def image_to_matrix(img_path: Path) -> Tuple[int, int, list]:
	im = Image.open(img_path).convert("1", dither=Image.NONE)
	w, h = im.size
	pixels = list(im.getdata())
	matrix = []
	for y in range(h):
		row = []
		for x in range(w):
			p = pixels[y * w + x]
			v = 255 if p else 0
			row.append(v)
		matrix.append(row)
	return w, h, matrix


def write_aggregate_files(base: str, entries: list, out_dir: Path) -> None:
	h_name = f"{base}.h"
	c_name = f"{base}.c"
	guard = f"_{base.upper()}_H_"

	h_path = out_dir / h_name
	c_path = out_dir / c_name

	with open(h_path, "w", encoding="utf-8") as fh:
		fh.write(f"#ifndef {guard}\n")
		fh.write(f"#define {guard}\n\n")
		fh.write("#include <stdbool.h>\n\n")
		for name, w, h, _ in entries:
			symbol = f"{base}_{name}"
			fh.write(f"extern const bool {symbol}[{h}][{w}];\n")
		fh.write("\n#endif /* {guard} */\n")

	with open(c_path, "w", encoding="utf-8") as fc:
		fc.write(f"#include \"{h_name}\"\n\n")
		for name, w, h, matrix in entries:
			symbol = f"{base}_{name}"
			fc.write(f"const bool {symbol}[{h}][{w}] = {{\n")
			for row in matrix:
				row_s = ", ".join(str(1 if v else 0) for v in row)
				fc.write(f"    {{ {row_s} }},\n")
			fc.write("};\n\n")


def main() -> None:
	parser = argparse.ArgumentParser(description="Convert images to C .h/.c arrays (uint8_t)")
	parser.add_argument("-i", "--input", required=True, help="Input folder containing images")
	parser.add_argument("-o", "--output", default=".", help="Output folder for generated .h/.c files")
	args = parser.parse_args()

	inp = Path(args.input)
	out = Path(args.output)
	if not inp.is_dir():
		raise SystemExit(f"Input folder not found: {inp}")
	out.mkdir(parents=True, exist_ok=True)

	exts = {".png", ".bmp", ".jpg", ".jpeg", ".gif"}

	entries = []
	for p in sorted(inp.iterdir()):
		if not p.is_file():
			continue
		if p.suffix.lower() not in exts:
			continue
		base = p.stem
		name = sanitize_name(base)
		w, h, matrix = image_to_matrix(p)
		entries.append((name, w, h, matrix))

	if not entries:
		print(f"No images found in {inp}")
		return

	base_name = sanitize_name(inp.name)
	write_aggregate_files(base_name, entries, out)
	print(f"Wrote {base_name}.h and {base_name}.c with {len(entries)} images")


if __name__ == "__main__":
	main()


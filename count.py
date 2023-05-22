import os

def count_lines_of_code(directory):
    total_lines = 0
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".c"):
                filepath = os.path.join(root, file)
                with open(filepath, "r") as f:
                    lines = len(f.readlines())
                    total_lines += lines
    return total_lines

directory = "./"
lines_of_code = count_lines_of_code(directory)
print(f"Total lines of code in {directory}: {lines_of_code}")


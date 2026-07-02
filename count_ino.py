import pathlib

def count_sketches():
    # Scan for all files ending with .ino recursively
    ino_files = list(pathlib.Path('.').rglob('*.ino'))
    print("=" * 40)
    print(f"Total Arduino (.ino) files found: {len(ino_files)}")
    for index, file_path in enumerate(ino_files, 1):
        print(f"  {index}. {file_path}")
    print("=" * 40)

if __name__ == '__main__':
    count_sketches()

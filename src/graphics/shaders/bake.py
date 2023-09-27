import pathlib

directory = pathlib.Path(__file__).parent.absolute()


def processFile(file: pathlib.Path) -> str:
    with open(file, "r") as f:
        return f"const char {file.name.replace('.', '_')}[] = R\"({f.read()})\";\n"


with open(directory / "shaders.inc", "w") as f:
    f.write("".join(processFile(file) for file in directory.iterdir()))

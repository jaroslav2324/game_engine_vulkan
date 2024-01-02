
# Count amount of code strings in src folder(*.c and *.cpp files)

import os
import pathlib

linesAmount = 0

rootPath = pathlib.Path(__file__).parent.resolve()
rootPath = os.path.join(rootPath, 'src')

for root, dirs, files in os.walk(rootPath):
   for name in files:
        nameList = name.split(".")
        if (nameList[-1] != "cpp" and nameList[-1] != "c"):
            continue
        filePath = os.path.join(root, name)
        with open(filePath, 'r') as f:
            for line in f:
                linesAmount += 1


print(linesAmount, end="")
print(" - src")

# Count amount of code strings in include/eng folder(*.h and *.hpp files)

linesAmount = 0

rootPath = pathlib.Path(__file__).parent.resolve()
rootPath = os.path.join(rootPath, 'include/eng')

for root, dirs, files in os.walk(rootPath):
   for name in files:
        nameList = name.split(".")
        if (nameList[-1] != "hpp" and nameList[-1] != "h"):
            continue
        filePath = os.path.join(root, name)
        with open(filePath, 'r') as f:
            for line in f:
                linesAmount += 1

print(linesAmount, end="")
print(" - headers")
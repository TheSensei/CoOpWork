#!/usr/bin/python
#
# Firmware image tagger
#
# Tags a firmware image file with a version and removes unwanted data records.
#

from sys import stderr, argv
from getopt import getopt, GetoptError




usageMessage = """
Firmware image tagger.

Usage:
    %s -v version -i infile [-o outfile]
    %s -v version [-o outfile] infile
    %s -v version -i infile > outfile
    %s -h | --help

Options:
    -h --help      Show this help message.
    -v --version   Tag file with given version.
    -i --infile    Read from given input file.
    -o --infile    Write to given output file.
    -r --retag     Allow retagging.
""" % (argv[0], argv[0], argv[0], argv[0])




def usage(reason=None):
    if reason:
        print("\nError: %s" % reason)

    print(usageMessage)




try:
    options, remainder = getopt(argv[1:], "hv:i:o:r",
                        ["help", "version=", "infile=", "outfile=", "retag"])
except GetoptError:
    usage("Invalid arguments")
    exit(1)

version = None
inPath = None
outPath = None
allowRetag = False

for option, argument in options:
    if option in ("h", "--help"):
        usage()
        exit(1)
    if option in ("-v", "--version"):
        version = argument
    if option in ("-i", "--infile"):
        inPath = argument
    if option in ("-o", "--outfile"):
        outPath = argument
    if option in ("-r", "--retag"):
        allowRetag = True

if not inPath and len(remainder) == 1:
    inPath = remainder[0]

if inPath and not outPath:
    outPath = inPath

if not version or not inPath:
    usage("Did not supply a required option or argument")
    exit(1)

try:
    with open(inPath, "r") as imageFile:
        data = imageFile.read()
except IOError:
    print("Error: Cannot read input file")
    exit(1)

lines = data.splitlines()
hexLines = [line for line in lines if line.startswith(":")]
tagLines = [line for line in lines if not line.startswith(":")]

if len(tagLines) != 0 and not allowRetag:
    print("Error: File has already been tagged.")
    print("       Use --retag to allow retagging")
    exit(1)

removing = []
baseAddress = 0x0

# Find data records we want to remove.
for wholeLine in hexLines:
    line = wholeLine[1:]

    byteCount = int(line[0:2], 16)
    recordType = int(line[6:8], 16)

    charCount = 2 * byteCount

    if charCount != 0:
        recordData = int(line[8 : (8 + 2 * byteCount)], 16)
    else:
        recordData = 0

    if recordType == 04:
        baseAddress = 64 * 1024 * recordData
        continue

    address = baseAddress + int(line[2:6], 16)
    word = address / 2

    # We want to remove records at these predetermined locations.
    if word == 0xabf0 or word == 0xabfc or word == 0xabfe:
        removing += [wholeLine]

hexLines = [line for line in hexLines if line not in removing]

hexLines.reverse()
reversedLines = hexLines
hexLines = []
foundData = False

# Keep the EOF record.
hexLines = [reversedLines[0]]

# Skip each line until we find something that's not a type 04 record. Then copy
# all remaining lines. This only eliminates 04 records at the end of the file.
for i in range(1, len(reversedLines)):
    if not foundData:
        if int(reversedLines[i][7:9], 16) == 4:
            continue
        else:
            foundData = True

    hexLines += [reversedLines[i]]

hexLines.reverse()

data = "\n".join(hexLines) + "\n"
data = "version: %s\n" % version + data

try:
    with open(outPath, "w") as imageFile:
        imageFile.write(data)
except IOError:
    print("Error: Cannot write output file")
    exit(1)

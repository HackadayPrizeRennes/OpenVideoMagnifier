#!/usr/bin/python
import ATD
import sys

f = ""
if len(sys.argv) != 2:
    print("Give just one file")
else:
    f = sys.argv[1]
    toCorrectFile = open(f, 'r')
    toCorrect = toCorrectFile.read()
    print("Input : " + toCorrect)
    ATD.setDefaultKey("OpenVideoMagnifier")
    errors = ATD.checkDocument(toCorrect)
    correction = toCorrect
    for error in errors:
        correction = correction.replace(error.string, error.suggestions[0],1)
    print("Output : " + correction)
    toCorrectFile.close()
    toCorrectFile = open(f, 'w')
    toCorrectFile.write(correction)
    toCorrectFile.close()

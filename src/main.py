#! /usr/bin/env python3

import parser as parser
from joint import Joint
from scriptWriter import ScriptWriter
import sys


def main():
    root = parser.parse(sys.argv[1]) if len(sys.argv) > 1 else parser.parse("../bvhFiles/walk.bvh")
    scriptWriter = ScriptWriter(root)
    scriptWriter.writeScript()
    # parser.printH(root)


main()
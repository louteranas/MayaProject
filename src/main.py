#! /usr/bin/env python3

import parser as parser
from joint import Joint
from scriptWriter import ScriptWriter
import sys


def main():
    root = parser.parse(sys.argv[1])
    scriptWriter = ScriptWriter(root)
    scriptWriter.writeScript()
    # parser.printH(root)

main()
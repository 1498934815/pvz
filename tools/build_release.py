#!/usr/bin/env python3
#-*- coding: utf-8 -*-
"""
File    : tools/build_release.py
Project :
Author  : ze00
Email   : zerozakiGeek@gmail.com
Date    : 2019-03-24
"""
import zipfile, os, sys
from os import path
tools = path.dirname(__file__)
out = path.join(tools, os.pardir, 'out')
signapk = path.join(tools, 'uber-apk-signer-1.0.0.jar')
version = sys.argv[1]
def build_release(arg):
    (origin, prefix, lib) = arg.split(':')
    tempf = '/tmp/{}_{}.apk'.format(path.basename(origin), version)
    os.system('cp {} {}'.format(origin, tempf))
    o = zipfile.ZipFile(tempf, mode='a')
    for i in lib.split(','):
        o.write(i, path.join(prefix, i))
    o.close()
    os.system('java  -jar {} -a {} -o {}'.format(signapk, tempf, out))
    os.remove(tempf)
for i in sys.argv[2:]:
    unsigned = build_release(i)

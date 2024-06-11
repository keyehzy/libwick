#!/usr/bin/env python3

import os
import subprocess

ROOT = subprocess.check_output(['git', 'rev-parse', '--show-toplevel']).decode().strip()
main_example_file = os.path.join(ROOT, "examples", "main.cpp")
readme_file = os.path.join(ROOT, "README.md")

text = open(main_example_file, 'r').read()
readme_text = open(readme_file, 'r').read()

if not text in readme_text:
   print(f"Text in README does not match '{main_example_file}'.")
   exit(1)


# Copyright (c) 2024 Matheus Sousa
# SPDX-License-Identifier: BSD-2-Clause

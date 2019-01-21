#!/usr/bin/env python3

import sys
import shutil
import os


def preprocess_file(file_path, m_vars):
    with open(file_path, 'r') as file:
        file_data = file.read()

    for k, v in m_vars.items():
        file_data = file_data.replace('%' + k + '%', v)

    with open(file_path, 'w') as file:
        file.write(file_data)


def main():
    if len(sys.argv) < 3:
        return

    path = sys.argv[1]
    name = sys.argv[2]

    folder = path + '/' + name
    shutil.copytree('template', folder)

    m_vars = {
        'A2D_PROJECT_NAME': name,
        'A2D_ENGINE_PATH': os.path.relpath('../', folder).replace('\\', '/')
    }

    for root, dirs, files in os.walk(folder):
        for file in files:
            if file == "CMakeLists.txt":
                preprocess_file(os.path.join(root, file), m_vars)


if __name__ == "__main__":
    main()

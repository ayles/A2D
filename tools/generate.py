#!/usr/bin/env python3

import shutil
import os
import argparse


def preprocess_file(file_path, m_vars):
    with open(file_path, 'r') as file:
        file_data = file.read()

    for k, v in m_vars.items():
        file_data = file_data.replace('%' + k + '%', v)

    with open(file_path, 'w') as file:
        file.write(file_data)


def main():
    parser = argparse.ArgumentParser(description="A2D project generator")

    parser.add_argument('-d', '--dest', help='destination folder', default='.')
    parser.add_argument('name', help='project name')

    args = parser.parse_args()

    path = args.dest
    name = args.name

    folder = path + '/' + name

    if os.path.exists(folder):
        print('Project', name, 'exists. Overwriting projects files...')
        shutil.rmtree(folder + '/projects')
        shutil.copytree('template/projects', folder + '/projects')
    else:
        print('Generating project ' + name + '...')
        shutil.copytree('template', folder)

    m_vars = {
        'A2D_PROJECT_NAME': name,
        'A2D_ENGINE_PATH': os.path.relpath('../', folder).replace('\\', '/')
    }

    print('Substituting vars...')

    for root, dirs, files in os.walk(folder):
        for file in files:
            if file == "CMakeLists.txt":
                preprocess_file(os.path.join(root, file), m_vars)

    print('Done.')


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
import csv
import itertools
import random
import subprocess
import os
import sys
from multiprocessing.pool import ThreadPool
import shutil
import glob
import argparse
import yaml
from collections import defaultdict 
from pathlib import Path

my_env = os.environ.copy()
script_path = Path(__file__).parent.absolute()

parser = argparse.ArgumentParser()
parser.add_argument("config", type=str, help="The name of the yaml config file used for simulation")
args = parser.parse_args()

with open(args.config, 'r') as stream:
    try:
        config = yaml.safe_load(stream)
    except yaml.YAMLError as exc:
        print(exc)

PWD = os.path.dirname(os.path.abspath(__file__))

random.seed("RG,IGL,OS")

ignored_keys = set({"version", "script", "repeats", "path", "name"})
options_names = []
options_values = []

for key, value in config.items():
    if key in ignored_keys:
        continue
    print("--%s: %s" % (key, value))
    options_names.append(key)
    options_values.append(value)

repeats = config["repeats"]

options_names.append('RngRun')
options_values.append(list(range(repeats)))

script_name = config["script"]
simulation_name = config["name"]

combinations = list(itertools.product(*options_values))
print("%s combinations. Will take some time…" % (len(combinations)))

def work(combination):
    arguments = []
    filename_pieces = [script_name, str(repeats)]
    for i, value in enumerate(combination):
        arguments.append("--%s=%s" % (options_names[i], value))
        filename_pieces.append("%s=%s" % (options_names[i], value))

    filename = "_".join(filename_pieces).replace("/", "-")
    cli_string = " ".join(arguments)
    path = "%s/OUTPUT/%s" % (script_path, simulation_name)
    if not os.path.isdir(path):
        try:
            print ("Creating the directory as it appears not to exist")
            os.mkdir(path)
        except OSError as e:
            print ("Creation of the directory %s failed" % path)
            print("%s" % e)
            exit(0)

    filenames = defaultdict(list)
    finished_filename = "%s/%s_%s.csv" % (path, "finished", filename)
    print(finished_filename)
    exists = os.path.isfile(finished_filename)

    if exists:
        print("Skipping %s" % finished_filename)
        return

    command_line = ['./%s %s' % (script_name, cli_string)]
    try:
        print(" ".join(command_line))
        my_subprocess = subprocess.check_output(command_line, cwd='./', env=my_env, shell = True, stderr=subprocess.STDOUT)
        output = my_subprocess.decode("utf-8")
    except subprocess.CalledProcessError as grepexc:
        output = grepexc.output.decode("utf-8")

    for line in output.split("\n"):
        splitted = line.split(",")
        if(len(splitted) > 1):
            tag = splitted[0]
            filenames[tag].append(splitted[1:])

    for group in filenames:
        full_path = "%s/%s_%s.csv" % (path, group, filename)
        with open(full_path, "w") as csvfile:
            writer = csv.writer(csvfile, delimiter=",")
            for line in filenames[group]:
                writer.writerow(line)
            csvfile.flush()
            csvfile.close()
    Path(finished_filename).touch()

num = None
tp = ThreadPool(num)

random.shuffle(combinations)

for combination in combinations:
    tp.apply_async(work, (combination,))

tp.close()
tp.join()

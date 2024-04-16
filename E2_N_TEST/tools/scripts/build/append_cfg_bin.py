import sys
import re
import os
import io
import fileinput

class ROM:
    def __init__(self, file="", name="", begin_address=0):
        self.file = file
        self.name = name
        self.begin_address = begin_address

class REGION:
    def __init__(self, name="", address_type=""):
        self.name = name
        self.address_type = address_type
        self.rom = []

    def add_rom(self, file="", name="", begin_address=0):
        child = ROM(file, name, begin_address)
        self.rom.append(child)
        return child

class CFG:
    def __init__(self, name=""):
        self.name = name
        self.region = []

    def add_region(self, name=""):
        child = REGION(name)
        self.region.append(child)
        return child

    def get_region(self, name):
        for region in self.region:
            if region.name == name:
                return region

    def parse(self, file_name=""):
        fcfg = open(file_name, "r")
        lines = fcfg.readlines()
        fcfg.close()
        for line in lines:
            regx = re.compile(r'^([a-z]+)_region:')
            result = regx.match(line.rstrip('\n'))
            if result:
                region = self.add_region(result.group(1))
                continue
            regx = re.compile(r'\s+address_type:\s([a-z]+)')
            result = regx.match(line.rstrip('\n'))
            if result:
                region.address_type = result.group(1)
                continue
            regx = re.compile(r'\s+- rom:')
            result = regx.match(line.rstrip('\n'))
            if result:
                rom = region.add_rom()
                continue
            regx = re.compile(r'\s+file:\s(\S+)')
            result = regx.match(line.rstrip('\n'))
            if result:
                rom.file = result.group(1)
                continue
            regx = re.compile(r'\s+name:\s(\S+)')
            result = regx.match(line.rstrip('\n'))
            if result:
                rom.name = result.group(1)
                continue
            regx = re.compile(r'\s+begin_address:\s(\S+)')
            result = regx.match(line.rstrip('\n'))
            if result:
                rom.begin_address = result.group(1)
                continue
 
# =========================================================================================================
if len(sys.argv) != 4:
    print("Usage:\n python ./append_cfg_bin.py source_cfg append_cfg output_cfg")
    exit(1)

print("append_cfg_bin.py")
source_cfg_file = sys.argv[1]
append_cfg_file = sys.argv[2]
output_cfg_file = sys.argv[3]

#parse append config
append_cfg = CFG("append")
append_cfg.parse(append_cfg_file)

#output config
fcfg = open(source_cfg_file, "r")
lines = fcfg.readlines()
fcfg.close()
fcfg = open(output_cfg_file, "w")
region = None
for line in lines:
    fcfg.write(line)
    if region == None:
        regx = re.compile(r'^([a-z]+)_region:')
        result = regx.match(line.rstrip('\n'))
        if result:
            region = append_cfg.get_region(result.group(1))
            if region:
                continue
    else:
        regx = re.compile(r'\s+rom_list:')
        result = regx.match(line.rstrip('\n'))
        if result:
            for rom in region.rom:
                fcfg.write("        - rom:\n")
                fcfg.write("            file: " + rom.file + "\n")
                fcfg.write("            name: " + rom.name + "\n")
                fcfg.write("            begin_address: " + rom.begin_address + "\n")
            region = None
fcfg.close()

print("done")

#ipyxact example. Parses an IP-XACT XML file called generic_example.xml
#and prints out a C header of the register maps found
import argparse
import sys
import time

from ipyxact.ipyxact import Component

def parse_args():
    parser = argparse.ArgumentParser(
            description='Generate a Chisel code from an IP-XACT file')
    parser.add_argument('ipxact_file', help='IP-XACT file to parse')
    parser.add_argument('-o', dest='output_file', help='Write output to file')
    parser.add_argument('-g', '--header-guard', dest='header_guard',
            help='Header guard. If not specified it will be derived from the '
                 'output file name else there will be no guard.')
    return parser.parse_args();

def open_output(output):
    return open(output, 'w') if output else sys.stdout

# class PeripheralMasterIf extends Bundle {
# val in_data  = UInt(INPUT, 32)
# val in_ready = UInt(INPUT, 1)
# val out_data = UInt(OUTPUT, 32)
# val out_ready = UInt(OUTPUT, 32)
# }

# <spirit:register>
#   <spirit:name>output_ready</spirit:name>
#   <spirit:displayName>Output ready flag</spirit:displayName>
#   <spirit:description>When this flag is raised the output should be done</spirit:description>
#   <spirit:addressOffset>0x18</spirit:addressOffset>
#   <spirit:size>32</spirit:size>
#   <spirit:volatile>false</spirit:volatile>
#   <spirit:access>read-only</spirit:access>
# </spirit:register>

def write_scala_interface(of, memory_maps, offset=0, name=None):
    registers = []

    for m in memory_maps.memoryMap:
        if name:
            mname=name.upper()
        else:
            mname = m.name.upper()
        multiblock = len(m.addressBlock) > 1

        for block in m.addressBlock:
            if multiblock:
                bname = mname + '_' + block.name.upper()
            else:
                bname = mname

            for reg in sorted(block.register, key=lambda addr: addr.addressOffset):
                reg_name = '{}_{}'.format(bname, reg.name.upper().replace('-', '_'))

                if reg.access == "read-only":
                    direction = "INPUT"
                elif reg.access == "write-only":
                    direction = "OUTPUT"
                else:
                    print("Invalid direction: " + reg.access)
                    raise ValueError(reg.access)

                registers.append('\tval {} = UInt({}, {})\n'.format(reg_name,direction,reg.size))

    of.write('class {} extends Bundle {{\n'.format(bname))
    of.write(''.join(registers))
    of.write('}\n')


if __name__ == '__main__':
    args = parse_args()
    with open(args.ipxact_file) as f:
        name = None
        offset = 0

        component = Component()
        component.load(f)

        with open_output(args.output_file) as of:
            # write_prologue(of, args);
            write_scala_interface(of, component.memoryMaps, offset, name)
            # write_epilogue(of, args);

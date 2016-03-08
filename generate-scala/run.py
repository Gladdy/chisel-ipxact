import argparse
import sys
import time

from ipyxact.ipyxact import Component
from ipyxact.gen_scala import *

def parse_args():
    parser = argparse.ArgumentParser(description='Generate a Chisel code from an IP-XACT file')

    parser.add_argument('ipxact_files', nargs='*', action='append', help='IP-XACT files to parse')

    parser.add_argument('-t', dest='output_type', help='Type of output to generate, [peripheral_interface, slave_interface, access_code]')
    parser.add_argument('-o', dest='output_file', help='Write output to file')

    return parser.parse_args();

def open_output(output):
    return open(output, 'w') if output else sys.stdout


if __name__ == '__main__':
    args = parse_args()

    components = analyze_ipxact_files(args.ipxact_files[0])

    with open_output(args.output_file) as of:

        if args.output_type == "all" or args.output_type == "peripheral_interface":
            write_peripheral_interface(of, components)

        if args.output_type == "all" or args.output_type == "slave_interface":
            write_slave_interface(of, components)

        if args.output_type == "all" or args.output_type == "access_code":
            write_access_code(of, components)

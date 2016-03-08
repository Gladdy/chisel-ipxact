from jinja2 import Environment, PackageLoader
from ipyxact.ipyxact import Component
from ipyxact.gen_scala import *

import argparse
import sys
import time

def open_output(output):
    return open(output, 'w') if output else sys.stdout

def parse_args():
    parser = argparse.ArgumentParser(description='Generate Chisel glue logic based on IPXACT specifications of interfaces')
    parser.add_argument('ipxact_files', nargs='*', action='append', help='IP-XACT files to parse')
    parser.add_argument('-o', dest='output_file', help='Write output to file')
    return parser.parse_args();


if __name__ == '__main__':
    args = parse_args()

    components = analyze_ipxact_files(args.ipxact_files[0])

    with open_output(args.output_file) as of:

        env = Environment(loader=PackageLoader('bridge', 'templates'))

        template = env.get_template('axi4lite.scala.template')

        print args

        render_options = {
                "peripheral_slave_interfaces":peripheral_slave_interfaces(components, debug=True)
            ,   "peripheral_slave_instances":peripheral_slave_instances(components, debug=True)
            ,   "read_request_init":read_request_init(components, debug=True)
            ,   "read_request_handle":read_request_handle(components, debug=True)
            ,   "read_request_forward":read_request_forward(components, debug=True)
            ,   "write_request_init":write_request_init(components, debug=True)
            ,   "write_request_handle":write_request_handle(components, debug=True)
            ,   "write_request_forward":write_request_forward(components, debug=True)
        }

        with open_output(args.output_file) as fh:
            fh.write(template.render(render_options))

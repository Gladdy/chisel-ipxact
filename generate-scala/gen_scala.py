#ipyxact example. Parses an IP-XACT XML file called generic_example.xml
#and prints out a C header of the register maps found
import argparse
import sys
import time

from ipyxact.ipyxact import Component

def parse_args():
    parser = argparse.ArgumentParser(description='Generate a Chisel code from an IP-XACT file')

    parser.add_argument('ipxact_files', nargs='*', action='append', help='IP-XACT files to parse')

    parser.add_argument('-t', dest='output_type', help='Type of output to generate, [peripheral_interface, slave_interface, access_code]')
    parser.add_argument('-o', dest='output_file', help='Write output to file')

    return parser.parse_args();

def open_output(output):
    return open(output, 'w') if output else sys.stdout

class IPComponent:
    def __init__(self):
        self.name = ""
        self.registers = []
        self.baseAddress = 0

    def show(self):
        print self.name + " at " + str(self.baseAddress)
        for reg in self.registers:
            reg.show()
        print "\n"


class IPRegister:
    def __init__(self, name, size, access, offset):
        self.name = name
        self.size = size
        self.access = access
        self.offset = offset


    def show(self):
        print str(self.name) + "\t" +  str(self.size) + "\t" + str(self.access) + "\t" + str(self.offset)

# <spirit:register>
#   <spirit:name>output_ready</spirit:name>
#   <spirit:displayName>Output ready flag</spirit:displayName>
#   <spirit:description>When this flag is raised the output should be done</spirit:description>
#   <spirit:addressOffset>0x18</spirit:addressOffset>
#   <spirit:size>32</spirit:size>
#   <spirit:volatile>false</spirit:volatile>
#   <spirit:access>read-only</spirit:access>
# </spirit:register>
def analyze_ipxact_files(ipxact_files):

    ip_components = []
    names = set()

    for ipxact_file in ipxact_files:

        with open(ipxact_file) as f:

            xml_component = Component()
            xml_component.load(f)

            name = xml_component.name
            name_test = name
            i = 2
            while name_test in names:
                name_test = name + "_" + str(i)
                i += 1

            name = name_test
            names.add(name)

            ip_component = IPComponent()
            ip_component.name = name + "_interface"

            for m in xml_component.memoryMaps.memoryMap:
                for block in m.addressBlock:
                    ip_component.baseAddress = block.baseAddress
                    for reg in sorted(block.register, key=lambda addr: addr.addressOffset):
                        r = IPRegister(reg.name, reg.size, reg.access, reg.addressOffset)
                        ip_component.registers.append(r)

        ip_components.append(ip_component)

    for c in ip_components:
        c.show()



    print "Finished analysis"
    return ip_components



# class PeripheralMasterIf extends Bundle {
# val in_data  = UInt(INPUT, 32)
# val in_ready = UInt(INPUT, 1)
# val out_data = UInt(OUTPUT, 32)
# val out_ready = UInt(OUTPUT, 32)
# }
def write_peripheral_interface(of, components):

    of.write('\nPeripheral interfaces\n')

    for component in components:
        registers = []

        for r in component.registers:

            if r.access == "read-only":
                access = "OUTPUT"
            elif r.access == "write-only":
                access = "INPUT"
            else:
                print("Invalid direction: " + r.access)
                raise ValueError(r.access)

            registers.append('\tval {} = UInt({}, {})\n'.format(r.name,access,r.size))


        of.write('class {} extends Bundle {{\n'.format(component.name))
        of.write(''.join(registers))
        of.write('}\n')



# val io = new Bundle {
#
#   val axi = new Axi4LiteSlaveIf
#
#   var peripherals = new Array[Peripheral](10)
#   // attached peripheral
#   // val slaveA = new PeripheralMasterIf
#   // val slaveB = new PeripheralMasterIf
# }
def write_slave_interface(of, components):
    of.write('\nSlave interface\n')
    of.write('val io = new Bundle {\n')
    of.write('\tval axi = new Axi4LiteSlaveIf\n')

    component_lines = []

    for c in components:
        name = c.name + "_instance"
        component_lines.append('\tval {} = new {}\n'.format(name,c.name))

    of.write(''.join(component_lines))
    of.write('}\n')

# if(access == "write-only") {
#     when(waddr === UInt(address)) {
#         io.peripherals(pc).io.in.bits.wdata := io.axi.wchannel.bits.wdata
#         io.peripherals(pc).io.in.valid := io.axi.wchannel.valid
#         io.axi.wchannel.ready := io.peripherals(pc).io.in.ready && wvalid && !wresp_valid
#     }.otherwise {
#         io.peripherals(pc).io.in.bits.wdata := UInt(0x00)
#         io.peripherals(pc).io.in.valid := Bool(false)
#         io.axi.wchannel.ready := Bool(false)
#         }
# } else if (access == "read-only") {
#     when(addr === UInt(address)) {
#         io.axi.rchannel.bits.rdata := io.peripherals(pc).io.out.bits.rdata
#         io.peripherals(pc).io.out.ready := io.axi.rchannel.ready
#         io.axi.rchannel.valid := io.peripherals(pc).io.out.valid && rvalid
#     }.otherwise {
#         io.axi.rchannel.bits.rdata := UInt(0x00)
#         io.peripherals(pc).io.out.ready := Bool(false)
#         io.axi.rchannel.valid := Bool(false)
#     }
# } else {
#     println("WARNING: Unsupported access mode: " + access)
# }

def write_write_code(of, components):
    of.write('\nAccess code\n')

    for component in components:
        for r in component.registers:
            if r.access == "write-only":

                of.write('when(waddr === UInt(adress)) {\n')
                of.write('\tio.{}.{}')


def write_access_code(of, components):
    pass

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

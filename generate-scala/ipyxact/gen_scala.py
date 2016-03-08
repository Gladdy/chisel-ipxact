#ipyxact example. Parses an IP-XACT XML file called generic_example.xml
#and prints out a C header of the register maps found
import argparse
import sys
import time
import StringIO
from contextlib import closing
from ipyxact import Component

def open_output(output):
    return open(output, 'w') if output else sys.stdout

class IPComponent:
    def __init__(self):
        self.name = ""
        self.name_if = ""
        self.name_int = ""
        self.registers = []
        self.baseAddress = 0

    def show(self):
        print self.name + " at " + str(self.baseAddress)
        for reg in self.registers:
            reg.show()
        print "\n"


class IPRegister:
    def __init__(self, component, name, size, access, offset):
        self.name = name
        self.name_rb = component.name + "_" + name + "_readbuffer"
        self.name_wb = component.name + "_" + name + "_writebuffer"
        self.size = size
        self.access = access
        self.offset = offset

    def isReadOnly(self):
        return self.access == "read-only"

    def isWriteOnly(self):
        return self.access == "write-only"


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
            ip_component.name = name
            ip_component.name_if = name + "_interface"
            ip_component.name_int = name + "_instance"

            for m in xml_component.memoryMaps.memoryMap:
                for block in m.addressBlock:
                    ip_component.baseAddress = block.baseAddress
                    for reg in sorted(block.register, key=lambda addr: addr.addressOffset):
                        r = IPRegister(ip_component, reg.name, reg.size, reg.access, reg.addressOffset)
                        ip_component.registers.append(r)

        ip_components.append(ip_component)

    for c in ip_components:
        c.show()



    print "Finished analysis"
    return ip_components



# class PeripheralMasterIf extends Bundle {
#   val in_data  = UInt(INPUT, 32)
#   val in_ready = UInt(INPUT, 1)
#   val out_data = UInt(OUTPUT, 32)
#   val out_ready = UInt(OUTPUT, 32)
# }
def peripheral_slave_interfaces(components, debug=False):
    code = ""

    for c in components:
        registers = []
        for r in c.registers:

            if r.isReadOnly():
                access = "OUTPUT"
            elif r.isWriteOnly():
                access = "INPUT"
            else:
                print("Invalid direction: " + r.access)
                raise ValueError(r.access)

            registers.append('val {} = UInt({}, {})\n'.format(r.name,access,r.size))


        code += 'class {} extends Bundle {{\n'.format(c.name_if)
        code += ''.join(registers)
        code += '}\n'


    if debug:
        print("peripheral_slave_interfaces")
        print code

    return code


# val slaveA = new PeripheralMasterIf
# val slaveB = new PeripheralMasterIf
def peripheral_slave_instances(components, debug=False):
    code = ""

    component_lines = []
    for c in components:
        component_lines.append('val {} = new {}().flip()\n'.format(c.name_int,c.name_if))

    code += ''.join(component_lines)

    if debug:
        print("peripheral_slave_instances")
        print code

    return code


# val slaves_read = Array.fill[UInt](2)(Reg(init = UInt(0, 32)))
def read_request_init(components, debug=False):
    code = ""

    for c in components:
        for r in c.registers:
            if r.isReadOnly():
                code += "val {} = Reg(init = UInt(0, {}))\n".format(r.name_rb, r.size)

    if debug:
        print("read_request_init")
        print code

    return code

# when (rvalid && addr === UInt(0x00)) {
#     io.axi.rchannel.valid := Bool(true)
#     //slaves_read_ready := Bool(true)
#     io.axi.rchannel.bits.rdata := slaves_read(0)
# } .elsewhen (rvalid && addr === UInt(0x04)) {
#     io.axi.rchannel.valid := Bool(true)
#     //slaves_read_ready := Bool(true)
#     io.axi.rchannel.bits.rdata := slaves_read(1)
# } .otherwise {
#     io.axi.rchannel.valid := Bool(false)
#     //slaves_read_ready := Bool(false)
#     io.axi.rchannel.bits.rdata := UInt(0x00)
# }
def read_request_handle(components, debug=False):
    code = ""

    first_clause = True

    for c in components:
        for r in c.registers:
            if r.isReadOnly():

                address = c.baseAddress + r.offset

                if first_clause:
                    code += "when(rvalid && addr === UInt({})) {{\n".format(address)
                    first_clause = False
                else:
                    code += ".elsewhen(rvalid && addr === UInt({})) {{\n".format(address)

                code += "  io.axi.rchannel.valid := Bool(true)\n"
                code += "  io.axi.rchannel.bits.rdata := {}\n".format(r.name_rb)
                code += "}"

    # Set the default values of the read channel
    if not first_clause:
        code += ".otherwise {\n"


    code += "  io.axi.rchannel.valid := Bool(false)\n"
    code += "  io.axi.rchannel.bits.rdata := UInt(0x00)\n"

    if not first_clause:
        code += "}\n"

    code += "\n"

    if debug:
        print("read_request_handle")
        print code

    return code

# for (i <- 0 until io.slaves.length) {
#   slaves_read(i) := io.slaves(i).data_out
# }
def read_request_forward(components, debug=False):
    code = ""

    for c in components:
        for r in c.registers:
            if r.isReadOnly():
                code += "{} := io.{}.{}\n".format(r.name_rb, c.name_int, r.name)

    code += "\n"

    if debug:
        print("read_request_forward")
        print code

    return code

# val slaves_write = Array.fill[UInt](2)(Reg(init = UInt(0, 32)))
def write_request_init(components, debug=False):
    code = ""

    for c in components:
        for r in c.registers:
            if r.isWriteOnly():
                code += "val {} = Reg(init = UInt(0, {}))\n".format(r.name_wb, r.size)

    if debug:
        print("write_request_init")
        print code

    return code


# when (wvalid && addr === UInt(0x00) && io.axi.wchannel.valid) {
#     slaves_write(0) := io.axi.wchannel.bits.wdata
# } .elsewhen (wvalid && addr === UInt(0x04) && io.axi.wchannel.valid) {
#     slaves_write_valid(0) := Bool(io.axi.wchannel.bits.wdata(0))
# } .elsewhen (wvalid && addr === UInt(0x08) && io.axi.wchannel.valid) {
#     slaves_write(1) := io.axi.wchannel.bits.wdata
# } .elsewhen (wvalid && addr === UInt(0x10) && io.axi.wchannel.valid) {
#     slaves_write_valid(1) := Bool(io.axi.wchannel.bits.wdata(0))
# }
def write_request_handle(components, debug=False):
    code = ""

    first_clause = True

    for c in components:
        for r in c.registers:
            if r.isWriteOnly():

                address = c.baseAddress + r.offset

                if first_clause:
                    code += "when(wvalid && addr === UInt({}) && io.axi.wchannel.valid) {{\n".format(address)
                    first_clause = False
                else:
                    code += ".elsewhen(wvalid && addr === UInt({}) && io.axi.wchannel.valid) {{\n".format(address)

                code += "  {} := io.axi.wchannel.bits.wdata\n".format(r.name_wb)
                code += "}"

    code += "\n"

    if debug:
        print("write_request_handle")
        print code

    return code

# for (i <- 0 until io.slaves.length) {
#     io.slaves(i).data_in := slaves_write(i)
#     io.slaves(i).valid_in := slaves_write_valid(i)
# }
def write_request_forward(components, debug=False):
    code = ""

    for c in components:
        for r in c.registers:
            if r.isWriteOnly():
                code += "io.{}.{} := {}\n".format(c.name_int, r.name, r.name_wb)

    code += "\n"

    if debug:
        print("write_request_forward")
        print code

    return code

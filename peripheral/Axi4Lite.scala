package cam.cl.chiselipxact;

import Chisel._;
import scala.xml.XML
  import scala.reflect.runtime.{universe => ru}

object AXI4LiteBusWidth {
  val AXI32 = 32
  val AXI64 = 64
}

object AXI4LiteWResp {
  val OKAY = UInt(0, 2)
  val EXOKAY = UInt(1, 2)
  val SLVERR = UInt(2, 2)
  val DECERR = UInt(3, 2)
}

// XXX replace
class PeripheralSlaveIO extends Bundle {
  val data_in = UInt(INPUT, 32)
  val valid_in = Bool(INPUT)
  val data_out = UInt(OUTPUT, 32)
  val valid_out = Bool(OUTPUT)
}

class Axi4LiteSlaveIO extends Bundle {
    // write address channel
    val awchannel = Decoupled(new Bundle {
      val awaddr = UInt(INPUT, AXI4LiteBusWidth.AXI32)
      val awprot = Bits(INPUT, 3)
    }).flip()

    // write data channel
    val wchannel = Decoupled(new Bundle {
      val wdata = UInt(INPUT, AXI4LiteBusWidth.AXI32)
      val wstrb = UInt(INPUT, 4)
    }).flip()

    // write response channel
    val bchannel = Decoupled(new Bundle {
      val bresp = UInt(OUTPUT, 2)
    })

    // read address channel
    val archannel = Decoupled(new Bundle {
      val araddr = UInt(INPUT, AXI4LiteBusWidth.AXI32)
      val arprot = Bits(INPUT, 3)
    }).flip()

    // read data channel
    val rchannel = Decoupled(new Bundle {
      val rdata = UInt(OUTPUT, AXI4LiteBusWidth.AXI32)
      val rresp = Bits(OUTPUT, 2)
    })
}

class Peripheral(i: Int) extends Module {
  val io = new PeripheralSlaveIO

  val out = Reg(init = UInt(i, AXI4LiteBusWidth.AXI32))
  //io.valid_in := Bool(true)
  io.valid_out := Bool(true)
  io.data_out := out

  // when (io.out.ready) {
  // }
  when (io.valid_in) {
    out := io.data_in
  }
}

class AXI4LiteSlave extends Module {
  val io = new Bundle {

    val axi = new Axi4LiteSlaveIO
    // attached peripheral
    // XXX replace
    val slaves = new Array[PeripheralSlaveIO](2)
    slaves(0) = new PeripheralSlaveIO().flip()
    slaves(1) = new PeripheralSlaveIO().flip()

    var peripherals = new Array[Peripheral](10)
    // attached peripheral
    // val slaveA = new PeripheralMasterIf
    // val slaveB = new PeripheralMasterIf
  }

  /* handle read address channel for multiplexing */
  // cached address
  val addr = Reg(init = UInt(0, AXI4LiteBusWidth.AXI32))
  // read ok after address has been set
  val rvalid = Reg(init = Bool(false))

  io.axi.archannel.ready := ! rvalid

  // set address when it becomes valid
  when (io.axi.archannel.valid && io.axi.archannel.ready) {
    addr := io.axi.archannel.bits.araddr
    rvalid := Bool(true)
  }

  // reset address valid after read
  when (io.axi.rchannel.valid && io.axi.rchannel.ready) {
    rvalid := Bool(false)
  }

  /* handle read from slave latches 
   There is a register that holds the current valid value for each peripheral,
   AXI transactions read from these registers
   */

  // XXX replace
  val slaves_read = Array.fill[UInt](2)(Reg(init = UInt(0, 32)))

  // hold the rchannel valid state to properly wait one cycle
  // so that the latch is updated properly
  //val slaves_read_ready = Reg(init = Bool(false))

  //io.axi.rchannel.valid := slaves_read_ready

  // XXX replace
  when (rvalid && addr === UInt(0x00)) {
    io.axi.rchannel.valid := Bool(true)
    //slaves_read_ready := Bool(true)
    io.axi.rchannel.bits.rdata := slaves_read(0)
  } .elsewhen (rvalid && addr === UInt(0x04)) {
    io.axi.rchannel.valid := Bool(true)
    //slaves_read_ready := Bool(true)
    io.axi.rchannel.bits.rdata := slaves_read(1)
  } .otherwise {
    io.axi.rchannel.valid := Bool(false)
    //slaves_read_ready := Bool(false)
    io.axi.rchannel.bits.rdata := UInt(0x00)
  }

  // XXX replace
  for (i <- 0 until io.slaves.length) {
    when (io.slaves(i).valid_out) {
      slaves_read(i) := io.slaves(i).data_out
    }
  }

  /* handle write address channel for multiplexing */

  // cached address
  val waddr = Reg(init = UInt(0, AXI4LiteBusWidth.AXI32))
  // write ok after address has been set
  val wvalid = Reg(init = Bool(false))

  io.axi.awchannel.ready := ! wvalid

  // set address when it becomes valid
  when (io.axi.awchannel.valid && io.axi.awchannel.ready) {
    addr := io.axi.awchannel.bits.awaddr
    wvalid := Bool(true)
  }

  // reset address valid after write
  when (io.axi.wchannel.valid && io.axi.wchannel.ready) {
    wvalid := Bool(false)
  }

  /* handle the write to latches */

  var pc = 0

  for(fname <- List("badhash_1.xml","badhash_2.xml")) {

    val xml = XML.loadFile(fname)
    val component = (xml \\ "component")
    val addressBlock = (xml \\ "memoryMap" \ "addressBlock")

    val name = (component \ "name").text
    val desc = (component \ "description").text
    val baseAddress = (addressBlock \ "baseAddress").text.substring(2).toInt
    val range = (addressBlock \ "range").text.substring(2).toInt

    io.peripherals(pc) = Module(new Peripheral(0x7eadbeef))


    val registers = (addressBlock \ "register")
    for (register <- registers) {
      val name = (register \ "displayName").text
      val address = (register \ "addressOffset").text.substring(2).toInt + baseAddress
      val access = (register \ "access").text


      if(access == "write-only") {
        when(waddr === UInt(address)) {
          io.peripherals(pc).io.in.bits.wdata := io.axi.wchannel.bits.wdata
          io.peripherals(pc).io.in.valid := io.axi.wchannel.valid
          io.axi.wchannel.ready := io.peripherals(pc).io.in.ready && wvalid && !wresp_valid
        }.otherwise {
          io.peripherals(pc).io.in.bits.wdata := UInt(0x00)
          io.peripherals(pc).io.in.valid := Bool(false)
          io.axi.wchannel.ready := Bool(false)
        }
      } else if (access == "read-only") {
        when(addr === UInt(address)) {
          io.axi.rchannel.bits.rdata := io.peripherals(pc).io.out.bits.rdata
          io.peripherals(pc).io.out.ready := io.axi.rchannel.ready
          io.axi.rchannel.valid := io.peripherals(pc).io.out.valid && rvalid
        }.otherwise {
          io.axi.rchannel.bits.rdata := UInt(0x00)
          io.peripherals(pc).io.out.ready := Bool(false)
          io.axi.rchannel.valid := Bool(false)
        }
      } else {
        println("WARNING: Unsupported access mode: " + access)
      }
    }

    pc = pc + 1
  }

  /* This part handles the write channel of the AXI so that transactions write to registers
   there is one register for each data input of an interface, the same is for the read channel
   */
  // XXX replace
  val slaves_write = Array.fill[UInt](2)(Reg(init = UInt(0, 32)))
  val slaves_write_valid = Array.fill[Bool](2)(Reg(init = Bool(false)))

  io.axi.wchannel.ready := wvalid

  // XXX replace
  when (wvalid && addr === UInt(0x00) && io.axi.wchannel.valid) {
    slaves_write(0) := io.axi.wchannel.bits.wdata

  } .elsewhen (wvalid && addr === UInt(0x04) && io.axi.wchannel.valid) {
    slaves_write_valid(0) := Bool(io.axi.wchannel.bits.wdata(0))

  } .elsewhen (wvalid && addr === UInt(0x08) && io.axi.wchannel.valid) {
    slaves_write(1) := io.axi.wchannel.bits.wdata

  } .elsewhen (wvalid && addr === UInt(0x10) && io.axi.wchannel.valid) {
    slaves_write_valid(1) := Bool(io.axi.wchannel.bits.wdata(0))
  }

  // XXX replace
  for (i <- 0 until io.slaves.length) {
    io.slaves(i).data_in := slaves_write(i)
    io.slaves(i).valid_in := slaves_write_valid(i)
  }

  /* handle the write response */

  val wresp_valid = Reg(init = Bool(false))
  io.axi.bchannel.valid := wresp_valid
  io.axi.bchannel.bits.bresp := AXI4LiteWResp.OKAY

  when (io.axi.wchannel.valid && io.axi.wchannel.ready) {
    wresp_valid := Bool(true)
  }

  when (io.axi.bchannel.valid && io.axi.bchannel.ready) {
    wresp_valid := Bool(false)
  }

}
  // /* forward read channel */
  // when (addr === UInt(0x00)) {
  //   io.axi.rchannel.bits.rdata := io.slaveA.out.bits.rdata
  //   io.slaveA.out.ready := io.axi.rchannel.ready
  //   io.axi.rchannel.valid := io.slaveA.out.valid && rvalid
  //
  //   io.slaveB.out.ready := Bool(false)
  //
  // } .elsewhen (addr === UInt(0x04)) {
  //   io.axi.rchannel.bits.rdata := io.slaveB.out.bits.rdata
  //   io.slaveB.out.ready := io.axi.rchannel.ready
  //   io.axi.rchannel.valid := io.slaveB.out.valid && rvalid
  //
  //   io.slaveA.out.ready := Bool(false)
  //
  // } .otherwise {
  //   io.axi.rchannel.bits.rdata := UInt(0x00)
  //   io.slaveA.out.ready := Bool(false)
  //   io.slaveB.out.ready := Bool(false)
  //   io.axi.rchannel.valid := Bool(false)
  // }
  //
  // /* forward write channel */
  //
  // when (addr === UInt(0x00)) {
  //   io.slaveA.in.bits.wdata := io.axi.wchannel.bits.wdata
  //   io.slaveA.in.valid := io.axi.wchannel.valid
  //   io.axi.wchannel.ready := io.slaveA.in.ready && wvalid && !wresp_valid
  //
  //   io.slaveB.in.valid := Bool(false)
  //   io.slaveB.in.bits.wdata := UInt(0x00)
  //
  // } .elsewhen (addr === UInt(0x04)) {
  //   io.slaveB.in.bits.wdata := io.axi.wchannel.bits.wdata
  //   io.slaveB.in.valid := io.axi.wchannel.valid
  //   io.axi.wchannel.ready := io.slaveB.in.ready && wvalid && !wresp_valid
  //
  //   io.slaveA.in.valid := Bool(false)
  //   io.slaveA.in.bits.wdata := UInt(0x00)
  //
  // } .otherwise {
  //   io.slaveA.in.bits.wdata := UInt(0x00)
  //   io.slaveB.in.bits.wdata := UInt(0x00)
  //   io.slaveA.in.valid := Bool(false)
  //   io.slaveB.in.valid := Bool(false)
  //   io.axi.wchannel.ready := Bool(false)
  // }

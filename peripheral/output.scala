package cam.cl.chiselipxact;

import Chisel._;
import scala.xml.XML

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
class badhash_interface extends Bundle {
val input_data = UInt(INPUT, 64)
val input_ready = UInt(INPUT, 64)
val output_data = UInt(OUTPUT, 64)
val output_ready = UInt(OUTPUT, 64)
}
class badhash_2_interface extends Bundle {
val input_data = UInt(INPUT, 64)
val input_ready = UInt(INPUT, 64)
val output_data = UInt(OUTPUT, 64)
val output_ready = UInt(OUTPUT, 64)
}

/*
class PeripheralSlaveIO extends Bundle {
  val data_in = UInt(INPUT, 32)
  val valid_in = Bool(INPUT)
  val data_out = UInt(OUTPUT, 32)
  val valid_out = Bool(OUTPUT)
}*/

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

class AXI4LiteSlave extends Module {
  val io = new Bundle {

    val axi = new Axi4LiteSlaveIO
    // attached peripheral
    // XXX replace
    val badhash_instance = new badhash_interface().flip()
val badhash_2_instance = new badhash_2_interface().flip()

    /*
    val slaves = new Array[PeripheralSlaveIO](2)
    slaves(0) = new PeripheralSlaveIO().flip()
    slaves(1) = new PeripheralSlaveIO().flip()
    */
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
  val badhash_output_data_readbuffer = Reg(init = UInt(0, 64))
val badhash_output_ready_readbuffer = Reg(init = UInt(0, 64))
val badhash_2_output_data_readbuffer = Reg(init = UInt(0, 64))
val badhash_2_output_ready_readbuffer = Reg(init = UInt(0, 64))

  /*
  val slaves_read = Array.fill[UInt](2)(Reg(init = UInt(0, 32)))
  */

  // XXX replace
  when(rvalid && addr === UInt(16)) {
  io.axi.rchannel.valid := Bool(true)
  io.axi.rchannel.bits.rdata := badhash_output_data_readbuffer
}.elsewhen(rvalid && addr === UInt(24)) {
  io.axi.rchannel.valid := Bool(true)
  io.axi.rchannel.bits.rdata := badhash_output_ready_readbuffer
}.elsewhen(rvalid && addr === UInt(48)) {
  io.axi.rchannel.valid := Bool(true)
  io.axi.rchannel.bits.rdata := badhash_2_output_data_readbuffer
}.elsewhen(rvalid && addr === UInt(56)) {
  io.axi.rchannel.valid := Bool(true)
  io.axi.rchannel.bits.rdata := badhash_2_output_ready_readbuffer
}.otherwise {
  io.axi.rchannel.valid := Bool(false)
  io.axi.rchannel.bits.rdata := UInt(0x00)
}


  /*
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
  }*/

  // XXX replace
  badhash_output_data_readbuffer := io.badhash_instance.output_data
badhash_output_ready_readbuffer := io.badhash_instance.output_ready
badhash_2_output_data_readbuffer := io.badhash_2_instance.output_data
badhash_2_output_ready_readbuffer := io.badhash_2_instance.output_ready


  /*
  for (i <- 0 until io.slaves.length) {
    when (io.slaves(i).valid_out) {
      slaves_read(i) := io.slaves(i).data_out
    }
  }*/

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


  /* This part handles the write channel of the AXI so that transactions write to registers
   there is one register for each data input of an interface, the same is for the read channel
   */
  // XXX replace
  val badhash_input_data_writebuffer = Reg(init = UInt(0, 64))
val badhash_input_ready_writebuffer = Reg(init = UInt(0, 64))
val badhash_2_input_data_writebuffer = Reg(init = UInt(0, 64))
val badhash_2_input_ready_writebuffer = Reg(init = UInt(0, 64))

  /*
  val slaves_write = Array.fill[UInt](2)(Reg(init = UInt(0, 32)))
  val slaves_write_valid = Array.fill[Bool](2)(Reg(init = Bool(false)))
  */

  io.axi.wchannel.ready := wvalid

  // XXX replace
  when(wvalid && addr === UInt(0) && io.axi.wchannel.valid) {
  badhash_input_data_writebuffer := io.axi.wchannel.bits.wdata
}.elsewhen(wvalid && addr === UInt(8) && io.axi.wchannel.valid) {
  badhash_input_ready_writebuffer := io.axi.wchannel.bits.wdata
}.elsewhen(wvalid && addr === UInt(32) && io.axi.wchannel.valid) {
  badhash_2_input_data_writebuffer := io.axi.wchannel.bits.wdata
}.elsewhen(wvalid && addr === UInt(40) && io.axi.wchannel.valid) {
  badhash_2_input_ready_writebuffer := io.axi.wchannel.bits.wdata
}

  /*
  when (wvalid && addr === UInt(0x00) && io.axi.wchannel.valid) {
    slaves_write(0) := io.axi.wchannel.bits.wdata

  } .elsewhen (wvalid && addr === UInt(0x04) && io.axi.wchannel.valid) {
    slaves_write_valid(0) := Bool(io.axi.wchannel.bits.wdata(0))

  } .elsewhen (wvalid && addr === UInt(0x08) && io.axi.wchannel.valid) {
    slaves_write(1) := io.axi.wchannel.bits.wdata

  } .elsewhen (wvalid && addr === UInt(0x10) && io.axi.wchannel.valid) {
    slaves_write_valid(1) := Bool(io.axi.wchannel.bits.wdata(0))
  }*/

  // XXX replace
  io.badhash_instance.input_data := badhash_input_data_writebuffer
io.badhash_instance.input_ready := badhash_input_ready_writebuffer
io.badhash_2_instance.input_data := badhash_2_input_data_writebuffer
io.badhash_2_instance.input_ready := badhash_2_input_ready_writebuffer


  /*
  for (i <- 0 until io.slaves.length) {
    io.slaves(i).data_in := slaves_write(i)
    io.slaves(i).valid_in := slaves_write_valid(i)
  }*/

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
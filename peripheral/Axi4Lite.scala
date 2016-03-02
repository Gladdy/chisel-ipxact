package cam.cl.chiselipxact;

import Chisel._;

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

class PeripheralSlaveIf extends Bundle {
    val in = Decoupled(new Bundle {
      val wdata = UInt(INPUT, AXI4LiteBusWidth.AXI32)
    }).flip()

    val out = Decoupled(new Bundle {
      val rdata = UInt(OUTPUT, AXI4LiteBusWidth.AXI32)
    })
}

class PeripheralMasterIf extends Bundle {
  val in = Decoupled(new Bundle {
    val wdata = UInt(OUTPUT, AXI4LiteBusWidth.AXI32)
  })

  val out = Decoupled(new Bundle {
    val rdata = UInt(INPUT, AXI4LiteBusWidth.AXI32)
  }).flip()
}

class Axi4LiteSlaveIf extends Bundle {
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
  val io = new PeripheralSlaveIf

  val out = Reg(init = UInt(i, AXI4LiteBusWidth.AXI32))
  io.in.ready := Bool(true)
  io.out.valid := Bool(true)
  io.out.bits.rdata := out

  // when (io.out.ready) {
  // }
  when (io.in.valid) {
    out := io.in.bits.wdata
  }
}

class AXI4LiteSlave extends Module {
  val io = new Bundle {

    val axi = new Axi4LiteSlaveIf
    // attached peripheral
    val slaveA = new PeripheralMasterIf
    val slaveB = new PeripheralMasterIf

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

  // reset address after read
  when (io.axi.rchannel.valid && io.axi.rchannel.ready) {
    rvalid := Bool(false)
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

  // reset address after read
  when (io.axi.wchannel.valid && io.axi.wchannel.ready) {
    wvalid := Bool(false)
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

  /* forward read channel */

  when (addr === UInt(0x00)) {
    io.axi.rchannel.bits.rdata := io.slaveA.out.bits.rdata
    io.slaveA.out.ready := io.axi.rchannel.ready
    io.axi.rchannel.valid := io.slaveA.out.valid && rvalid

    io.slaveB.out.ready := Bool(false)

  } .elsewhen (addr === UInt(0x04)) {
    io.axi.rchannel.bits.rdata := io.slaveB.out.bits.rdata
    io.slaveB.out.ready := io.axi.rchannel.ready
    io.axi.rchannel.valid := io.slaveB.out.valid && rvalid

    io.slaveA.out.ready := Bool(false)

  } .otherwise {
    io.axi.rchannel.bits.rdata := UInt(0x00)
    io.slaveA.out.ready := Bool(false)
    io.slaveB.out.ready := Bool(false)
    io.axi.rchannel.valid := Bool(false)
  }

  /* forward write channel */

  when (addr === UInt(0x00)) {
    io.slaveA.in.bits.wdata := io.axi.wchannel.bits.wdata
    io.slaveA.in.valid := io.axi.wchannel.valid
    io.axi.wchannel.ready := io.slaveA.in.ready && wvalid && !wresp_valid

    io.slaveB.in.valid := Bool(false)
    io.slaveB.in.bits.wdata := UInt(0x00)

  } .elsewhen (addr === UInt(0x04)) {
    io.slaveB.in.bits.wdata := io.axi.wchannel.bits.wdata
    io.slaveB.in.valid := io.axi.wchannel.valid
    io.axi.wchannel.ready := io.slaveB.in.ready && wvalid && !wresp_valid

    io.slaveA.in.valid := Bool(false)
    io.slaveA.in.bits.wdata := UInt(0x00)

  } .otherwise {
    io.slaveA.in.bits.wdata := UInt(0x00)
    io.slaveB.in.bits.wdata := UInt(0x00)
    io.slaveA.in.valid := Bool(false)
    io.slaveB.in.valid := Bool(false)
    io.axi.wchannel.ready := Bool(false)
  }


}

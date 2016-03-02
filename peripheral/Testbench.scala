
package cam.cl.chiselipxact;

import Chisel._;

class Axi4LiteTest(c: AXI4LiteSlave) extends Tester(c) {

  println("Test start")
  println("Test read")
  poke(c.io.axi.archannel.valid, true)
  poke(c.io.axi.archannel.bits.araddr, 0x00)
  poke(c.io.axi.archannel.bits.arprot, 0x00)
  step(1)
  expect(peek(c.io.axi.rchannel.bits.rdata) == 0xdeadbeef, "simple read")
  // while (peek(c.io.archannel.ready) == 0)
  //   step(1)
  println("Test end")

}

class IPBlockTest(c: IPBlock) extends Tester(c) {

  def read (addr: Int, good: Int) = {
    poke(c.io.archannel.valid, true)
    poke(c.io.archannel.bits.araddr, addr)
    poke(c.io.archannel.bits.arprot, 0x00)
    // complete addr hs
    while (peek(c.io.archannel.ready) == 0)
      step(1)
    step(1)
    poke(c.io.archannel.valid, false)
    poke(c.io.rchannel.ready, true)
    // complete read hs
    while (peek(c.io.rchannel.valid) == 0)
      step(1)
    expect(peek(c.io.rchannel.valid) == 1, "valid read")
    expect(peek(c.io.rchannel.bits.rdata) == good, "data read")
    // wait at least 1 cycle before deasserting the ready signal
    step(1)
    poke(c.io.rchannel.ready, false)
    expect(peek(c.io.archannel.ready) == 1, "raddr channel ready")
   }

  println("Test start")
  println("Test read")
  // test read
  read(0x00, 0x7eadbeef)
  read(0x04, 0x0000beef)

  step(1)
  /*
  // test write
  poke(c.io.awchannel.valid, true)
  poke(c.io.awchannel.bits.awaddr, 0x04)
  poke(c.io.awchannel.bits.awprot, 0x04)
  while(peek(c.io.awchannel.ready) == 0)
    step(1)
  poke(c.io.wchannel.valid, true)
  poke(c.io.wchannel.bits.wdata, 0x7fffffff)
  while(peek(c.io.wchannel.ready) == 0)
    step(1)
  step(1) // ensure the wchannel valid stays for at least one clock
  poke(c.io.wchannel.valid, false)
  // check that the write response is ready
  while(peek(c.io.bchannel.valid) == 0)
    step(1)
  expect(peek(c.io.bchannel.valid) == 1, "write response valid")
  expect(peek(c.io.bchannel.bits.bresp) == 0x00, "write response value ok")
  poke(c.io.bchannel.ready, true)
  step(1)
  expect(peek(c.io.bchannel.valid) == 0, "write response end")
  // check the write
  read(0x04, 0x7fffffff)
  */
  // while (peek(c.io.archannel.ready) == 0)
  //   step(1)
  println("Test end")

}

class IPBlock extends Module {

  val s = Module(new AXI4LiteSlave())
  val pa = Module(new Peripheral(0x7eadbeef))
  val pb = Module(new Peripheral(0x0000beef))

  val io = new Axi4LiteSlaveIf()

  s.io.axi <> io

  pa.io <> s.io.slaveA
  pb.io <> s.io.slaveB

}

// object Testbench {

//   def main(args: Array[String]): Unit = {
//     val testArgs = args.slice(1, args.length)
//     chiselMainTest(testArgs, () => Module(new AXI4LiteSlave
// )) { c => new Axi4LiteTest(c)}
//   }
// }
object Testbench {

  def main(args: Array[String]): Unit = {
    val testArgs = args.slice(1, args.length)
    chiselMainTest(testArgs, () => Module(new IPBlock
)) { c => new IPBlockTest(c)}
  }
}

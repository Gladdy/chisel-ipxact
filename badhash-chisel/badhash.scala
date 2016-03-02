package BadHash

import Chisel._

class BadHash extends Module {
  val io = new Bundle {
    val in_data  = UInt(INPUT, 32)
    val in_ready = UInt(INPUT, 1)
    val out_data = UInt(OUTPUT, 32)
    val out_ready = UInt(OUTPUT, 32)
  }

  io.out_ready := UInt(0)
  io.out_data := UInt(0)

  val running = Reg(init=UInt(0,1))
  val operating_register = Reg(init=UInt(0, 32))

  when(io.in_ready === UInt(1) && running === UInt(0)) {
    // Load the value from the input if the in_ready is asserted
    running := UInt(1)
    operating_register := io.in_data
  }.elsewhen(running === UInt(1)) {

    // Compute the hash and push it to the output
    val x = operating_register ^ UInt(513886569);
    // io.out_data := Cat(x(29,29),x(31,30), x(23,22), x(0,0), x(6,1), x(17,15), x(12,9), x(28,24), x(13,12), x(21,18), x(8,7))
    io.out_data := x
    io.out_ready := UInt(1)
    running := UInt(0)
  }


  // accumulator := accumulator + io.in
  // io.out := accumulator
  //
  // io.out_data := io.in_data
  // io.out_ready := io.in_ready
}

class BadHashTest(c: BadHash) extends Tester(c) {
  // var tot = 0
  for (t <- 0 until 16) {
    val in = rnd.nextInt(1000000)
    poke(c.io.in_data, in)

    // step(1)
    // if (in == 1) tot += 1
    expect(c.io.out_data, in)
  }
}

object BadHash {
  def main(args: Array[String]): Unit = {
    // val tutArgs = args.slice(1, args.length)
    chiselMainTest(args, () => Module(new BadHash())) {
      c => new BadHashTest(c) }
  }
}

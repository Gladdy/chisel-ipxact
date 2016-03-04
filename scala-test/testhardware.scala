package TestHardware

import scala.xml.XML
import Chisel._
import scala.reflect.runtime.{universe => ru}

// class Peripheral(functionality: String) extends Module {
class Peripheral extends Module {
  val io = new Bundle {
    val in_data  = UInt(INPUT, 32)
    val write = UInt(INPUT, 1)
    val out_data = UInt(OUTPUT, 32)
  }

  println("WARNING: Please to not initialize a bare peripheral")
  io.out_data := UInt(1)
}

class Hasher extends Peripheral {
  io.out_data := UInt(1337)
}

class Buffer extends Peripheral {
  val register  = Reg(UInt())

  when(io.write === UInt(1)) {
    register := io.in_data
    io.out_data := UInt(0)
  }.otherwise {
    io.out_data := register
  }
}

class BasicBridge extends Module {

  var baseAddress: Int = 0
  var range: Int = 0
  var component_name: String = ""

  def getRegisters(fname: String) : scala.xml.NodeSeq = {
    // Test read the XML
    val xml = XML.loadFile(fname)
    val component = (xml \\ "component")
    val addressBlock = (xml \\ "memoryMap" \ "addressBlock")

    component_name = (component \ "name").text
    baseAddress = (addressBlock \ "baseAddress").text.substring(2).toInt
    range = (addressBlock \ "range").text.substring(2).toInt

    val registers = (addressBlock \ "register")
    return registers
  }

  val io = new Bundle {
    val in_data  = UInt(INPUT, 32)
    val write = UInt(INPUT, 1)
    val address = UInt(INPUT, 32)
    val out_data = UInt(OUTPUT, 32)
  }

  io.out_data := UInt(0)

  val registers = getRegisters("ipxact.xml")
  println(s"Name: $component_name")
  println(s"Base address: $baseAddress")
  println(s"Range: $range")

  var peripherals = new Array[Peripheral](10)
  var pc = 0

  for (register <- registers) {
    val name = (register \ "name").text
    val offset = (register \ "addressOffset").text.substring(2).toInt
    val t = (register \ "access").text

    println(s"Name: $name")
    println(s"Address offset: $offset")
    println(s"Type: $t")
    println("")

    println(Class.forName(name))

    // peripherals(pc) = Module(new Peripheral(name))
    peripherals(pc) = Module(new Hasher())

    when(io.address === UInt(baseAddress + offset)) {
      peripherals(pc).io.in_data := io.in_data
      peripherals(pc).io.write := io.write
      io.out_data := peripherals(pc).io.out_data
    }.otherwise {
      peripherals(pc).io.in_data := UInt(0)
      peripherals(pc).io.write := UInt(0)
    }

    pc = pc + 1
  }
}

class BasicBridgeTest(c: BasicBridge) extends Tester(c) {

  /*
    TEST THE BUFFER
  */
  // Write to the buffer behind the BasicBridge
  poke(c.io.in_data, 512)
  poke(c.io.write, 1)
  poke(c.io.address, 0)
  step(1)

  // Read from the same buffer
  poke(c.io.write, 0)
  poke(c.io.address, 0)
  step(1)
  expect(c.io.out_data, 512)

  // Write other data to an invalid address
  poke(c.io.in_data, 999)
  poke(c.io.write, 1)
  poke(c.io.address, 999)
  step(1)

  // Read from the same buffer (should not have changed the value 512)
  poke(c.io.write, 0)
  poke(c.io.address, 0)
  step(1)
  expect(c.io.out_data, 512)

  /*
    TEST THE (UNIMPLEMENTED) HASHER
  */
  // Write to the hasher behind the BasicBridge
  poke(c.io.in_data, 512)
  poke(c.io.write, 1)
  poke(c.io.address, 4)
  step(1)

  // Read from the hasher
  poke(c.io.write, 0)
  poke(c.io.address, 4)
  step(1)
  expect(c.io.out_data, 1337)
}

object TestBasicBridge {
  def main(args: Array[String]): Unit = {
    // val tutArgs = args.slice(1, args.length)
    chiselMainTest(args, () => Module(new BasicBridge())) {
      c => new BasicBridgeTest(c) }
  }
}

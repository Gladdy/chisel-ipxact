package TestXML

import scala.xml.XML
import Chisel._

object TestXML {
  def nomain(args: Array[String]): Unit = {

    // println("Full file: ")
    // println(xml)

    val xml = XML.loadFile("../badhash.xml")

    val component = (xml \\ "component")
    val addressBlock = (xml \\ "memoryMap" \ "addressBlock")


    val name = (component \ "name").text
    println(name)

    val baseAddress = (addressBlock \ "baseAddress").text
    println(s"Base address: $baseAddress")

    val range = (addressBlock \ "range").text
    println(s"Range: $range")

    println("")

    val registers = (addressBlock \ "register")
    for (register <- registers) {

      val name = (register \ "displayName").text
      println(s"Name: $name")

      val offset = (register \ "addressOffset").text
      println(s"Address offset: $offset")

      val t = (register \ "access").text
      println(s"Type: $t")

      println("")
    }
  }
}

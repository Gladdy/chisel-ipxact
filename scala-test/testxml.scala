package TestXML

import scala.xml.XML
import Chisel._

object TestXML {
  def main(args: Array[String]): Unit = {

    for(fname <- args) {

      val xml = XML.loadFile(fname)
      val component = (xml \\ "component")
      val addressBlock = (xml \\ "memoryMap" \ "addressBlock")

      val name = (component \ "name").text
      val desc = (component \ "description").text
      val baseAddress = (addressBlock \ "baseAddress").text.substring(2).toInt
      val range = (addressBlock \ "range").text.substring(2).toInt

      println(s"Component name: $name - $desc")
      println(s"Base address: $baseAddress")
      println(s"Range: $range")
      println("")

      val registers = (addressBlock \ "register")
      for (register <- registers) {
        val name = (register \ "displayName").text
        val address = (register \ "addressOffset").text.substring(2).toInt + baseAddress
        val access = (register \ "access").text

        println(s"Name: $name")
        println(s"Address: $address")
        println(s"Access: $access")
        println("")
      }
    }
  }
}

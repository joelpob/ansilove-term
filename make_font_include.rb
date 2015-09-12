#!/usr/bin/env ruby

def file_to_font_data(filename)
  data = Array.new
  File.open(filename, "rb") do |io|
    io.read.each_byte do |c|
      data.push(c)
    end
  end
  font_height = data[9]
  return font_height, data[59...(font_height * 256 + 59)]
end

def file_to_hexes(filename)
  font_height, data = file_to_font_data(filename)
  hexes = Array.new
  data.map! {|byte| "0x%02x" % byte}
  (0...data.length).step(font_height) do |i|
    hexes.push(data.slice(i, font_height).join(", "))
  end
  font_name = filename.split("/").last.split(".").first
  puts "font_definition_t #{font_name} = \n    {\n        font_type_t::#{font_name},\n        #{font_height},\n        {\n"
  hexes.each_index do |n|
    puts "            {#{hexes[n]}}" + ((n == hexes.length - 1) ? "" : ",")
  end
  puts "        }\n    };\n\n"
end

puts "#ifndef FONTS_H\n#define FONTS_H\n\n#include <vector>\n#include \"../image_data.h\"\n\n"
Dir.glob("font_sources/*.xb").each do |path|
  file_to_hexes(path)
end
puts "#endif\n"

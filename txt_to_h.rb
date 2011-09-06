#!/usr/bin/env ruby

# this turns a text file which contains rows and columns made up of spaces and
# "o" characters into a .h file which can be included in a game and drawn onto
# the pegboard in monochrome.
#
# input files should have the .txt extension. output files will be input_file.h
# (where +input_file+ is the name of the input file).
#
# an icon variable will be created in the .h file that looks like this:
#
#   uint16_t icon_foo[] = {w, h, ...};
#
# the first two members of the array are the width and height of the icon.

def make_icon(input_file)
  basename = File.basename(input_file, '.txt')
  output_file = '%s.h' % basename

  width = 0
  height = 0
  nums = 
    File.read(input_file).each_line.collect do |line|
      line.chomp!
      width = [width, line.length].max
      height += 1
      sum = 0
      line.chars.to_a.reverse.each_with_index do |char, i|
        sum += 2 ** i if char == 'o'
      end
      sum
    end

  nums.unshift(width, height);

  File.open(output_file, 'w') do |out|
    out.puts "uint16_t #{basename}[] = {#{nums * ', '}};"
  end
end

if $0 == __FILE__
  make_icon ARGV.first
end

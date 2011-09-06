task(:icons) do
  load 'txt_to_h.rb'
  Dir['icon_*.txt'].each do |icon_file|
    make_icon icon_file
  end
  `cat icon_*.h > icons.h`
end

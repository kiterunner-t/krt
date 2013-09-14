# Copyleft (C) KRT, 2013 by kiterunner_t


define print_array
  
  dont-repeat
  if $argc != 2
    help print_array
  end

  set $arr = $arg0
  set $size = $arr.nelts
  set $i = 0

  while $i < $size
    p (($arg1 *) ($arr.elts))[$i]
    set $i++
  end
end


document print_array
  print_array <array> <array-ele-type>
end


set print pretty on


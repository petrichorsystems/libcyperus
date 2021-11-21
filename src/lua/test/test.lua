function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

require('Cyperus')
local cyperus
cyperus = Cyperus.new('5000', '127.0.0.1', '5001')

local root_bus
root_bus = cyperus.get_root()

print(dump(root_bus))

root_bus.add_module('audio/oscillator/pulse', 440.0, 0.5, 1.0, 0.0)
root_bus.add_module('audio/filter/moogff', 800.0, 1.0, 0.0, 1.0, 0.0)


print('end of lua script')
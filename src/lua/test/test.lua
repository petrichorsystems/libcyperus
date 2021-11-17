
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

ins = cyperus.get_ins()
outs = cyperus.get_outs()

local root_bus
root_bus = cyperus.get_root()

print(dump(root_bus))
print(dump(root_bus.bus_id))

print(dump(ins))
print(dump(outs))


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

print(root_bus)
print(root_bus.ins)
print(dump(root_bus))
print(dump(root_bus.ins))

print(root_bus.ins[1])
print(dump(root_bus.ins[1]))
root_bus.ins[1].connect()

print(root_bus.outs[1])
print(dump(root_bus.outs[1]))
root_bus.outs[1].connect()


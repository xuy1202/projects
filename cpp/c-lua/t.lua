

local s = "Hello world!";
--print(s);
--
--sum = 0
--num = 1
--while num <= 100 do
--    sum = sum + num
--    num = num + 1
--end
--print("sum = "..sum)
--
--print(pairs(_G))
--
--print('-----------------------------------------')
--for k, v in pairs(_G) do
--    print(k.."(", type(v)..")=> ", v)
--end

if not table.foreach then
    table.foreach = function(t, f)
	    for k, v in pairs(t) do f(k, v) end
	end
end

--for k, v in pairs(table) do
--    print(k, v)
--end

table.foreach(_G, print)

--Window_Prototype = {x=0, y=0, width=100, height=100}
--MyWin = {title="Hello"}
--setmetatable(MyWin, {__index = Window_Prototype})
--print(MyWin.title)
--print(MyWin.x)
--print(MyWin.width)
--print(MyWin.s)

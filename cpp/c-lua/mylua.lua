

if not table.foreach then
    table.foreach = function(t, f)
	    for k, v in pairs(t) do f(k, v) end
	end
end

local rname_map = {};


function feed(a)
    if rname_map[a] == nil then
        rname_map[a] = 1
    else
        rname_map[a] = rname_map[a] + 1;
    end
    table.foreach(rname_map, print)
    print(#rname_map);
    print("=============")
end


feed("a");
feed("a");
feed("a");
feed("b");
feed("b");

print(#rname_map);


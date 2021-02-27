local function decode(t,int)
    int=int or 0
    for k,v in pairs(t) do
        if type(v)=="table" then print(string.rep("\t",int),k) decode(v,int+1) else print(string.rep("\t",int),k,v) end
    end
end
package.cpath=package.cpath..";/data/ejdb-lua/lib/liblua-ejdb.so"
local ejdb=require"ejdb"

for i=1,1 do
    print("-------------------query",i)
--    local db = assert(ejdb.open("/test.db","w"))

    local ok,db=pcall(function()
        return ejdb.open("/test.db","w")
    end)
    if not ok then
        print("err is ::::::::::::::::::::::   ",db)
    end

    print("11111111111111111")
    print("22222222222222222")
    print("333333333333333333")

--    db:ensure_index("parrots","/name","us")

--    local id=db:put(  "parrots",{name="Bianca",age=4})
    print("4444444444444444444444 " )
    assert(db:put( 2 , "parrots",{name="Bianca",age=4}) )
--      assert(db:put(8,"parrots","{\"name\":\"Darko\", \"age\":8}"))

--    do
--            local q=assert(ejdb.query("parrots","/*"))
--            local aa=db:exec(q,function() end)
--            assert(db:exec(q,function(id,data)
--              print(id)
--              decode(data)
--
--            end))
--    end
    print("55555555555555555555555")
  --  do
  --    print("Ageing up Bianca")
  --    assert(db:exec(ejdb.query("parrots",'/[name = Bianca] | apply {"age": 5}'),function()end))
  --  end
  --  do
  --    print("Deleting Darko")
  --    --assert(db:exec(ejdb.query("parrots",'/[name = Darko] | del'),function()end))
  --    assert(db:del("parrots",8))
  --  end
  --  do
  --    print("Querying parrots")
  --    local q=assert(ejdb.query("parrots","/*"))
  --    assert(db:exec(q,function(id,data) decode(data) end))
  --    --print("Queryed parrots")
  --  end
  --  print("quering parrot 1")
  --  decode(assert(db:get("parrots",1)))
  --  decode(db:get_meta())
    db:close()

end

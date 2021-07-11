local cjson=require"cjson"


package.cpath = "E:/c_workspace/ejdb-lua-v2/cmake-build-debug/lib/libejdb.dll;"..package.cpath   --只能加后缀package.cpath
--package.cpath=package.cpath..";/data/ejdb-lua/lib/liblua-ejdb.so"


local json_str=[[
{
  "_id" : "56d7c4d5f2b4261738de570a",
  "basic" : {
    "state" : 2,
    "age" : 2.33,
    "succ":false,
    "emb":{"sss":"aaaaa"},
    "ent_name" : "全国大中专教材网"
  },
  "ct" : "2016-03-03 13:00:05.465",
  "userId" : "56d7c361f2b4261738de5708"
}

]]
--print("11111111111111111111")

local ejdb=require"ejdb"
local db = ejdb.open("c:/db.jb","w")

db:put( "docs", json_str)
local q= ejdb.query("docs","/*  | /{userId,basic}")
local nss=db:exec(q,function(id,data)
    print("\n============================id is  ",id,type(data),data.name)
    print(cjson.encode(data) )
end)
print("all num is:::::::::::::::::::: ",nss)
db:close()


--os.execute("ping -n  5 localhost > NUL")




# LuaReader
C++ 封装的一个简单的lua读取器

---
## 需求来源
在实际项目中, 经常会使用lua作为c++工程的配置文件，但是lua为c/c++提供的读取接口太难用了, 于是我封装了LuaReader

## 例子
以下lua文件 test.lua

    count = 10;
	x = count + 5;
	count2 = count + 9;
	user_table = {
	        u = 'this is string .',
	        user2 = {
	                u4 = false,
	                u2 = 90,
	        },
	};

1. 首先需要打开该文件
    `reader.Open("./test.lua")`
2. 现在处于最外面一层, 可以直接读取全局变量
	`reader.GetValue(key, storeValue)` , GetValue支持int, string, double, bool 四种类型
3. 可以使用EnteerTable进入某个table, 读取里面的变量
	`reader.EnterTable("user_table")`
4. 进入table成功后, 需要退出table然后读取后面的变量
	`reader.GetValue("u", str)`
5. 在关闭了LuaReader后， 可以再次打开其他的lua文件进行读取
	`reader.Close()`

	
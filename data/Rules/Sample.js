RN.PARAMS.push({
	"字符串参数" 			: "这是一个字符串",
	"字符串参数_DESC"		: "这是一个描述",
	
	"整数参数" 				: 123,
	"整数参数_DESC"			: "这是一个描述",
	
	"浮点数参数" 			: 123.456,
	"浮点数参数_DESC"		: "这是一个描述",
	
	"布尔值参数" 			: true,
	"布尔值参数_DESC"		: "这是一个描述",
	
	"AString" 				: "这是一个英文字符串",
	"AString_DESC"			: "这是一个描述",
	
	"_T_HasPrinted"			: false,
	"_T_HasPrinted_DESC"	: "这是一个用于上下文传递数据的临时变量,变量名使用_T_开头的变量即为临时变量。临时变量不会出现在属性列表中"
});

RN.RULES.push(function(filePath, isFile, params)
{
	if (!params['_T_HasPrinted'])
	{//这里只会进入一次
		params['_T_HasPrinted'] = true;//这个更改会影响且只会影响本次会话
		
		print(params['字符串参数'] + '\n');
		print(params['整数参数'] + '\n');
		print(params['浮点数参数'] + '\n');
		print(params['布尔值参数'] + '\n');
		print(params['AString'] + '\n');
		
		//以下是文件路径的相关辅助函数，可以在根目录的init.js文件中被找到
		var fileDir = RN.GetPath(filePath);//获得文件路径
		var fileName = RN.GetFileName(filePath);//获得文件名或文件夹名
		var fileNameNoEx = RN.GetFileNameWithoutEx(filePath);//获得文件名或文件夹名(不带后缀)
		var fileEx = RN.GetFileExtension(filePath);//获得文件后缀
		
		var printStr = "文件路径：[{0}] 这[{5}]一个文件\n    目录：[{1}]\n    文件名：[{2}]\n    不带后缀文件名：[{3}]\n    后缀：[{4}]";
		printStr = printStr.format( filePath, fileDir, fileName, fileNameNoEx, fileEx, (isFile?'是':'不是') );
		print(printStr + '\n');
	}
	
	//如果要结束掉本次会话，则可以调用RN.Exit()
	//RN.Exit();
	
	return "";//返回相同的文件名或空文件名则视为不符合规则
});


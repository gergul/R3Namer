RN.PARAMS.push({
	"查找目标" 					: "",
	"查找目标_DESC"				: "将要被替换的字符串",
	"是否使用正则表达式"		: false,
	"是否使用正则表达式_DESC"	: "“查找目标”是否为正则表达式",
	"是否为全局替换模式"		: true,		
	"是否为全局替换模式_DESC" 	: "如果使用正则表达式时，是否使用全局模式",
	
	"替换为"					: "",
	"替换为_DESC"				: "替换到的字符串",
	
	"目标类型"					: 0,
	"目标类型_DESC"				: "目标类型：0->同时应用文件和文件夹（默认）；1->只应用文件；2->只应用文件夹"
});

RN.RULES.push(function(filePath, isFile, params)
{
	if ( (params["目标类型"] != 0 && ((params["目标类型"]==1&& !isFile) || (params["目标类型"]==2&& isFile))) ||
		 (params["目标类型"] != 0 && params["目标类型"] != 1 && params["目标类型"] != 2) )
		return "";
		
	if (!params["查找目标"] || params["查找目标"] == "")
	{
		print("错误：未设置“查找目标”参数。\n");
		RN.Exit();
		return "";
	}
	
	var fileName = RN.GetFileName(filePath);
	if (params["是否使用正则表达式"])
	{
		var regstr = params["查找目标"];
		regstr.replace(/\\/g, "\\\\");
		var reg = new RegExp(regstr, (params["是否为全局替换模式"]? "g" : "") );
		fileName = fileName.replace(reg, params["替换为"]);	
	}
	else
	{
		fileName = fileName.ReplaceG(params["查找目标"], params["替换为"]);
	}
	
	var newFilePath = RN.GetDir(filePath) + fileName;
	return newFilePath;
});